clc;
clear all;
close all;

img = imread('house.tif');
[m, n] = size(img);
T = 127;
img = double(img);

f = 255 * (img / 255).^2.2;
e = zeros(m,n);
b = zeros(m,n);

for i=1:m-1
    for j=1:n
        b(i,j) = (f(i,j)>127)*255;
        e = f(i,j) - b(i,j);
        if(j<n)
            f(i,j+1) = f(i,j+1) + e*7/16;
    		f(i+1,j+1) = f(i+1,j+1) + e*1/16;
        end
        if(j>1)
            f(i+1,j-1) = f(i+1,j-1) + e*3/16;
        end
		f(i+1,j) = f(i+1,j) + e*5/16;
    end
end

colormap(gray(256));
image(b);
truesize
imwrite(b,'Error Diffusion.tiff')

RMSE = 0;
for i = 1:m
    for j = 1:n
        RMSE = RMSE+ (img(i,j)-b(i,j))^2;
    end
end
RMSE = sqrt(RMSE/(m*n))

fid = fidelity(img, b)



