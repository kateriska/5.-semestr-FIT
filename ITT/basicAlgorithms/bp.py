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

def orientFieldEstimation(img):
    rows = np.size(img, 0)
    cols = np.size(img, 1)
    print(rows)
    print(cols)

    step = 16
    x = 16
    y = 16
    # draw grid for squares 16 x 16
    while x < rows:
        cv2.line(img, (x, 0), (x, rows), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
        x += step

    while y < cols:
        cv2.line(img, (0, y), (cols, y), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
        y += step

    cv2.imshow('Norm Image With Grid', img)

    block_div = 8
    shape_img = img.shape

    print(img.shape)
    print(img.dtype)

    grad_x = np.zeros(shape_img, dtype=np.uint8)
    grad_y = np.zeros(shape_img, dtype=np.uint8)
    Vx = np.zeros(shape_img, dtype=np.uint8)
    Vy = np.zeros(shape_img, dtype=np.uint8)
    theta = np.zeros(shape_img, dtype=np.uint8)
    phi_x_array = np.zeros(shape_img, dtype=np.uint8)
    phi_y_array = np.zeros(shape_img, dtype=np.uint8)
    Orientation = np.zeros(shape_img, dtype=np.uint8)
    filtering_x = np.zeros(shape_img, dtype=np.uint8)
    filtering_y = np.zeros(shape_img, dtype=np.uint8)
    orientation_matrix = np.zeros(shape_img, dtype=np.uint8)
    magnitude_array = np.zeros(shape_img, dtype=np.uint8)


    filtering = 5
    filtering_div = 2.5
    right_angle  = math.pi / 2



    #compute gradients for each pixel
    # gradient for uint8 image
    grad_x = cv2.Sobel(img,cv2.CV_8UC1,1,0)
    grad_y = cv2.Sobel(img,cv2.CV_8UC1,0,1)

    for i in range(block_div, rows-block_div, step):
        for j in range(block_div, cols-block_div, step):
            sum_Vx = 0.0
            sum_Vy = 0.0
            for u in range(i-block_div, i+block_div):
                for v in range(j-block_div, j+block_div):
                    sum_Vx = sum_Vx + (2 * grad_x[u][v] * grad_y[u][v])
                    sum_Vy = sum_Vy + (grad_x[u][v]**2 * grad_y[u][v]**2)
                    '''
                    print("===Sum VX===")
                    print(sum_Vx)
                    print("===Sum VY===")
                    print(sum_Vy)
                    '''

            Vy[i][j] = sum_Vy
            Vx[i][j] = sum_Vx
            theta_calculation = 0.0

            if (sum_Vx != 0 ):
                theta_calculation = 0.5 * math.atan(sum_Vy / sum_Vx)
                #print(theta_calculation)
            else:
                theta_calculation = 0.0

            #print(theta_calculation)
            theta[i][j] = theta_calculation

            # compute the magnitude
            magnitude = math.sqrt((Vx[i][j] * Vx[i][j]) + (Vy[i][j] * Vy[i][j]))
            magnitude_array[i][j] = magnitude

            #print(magnitude)
            phi_x = magnitude * math.cos(2*theta_calculation)

            #print(phi_x)
            phi_y = magnitude * math.sin(2*theta_calculation)
            #print(phi_y)
            #phi_x = phi_x_array[i][j]
            #phi_y = phi_y_array[i][j]
            #phi_x_array[i][j] = phi_x
            #phi_y_array[i][j] = phi_y

            #print(phi_x)
            orientation = 0.0
            if (phi_x != 0):
                orientation = 0.5 * math.atan(phi_y / phi_x)
                print(orientation)
            else:
                orientation = 0.0

            #Orientation[i][j] = orientation

            X0 = i + 8
            Y0 = j + 8
            r = 8

            X1 = r*math.cos(orientation - right_angle)+X0
            X1 = int (X1)

            Y1 = r*math.sin(orientation - right_angle)+Y0
            Y1 = int (Y1)

            #print(Y1)
            #print(X0, Y0)
            #print(X1, Y1)
            #start_point = cv2.Point2f(X0, Y0)
            #end_point = cv2.Point2f(X1, Y1)
            img = cv2.line(img,(X0,Y0) , (X1,Y1), (0,255,0), 3)
            cv2.imshow('Oriented image', img)
            #print(magnitude)
            #orientation_matrix[i][j] = theta_calculation
    return img


img = cv2.imread("104_1.tif", 0) # uint8 image
cv2.imshow('Original uint8 image', img)
segmented_image = imgSegmentation(img)
cv2.imshow('Final segmented image', segmented_image)
thinned_image = imgThinning(segmented_image)
cv2.imshow('Final segmented and thinned image', thinned_image)
oriented_image = orientFieldEstimation(thinned_image)
cv2.imshow('Final segmented, thinned and orient image', oriented_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
