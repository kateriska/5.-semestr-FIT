import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

### IMAGE SEGMENTATION WITH MORPHOLOGY OPERATIONS
def imgSegmentation(img):
    ret, tresh_img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU)
    #cv2.imshow('Tresholded image', tresh_img)

    # noise removal
    kernel = np.ones((12,12), np.uint8)
    opening = cv2.morphologyEx(tresh_img, cv2.MORPH_OPEN,kernel)
    #cv2.imshow('Opening', opening)
    im2, contours, hierarchy = cv2.findContours(opening, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    #cv2.drawContours(opening, contours, -1, (0,255,0), 3)
    #cv2.imshow('Opening with contours', opening)
    cv2.Canny(opening, 100, 200);
    result = cv2.add(tresh_img, opening)
    #cv2.imshow('Img after noise removal', result)
    return result

### THINNING OF IMAGE
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
    #cv2.imshow("Thinned image", skeleton)
    cv2.imwrite('thinned_img.tif', skeleton)
    return skeleton

### ORIENTED FIELD ESTIMATION
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

            X1 = r * math.cos(math.radians(orient) - right_angle)+ X0
            X1 = int (X1)
            print(X1)

            Y1 = r * math.sin(math.radians(orient) - right_angle)+ Y0
            Y1 = int (Y1)

            orient_img = cv2.line(orig_img,(X0,Y0) , (X1,Y1), (0,255,0), 3)
            #cv2.imshow('Oriented image', orient_img)
            white_img = cv2.line(white,(X0,Y0) , (X1,Y1), (0,255,0), 3)
            #cv2.imshow('Oriented skeleton', white_img)
            rotated_img = cv2.rotate(white_img, cv2.ROTATE_90_CLOCKWISE)
            #cv2.imshow('Oriented rotated skeleton', rotated_img)
            flip_horizontal_img = cv2.flip(rotated_img, 1)

    return flip_horizontal_img

### GABOR IMAGE FILTERING
def gaborFilter(orig_img):
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
    #cv2.imshow('gabor1', gabor1)
    #cv2.imshow('gabor2', gabor2)
    #cv2.imshow('gabor3', gabor3)
    #cv2.imshow('gabor4', gabor4)
    adding1 = cv2.add(gabor1, gabor2)
    adding2 = cv2.add(adding1, gabor3)
    adding3 = cv2.add(adding2, gabor4)
    #cv2.imshow('sym', adding1)
    cv2.imshow('Gabor image', adding2)
    #cv2.imshow('final', adding3)
    cv2.imwrite('gabor_img.tif', adding2)
    return

### MINUTIAE EXTRACTION
def minutiaeExtraction(img):
    ret, tresh_img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    rows = np.size(tresh_img, 0)
    cols = np.size(tresh_img, 1)

    ret, tresh_ridges = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    ret, tresh_bifurcations = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
    ret, tresh_ridges_corrected = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

    ridge = 0
    ridcheck = 0
    bif = 0
    bifcheck =0

    ridge_point_list = list()
    ridge_point_delete = list()

    for x in range(0,rows-1):
        for y in range(0, cols-1):
            pix1 = 0
            pix2 = 0
            pix3 = 0
            pix4 = 0
            pix5 = 0
            pix6 = 0
            pix7 = 0
            pix8 = 0
            pix9 = 0

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

            ############ RIDGES ################################

            if (pix1 == 255 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 0 and pix9 == 0):
                ridge = ridge + 1
                ridcheck = ridcheck + 1
            elif (pix1 == 0 and pix2 == 255 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 0 and pix9 == 0):
                ridge = ridge + 1
                ridcheck = ridcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 255 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 0 and pix9 == 0):
                ridge = ridge + 1
                ridcheck = ridcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 0 and pix4 == 255 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 0 and pix9 == 0):
                ridge = ridge + 1
                ridcheck = ridcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 255 and pix7 == 0 and pix8 == 0 and pix9 == 0):
                ridge = ridge + 1
                ridcheck = ridcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 255 and pix8 == 0 and pix9 == 0):
                ridge = ridge + 1
                ridcheck = ridcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 255 and pix9 == 0):
                ridge = ridge + 1
                ridcheck = ridcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 0 and pix9 == 255):
                ridge = ridge + 1
                ridcheck = ridcheck + 1

            ######### BIFURCATIONS ##################################
            elif (pix1 == 255 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 255 and pix7 == 255 and pix8 == 0 and pix9 == 0):
                bif = bif + 1
                bifcheck = bifcheck + 1
            elif (pix1 == 0 and pix2 == 255 and pix3 == 0 and pix4 == 255 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 0 and pix9 == 255):
                bif = bif + 1
                bifcheck = bifcheck + 1
            elif (pix1 == 255 and pix2 == 0 and pix3 == 255 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 255 and pix9 == 0):
                bif = bif + 1
                bifcheck = bifcheck + 1
            elif (pix1 == 0 and pix2 == 255 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 255 and pix7 == 255 and pix8 == 0 and pix9 == 0):
                bif = bif + 1
                bifcheck = bifcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 255 and pix4 == 255 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 0 and pix9 == 255):
                bif = bif + 1
                bifcheck = bifcheck + 1
            elif (pix1 == 255 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 255 and pix7 == 0 and pix8 == 255 and pix9 == 0):
                bif = bif + 1
                bifcheck = bifcheck + 1
            elif (pix1 == 0 and pix2 == 255 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 255 and pix8 == 0 and pix9 == 255):
                bif = bif + 1
                bifcheck = bifcheck + 1
            elif (pix1 == 0 and pix2 == 0 and pix3 == 255 and pix4 == 255 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8 == 255 and pix9 == 0):
                bif = bif + 1
                bifcheck = bifcheck + 1

            if (ridge > 0):
                cv2.rectangle(tresh_ridges, (x-2,y-2), (x+2,y+2), (255,0,0),2)
                x_str = str(x)
                y_str = str(y)
                ridge_point_list.append(x_str + " "+ y_str)
                ridge = 0

            if (bif > 0):
                cv2.rectangle(tresh_bifurcations, (x-2,y-2), (x+2,y+2), (255,0,0),2)
                bif = 0

    point_before = ridge_point_list[0]
    point_before_x_str = point_before.split(" ", 1)[0]
    point_before_x = int (point_before_x_str)
    point_before_y_str = point_before.split(" ", 1)[1]
    point_before_y = int (point_before_y_str)

    i = 1

    for i in range(1, len(ridge_point_list)-1):
        #print(ridge_point_list[i])

        point = ridge_point_list[i]
        #print(point)
        point_x_str = point.split(" ", 1)[0]
        point_x = int (point_x_str)
        #print(point_x)
        point_y_str = point.split(" ", 1)[1]
        point_y = int (point_y_str)
        '''
        print("Point before:")
        print(point_before)
        print("X of point before:")
        print(point_before_x)
        print("Y of point before:")
        print(point_before_y)
        print("This point:")
        print(point)
        print("X of this point:")
        print(point_x)
        print("Y of this point:")
        print(point_y)
        '''

        '''
        if (point_before == point -1 or point_before == point +1) or (point_before == point -2 or point_before == point +2) or (point_before == point -3 or point_before == point +3):
            print("DELETING")
            ridge_point_delete.append(ridge_point_list[i])
            ridge_point_delete.append(point_before)
        '''
        if (point_before_x == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_y == point_y and (point_before_x == point_x -1 or point_before_x == point_x +1 or  point_before_x == point_x -2 or point_before_x == point_x +2 or point_before_x == point_x -3 or point_before_x == point_x +3 or point_before_x == point_x -4 or point_before_x == point_x +4 or point_before_x == point_x -5 or point_before_x == point_x +5 or point_before_x == point_x -6 or point_before_x == point_x +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x -6 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x -5 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x -4 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x -3 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x -2 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x -1 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x +1 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x +2 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x +3 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x +4 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x +5 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)

        if (point_before_x +6 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
            print("DELETING")
            ridge_point_delete.append(point)
            ridge_point_delete.append(point_before)


        point_before = point
        point_before_x_str = point_before.split(" ", 1)[0]
        point_before_x = int (point_before_x_str)
        point_before_y_str = point_before.split(" ", 1)[1]
        point_before_y = int (point_before_y_str)


        i +=1


    #for p in ridge_point_delete:
        #print(p)



    for p in ridge_point_list:
        if p in ridge_point_delete:
            #print("Yes, delete")
            ridge_point_list.remove(p)

    for p in ridge_point_list:
        print(p)

    #print("Count of reduced ridges")
    #print(len(ridge_point_list))
    #print("==========================")
    for i in range(len(ridge_point_list)):
        #print(ridge_point_list[i])

        point = ridge_point_list[i]
        point = ridge_point_list[i]
        point_x_str = point.split(" ", 1)[0]
        point_x = int (point_x_str)
        point_y_str = point.split(" ", 1)[1]
        point_y = int (point_y_str)

        cv2.circle(tresh_ridges_corrected, (point_x,point_y), 2, (255,0,0),1)

    print("Count of ridges: " + repr(ridcheck))
    print("Count of bifurcations: " + repr(bifcheck))
    cv2.imshow("Ridges", tresh_ridges);
    cv2.imshow("Bifurcations", tresh_bifurcations);
    cv2.imshow("Ridges corrected", tresh_ridges_corrected);
    return tresh_img

img = cv2.imread("102_1.tif",0) # uint8 image in grayscale
img = cv2.resize(img,(388,374)) # resize of image
img = cv2.normalize(img,None,0,255,cv2.NORM_MINMAX) # normalize image
cv2.imwrite('norm_img.tif', img)
cv2.imshow('Original normalized uint8 image', img)
segmented_image = imgSegmentation(img) # segmented image with morphological operations
cv2.imshow('Segmented image', segmented_image)
cv2.imwrite('segmented_img.tif', segmented_image)
thinned_image = imgThinning(segmented_image) # image thinning
cv2.imshow('Segmented and thinned image', thinned_image)
cv2.imwrite('thin_seg_img.tif', thinned_image)
orig_img = cv2.imread("thin_seg_img.tif")
# oriented field estimation with morphological segmented and thinned image
oriented_image = orientFieldEstimation(orig_img)
cv2.imshow('Segmented, thinned and orient image', oriented_image)

img = cv2.imread("norm_img.tif")
img = cv2.resize(img,(388,374))
gaborFilter(img) # gabor filtering of normalized image
gabor_image_for_orient = cv2.imread("gabor_img.tif")
gabor_image_for_orient = cv2.bitwise_not(gabor_image_for_orient) # switch colors in gabor filtered image
cv2.imshow('Gabor for orientation', gabor_image_for_orient)
# oriented field estimation of gabor filtered image
oriented_image_gabor = orientFieldEstimation(gabor_image_for_orient)
cv2.imshow('Gabor oriented image', oriented_image_gabor)

img = cv2.imread("norm_img.tif")
img = cv2.resize(img,(388,374))
gaborFilter(img) # gabor filtering of normalized image
gabor_image_for_orient = cv2.imread("gabor_img.tif", 0)
gabor_image_for_orient = cv2.bitwise_not(gabor_image_for_orient)
#cv2.imshow('Gabor for orient2', gabor_image_for_orient)
imgThinning(gabor_image_for_orient) # thinning of gabor filtered image
gabor_thinned_image = cv2.imread("thinned_img.tif")
# oriented field estimation of gabor filtered and thinned image
oriented_thinned_image_gabor = orientFieldEstimation(gabor_thinned_image)
cv2.imshow('Gabor oriented thinned image', oriented_thinned_image_gabor)

img = cv2.imread("norm_img.tif")
img = cv2.resize(img,(388,374))
gaborFilter(img) # gabor filtering of normalized image
gabor_image = cv2.imread("gabor_img.tif", 0)
thinned_gabor = imgThinning(gabor_image) # thinning of gabor filtered image
# minutiae extraction of gabor filtered and thinned image
minutiae_image = minutiaeExtraction(thinned_gabor)

cv2.waitKey(0)
cv2.destroyAllWindows()
