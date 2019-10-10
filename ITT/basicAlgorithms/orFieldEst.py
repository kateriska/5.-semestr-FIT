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

filtering = 5
filtering_div = 2.5


#compute gradients for each pixel
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

        rows_theta, cols_theta = theta.shape
        #print(theta.shape)

        phi_x[i][j] = math.cos(2*theta[i][j])
        phi_y[i][j] = math.sin(2*theta[i][j])
        #print(phi_x[i][j])

        sum_filter_x = 0.0
        sum_filter_y = 0.0
        '''

        for u in np.arange (-filtering_div, filtering_div):
            for v in np.arange (-filtering_div, filtering_div):
                i_matrix = i - u*filtering
                j_matrix = j - v*filtering
                sum_filter_x = sum_filter_x + phi_x[i_matrix][j_matrix]
                sum_filter_y = sum_filter_y + (phi_y[i_matrix][j_matrix])

        filtering_x[i][j] = sum_filter_x
        filtering_y[i][j] = sum_filter_y

        #phi_x = cv2.blur(phi_x, (5,5))
        #phi_y = cv2.blur(phi_y, (5,5))

        if (phi_x[i][j] != 0):
            Orientation[i][j] = 0.5 * math.atan(phi_y[i][j] / phi_x[i][j] )
            #print(Orientation)
        '''

cv2.waitKey(0)
cv2.destroyAllWindows()
