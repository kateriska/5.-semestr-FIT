import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

orig_img = cv2.imread("segmented_img.tif")  # uint8 image
img = np.float32(orig_img)
shape_img = img.shape

kernel1 = cv2.getGaborKernel((9,9), 5, 0, 8, 0.02, 0, cv2.CV_32F);
kernel2 = cv2.getGaborKernel((9,9), 5, 45, 8, 0.02, 0, cv2.CV_32F);
kernel3 = cv2.getGaborKernel((9,9), 5, 90, 8, 0.02, 0, cv2.CV_32F);
kernel4 = cv2.getGaborKernel((9,9), 5, 135, 8, 0.02, 0, cv2.CV_32F);
gabor1 = cv2.filter2D(img, -1, kernel1);
gabor2 = cv2.filter2D(img, -1, kernel2);
gabor3 = cv2.filter2D(img, -1, kernel3);
gabor4 = cv2.filter2D(img, -1, kernel4);
cv2.imshow('gabor1', gabor1)
cv2.imshow('gabor2', gabor2)
cv2.imshow('gabor3', gabor3)
cv2.imshow('gabor4', gabor4)
adding1 = cv2.add(gabor1, gabor2)
adding2 = cv2.add(adding1, gabor3)
adding3 = cv2.add(adding2, gabor4)
#cv2.imshow('sym', adding1)
cv2.imshow('final', adding3)
cv2.imwrite('gabor.tif', adding3)
cv2.waitKey(0)
cv2.destroyAllWindows()
