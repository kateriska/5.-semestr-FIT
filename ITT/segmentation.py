# Python program to transform an image using
# threshold.
import numpy as np
import cv2
from matplotlib import pyplot as plt

# Image operation using thresholding
img = cv2.imread('images.jpeg')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

ret, thresh = cv2.threshold(gray, 0, 255,
                            cv2.THRESH_BINARY +
                            cv2.THRESH_OTSU)
cv2.imshow('image', thresh)
'''

fg = cv2.erode(thresh,None,iterations = 2)
cv2.imshow('image', fg) # foreground
'''



# noise removal
kernel = np.ones((3,3),np.uint8)
opening = cv2.morphologyEx(thresh,cv2.MORPH_OPEN,kernel, iterations = 2)
# sure background area
sure_bg = cv2.dilate(opening,kernel,iterations=3)
# Finding sure foreground area
dist_transform = cv2.distanceTransform(opening,cv2.DIST_L2,5)
ret, sure_fg = cv2.threshold(dist_transform,0.7*dist_transform.max(),255,0)
# Finding unknown region
sure_fg = np.uint8(sure_fg)
unknown = cv2.subtract(sure_bg,sure_bg)
cv2.imshow('image', sure_bg) # foreground

#adding_img = cv2.bitwise_and(img, sure_bg)

#cv2.imshow('RESULT', adding_img)
result = cv2.add(gray, sure_bg)
cv2.imshow('RESULT', result)
cv2.waitKey(0)
cv2.destroyAllWindows()
