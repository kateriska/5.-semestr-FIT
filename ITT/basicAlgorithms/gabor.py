import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

orig_img = cv2.imread("104_1.tif")  # uint8 image
img = np.float32(orig_img)
shape_img = img.shape

kernel_size = (12,12)
sigma = 6
lambda_v = 10
gamma = 0.05
psi = 0

theta1 = 0
theta2 = 45
theta3 = 90
theta4 = 135

enhanced = np.zeros(shape_img, dtype=np.float32)

kernel1 = cv2.getGaborKernel(kernel_size, sigma, theta1, lambda_v, gamma, psi, cv2.CV_32F);
kernel2 = cv2.getGaborKernel(kernel_size, sigma, theta2, lambda_v, gamma, psi, cv2.CV_32F);
kernel3 = cv2.getGaborKernel(kernel_size, sigma, theta3, lambda_v, gamma, psi, cv2.CV_32F);
kernel4 = cv2.getGaborKernel(kernel_size, sigma, theta4, lambda_v, gamma, psi, cv2.CV_32F);
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
cv2.imshow('sym', adding1)
cv2.imshow('sym2', adding2)
cv2.imshow('final', adding3)
cv2.imwrite('gabor.tif', adding2)

cv2.addWeighted(enhanced , 0, gabor1, 1, 0, enhanced );
cv2.addWeighted(enhanced , 1, gabor2, 1, 0, enhanced );
cv2.addWeighted(enhanced , 1, gabor3, 1, 0, enhanced );
cv2.addWeighted(enhanced , 1, gabor4, 1, 0, enhanced );

#gblur = cv2.GaussianBlur(adding3, (5, 5), 0)
#cv2.imshow('blur', gblur)
#cv2.imwrite('blur.tif', gblur)
#gblur_img = cv2.imread("blur.tif",0)  # uint8 image
#ret, result = cv2.threshold(gblur_img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU) # transform to binary image
cv2.imshow('result', enhanced)
cv2.waitKey(0)
cv2.destroyAllWindows()
