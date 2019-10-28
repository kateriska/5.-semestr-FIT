import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

img = cv2.imread("thinned_gabor.tif",0)  # uint8 image

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
        print(pix5)
        pix8 = tresh_img[x][y+1]
        pix9 = tresh_img[x+1][y+1]

        if(pix1 == 255 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
        if(pix1==0 and pix2==255 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
        if (pix1==0 and pix2==0 and pix3==255 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
        if (pix1==0 and pix2==0 and pix3==0 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
        if (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
        if (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==255 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
        if (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==255 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
        if (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==255):
            ridge = ridge + 1
            ridcheck = ridcheck + 1
##################################################################################################################################
        if (pix1==255 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==255 and pix8==0 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        if (pix1==0 and pix2==255 and pix3==0 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==255):
            bif = bif + 1
            bifcheck = bifcheck + 1
        if (pix1==255 and pix2==0 and pix3==255 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==255 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        if (pix1==0 and pix2==255 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==255 and pix8==0 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        if (pix1==0 and pix2==0 and pix3==255 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==255):
            bif = bif + 1
            bifcheck = bifcheck + 1
        if (pix1==255 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==0 and pix8==255 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        if (pix1==0 and pix2==255 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==255 and pix8==0 and pix9==255):
            bif = bif + 1
            bifcheck = bifcheck + 1
        if (pix1==0 and pix2==0 and pix3==255 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==255 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1

        if (ridge > 0):
            cv2.circle(tresh_img, (x,y), 4, (255,0,0), 2)
            ridge = 0
        #if (bif > 0):
            #cv2.rectangle(tresh_img, (x-2,y-2), (x+2,y+2), (255,0,0),2)
            #bif = 0

        pix1 = 0
        pix2 = 0
        pix3 = 0
        pix4 = 0
        pix5 = 0
        pix6 = 0
        pix7 = 0
        pix8 = 0
        pix9 = 0


print(ridcheck)
print(bifcheck)
cv2.imshow("Ridges", tresh_img);
cv2.waitKey(0)
cv2.destroyAllWindows()
