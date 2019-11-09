import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

scale = 1
delta = 0
ddepth = cv2.CV_16S
img = cv2.imread("fake.png")

cv2.GaussianBlur( img, (3,3), 0, 0, cv2.BORDER_DEFAULT )
cv2.imshow('Result', img)
img_gray = cv2.cvtColor( img, cv2.COLOR_BGR2GRAY )
cv2.imshow('g', img_gray)
img_gray = np.float32(img_gray)
shape_img = img_gray.shape
print(shape_img)
grad_x = np.zeros(shape_img, dtype=np.float32)
grad_y = np.zeros(shape_img, dtype=np.float32)
grad_x = cv2.Sobel(img_gray,cv2.CV_32FC1,1, 0)
grad_y = cv2.Sobel(img_gray,cv2.CV_32FC1,0, 1)

grad = cv2.addWeighted( grad_x, 0.5, grad_y, 0.5, 0)
cv2.imwrite('sobe.png', grad)
cv2.imshow("sobel filter", grad)

cv2.waitKey(0)
cv2.destroyAllWindows()
