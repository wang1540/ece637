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

dcimg = zeros(m/8,n/8);
for i=1:m/8
    for j=1:n/8
        dcimg(i,j)=dct_blk((i-1)*8+1,(j-1)*8+1)+128;
    end
end


figure;
image(uint8(dcimg));
truesize;
colormap(gray(256));



