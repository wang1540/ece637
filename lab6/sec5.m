close all;
clear all;

[x, y] = meshgrid(0:0.005:1);
z = 1 - y - x;

RGB_709 = [0.64 0.3 0.15;0.33 0.6 0.06;0.03 0.1 0.79];
M_709 = RGB_709 * eye(3);
RGB_CIE_1931 = [0.73467 0.26533 0.0;0.27376 0.71741 0.00883;0.16658 0.00886 ...
0.82456;0.73467 0.26533 0.0];
RGB_709 = [0.64 0.33 0.03;0.3 0.6 0.1;0.15 0.06 0.79;0.64 0.33 0.03];
D_65 = [0.3127 0.3290 0.3583];
EE = [0.3333 0.3333 0.3333];

[m, n] = size(x);
XYZ = zeros(m,n,3);

XYZ(:,:,1) = x;
XYZ(:,:,2) = y;
XYZ(:,:,3) = z;

rgb = zeros(m,n,3);

for p = 1:m
	for q = 1:n
		rgb(p,q,:) = M_709 \ permute(XYZ(p,q,:), [1 3 2])';
		if any(rgb(p,q,:) < 0)
			rgb(p,q,:) = [1, 1, 1];
		end
	end
end

gc_rgb = zeros(m,n,3);
gc_rgb = ((rgb).^(1/2.2));

figure(1);
image([0:0.005:1], [0:0.005:1], gc_rgb);
axis('xy');
hold on;

data = load('data.mat');
x0 = data.x;
y0 = data.y;
z0 = data.z;

x1 = x0(:) ./ (x0(:) + y0(:) + z0(:));
y1 = y0(:) ./ (x0(:) + y0(:) + z0(:));
plot(x1, y1);
plot(RGB_CIE_1931(:,1), RGB_CIE_1931(:,2));
text(RGB_CIE_1931(:,1), RGB_CIE_1931(:,2), 'RGB_{CIE_ 1931}');
plot(RGB_709(:,1), RGB_709(:,2));
text(RGB_709(:,1), RGB_709(:,2), 'RGB_{709}');
plot(D_65(1), D_65(2),'b');
text(D_65(1), D_65(2), 'D_{65}');
plot(EE(1), EE(2), 'm.');
text(EE(1), EE(2), 'EE');
title('Chromaticity diagram');