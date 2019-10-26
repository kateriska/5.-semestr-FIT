import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

orig_img = cv2.imread("thinned_img.tif")  # uint8 image
white = cv2.imread("white.jpg")
white = cv2.resize(white,(388,374))
print(orig_img.dtype)
img = np.float32(orig_img)
shape_img = orig_img.shape
print(img.dtype)

rows = np.size(img, 0)
cols = np.size(img, 1)
grad_x = cv2.Sobel(img,cv2.CV_32FC1,1, 0)
grad_y = cv2.Sobel(img,cv2.CV_32FC1,0, 1)
orientatin_matrix = np.zeros(shape_img, dtype=np.float32)
right_angle  = math.pi / 2

for i in range(8, rows-8, 16):
    for j in range(8, cols-8, 16):
        sum_Vx = 0.0
        sum_Vy = 0.0
        counter = 0
        for u in range(i-8, i+8):
            for v in range(j-8, j+8):
                grad_x_value_np = grad_x[u][v]
                grad_x_value_str = np.array2string(grad_x_value_np)
                grad_x_value_str = grad_x_value_str.split(".", 1)[0]
                grad_x_value_str = grad_x_value_str[1:]
                grad_x_value_str = grad_x_value_str.strip();
                grad_x_value = int (grad_x_value_str)

                grad_y_value_np = grad_y[u][v]
                grad_y_value_str = np.array2string(grad_y_value_np)
                grad_y_value_str = grad_y_value_str.split(".", 1)[0]
                grad_y_value_str = grad_y_value_str[1:]
                grad_y_value_str = grad_y_value_str.strip();
                grad_y_value = int (grad_y_value_str)

                sum_Vx = sum_Vx + (2*grad_x_value * grad_y_value)
                sum_Vy = sum_Vy + ((grad_x_value * grad_x_value)- (grad_y_value * grad_y_value))

        if (sum_Vx != 0):
            tan_arg = sum_Vy / sum_Vx
            result = 0.5 * cv2.fastAtan2(sum_Vy, sum_Vx);
            print(result)
        #orientatin_matrix[i][j] = result
        else:
            result = 0.0

        X0 = i + 8
        Y0 = j + 8
        r = 8

        result_rad = result * math.pi / 180.0

        X1 = r*math.cos(result_rad - right_angle)+X0
        X1 = int (X1)
        print(X1)

        Y1 = r*math.sin(result_rad - right_angle)+Y0
        Y1 = int (Y1)

        orient_img = cv2.line(orig_img,(X0,Y0) , (X1,Y1), (0,255,0), 3)
        cv2.imshow('Oriented image', orient_img)
        white_img = cv2.line(white,(X0,Y0) , (X1,Y1), (0,255,0), 3)
        cv2.imshow('Oriented skeleton', white_img)

cv2.waitKey(0)
cv2.destroyAllWindows()
