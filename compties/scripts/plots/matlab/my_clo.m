%% Coupled oscillator model 
function y = my_clo(start_state,time,params)
%    A = zeros(4, 4);
%    A(1, 3) = 1;
%    A(2, 4) = 1;
%    ratio = params(1);
%    A(3, 1) = -(params(2) + params(3)) * ratio;
%    A(3, 2) = ratio * params(3);
%    A(4, 1) = params(3);
%    A(4, 2) = -params(3) - params(4);
%    A(3, 3) = -(params(5) + params(6))* ratio;
%    A(3, 4) = ratio * params(6); 
%    A(4, 3) = params(6);
%    A(4, 4) = -params(6) - params(7);
%    T = size(time, 2);
%    y = zeros(T, 4);
%    size(start_state)
%    y(1, :) = start_state';
%    for t = 2:T
%        delta_t = time(t) - time(t-1);
%        eA = expm(A*delta_t);
%        y(t, :) = eA * y(t-1, :)';
%    end
    %for t = 1:T
    %    if size(params, 2) >= 8
    %        y(t, 1) = y(t, 1) + params(8);
    %        y(t, 2) = y(t, 2) + params(9);
    %    elseif size(params, 2) == 11
    %        y(t, 1) = y(t, 1) + params(10) * t;
    %        y(t, 2) = y(t, 2) + params(11) * t;
    %    end 
    %end

    A = zeros(4, 4);
    A(1, 3) = 1;
    A(2, 4) = 1;
    k_1 = params(1);
    k_2 = params(2);
    c_1 = params(3);
    c_2 = params(4);
    d_1 = params(5);
    d_2 = params(6);
    A(3, 1) = -k_1 - c_1;
    A(3, 2) = c_1;
    A(4, 1) = c_2;
    A(4, 2) = -k_2 - c_2;
    A(3, 3) = -d_1;
    A(3, 4) = 0.0; 
    A(4, 3) = 0.0;
    A(4, 4) = -d_2;
    x_o = start_state;
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
