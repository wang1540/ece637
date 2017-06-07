clc;
clear;

x = imread('images/img14sp.tif');
y = imread('images/img14g.tif');
x = double(x);
y = double(y);

[r,c] = size(y);
N = floor(r/20)*floor(c/20);
Z = zeros(N,49);
Y = zeros(floor(r/20),floor(c/20));
t = 1;
for i=1:floor(r/20)
    for j=1:floor(c/20)
        Y(i,j) = y(i*20,j*20);
        Z(t,:) = reshape(x(i*20-3:i*20+3, j*20-3:j*20+3)', 1, []);
        t = t+1;
    end
end
Zt = Z;
Y = reshape(Y', [], 1);
R_zz = Z'*Z ./ N;
r_zy = Z'*Y ./ N;

theta = inv(R_zz)*r_zy;



x_bord = zeros(r+6,c+6);
x_bord(4:r+3,4:c+3)=x;

for i = 1:r
    for j = 1:c
        temp = reshape(x_bord(i:i+6,j:j+6)',1,49);
        yy(i,j) = temp*theta;
    end
end

temp = reshape(x_bord(1:7,1:7)',1,49);

imshow(uint8(yy));


theta = reshape(theta, 7, 7);
theta'
