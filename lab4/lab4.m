clc
clear

% section 1
% x=imread('kids.tif');
% hist(x(:),[0:255])
% xlabel('pixel intensity')
% ylabel('number of pixels')
% title('Histogram of kids.tif')


%section 2
% X=imread('kids.tif');
% Y = equalize(X);


%section 3
% X=imread('kids.tif');
% T1 = 70;
% T2 = 180;
% output = stretch(X, T1, T2);

%section 4
x = imread('gamma15.tif');
corLinearImg = 255*(double(x)/255).^(double(1.5/2.5557));
figure(1)
image(uint8(corLinearImg)+1);
axis('image');
graymap = [0:255;0:255;0:255]'/255;
colormap(graymap);
% print('-dpng', '-r300', 'gamma15_gamma.png');
% gray_level = 190;
% pattern=[255,255,0,0;255,255,0,0;0,0,255,255;0,0,255,255];%create the checkboard pattern
% single_line=zeros(16,256); %create a single dot line
% 
% for j=1:4:256
%     for i=1:4:16
%         single_line(i:i+3,j:j+3)=pattern;
%     end
% end
% % single_line=double(single_line/255);
% % colormap(gray(256))
% % imshow(single_line)
% 
% gray_line=ones(16,256)*gray_level;
% img=zeros(256,256);
% 
% for i=1:32:240
%     img(i:i+15,:)=single_line;
% end
% 
% for i=17:32:256
%     img(i:i+15,:)=gray_line;
% end
% 
% figure(1)
% image(img+1);
% axis('image');
% graymap = [0:255; 0:255; 0:255]'/255;
% colormap(graymap);








