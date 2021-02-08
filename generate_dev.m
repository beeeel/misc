%% Creating hidden images

% Folder used
Dir = '~/Documents/hidden_images/';
% FileBase = input('Name your file ','s');
FileBase = 'gray_';
MaskFile = [Dir 'mask/' FileBase];

% Create blank map
ImSz = [300, 300];
Mask = zeros([ImSz, 3]);

imwrite(Mask(:,:,1), [MaskFile 'R.png'],'png')
imwrite(Mask(:,:,2), [MaskFile 'G.png'],'png')
imwrite(Mask(:,:,3), [MaskFile 'B.png'],'png')

% Wait for user input
input('Press enter when ready','s');
%%
Mask(:,:,1) = imread([MaskFile 'R.png'],'png');
Mask(:,:,2) = imread([MaskFile 'G.png'],'png');
Mask(:,:,3) = imread([MaskFile 'B.png'],'png');

fgSF = cat(3,0.75,0.75,0.85);
bgSF = cat(3,0.25,0.25,0.15);

Noise = zeros(ImSz);%randi(255, [ImSz 3]).*bgSF;

FGpx = (Mask .* fgSF + Noise);
Image = uint8(Noise + FGpx);

plotCMY = false;

n_plots = 2 + plotCMY;

figure(1)
subplot(n_plots,1,1)
imagesc(Image,[1 255])
axis image
title('colour')
subplot(n_plots,3,4)
imagesc(Image(:,:,1),[1 255])
axis image, colormap gray
title('Red light')
subplot(n_plots,3,5)
imagesc(Image(:,:,2),[1 255])
axis image, colormap gray
title('Green light')
subplot(n_plots,3,6)
imagesc(Image(:,:,3),[1 255])
axis image, colormap gray
title('Blue light')

if plotCMY
    subplot(n_plots,3,7)
    imagesc(sum(Image(:,:,[1 2]),3)/2, [1 255])
    axis image, colormap gray
    title('Yellow light')
    subplot(n_plots,3,8)
    imagesc(sum(Image(:,:,[1 3]),3)/2, [1 255])
    axis image, colormap gray
    title('Magenta light')
    subplot(n_plots,3,9)
    imagesc(sum(Image(:,:,[2 3]),3)/2, [1 255])
    axis image, colormap gray
    title('Cyan light')
end