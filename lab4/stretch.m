function output = stretch(input, T1, T2)
[m,n] = size(input);
output = zeros(m,n);

for (i=1:m)
    for (j=1:n)
        if (input(i,j) < T1)
            output(i,j) = uint8 (0);            
        elseif (input(i,j) > T2)
            output(i,j) = uint8 (255);
        else
            output(i,j) = uint8 (255 / (T2-T1) * (input(i,j)-T1));
        end
    end
end

figure(1);
image(output+1);
axis('image');
graymap = [0:255;0:255;0:255]'/255;
colormap(graymap);


figure(2)
hist(output(:),[0:255])
xlabel('pixel intensity')
ylabel('number of pixels')
title('Histogram of stretched kids.tif')
xlim([0 255])
end
