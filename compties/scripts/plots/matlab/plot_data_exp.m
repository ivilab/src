function plot_data_exp(data_file, exp_file, name, model_name)
   close all;
   offset = 0.05;
   fontsize = 12;
   data=importdata(data_file);
   w_values=data.data(:,2);
   m_values=data.data(:,3);
   exp_data=load(exp_file);
   w_exp=exp_data(:,2);
   m_exp=exp_data(:,1);
   T = [1 : size(w_values)];
   cur_plot = figure('PaperPositionMode', 'auto');
   plot(T, m_values, 'b',  'LineWidth', 3);
   hold on;
   plot(T, w_values, 'r',  'LineWidth', 3);
   plot(T, m_exp, 'r--',  'LineWidth', 3);
   plot(T, w_exp, 'b--',  'LineWidth', 3);
 
   %get figure size
   posFig = get(gca,'Position');
   fYLabelRotation = 90.0;
   I = ylabel('GSR','FontSize', fontsize);
   set(get(gca, 'YLabel'), 'Rotation', fYLabelRotation);
 
   %set figure size
   %posFigNew = posFig + [offset 2*offset 0 -3.0*offset];
   %set(gca,'Position',posFigNew)
   xlabel_h=xlabel('$t$', 'FontSize', fontsize, 'Interpreter', 'latex');
   % set label size 
   %set(xlabel_h,'position',get(xlabel_h,'position')+[0.0 -offset 0.0]);
   %I = legend(['data $x^0$'], ['data $x^1$'], ['model $x^0$'], ['model $x^1$'], 'fontsize', 12);
   %I = legend(['data $x^0$'], ['data $x^1$'], ['model $x^0$'], ['model $x^1$'] );
   %I = legend(['Individual 0 data'], ['Individual 1 data'], ['Individual 0 model'], ['Individuadl 1 model']);
   I = legend(['data 0'], ['data 1'], ['model 0'], ['model 1']);
   %set(I, 'interpreter', 'latex');

   w_max= max(w_values);
   w_min = min(w_values);
   m_max = max(m_values);
   m_min = min(m_values);
   w_max = max(w_max, m_max);
   w_min = min(w_min, m_min);
   axis([1 size(w_values, 1) w_min-0.1 w_max+0.1]);
   %axis([1 size(w_values, 1) -1.20 1.2]);
   %set(gca,'ytick',[])
   set(gca, 'FontSize', fontsize); 
   %set(gca,'Units','normalized')
   %set(get(gca,'XLabel'),'Units','data')

   %% plot the training line
   num_train = ceil(size(T,2) * 0.8);
   x_trains = [num_train num_train];
   y_trains = [w_min-0.1 w_max + 0.1];
   plot(x_trains, y_trains, 'm', 'LineWidth', 4);


   %file_name = sprintf('bmiave_(%0.2f)_bmidif_(%0.2f).pdf', bmiave, bmidif)
   title(model_name);
   file_name=[name '.pdf'];
   %title(['$\overline{\omega}$ = ', num2str(bmiave), ', $\hat{\omega}$ = ', num2str(bmidif)], 'FontSize', 40, 'Interpreter','latex');
   saveas(cur_plot, file_name);
   file_name=[name '.eps'];
   saveas(cur_plot, file_name, 'epsc');
   file_name=[name '.jpg'];
   saveas(cur_plot, file_name, 'jpg');
end 
