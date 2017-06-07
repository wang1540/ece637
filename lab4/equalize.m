function [Y] = equalize(X)

counts = hist(X(:),[0:255]);
total = sum(counts);

Y = counts; % 0 - 255
for (i=1:255)
    Y(i+1) = Y(i) + Y(i+1);
end
Y = Y./total;
Ymin = Y(min(X(:)));
Ymax = Y(max(X(:)));

Z = 255*((Y(X) - Ymin)/(Ymax - Ymin));

figure(1)
plot([0:255],Y);
xlabel('pixel intensity')
ylabel('Cumulative Distribution')
title('cumulative distribution function of kids.tif')
grid on
xlim([0, 255])


figure(2);
image(Z+1);
axis('image');
graymap = [0:255;0:255;0:255]'/255;
colormap(graymap);


figure(3)
hist(Z(:),[0:255])
xlabel('pixel intensity')
ylabel('number of pixels')
title('Histogram of equalized kids.tif')
xlim([0 255])

end




