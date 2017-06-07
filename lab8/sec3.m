clc;
clear all;
close all;

img = imread('house.tif');
[m, n] = size(img);
T = 127;
img = double(img);
b = zeros(m,n);
b((img>127)) = 255;
% colormap(gray(255));
% image(b);
% image(img);


RMSE = 0;
for i = 1:m
    for j = 1:n
        RMSE = RMSE+ (img(i,j)-b(i,j))^2;
    end
end
RMSE = sqrt(RMSE/(m*n));

fid = fidelity(img, b)