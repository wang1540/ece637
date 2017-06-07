clc;
clear all;
close all;

img = imread('house.tif');
[m, n] = size(img);
img = double(img);
fg = 255*(img/255).^(2.2);

I2 = [1 2; 3 0];
I4 = [4*I2+1 4*I2+2; 4*I2+3 4*I2];
I8 = [4*I4+1 4*I4+2; 4*I4+3 4*I4];


T2 = 255*(I2+0.5)/(2^2);
T4 = 255*(I4+0.5)/(4^2);
T8 = 255*(I8+0.5)/(8^2);

b2 = zeros(m,n);
b4 = zeros(m,n);
b8 = zeros(m,n);

for i=1:m
    for j=1:n
        if (fg(i,j)>T2(mod(i-1,2)+1,mod(j-1,2)+1))
            b2(i,j)=255;
        end
    end
end

for i=1:m
    for j=1:n
        if (fg(i,j)>T4(mod(i-1,4)+1,mod(j-1,4)+1))
            b4(i,j)=255;
        end
    end
end

for i=1:m
    for j=1:n
        if (fg(i,j)>T8(mod(i-1,8)+1,mod(j-1,8)+1))
            b8(i,j)=255;
        end
    end
end

imwrite(b2, 'house2.tif')
imwrite(b4, 'house4.tif')
imwrite(b8, 'house8.tif')

RMSE = 0;
for i = 1:m
    for j = 1:n
        RMSE = RMSE+ (img(i,j)-b2(i,j))^2;
    end
end
RMSE = sqrt(RMSE/(m*n))
fid = fidelity(img, b2)

RMSE = 0;
for i = 1:m
    for j = 1:n
        RMSE = RMSE+ (img(i,j)-b4(i,j))^2;
    end
end
RMSE = sqrt(RMSE/(m*n))
fid = fidelity(img, b4)


RMSE = 0;
for i = 1:m
    for j = 1:n
        RMSE = RMSE+ (img(i,j)-b8(i,j))^2;
    end
end
RMSE = sqrt(RMSE/(m*n))
fid = fidelity(img, b8)
