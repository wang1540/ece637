clc;
clear;


data = load('data.mat');
reflect =  load('reflect.mat');
R = reflect.R;
illu1 = data.illum1;
illu2 = data.illum2;
wavelength = [400:10:700];
x0 = data.x;
y0 = data.y;
z0 = data.z;
mat_x = zeros(3,31);
mat_x(1,:) = x0;
mat_x(2,:) = y0;
mat_x(3,:) = z0;

[m n d] = size(R);
XYZ = zeros(m,n,3);

for i = 1:1:31
    I(:,:,i) = R(:,:,i)*illu1(i);
%     I(:,:,i) = R(:,:,i)*illu2(i);
end

for i = 1:1:m
    for j = 1:1:n
        q = reshape(I(i,j,:),31,1);
        XYZ(i,j,:) = mat_x * q;
    end
end

D65_white = [0.3127, 0.3290, 0.3583];
D65_white = D65_white./D65_white(2);
M1=[0.640, 0.330, 0.030;0.300, 0.600, 0.100;0.150, 0.060, 0.790];%R709 primaries

k =M1'\D65_white';
M=M1'*diag(k);

RGB = zeros(m,n,3);
for i = 1:1:m
    for j = 1:1:n
        p = reshape(XYZ(i,j,:),3,1);
        RGB(i,j,:) = M\p;
        
        for k=1:3
            if RGB(i,j,k)<0
                RGB(i,j,k)=0;
            elseif  RGB(i,j,k)>1.0
                RGB(i,j,k)=1;
            else
                continue;
            end
        end
    end
end

gam = 2.2;
RGB=RGB.^(1/gam);
image(RGB);