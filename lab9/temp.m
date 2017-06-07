
img = imread('img03y.tif');
gamma = 4;

	run('Qtables.m');
	img = double(img) - 128;
    
    fn = @(x) round(dct2(x.data, [8 8])./(Quant*gamma));
	dct_blk = blockproc(img, [8 8], fn);

	f = fopen('img03y.dq', 'w');
    [m,n] = size(dct_blk);
	fwrite(f, m, 'integer*2');
	fwrite(f, n, 'integer*2');
	fwrite(f, dct_blk', 'integer*2');
    


	f = fopen('img03y.dq', 'r');
	data = fread(f, 'integer*2');
	imgRes = reshape(data(3:end), [data(2) data(1)])';
	imgRes = blockproc(imgRes, [8 8], ...
		@(x) round(idct2(x.data.*Quant*gamma, [8 8])));
	imgRes = imgRes + 128;
	imgRes = uint8(imgRes);

    
    img = uint8(img + 128);
    imgDiff = 10 * (img - imgRes) + 128;
%     
%     
%     figure;
% 	image(img);
% 	truesize;
% 	colormap(gray(256));
% 
% 	figure;
% 	image(imgRes);
% 	truesize;
% 	colormap(gray(256));
    
    figure;
	image(imgDiff);
	truesize;
	colormap(gray(256));