import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

def imgSegmentation(img):
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
    result = cv2.add(tresh_img, opening)
    cv2.imshow('Img after noise removal', result)
    return result

def imgThinning(img):
    shape_img = img.shape
    size_img = img.size

    skeleton = np.zeros(shape_img, dtype=np.uint8)

    element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3)) # structuring element for morphological operations
    done = False

    while (not done):
        temp_img = cv2.morphologyEx(img, cv2.MORPH_OPEN, element); # storing to temporary image
        temp_img = cv2.bitwise_not(temp_img, element);
        temp_img = cv2.bitwise_and(img, temp_img);
        skeleton = cv2.bitwise_or(skeleton, temp_img);
        img = cv2.erode(img, element);

        zeros = size_img - cv2.countNonZero(img)
        if zeros == size_img:
            done = True

    skeleton = cv2.bitwise_not(skeleton)
    cv2.imshow("Thinned image", skeleton)
    return skeleton

def orientFieldEstimation(orig_img):
    white = cv2.imread("white.jpg")
    white = cv2.resize(white,(388,374))
    img = np.float32(orig_img)

    rows = np.size(img, 0)
    cols = np.size(img, 1)

    shape_img = img.shape

    grad_x = np.zeros(shape_img, dtype=np.float32)
    grad_y = np.zeros(shape_img, dtype=np.float32)
    Vx = np.zeros(shape_img, dtype=np.float32)
    Vy = np.zeros(shape_img, dtype=np.float32)
    theta = np.zeros(shape_img, dtype=np.float32)
    phi_x_array = np.zeros(shape_img, dtype=np.float32)
    phi_y_array = np.zeros(shape_img, dtype=np.float32)
    magnitude_array = np.zeros(shape_img, dtype=np.float32)

    grad_x = cv2.Sobel(img,cv2.CV_32FC1,1, 0)
    grad_y = cv2.Sobel(img,cv2.CV_32FC1,0, 1)

    block_div = 8
    right_angle  = math.pi / 2
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
                    grad_x_value_str = grad_x_value_str.split("[", 1)[1]
                    grad_x_value_str = grad_x_value_str.split(".", 1)[0]
                    grad_x_value_str = grad_x_value_str.strip();
                    grad_x_value = int (grad_x_value_str)

                    grad_y_value_np = grad_y[u][v]
                    grad_y_value_str = np.array2string(grad_y_value_np)
                    grad_y_value_str = grad_y_value_str.split("[", 1)[1]
                    grad_y_value_str = grad_y_value_str.split(".", 1)[0]
                    grad_y_value_str = grad_y_value_str.strip();
                    grad_y_value = int (grad_y_value_str)


                    sum_Vx = sum_Vx + (2*grad_x_value * grad_y_value)
                    sum_Vy = sum_Vy + ((grad_x_value * grad_x_value)- (grad_y_value * grad_y_value))

            if (sum_Vy != 0):
                #tan_arg = sum_Vy / sum_Vx
                result = 0.5 * cv2.fastAtan2(sum_Vy, sum_Vx);
                print(result)
            #orientatin_matrix[i][j] = result
            else:
                result = 0.0

            magnitude = math.sqrt((sum_Vx * sum_Vx) + (sum_Vy * sum_Vy))
            phi_x = magnitude * math.cos(2*(math.radians(result)))
            phi_y = magnitude * math.sin(2*(math.radians(result)))
            if (phi_x != 0):
                orient = 0.5 * cv2.fastAtan2(phi_y, phi_x)
            else:
                orient = 0.0

            X0 = i + 8
            Y0 = j + 8
            r = 8

            #result_rad = result * math.pi / 180.0

            X1 = r * math.cos(math.radians(orient))+ X0
            X1 = int (X1)
            print(X1)

            Y1 = r * math.sin(math.radians(orient))+ Y0
            Y1 = int (Y1)

            orient_img = cv2.line(orig_img,(X0,Y0) , (X1,Y1), (0,255,0), 3)
            cv2.imshow('Oriented image', orient_img)
            white_img = cv2.line(white,(X0,Y0) , (X1,Y1), (0,255,0), 3)
            cv2.imshow('Oriented skeleton', white_img)
            rotated_img = cv2.rotate(white_img, cv2.ROTATE_90_CLOCKWISE)
            cv2.imshow('Oriented rotated skeleton', rotated_img)

    return rotated_img


img = cv2.imread("104_1.tif",0) # uint8 image
img = cv2.resize(img,(388,374))
img = cv2.normalize(img,None,0,255,cv2.NORM_MINMAX)
cv2.imshow('Original uint8 image', img)
segmented_image = imgSegmentation(img)
cv2.imshow('Final segmented image', segmented_image)
cv2.imwrite('segmented_img.tif', segmented_image)
thinned_image = imgThinning(segmented_image)
cv2.imshow('Final segmented and thinned image', thinned_image)
cv2.imwrite('thinned_img.tif', thinned_image)
orig_img = cv2.imread("thinned_img.tif")
oriented_image = orientFieldEstimation(orig_img)
cv2.imshow('Final segmented, thinned and orient image', oriented_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
