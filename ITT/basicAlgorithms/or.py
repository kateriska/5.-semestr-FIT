import numpy as np
import cv2
import math

img = cv2.imread("104_1.tif")  # uint8 image
print(img.dtype)
img = np.int16(img)
print(img.dtype)

rows = np.size(img, 0)
cols = np.size(img, 1)
print(rows)
print(cols)

shape_img = img.shape

grad_x = np.zeros(shape_img, dtype=np.int16)
grad_y = np.zeros(shape_img, dtype=np.int16)
Vx = np.zeros(shape_img, dtype=np.int16)
Vy = np.zeros(shape_img, dtype=np.int16)
theta = np.zeros(shape_img, dtype=np.int16)

grad_x = cv2.Sobel(img,cv2.CV_16SC1,1, 0)
grad_y = cv2.Sobel(img,cv2.CV_16SC1,0, 1)

block_div = 8
step = 16

for i in range(block_div, rows-block_div, step):
    for j in range(block_div, cols-block_div, step):
        sum_Vx = 0.0
        sum_Vy = 0.0
        for u in range(i-block_div, i+block_div):
            for v in range(j-block_div, j+block_div):
                #print(grad_x[u][v])
                grad_x_value_np = grad_x[u][v]
                grad_x_value_str = np.array2string(grad_x_value_np)
                grad_x_value_str = grad_x_value_str.split(" ", 1)[0]
                grad_x_value_str = grad_x_value_str[1:]
                grad_x_value = int (grad_x_value_str)
                #print(grad_x_value_str)
                grad_y_value_np = grad_y[u][v]
                grad_y_value_str = np.array2string(grad_y_value_np)
                grad_y_value_str = grad_y_value_str.split(" ", 1)[0]
                grad_y_value_str = grad_y_value_str[1:]
                grad_y_value = int (grad_y_value_str)
                #print(grad_y_value_str)
                sum_Vx = sum_Vx + (2 * grad_x_value * grad_y_value)
                sum_Vy = sum_Vy + (grad_x_value**2 * grad_y_value**2)

        #print(sum_Vx)

        theta_calculation = 0.0

        if (sum_Vx != 0 ):
            theta_calculation = 0.5 * math.atan(sum_Vy / sum_Vx)
            #print(theta_calculation)
        else:
            theta_calculation = 0.0

        print(theta_calculation)
