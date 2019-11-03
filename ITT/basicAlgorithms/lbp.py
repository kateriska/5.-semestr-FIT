import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

orig_img = cv2.imread("thinned_img.tif",0)  # uint8 image
ret, tresh_img = cv2.threshold(orig_img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
cv2.imshow("Inverse treshold", tresh_img)
cv2.imwrite("tolbp.tif", tresh_img)

rows = np.size(tresh_img, 0)
cols = np.size(tresh_img, 1)

step = 8
'''
pix1 = -1
pix2 = -1
pix3 = -1
pix4 = -1
pix5 = -1
pix6 = -1
pix7 = -1
pix8 = -1
pix9 = -1
'''
for x in range(0,rows-1):
    for y in range(0, cols-1):

        pix1 = tresh_img[x-1][y-1]
        pix2 = tresh_img[x][y-1]
        pix3 = tresh_img[x+1][y-1]
        pix4 = tresh_img[x-1][y]
        pix5 = tresh_img[x][y]
        pix6 = tresh_img[x+1][y]
        pix7 = tresh_img[x-1][y+1]
        #print(pix5)
        pix8 = tresh_img[x][y+1]
        pix9 = tresh_img[x+1][y+1]

        #if (pix1 == -1 or pix2 == -1 or pix3 == -1 or pix4 == -1 or pix5 == -1 or pix6 == -1 or pix7 == -1 or  pix8 == -1 or pix9 == -1):
            #continue

        if (pix5 > pix1):
            pix1 = 1
        else:
            pix1 = 0

        if (pix5 > pix2):
            pix2 = 1
        else:
            pix2 = 0

        if (pix5 > pix3):
            pix3 = 1
        else:
            pix3 = 0

        if (pix5 > pix4):
            pix4 = 1
        else:
            pix4 = 0

        if (pix5 > pix6):
            pix6 = 1
        else:
            pix6 = 0

        if (pix5 > pix7):
            pix7 = 1
        else:
            pix7 = 0

        if (pix5 > pix8):
            pix8 = 1
        else:
            pix8 = 0

        if (pix5 > pix9):
            pix9 = 1
        else:
            pix9 = 0

        #print(str(pix9)+str(pix6)+str(pix3)+str(pix2)+str(pix1)+str(pix4)+str(pix7)+str(pix8))
        bin_value = str(pix9)+str(pix6)+str(pix3)+str(pix2)+str(pix1)+str(pix4)+str(pix7)+str(pix8)
        #bin_value = int (bin_value)
        dec_value = (pix9*(2^7)) + (pix6*(2^6)) + (pix3*(2^5)) + (pix2*(2^4)) + (pix1*(2^3)) + (pix4*(2^2)) + (pix7*(2^1)) + (pix8*(2^0))
        print(dec_value)
        tresh_img[x][y] = dec_value



cv2.waitKey(0)
cv2.destroyAllWindows()
