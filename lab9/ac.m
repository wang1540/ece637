clc
clear all
gamma = 1;
run('Qtables.m');

img = imread('img03y.tif');

% % write
img = double(img)-128;
fn = @(x) round(dct2(x.data,[8,8])./(Quant*gamma));
dct_blk = blockproc(img,[8,8],fn);
[m,n] = size(dct_blk);

acimg = zeros((m/8)*(n/8),63);
k=1;
for i=1:m/8
    for j=1:n/8
       block=dct_blk((i-1)*8+1:i*8,(j-1)*8+1:j*8);
       temp = block(Zig);
       acimg(k,:) = temp(2:64);
       k=k+1;
    end
end
acimg = abs(acimg);
acimg = sum(acimg)/((m/8)*(n/8));
% acimg = reshape(acimg', [1,(m/8)*(n/8)*63]);

plot([2:64],acimg)
title('Mean Absolute Value of AC Coefficient in Zig-Zag Order');
xlabel('Zig-Zag Order');
ylabel('Mean Absolute Value of AC Coefficient');
grid on;
