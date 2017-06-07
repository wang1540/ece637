function [] = BetterSpecAnal(img)

X = double(img);

% Select an NxN region of the image and store it in the variable "z"
N = 64;

% Multiply each 64 x 64 window by a 2-D separable Hamming window
W = hamming(N)*hamming(N)';

% Break image into K^2 regions each with N pixels
K = 5;
[Xlen, Xwid] = size(X); %512; 768

% center point of X is 
len = Xlen/2;
wid = Xwid/2;

% start point (top left corner)
slen = len - K/2*N;
swid = wid - K/2*N;

Z = zeros(N, N);

for k1 = 1:1:K
    for k2 = 1:1:K
        Xtemp = X(((slen+N*(k1-1)):1:(slen+N*k1-1)), ((swid+N*(k2-1)):1:(swid+N*k2-1)));
        Z = Z + (abs(fft2(Xtemp.*W)).^2) / ((K*N)^2);
    end
end

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
title('better power spectral density plot for block size of 64 x 64')



