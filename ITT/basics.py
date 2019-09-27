# Smoothing images

import sys

import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('lenna.png')
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) /#matplot read in brg, rgb read opencv

kernel = np.ones((5, 5), np.float32)/25
dst = cv2.filter2D(img, -1, kernel) # - 1 - desired depth of image
blur = cv2.blur(img, (5, 5))
gblur = cv2.GaussianBlur(img, (5, 5), 0)
median = cv2.medianBlur(img, 5) # kernel size must be odd here except 1
bilateralFilter = cv2.bilateralFilter(img, 9, 75, 75)

titles = ['image', '2D Convolution', 'blur', 'GaussianBlur', 'median', 'bilateralFilter']
images = [img, dst, blur, gblur, median, bilateralFilter]

for i in range(6):
    plt.subplot(2, 3, i+1), plt.imshow(images[i], 'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])

plt.show()

'''
lineary filtres:
Homogenous, gaussian, median, bilateral
each output pixel is the mean of its kernel neighbours
each equil width
kernel - shape on image
numpy - matrix of ones * 1/25

LPF - helps in removing noises, blurring the images
HPF - helps in finding edges in the images

Gaussian filter - using different-weight-lernel in both x and y direction, pixel is center have heigher width, on edges lower
Median filter - replace each pixel's value with the median of its neighboring pixels, great method for salt and pepper noise
Bilatteral - edges are softer, higly effective in noise removel but keeping edges sharp
'''
