% BY   SCOTT                                                                                                                                              % red2blue
% change red to blue
clear all;
clc;
rgb = imread('D:\Python-project\UNet_erwo\image\test\data\0.png');
figure;
imshow(rgb);
R=rgb(:,:,1); %red
G=rgb(:,:,2); %green
B=rgb(:,:,3); %blue
[x,y,z]=size(rgb);
for i=1:x
    for j=1:y
        if( (R(i,j) >= 180) && (R(i,j) <=255) && (G(i,j) <50) && (B(i,j) <50) )
            R(i,j) = 0;
            G(i,j) = 162;
            B(i,j) = 232;
        end
    end
end
 
% in this way
% blue(:,:,1)=R(:,:);
% blue(:,:,2)=G(:,:);
% blue(:,:,3)=B(:,:);
 
% another way
for i=1:x
    for j=1:y
        blue(i,j,1) = R(i,j);
        blue(i,j,2) = G(i,j);
        blue(i,j,3) = B(i,j);
    end
end
 
figure;
imshow(blue);