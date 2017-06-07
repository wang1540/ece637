clear all;
close all;

I22 = [1 2;3 0];
I44 = [4*I22 + 1, 4*I22 + 2; 4*I22 + 3, 4*I22];
I88 = [4*I44 + 1, 4*I44 + 2; 4*I44 + 3, 4*I44];

fg = imread('house.tif');
fl = 255 * (double(fg) / 255).^(2.2);
[m, n] = size(fg);

N2 = 2;
N4 = 4;
N8 = 8;

T2 = 255 * ((I22 + 0.5) / (N2^2));
T4 = 255 * ((I44 + 0.5) / (N4^2));
T8 = 255 * ((I88 + 0.5) / (N8^2));

fl_2 = zeros(m,n);
fl_4 = zeros(m,n);
fl_8 = zeros(m,n);

for p = 1:m
	for q = 1:n
		r = mod(p-1,N2) + 1;
		s = mod(q-1,N2) + 1;
		if fl(p,q) > T2(r,s)
			fl_2(p,q) = 255;
		else
			fl_2(p,q) = 0;
		end
	end
end
for p = 1:m
	for q = 1:n
		r = mod(p-1,N4) + 1;
		s = mod(q-1,N4) + 1;
		if fl(p,q) > T4(r,s)
			fl_4(p,q) = 255;
		else
			fl_4(p,q) = 0;
		end
	end
end
for p = 1:m
	for q = 1:n
		r = mod(p-1,N8) + 1;
		s = mod(q-1,N8) + 1;
		if fl(p,q) > T8(r,s)
			fl_8(p,q) = 255;
		else
			fl_8(p,q) = 0;
		end
	end
end

imwrite(fl_2, 'house_d2.tif');
imwrite(fl_4, 'house_d4.tif');
imwrite(fl_8, 'house_d8.tif');