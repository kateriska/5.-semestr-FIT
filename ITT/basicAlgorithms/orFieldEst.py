import numpy as np
import cv2
import math

img = cv2.imread("104_1.tif", 0)  # uint8 image

# normalize image
norm_image = cv2.normalize(img,None,0,255,cv2.NORM_MINMAX)
cv2.imshow('NormImage', norm_image)

rows = np.size(norm_image, 0)
cols = np.size(norm_image, 1)
print(rows)
print(cols)

step = 16
x = 16
y = 16
# draw grid for squares 16 x 16
while x < rows:
    cv2.line(norm_image, (x, 0), (x, rows), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
    x += step

while y < cols:
    cv2.line(norm_image, (0, y), (cols, y), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
    y += step

cv2.imshow('Norm Image With Grid', norm_image)

block_div = 8
shape_img = norm_image.shape

print(norm_image.shape)
print(norm_image.dtype)

grad_x = np.zeros(shape_img, dtype=np.uint8)
grad_y = np.zeros(shape_img, dtype=np.uint8)
Vx = np.zeros(shape_img, dtype=np.uint8)
Vy = np.zeros(shape_img, dtype=np.uint8)
theta = np.zeros(shape_img, dtype=np.uint8)
phi_x = np.zeros(shape_img, dtype=np.uint8)
phi_y = np.zeros(shape_img, dtype=np.uint8)
Orientation = np.zeros(shape_img, dtype=np.uint8)
filtering_x = np.zeros(shape_img, dtype=np.uint8)
filtering_y = np.zeros(shape_img, dtype=np.uint8)
orientation_matrix = np.zeros(shape_img, dtype=np.uint8)
magnitude_array = np.zeros(shape_img, dtype=np.uint8)


filtering = 5
filtering_div = 2.5
right_angle  = 1



#compute gradients for each pixel
# gradient for uint8 image
grad_x = cv2.Sobel(norm_image,cv2.CV_8UC1,1,0,ksize=5)
grad_y = cv2.Sobel(norm_image,cv2.CV_8UC1,0,1,ksize=5)

for i in range(block_div, rows-block_div, step):
    for j in range(block_div, cols-block_div, step):
        sum_Vx = 0.0
        sum_Vy = 0.0
        for u in range(i-block_div, i+block_div):
            for v in range(j-block_div, j+block_div):
                sum_Vy = sum_Vy + (2 * grad_x[u][v] * grad_y[u][v])
                sum_Vx = sum_Vx + (grad_x[u][v]**2 * grad_y[u][v]**2)
                #print(sum_Vx)
                #print(sum_Vy)

        Vy[i][j] = sum_Vy
        Vx[i][j] = sum_Vx
        theta_calculation = 0.0

        if (sum_Vx != 0 ):
            theta_calculation = 0.5 * math.atan(sum_Vy / sum_Vx)
            #print(theta_calculation)

        theta[i][j] = theta_calculation
        # compute the magnitude
        magnitude = math.sqrt((Vx[i][j] * Vx[i][j]) + (Vy[i][j] * Vy[i][j]))
        magnitude_array[i][j] = magnitude

        print(magnitude)
        phi_x = magnitude * math.cos(2*theta_calculation)
        phi_y = magnitude * math.sin(2*theta_calculation)
        print(phi_x)
        orientation = 0.0
        if (phi_x != 0):
            orientation = 0.5 * math.atan(phi_y / phi_x)
            print(orientation)
        X0 = i + 8
        Y0 = j + 8
        r = 8

        X1 = r*math.cos(theta_calculation - right_angle)+X0
        X1 = int (X1)

        Y1 = r*math.sin(theta_calculation - right_angle)+Y0
        Y1 = int (Y1)

        #print(Y1)
        #print(X0, Y0)
        #print(X1, Y1)
        #start_point = cv2.Point2f(X0, Y0)
        #end_point = cv2.Point2f(X1, Y1)
        norm_image = cv2.line(norm_image,(X0,Y0) , (X1,Y1), (0,255,0), 3)
        cv2.imshow('Oriented image', norm_image)
        #print(magnitude)
        #orientation_matrix[i][j] = theta_calculation






cv2.waitKey(0)
cv2.destroyAllWindows()
