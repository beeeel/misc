%% Creating hidden images

% Folder used
Dir = '~/Documents/hidden_images/';
FileBase = input('Name your file ','s');
MaskFile = [Dir 'mask/' FileBase];

% Create blank map
ImSz = [20, 20];
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

Se = strel('square',3);
MaskPerim = imdilate(Mask, Se) - Mask;

Noise = randi(127, [ImSz 3])*2;

FGR = 55;
FGpx = (Mask~=0) .* (255 - FGR + randi(FGR,[ImSz 3]));
BGpx = (MaskPerim~=0) .* randi(FGR,[ImSz 3]);
Image = uint8(Noise .* ~imdilate(Mask, Se) + FGpx + BGpx);

figure(1)
subplot(3,1,1)
imagesc(Image,[1 255])
axis image
title('colour')
subplot(3,3,4)
imagesc(Image(:,:,1),[1 255])
axis image, colormap gray
title('Red light')
subplot(3,3,5)
imagesc(Image(:,:,2),[1 255])
axis image, colormap gray
title('Green light')
subplot(3,3,6)
imagesc(Image(:,:,3),[1 255])
axis image, colormap gray
title('Blue light')
subplot(3,3,7)
imagesc(sum(Image(:,:,[1 2]),3)/2, [1 255])
axis image, colormap gray
title('Yellow light')
subplot(3,3,8)
imagesc(sum(Image(:,:,[1 3]),3)/2, [1 255])
axis image, colormap gray
title('Magenta light')
subplot(3,3,9)
imagesc(sum(Image(:,:,[2 3]),3)/2, [1 255])
axis image, colormap gray
title('Cyan light')