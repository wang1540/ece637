


Rx = [2 -1.2; -1.2 1];
W = randn(2, 1000);
[E,Lamda] = eig(Rx);
X_tilda = ((Lamda)^(1/2))*W;
X = E*X_tilda;

u = 1/1000*sum(X,2);
Z = [X(1,:) - u(1); X(2,:) - u(2)];
RR = 1/999 * (Z * Z')
[E,Lamda] = eig(RR);
X_tilda = E'*X;
W = (Lamda)^(-1/2)*E'*X;

u = 1/1000*sum(W,2);
Z = [W(1,:) - u(1); W(2,:) - u(2)];
RR = 1/999 * (Z * Z')


figure(1);
plot(W(1,:),W(2,:),'.');
xlabel('W_1');
ylabel('W_2');
title('Scatter Plot for W');


figure(2);
plot(X_tilda(1,:),X_tilda(2,:),'.');
xlabel('$\tilde{X_1}$','Interpreter','LaTex');
ylabel('$\tilde{X_2}$','Interpreter','LaTex');
title('Scatter Plot for $\tilde{X}$','Interpreter','LaTex');





