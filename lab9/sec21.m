gamma = 4;

% write
img = imread('img03y.tif');
img = double(img)-128;
run('Qtables.m');

fn = @(x) round(dct2(x.data,[8,8])./(Quant*gamma));
dct_blk = blockproc(img,[8,8],fn);

[m,n] = size(dct_blk);
id = fopen('img03y.dq','w');
fwrite(id, m, 'integer*2');
fwrite(id, n, 'integer*2');
fwrite(id, dct_blk', 'integer*2');
fclose(id);



% read
id = fopen('img03y.dq', 'r');
dct_blkk = fread(id, 'integer*2');
fclose(id);
imgg = reshape(dct_blkk(3:end), [dct_blkk(2) dct_blkk(1)])';

fn =  @(x) round(idct2(x.data.*Quant*gamma, [8 8]));
imgg = blockproc(imgg, [8 8],fn);
imgg = imgg + 128;
imgg = uint8(imgg);



% image
img = uint8(img + 128);
imgDiff = 10 * (img - imgg) + 128;
% figure;
% image(uint8(img));
% truesize;
% colormap(gray(256));
% 
% figure;
% image(uint8(imgg));
% truesize;
% colormap(gray(256));
% % imwrite(uint8(imgRes), sprintf('img03y_%d.tif', i));

figure;
image(uint8(imgDiff));
truesize;
colormap(gray(256));
% imwrite(uint8(imgDiff), sprintf('img03y_diff_%d.tif', i));
