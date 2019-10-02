import numpy as np
import cv2
#from matplotlib import pyplot as plt


img = cv2.imread("nwmPa.png", 0)  # uint8 image
height_orig = np.size(img, 0)
width_orig = np.size(img, 1)
print(height_orig)
print(width_orig)

#m = np.zeros(img.shape, dtype=np.uint8)

#norm_image = cv2.normalize(image, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
norm_image = cv2.normalize(img,None,0,255,cv2.NORM_MINMAX)
cv2.imshow('NormImage', norm_image)

height = np.size(norm_image, 0)
width = np.size(norm_image, 1)
print(height)
print(width)

# compute gradients at each pixel
sobelx = cv2.Sobel(img,cv2.CV_64F,1,0,ksize=5)
sobely = cv2.Sobel(img,cv2.CV_64F,0,1,ksize=5)

print(sobelx)
print(sobely)

step = 16
x = 16
y = 16

start_x = 0
start_y = 0

while x < width:
    cv2.line(norm_image, (x, 0), (x, height), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
    x += step

while y < height:
    cv2.line(norm_image, (0, y), (width, y), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
    y += step

cv2.imshow('Norm Image With Grid', norm_image)
'''
for (start_x, width, step):
    for (start_y, height, step):
'''








cv2.waitKey(0)
cv2.destroyAllWindows()
