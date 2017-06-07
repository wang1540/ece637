function [fid] = fidelity(f, b)

fid = 0;
[m,n] = size(f);

fl = 255*(f/255).^(2.2);
bl = 255*(b/255).^(2.2);

sigma2 = 2;
h = zeros(7,7);
for i=1:7
    for j=1:7
        h(i,j)=exp(-((i-4)^2+(j-4)^2)/(2*sigma2));
    end
end
h = h/(sum(sum(h)));

flp = conv2(fl, h, 'same');
blp = conv2(b, h, 'same');

ff = 255*(flp/255).^(1/3);
bb = 255*(blp/255).^(1/3);

for i = 1:m
    for j = 1:n
        fid = fid+ (ff(i,j)-bb(i,j))^2;
    end
end
fid = sqrt(fid/(m*n));