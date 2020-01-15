%clc;
%close all; 
clear;
%m_1 = 20.0;
%m_2 = 3.0;
theta_1 = 1.9
theta_2 = 2.1;
T1 = 50;
T2 = 100;
d1 = 0.00;
d2 = 0.00;
d_v = [d1, 0.0; 0.0 d2];

w_1 = (2*pi/T1) * (2*pi/T1);
w_2 = (2*pi/T2) * (2*pi/T2);
%if(theta_1 == 0.0)
   % theta_2 = pi/2.0
%else
 %   theta_2 = atan(-m_1/(m_2 * tan(theta_1)))
%end
alpha = 100.0;
if(theta_1 == 0)
    theta_2 = pi/2.0;
elseif(theta_2 == 0)
    theta_1 = pi/2.0;
else
    alpha = -sin(theta_1) * sin(theta_2)/(cos(theta_1)*cos(theta_2))
    m = [alpha, 0; 0, 1];
end
m1= 1;

%tan(theta_2) * tan(theta_1) 

m = [alpha*m1, 0; 0, m1]
%m = [m_1, 0; 0, m_2]
%d = inv(m)*[d1+d2, -d2; -d2, d2+d3];

temp1 = (cos(theta_1) * cos(theta_1) * alpha + sin(theta_1) * sin(theta_1))*m1;
temp2 = (cos(theta_2) * cos(theta_2) * alpha + sin(theta_2) * sin(theta_2))*m1;
%temp1 = (cos(theta_1) * cos(theta_1) * m_1 + sin(theta_1) * sin(theta_1))*m_2;
%temp2 = (cos(theta_2) * cos(theta_2) * m_1 + sin(theta_2) * sin(theta_2))*m_2;

r1 = sqrt(1.0/temp1);
r2 = sqrt(1.0/(-temp2));
%r1 = sqrt(1.0/(power(cos(theta_1),2) * m_1 + power(sin(theta_1),2) * m_2))
%r2 = sqrt(1.0/(power(cos(theta_2),2) * m_1 + power(sin(theta_2),2) * m_2))
%temp = r1 * r2 * m_1 * cos(theta_1) * cos(theta_2) + m_2 * sin(theta_1) * sin(theta_2)*r1 *r2

e = [r1*cos(theta_1), r2*cos(theta_2); r1*sin(theta_1), r2*sin(theta_2)]

t = [0:0.5:350]';
x = zeros(size(t,1), 4);

v = [w_1, 0; 0, w_2];

q = x;
for i = 1:2 
    damping_ratio(i) = d_v(i, i)/(2.0*(sqrt(v(i,i))))
    psi(i) = atan(damping_ratio(i)/(1.0- power(damping_ratio(i), 2.0)));
    w_d(i) = sqrt(v(i,i)) * (1.0-(damping_ratio(i)* damping_ratio(i)));
end
Ap = [2.0 2.0]
phi = [pi/4.0 0.0];

for i = 1:2
    q(1, i) = Ap(i) * cos(-phi(i));
    if(damping_ratio(i) > 1)
    end
end
for i = 3:4
    q(1, i) = -Ap(i - 2) * sin(-phi(i-2)) * w_d(i-2) + Ap(i-2) *(-damping_ratio(i-2)* sqrt(v(i-2, i-2))) * cos(-phi(i-2));
end
temp = q(1, :)
x_q(1, 1:4) = [1.0, -1.0, -1.0, 10.0];
%x_q(1, 1:2) = 10.0 * e(:,1) + 10.0 * e(:,2);
%x_q(1, 3:4) = 10.0 * e(:,1) + 10.0 * e(:,2);
q(1,1:2) = (inv(e) * x_q(1,1:2)')';
q(1,3:4) = (inv(e) * x_q(1,3:4)')';
%q(1, 1:4) = [1.0, 0.0, 0.0, 1.0];

x_q = x;
x_q(1, 1:2) = (e * q(1, 1:2)')';
x_q(1, 3:4) = (e * q(1, 3:4)')';
A_q = [ 0 0 1 0; 0 0 0 1; -v -d_v];
for i = 2: size(t, 1)
    A_q_t = expm(A_q * (t(i) - t(1)));
    q(i, :) = (A_q_t * q(1, :)')';
    x_q(i, 1:2) = (e * q(i, 1:2)')';
    %x_q(i, 3:4) = (e * q(i, 3:4)')';
end
hold on;
%plot(t, q(:,1), 'b-', t, q(:,2), 'r-', 'MarkerSize', 5, 'LineWidth', 5);
plot(t, x_q(:,1), 'b', t, x_q(:,2), 'r', 'MarkerSize',1, 'LineWidth', 1);

clear x_q;
%clear q;
%q(1, 1:4) = [1.0, 0.0, 0.0, 1.0];
d_v(1,2) = 0.0;
d_v(2,1) = 0.0;

x_q = x;
x_q(1, 1:2) = (e * q(1, 1:2)')';
x_q(1, 3:4) = (e * q(1, 3:4)')';
A_q = [ 0 0 1 0; 0 0 0 1; -v -d_v];
for i = 2: size(t, 1)
    A_q_t = expm(A_q * (t(i) - t(1)));
    q(i, :) = (A_q_t * q(1, :)')';
    x_q(i, 1:2) = (e * q(i, 1:2)')';
    x_q(i, 3:4) = (e * q(i, 3:4)')';
end
hold on;
%plot(t, q(:,1), 'm--', t, q(:,2), 'g--', 'MarkerSize', 2, 'LineWidth', 2);
plot(t, x_q(:,1), 'm', t, x_q(:,2), 'g', 'MarkerSize', 1, 'LineWidth', 5);
%plot(t, x_q(:,3), 'm', t, x_q(:,4), 'g', 'MarkerSize', 3, 'LineWidth', 3);
%clear x_q;
%clear q;
%x_q = x;
for i = 1:2
    new_q(1, i) = Ap(i) * cos(-phi(i));
end
for i = 3:4
    new_q(1, i) = -Ap(i - 2) * sin(-phi(i-2)) * w_d(i-2) + Ap(i-2) *(-damping_ratio(i-2)* sqrt(v(i-2, i-2))) * cos(-phi(i-2));
end
temp = new_q(1, :);
x_q_modal = x_q;
for tt = 2:size(t, 1)
    for i = 1:2
        term_1 = (q(1, i)/(sqrt(1- power(damping_ratio(i), 2.0))) * cos(w_d(i) * t(tt) - psi(i)) + q(1, i+2)/w_d(i) * sin(w_d(i) * t(tt)));
        term_2 = exp(-damping_ratio(i) * sqrt(v(i,i)) * t(tt));
        new_q(tt, i) =  term_1 * term_2;
        a1 = term_1 * (-damping_ratio(i) * sqrt(v(i, i))) * term_2;
        a2= term_2 * ( -q(1, i)/(sqrt(1 - power(damping_ratio(i), 2.0))) * sin(w_d(i) * t(tt) - psi(i)) * w_d(i)+ q(1, i+2) * cos(w_d(i) * t(tt)));
        new_q(tt, i+2) =  term_1 * (-damping_ratio(i) * sqrt(v(i, i))) * term_2  + term_2 * (-q(1, i)/(sqrt(1 - power(damping_ratio(i), 2.0))) * sin(w_d(i) * t(tt) - psi(i)) * w_d(i) + q(1, i+2) * cos(w_d(i) * t(tt))); 
        %new_q(tt, i) = Ap(i) * exp(-damping_ratio(i) * sqrt(v(i,i))*t(tt)) * cos(w_d(i) * t(tt) - phi(i));
    end
    x_q_modal(tt, 1:2) = (e * new_q(tt, 1:2)')';
    x_q_modal(tt, 3:4) = (e * new_q(tt, 3:4)')';
end
hold on;
%plot(t, new_q(:,1), 'bv', t, new_q(:,2), 'rv', 'MarkerSize', 2, 'LineWidth', 2);
plot(t, x_q_modal(:,1), 'b', t, x_q_modal(:,2), 'r', 'MarkerSize', 1, 'LineWidth', 2);
%plot(t, x_q(:,3), 'b', t, x_q(:,4), 'r', 'MarkerSize', 5, 'LineWidth', 2);

x_q(1, :) = x_q_modal(1,:);
D = inv(e)' * d_v * inv(e);
K = inv(e)' * v * inv(e) ;
A = [ 0 0 1 0; 0 0 0 1; -inv(m)* K -inv(m) * D];
for i = 2: 10%size(t, 1)
    A_t = expm(A * (t(i) - t(i-1)));
    x_q(i, :) = (A_t * x_q(i-1, :)')';
end
hold on;

plot(t, x_q(:,1), 'b', t, x_q(:,2), 'r*', 'LineWidth', 1);

%figure(2)
%plot(t, new_q(:,1), 'bv', t, new_q(:,2), 'rv', 'MarkerSize', 2, 'LineWidth', 2);
