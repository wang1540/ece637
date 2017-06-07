% 
% read_data.m
%
% ECE637
% Prof. Charles A. Bouman
% Image Processing Laboratory: Eigenimages and Principal Component Analysis
%
% Description:
%
% This is a Matlab script that reads in a set of training images into
% the Matlab workspace.  The images are sets of English letters written
% in various fonts.  Each image is reshaped and placed into a column
% of a data matrix, "X".
% 

% The following are strings used to assemble the data file names
datadir='/Users/xihui/Downloads/lab5/training_data';    % directory where the data files reside
dataset={'arial','bookman_old_style','century','comic_sans_ms','courier_new',...
  'fixed_sys','georgia','microsoft_sans_serif','palatino_linotype',...
  'shruti','tahoma','times_new_roman'};
datachar='abcdefghijklmnopqrstuvwxyz';

Rows=64;    % all images are 64x64
Cols=64;
n=length(dataset)*length(datachar);  % total number of images
p=Rows*Cols;   % number of pixels

X=zeros(p,n);  % images arranged in columns of X
k=1;
for dset=dataset
for ch=datachar
  fname=sprintf('%s/%s/%s.tif',datadir,char(dset),ch);
  img=imread(fname);
  X(:,k)=reshape(img,1,Rows*Cols);
  k=k+1;
end
end

% return

% display samples of the training data
% for k=1:length(dataset)
%   img=reshape(X(:,26*(k-1)+1),64,64);
%   figure(20); subplot(3,4,k); image(img); 
%   axis('image'); colormap(gray(256)); 
%   title(dataset{k},'Interpreter','none');
% end



% % % % % % % % %  part 4

% [p,n]=size(X);
% mu=1/n*sum(X,2);
% Z=(X-repmat(mu,1,n))/(sqrt(n-1));
% [U,S,V]=svd(Z,0);
% 
% % first 12 eigenimages
% Um=U(:,1:12);
% figure(1)
% for k=1:12
%   img=reshape(U(:,k),64,64);
%   figure(1); subplot(4,3,k); imagesc(img);
%   axis('image'); colormap(gray(256));
%   title(strcat('eigenvalue',num2str(k)),'Interpreter','none');
% end
% 
% 
% Y = U'*(bsxfun(@minus,X,mu));
% figure(2);
% for k = 1:4
% 	plot(1:10,Y(1:10,k));
% 	hold on;
% end
% legend('a','b','c','d');
% xlabel('Eigenvector Number');
% ylabel('Projection Coefficients');


% % % % % % % % %  part 5





[m n] = size(X);
mean = (1/n)*sum(X,2);
mean_mat = repmat(mean,1,n);
Z=(X-mean_mat)/(sqrt(n-1));
[U,S,V]=svd(Z,0);

Un=U(:,1:10);
Y = Un'*(X-mean_mat);

empty_cell=cell(26,2);
params=cell2struct(empty_cell,{'M','R'},2);

for k=1:26
    params(k).M=Y(:,k);
    for i=1:11
        params(k).M=Y(:,i*26+k)+params(k).M;
    end
params(k).M=params(k).M/12;
end

for k=1:26
    params(k).R=(Y(:,k)-params(k).M)*(Y(:,k)-params(k).M)';
    for i=1:11
        params(k).R=(Y(:,i*26+k)-params(k).M)*(Y(:,i*26+k)-params(k).M)'+params(k).R;
    end
    %params(k).R=params(k).R/11;
   
    %params(k).R=diag(diag(params(k).R/11));      %diag(X) is the main diagonal of X. diag(diag(X)) is a diagonal matrix.
    params(k).R=eye(10);          
end

