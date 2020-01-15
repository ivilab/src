function manipulate(mathfunc_full, XYLim, varargin)
%% MANIPULATE - Plot graphics with interactively manipulable parameters
% MANIPULATE   by itself, it displays the plot of a sinusoidal function with
% four slider controlling four parameters
% MANIPULATE(MATHFUNC, XYLIM, PARAM1, PARAM2, ...)
%  MATHFUNC is an 1 x 3 cell array in the following form:
%   {FUNC X INIT_PARAM}
%   ------------------
%   FUNC: the handle of a function that accepts two inputs: x and params,
%   and returns the corresponding y value(s) of this parametrized curve.
%   X:    the initial x values, must be a vector.
%   INIT_PARAMS: the initial parameters for the parametrized curve.
%
%  XYLIM is an 1 x 2 cell array in the following form:
%   {XLIM YLIM}
%   -----------
%   XLIM: an 1 x 2 double array describing the range of the x axis
%   YLIM: an 1 x 2 double array describing the range of the y axis
%
%   * If XYLIM is an empty cell array, then the axes will automatically
%   resize to fit the plot
%   
%  PARAM1, PARAM2, ... are cell arrays that could take one of the four forms:
%   {ITHPARAM}
%   {ITHPARAM, TAGS}
%   {ITHPARAM, LOWER, UPPER}
%   {ITHPARAM, TAG, LOWER, UPPER}
%    -----------------------------
%    ITHPARAM is a single integer ranged from 1 to numel(INIT_PARAMS). It
%    indicates which parameter should be manipulated.
%    TAG is a string used for labelling the slider
%    LOWER and UPPER are the lower and upper limit of the parameter,
%    respectively. They must be real numbers.
%  * The number of PARAMNs must not exceed numel(INIT_PARAMS);
%  * ITHPARAM must be different for each PARAMN.
% 
% EXAMPLES:
% ---------
% (1):
%    manipulate({@(x,param)(param(1)*x+param(2)),[-10:10],[2 0]},{[-10 10],[-10 10]}, {1,'Slope',1 10}, {2,'Shift',-5 5})
%    
%    MATHFUNC = {@(x,param)(param(1)*x+param(2)): 
%       Displays the plot for function "y = ax + b". x takes the values of
%       -10:10; a and b are initialized to 2 and 0, respectively;
%    XYLIM = {[-10 10],[-10 10]}:
%       Both the XLim and YLim of the axes are manually set to [-10 10];
%    PARAM1 = {1,'Slope',1 10},
%    PARAM2 = {2,'Shift',-5 5}:
%        Both a and b are manipulated. a ('Slope') ranges from 1 to 10, and b
%       ('Shift') ranges from -5 to 5
%
% (2):
%   manipulate({@(x,param)(cos(param(2)*sin(param(1)*x))),[-4*pi:0.1*pi:4*pi],[1 1]},{}, {2,0, 2*pi})
%
%   Displays the plot for function "y = cos(b*sin(a*x))". x ranges from
%   -4pi~4pi; a and b are both initially set to 1;
%   The axes are automatically rescaled to fit the plot;
%   Only the second parameter (b, ranged 0 ~ 2pi) is manipulated. No labels will be shown.
%
% Stellari Studio, 2012
% Mingjing Zhang, Vision and Media Lab @ Simon Fraser University
%

%% Constants
slider_thickness = 20;
slider_spacing = 10;

tag_width = 100;
paramval_width = 50;
T = 50;
time = [1:1:T]';
xs = zeros(T, 4);
xs(1, 1) = 1.0;
xs(1, 2) = -1.0;
xs(1, 3) = 0.0;
xs(1, 4) = 0.0;

%% Argument Checking
if nargin == 0
    % If no input is detected, then engage demo mode.
    x = -4*pi:0.01*pi:4*pi;
    init_params = [pi/2.0 pi/2.0 10 20.0 0.0 0.0 1.0];
    
    n_params2man = 7;
    params2man = [1 2 3 4 5 6 7];         % The parameters to manipulate
    %tags = {'delta','eta-1', 'eta-2', 'eta-3', 'zeta-1', 'zeta-2', 'zeta-3'};
    tags = {'theta_1','theta_2', 'T1', 'T2', 'd_1', 'd_2', 'no'};
    lowers = [-pi/2.0 -pi/2.0 0 0 -10 -10 1];
    uppers = [pi/2.0 pi/2.0 100 100 10 10 100];

    mathfunc = @my_clo_modal;
    
    XLim = [-0 50];
    YLim = [-5 5];
    LimMode = 'Manual';         % Fix the XLim and YLim of the axes
    
else
    % If there is some input, then use the input functions and parameters
    % - Check mathfunc -
    if numel(mathfunc_full)~=3
        error('Manipulate(): incorrect MATHFUNC.');
    else
        mathfunc = mathfunc_full{1};
        x = mathfunc_full{2};
        init_params = mathfunc_full{3};
    end
    
    % - Check XYLim -
    if numel(XYLim)~=2
        XLim = [0 1];   % Will not be used 
        YLim = [0 1];
       LimMode = 'Auto'; 
    else
        XLim = XYLim{1};
        YLim = XYLim{2};
        LimMode = 'Manual';
    end
    
    % - Check Param List -
    n_params2man = numel(varargin);
    params2man = zeros(1,n_params2man);
    tags = cell(1,n_params2man);
    lowers = ones(1,n_params2man).*0.01;
    uppers = ones(1,n_params2man);
    
    for index = 1:n_params2man
        this_param = varargin{index};
        n_this_param = numel(this_param);
        
        if n_this_param == 0
            continue;
        elseif n_this_param == 1
            params2man(index) = this_param{1};
        elseif n_this_param == 2
            params2man(index) = this_param{1};
            tags{index} = this_param{2};
        elseif n_this_param == 3
            params2man(index) = this_param{1};
            lowers(index) = this_param{2};
            uppers(index) = this_param{3};
        elseif n_this_param >= 4
            params2man(index) = this_param{1};
            tags{index} = this_param{2};
            lowers(index) = this_param{3};
            uppers(index) = this_param{4};
        end
    end
end

%% Calculate the initial function value
[init_y, q]= mathfunc(xs, time, init_params);

%% Prepare the figure and axes

monitor_pos = get(0, 'MonitorPositions');

fig_h = figure('CloseRequestFcn',@figureCloseReq_Callback);

% A little trick to make sure the axes is not expanded as the figure
% resizes
axes_h = axes('Parent',fig_h,'Units','pixels');

axes_pos_p = get(axes_h,'Position');

figure_pos = get(fig_h,'Position');

figure_pos(4) =  figure_pos(4) + (n_params2man+1)* (slider_thickness + slider_spacing);

if figure_pos(4) + figure_pos(2) > monitor_pos(4) - 100
    % Make sure that the figure header is always visible
    figure_pos(2) = monitor_pos(4) - figure_pos(4) - 100;
end

set(fig_h, 'Position', figure_pos);

% set(axes_h, 'Units', 'normalized');

%% Start plotting!!
           
plot_h = plot(time, init_y(:,1), 'r', 'Linewidth', 3);
hold on;
plot_h_2 = plot(time, init_y(:,2), 'b', 'LineWidth', 3);

set(axes_h,'XLim', XLim, 'XLimMode', LimMode, 'YLim', YLim, 'YLimMode', LimMode);

set(plot_h,'UserData', init_params);

%figure(2)
%%plot_q_ = plot(time, q(:,1), 'r', 'Linewidth', 3);
%hold on;
%plot_q_2 = plot(time, q(:,2), 'b', 'LineWidth', 3);


slider_h = zeros(1,n_params2man);
%slider_timer = zeros(1, n_params2man)
%paramval_h = zeros(1, n_params2man)
tag_h = zeros(1, n_params2man)
for j = 1:n_params2man    
    % Nicely arrange the sliders and texts 
    start_pos = [axes_pos_p(1), axes_pos_p(2) + axes_pos_p(4) + ...
        (n_params2man + 1 - j).*(slider_spacing + slider_thickness)];
    tag_h(j) = uicontrol('style','text','Units','pixels', ...
        'Position',[start_pos, tag_width, slider_thickness], ...
        'String', tags{j});
    slider_h(j) = uicontrol('Style','Slider', ...
        'Position', [start_pos+[tag_width 0] axes_pos_p(3)-(tag_width+paramval_width), slider_thickness], ...
        'Min', lowers(j), ...
        'Max', uppers(j), ...
        'Value', init_params(j));
    paramval_h(j) = uicontrol('style','text','Units','pixels',...
        'Position',[start_pos+[axes_pos_p(3)-paramval_width,0],paramval_width, slider_thickness], ...
        'String', num2str(init_params(j)));
%     cur_slider = slider_h(i);
    slider_timer(j) = timer('TimerFcn', {@slider_updater,slider_h(j), j},...
    'ExecutionMode','fixedRate',...
    'Period', 1./10, 'BusyMode', 'queue','UserData',params2man(j));
    start(slider_timer(j));
end

set(axes_h, 'Units', 'normalized');

    function slider_updater(obj, event, cur_slider_h, iParam)
        % SLIDER_UPDATER
        
        % This persistent variable is a relic from the time when
        % slider_updater was still a stand alone function.
        persistent last_slider_value
        
        %% Step 1: Get the current value
        cur_slider_value = get(cur_slider_h, 'Value');
        
        %% Step 2: If the slider value is not changed, then do nothing
        if cur_slider_value == last_slider_value
            return;
        end
        
        last_slider_value = cur_slider_value; % Store the last slider value
        
        %% Step 3: Do the mathematics
        %x = get(plot_h,'XData');
        %temp_params = get(plot_h,'UserData');
        %params = temp_params{1};
        params = get(plot_h,'UserData');
        params(iParam) = last_slider_value;
        y = mathfunc(xs,time, params);
        
        %% Step 4: Update the graphics
        set(plot_h, 'Color', 'blue');
        set(plot_h, 'YData', y(:,1));
        set(plot_h_2, 'Color', 'red');
        set(plot_h_2, 'YData', y(:,2));
        %set(plot_h(2), 'YData', y(:,2), 'b');
        set(plot_h, 'UserData', params);
        set(paramval_h(iParam), 'String', num2str(params(iParam)));
        drawnow;
    end
%% A demonstration sinusoidal function
    function y = my_sin(x,params)
        % my_sin sinusoidal function
        % y = my_sin(x, params) returns
        %   y = params(1) * sin(params(2)* x + params(3)) + params(4)
        %   All angles are represented in radians
        y = params(1) * sin(params(2)* x + params(3)) + params(4);
    end

%% Delete the timers when the window is closed
    function figureCloseReq_Callback(obj, event)
        for o = 1:n_params2man 
            stop(slider_timer(o));
            delete(slider_timer(o));
        end
        closereq;
    end
%% Coupled oscillator model 
    function y = my_clo(x,time,params)
        A = zeros(4, 4);
        A(1, 3) = 1;
        A(2, 4) = 1;
        A(3, 1) = -params(2) - params(1)*params(3);
        A(3, 2) = params(1) * params(3);
        A(4, 1) = params(3);
        A(4, 2) = -params(3) - params(4);
        A(3, 3) = -params(5) - params(1) * params(6);
        A(3, 4) = params(1) * params(6); 
        A(4, 3) = params(6);
        A(4, 4) = -params(6) - params(7);
        x_o = x(1, :);
        T = size(time, 2);
        y = zeros(T, 2);
        y(1, 1) = x_o(1);
        y(1, 2) = x_o(2);
        for t = 2:T
            delta_t = time(t) - time(1);
            temp = expm(A*delta_t) * x_o';
            y(t, 1) = temp(1);
            y(t, 2) = temp(2);
        end
    end
    function [y new_q]= my_clo_modal(start_state,time,params)
        theta_1 = params(1);
        theta_2 = params(2);
        T1 = params(3);
        T2 = params(4);
        d1 = params(5);
        d2 = params(6);
        m1= params(7);
        d_v = [d1, 0.0; 0.0 d2];

        w_1 = (2*pi/T1) * (2*pi/T1);
        w_2 = (2*pi/T2) * (2*pi/T2);
        alpha = -1.0;
        if(abs(theta_1 - pi/2.0)<1e-12 || abs(theta_1+pi/2.0) < 1e-12)
            theta_2 = 0.0
            alpha = 1.0;
        elseif(abs(theta_2-pi/2.0)< 1e-12 || abs(theta_2 - pi/2.0) < 1e-12)
            theta_1 = 0.0;
            alpha = 1.0;
        else
            alpha = -sin(theta_1) * sin(theta_2)/(cos(theta_1)*cos(theta_2));
            m = [alpha, 0; 0, 1];
        end
 
        m = [alpha*m1, 0; 0, m1];

        temp1 = (cos(theta_1) * cos(theta_1) * alpha + sin(theta_1) * sin(theta_1))*m1;
        temp2 = (cos(theta_2) * cos(theta_2) * alpha + sin(theta_2) * sin(theta_2))*m1;

        r1 = sqrt(1.0/temp1);
        r2 = sqrt(1.0/temp2);

        e = [r1*cos(theta_1), r2*cos(theta_2); r1*sin(theta_1), r2*sin(theta_2)];
        q = zeros(size(time,1), 4);
        x_q_modal = zeros(size(time,1), 4);
        v = [w_1, 0; 0, w_2];
        for i = 1:2 
            damping_ratio(i) = d_v(i, i)/(2.0*(sqrt(v(i,i))));
            psi(i) = atan(damping_ratio(i)/(1.0- power(damping_ratio(i), 2.0)));
            w_d(i) = sqrt(v(i,i)) * (1.0-(damping_ratio(i)* damping_ratio(i)));
        end
        Ap = [2.0 2.0];
        phi = [pi/4.0 0.0];

        %for i = 1:2
        %    q(1, i) = Ap(i) * cos(-phi(i));
        %end
        %for i = 3:4
        %    q(1, i) = -Ap(i - 2) * sin(-phi(i-2)) * w_d(i-2) + Ap(i-2) *(-damping_ratio(i-2)* sqrt(v(i-2, i-2))) * cos(-phi(i-2));
        %end
        %temp = q(1, :)
        x_q_modal(1, 1:4) = start_state(1,:);
        q(1,1:2) = (inv(e) * x_q_modal(1,1:2)')';
        q(1,3:4) = (inv(e) * x_q_modal(1,3:4)')';
        %q(1, 1:4) = [1.0, 0.0, 0.0, 1.0];

        for tt = 2:size(time, 1)
            for i = 1:2
                term_1 = (q(1, i)/(sqrt(1- power(damping_ratio(i), 2.0))) *...
                cos(w_d(i) * time(tt) - psi(i)) + q(1, i+2)/w_d(i) * sin(w_d(i)...
                * time(tt)));
                term_2 = exp(-damping_ratio(i) * sqrt(v(i,i)) * time(tt));
                new_q(tt, i) =  term_1 * term_2;

                a1 = term_1 * (-damping_ratio(i) * sqrt(v(i, i))) * term_2;
                a2= term_2 * ( -q(1, i)/(sqrt(1 - power(damping_ratio(i), 2.0))) ...
                * sin(w_d(i) * time(tt) - psi(i)) * w_d(i)+ q(1, i+2) * ...
                cos(w_d(i) * time(tt)));

                new_q(tt, i+2) =  term_1 * (-damping_ratio(i) * sqrt(v(i, i))) * ...
                term_2  + term_2 * (-q(1, i)/(sqrt(1 - power(damping_ratio(i), ...
                2.0))) * sin(w_d(i) * time(tt) - psi(i)) * w_d(i) + q(1, i+2) * ...
                cos(w_d(i) * time(tt))); 

                %new_q(tt, i) = Ap(i) * exp(-damping_ratio(i) * sqrt(v(i,i))*t(tt)) * cos(w_d(i) * t(tt) - phi(i));
            end
            x_q_modal(tt, 1:2) = (e * new_q(tt, 1:2)')';
            x_q_modal(tt, 3:4) = (e * new_q(tt, 3:4)')';
        end
        y = x_q_modal;
        %D = inv(e)' * d_v * inv(e)
        K = inv(e)' * v * inv(e)
    end
end
