
% Clear memory and close existing figures
clear
close

% Generate a 512 x 512 image, x, with independent random numbers
% each uniformly distributed on the interval [-0.5, 0.5]
x = rand(512) - 0.5; 
x_scaled = 255*(x+0.5);

imwrite(uint8(x_scaled),'random.tif');
y = zeros(513);
for m=1:1:512
    for n=1:1:512
        y(m+1,n+1) = 3*x(m,n)+0.99*y(m,n+1)+0.99*y(m+1,n)-0.9801*y(m,n);
    end
end
y = y(2:1:513,2:1:513) + 127;
imwrite(uint8(y),'randomy127.tif');


BetterSpecAnal(y);


N = 512;

% Compute the power spectrum for the NxN region
Z = (1/N^2)*abs(fft2(y)).^2;

% Use fftshift to move the zero frequencies to the center of the plot
Z = fftshift(Z);

% Compute the logarithm of the Power Spectrum.
Zabs = log( Z );

% Plot the result using a 3-D mesh plot and label the x and y axises properly. 
x = 2*pi*((0:(N-1)) - N/2)/N;
y = 2*pi*((0:(N-1)) - N/2)/N;
figure 
mesh(x,y,Zabs)
xlabel('\mu axis')
ylabel('\nu axis')
title('theoretically power spectral density for random image of block size 512 x 512')


