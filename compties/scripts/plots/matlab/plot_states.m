function plot_states(file, name, index)
   close all;
   offset = 0.05;
   data=importdata(file);
   fontsize = 12;
   w_values=data.data(:,index);
   m_values=data.data(:,index+1);
   T = [1 : size(w_values)];
   cur_plot= figure;
  
   cur_plot = figure('PaperPositionMode', 'auto');
   plot(T, m_values, 'b',  'LineWidth', 3);
   hold on;
   plot(T, w_values, 'r',  'LineWidth', 3);
 
   %get figure size
   %posFig = get(gca,'Position');
   fYLabelRotation = 90.0;
   I = ylabel('GSR','FontSize', fontsize);
   set(get(gca, 'YLabel'), 'Rotation', fYLabelRotation);
 
   %set figure size
   %posFigNew = posFig + [offset 2*offset 0 -3.0*offset];
   %set(gca,'Position',posFigNew)
   xlabel_h=xlabel('$t$', 'FontSize', fontsize, 'Interpreter', 'latex');
   % set label size 
   %set(xlabel_h,'position',get(xlabel_h,'position')+[0.0 -offset 0.0]);
   I = legend(['Individual 0'], ['Individual 1']);
 

   w_max= max(w_values);
   w_min = min(w_values);
   m_max = max(m_values);
   m_min = min(m_values);
   w_max = max(w_max, m_max);
   w_min = min(w_min, m_min);
   axis([1 size(w_values, 1) w_min-0.1 w_max+0.1]);
   %set(gca,'ytick',[])
   set(gca, 'FontSize', fontsize); 
   %set(gca,'Units','normalized')
   %set(get(gca,'XLabel'),'Units','data')

   %file_name = sprintf('bmiave_(%0.2f)_bmidif_(%0.2f).pdf', bmiave, bmidif)
   file_name=[name '.pdf'];
   %title(['$\overline{\omega}$ = ', num2str(bmiave), ', $\hat{\omega}$ = ', num2str(bmidif)], 'FontSize', 40, 'Interpreter','latex');

   saveas(cur_plot, file_name);
   %file_name = sprintf('bmiave_(%0.2f)_bmidif_(%0.2f).eps', bmiave, bmidif)
   file_name=[name '.eps'];
   saveas(cur_plot, file_name, 'epsc');
   %file_name = sprintf('bmiave_(%0.2f)_bmidif_(%0.2f).jpg', bmiave, bmidif)
   file_name=[name '.jpg'];
   saveas(cur_plot, file_name, 'jpg');
end 
