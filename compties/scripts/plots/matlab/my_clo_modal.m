function y = my_clo_modal(start_state,time,params)
theta_1 = params(1);
theta_2 = params(2);
T1 = params(3);
T2 = params(4);
d1 = params(5);
d2 = params(6);
d_v = [d1, 0.0; 0.0 d2];

%w_1 = (2*pi/T1) * (2*pi/T1);
%w_2 = (2*pi/T2) * (2*pi/T2);
w_1 = T1;
w_2 = T2;
alpha = 100.0;
if(theta_1 == 0)
    theta_2 = pi/2.0;
elseif(theta_2 == 0)
    theta_1 = pi/2.0;
else
    alpha = -sin(theta_1) * sin(theta_2)/(cos(theta_1)*cos(theta_2));
    m = [alpha, 0; 0, 1];
end
m1= 1;
m = [alpha*m1, 0; 0, m1];

temp1 = (cos(theta_1) * cos(theta_1) * alpha + sin(theta_1) * sin(theta_1))*m1;
temp2 = (cos(theta_2) * cos(theta_2) * alpha + sin(theta_2) * sin(theta_2))*m1;

r1 = sqrt(1.0/temp1);
r2 = sqrt(1.0/temp2);

e = [r1*cos(theta_1), r2*cos(theta_2); r1*sin(theta_1), r2*sin(theta_2)];

x = zeros(size(time,1), 4);

v = [w_1, 0; 0, w_2];
q = x;
for i = 1:2 
    damping_ratio(i) = d_v(i, i)/(2.0*(sqrt(v(i,i))));
    psi(i) = atan(damping_ratio(i)/(1.0- power(damping_ratio(i), 2.0)));
    w_d(i) = sqrt(v(i,i)) * (1.0-(damping_ratio(i)* damping_ratio(i)));
end
Ap = [2.0 2.0];
phi = [pi/4.0 0.0];

for i = 1:2
    q(1, i) = Ap(i) * cos(-phi(i));
    if(damping_ratio(i) > 1)
    end
end
for i = 3:4
    q(1, i) = -Ap(i - 2) * sin(-phi(i-2)) * w_d(i-2) + Ap(i-2) *(-damping_ratio(i-2)* sqrt(v(i-2, i-2))) * cos(-phi(i-2));
end
x_q(1, 1:4) = start_state';
q(1,1:2) = (inv(e) * x_q(1,1:2)')';
q(1,3:4) = (inv(e) * x_q(1,3:4)')';
%q(1, 1:4) = [1.0, 0.0, 0.0, 1.0];

x_q = x;
x_q(1, 1:2) = (e * q(1, 1:2)')';
x_q(1, 3:4) = (e * q(1, 3:4)')';
A_q = [ 0 0 1 0; 0 0 0 1; -v -d_v];
for i = 2: size(time, 1)
    A_q_t = expm(A_q * (time(i) - time(1)));
    q(i, :) = (A_q_t * q(1, :)')';
    x_q(i, 1:2) = (e * q(i, 1:2)')';
    %x_q(i, 3:4) = (e * q(i, 3:4)')';
end
hold on;

clear x_q;
%clear q;
%q(1, 1:4) = [1.0, 0.0, 0.0, 1.0];
d_v(1,2) = 0.0;
d_v(2,1) = 0.0;

x_q = x;
x_q(1, 1:2) = (e * q(1, 1:2)')';
x_q(1, 3:4) = (e * q(1, 3:4)')';
A_q = [ 0 0 1 0; 0 0 0 1; -v -d_v];
for i = 2: size(time, 1)
    A_q_t = expm(A_q * (time(i) - time(1)));
    q(i, :) = (A_q_t * q(1, :)')';
    x_q(i, 1:2) = (e * q(i, 1:2)')';
    x_q(i, 3:4) = (e * q(i, 3:4)')';
end

size(x_q);
y = x_q;
