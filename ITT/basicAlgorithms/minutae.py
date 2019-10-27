import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

img = cv2.imread("thinned_img.tif",0)  # uint8 image
ret, tresh_img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
cv2.imshow("Inverse treshold", tresh_img)
rows = np.size(tresh_img, 0)
cols = np.size(tresh_img, 1)
a=0
b=0
c=0
d=0
e=0
f=0
g=0
h=0
i=0
ridge = 0
ridcheck = 0
bif = 0
bifcheck =0

for x in range(0,rows-1):
    for y in range(0, cols-1):
        pix1 = tresh_img[x-1][y-1]
        pix2 = tresh_img[x][y-1]
        pix3 = tresh_img[x+1][y-1]
        pix4 = tresh_img[x-1][y]
        pix5 = tresh_img[x][y]
        pix6 = tresh_img[x+1][y]
        pix7 = tresh_img[x-1][y+1]
        print(pix7)
        pix8 = tresh_img[x][y+1]
        pix9 = tresh_img[x+1][y+1]


cv2.waitKey(0)
cv2.destroyAllWindows()
