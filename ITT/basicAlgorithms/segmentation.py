import numpy as np
import cv2
from matplotlib import pyplot as plt


img = cv2.imread("101_2.tif", 0) # uint8 image

ret, tresh_img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
cv2.imshow('Tresholded image', tresh_img)

# noise removal
kernel = np.ones((12,12), np.uint8)
opening = cv2.morphologyEx(tresh_img, cv2.MORPH_OPEN,kernel)
cv2.imshow('Opening', opening)
im2, contours, hierarchy = cv2.findContours(opening, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
#cv2.drawContours(opening, contours, -1, (0,255,0), 3)
cv2.imshow('Opening with contours', opening)
cv2.Canny(opening, 100, 200);
result = cv2.add(img, opening)
cv2.imshow('Img after noise removal', result)

cv2.waitKey(0)
cv2.destroyAllWindows()
