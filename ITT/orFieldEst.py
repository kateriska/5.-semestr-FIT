import numpy as np
import cv2
#from matplotlib import pyplot as plt


img = cv2.imread("104_1.tif", 0)  # uint8 image
height_orig = np.size(img, 0)
width_orig = np.size(img, 1)
print(height_orig)
print(width_orig)

#m = np.zeros(img.shape, dtype=np.uint8)

#norm_image = cv2.normalize(image, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
norm_image = cv2.normalize(img,None,0,255,cv2.NORM_MINMAX)
cv2.imshow('NormImage', norm_image)

height = np.size(norm_image, 0)
width = np.size(norm_image, 1)
print(height)
print(width)

# compute gradients at each pixel
sobelx = cv2.Sobel(img,cv2.CV_64F,1,0,ksize=5)
sobely = cv2.Sobel(img,cv2.CV_64F,0,1,ksize=5)

print(sobelx)
print(sobely)

step = 16
x = 16
y = 16

start_x = 0
start_y = 0

while x < width:
    cv2.line(norm_image, (x, 0), (x, height), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
    x += step

while y < height:
    cv2.line(norm_image, (0, y), (width, y), color=(0, 0, 0), lineType=cv2.LINE_AA, thickness=1)
    y += step

cv2.imshow('Norm Image With Grid', norm_image)


array_I = norm_image.astype(np.float)


for i in range(start_x, width - step + 1, step):
    for j in range(start_y, height - step + 1, step):
        D_x = cv2.Sobel(norm_image[i:i+step,j:j+step],-1,1,0)
        D_y = cv2.Sobel(norm_image[i:i+step,j:j+step],-1,1,0)
        #print(pixel_sobel_x)
        #print(pixel_sobel_y)

'''
array_I = np.array([width][height])
row, col = array_I.shape
array_I = array_I.astype(np.float)
w = 17
h = 17
Ox = array_I[0:row-h+1:h,0:col-w+1:w].copy()
Ox[:] = 0.0
Vx = Ox.copy()
Vy = Vx.copy()
Oy = Ox.copy()
angle = Vx.copy()#array to contain all the 17*17 blocks's orientatons
c = r = -1
for i in xrange(0, row-h+1, h):
    r+=1
    for j in xrange(0, col-w+1, w):
        c+=1
        Dx = cv2.Sobel(array_I[i:i+h,j:j+w],-1,1,0)#gradient component x for a 17*17block
        Dy = cv2.Sobel(array_I[i:i+h,j:j+w],-1,0,1)#gradient component y for 17*17 block
        for k in range(0,h):
            for l in range(0,w):
                Vy[r][c] += ((Dx[k][l])*(Dy[k][l]))**2
                Vx[r][c] += 2*(Dx[k][l])*(Dy[k][l])
    angle[r][c] = 0.5*(math.atan(Vy[r][c]/Vx[r][c]))#get the orientation angle for the given 16*16 block
'''
'''
def compute_real_orientation(array_I, w=17, h=17, low_pass_filter=cv2.blur,filter_size=(5,5),blur_size=(5,5)):
    row, col = array_I.shape
    array_I = array_I.astype(np.float)
    Ox = array_I[0:row-h+1:h,0:col-w+1:w].copy()
    Ox[:] = 0.0
    Vx = Ox.copy()
    Vy = Vx.copy()
    Oy = Ox.copy()
    angle = Vx.copy()#array to contain all the 17*17 blocks's orientatons
    c = r = -1
    for i in xrange(0, row-h+1, h):
        r+=1
        for j in xrange(0, col-w+1, w):
            c+=1
            Dx = cv2.Sobel(array_I[i:i+h,j:j+w],-1,1,0)#gradient component x for a 17*17block
            Dy = cv2.Sobel(array_I[i:i+h,j:j+w],-1,0,1)#gradient component y for 17*17 block
            for k in range(0,h):
                for l in range(0,w):
                    Vy[r][c] += ((Dx[k][l])*(Dy[k][l]))**2
                    Vx[r][c] += 2*(Dx[k][l])*(Dy[k][l])
        angle[r][c] = 0.5*(math.atan(Vy[r][c]/Vx[r][c]))#get the orientation angle for the given 16*16 block
    c = -1
    #smoothing process of the whole array angle
    row, col = angle.shape
    for i in range(0, row):
        for j in range(0, col):
            Ox[i][j] = math.cos(2*angle[i][j])
            Oy[i][j] = math.sin(2*angle[i][j])
    Ox = low_pass_filter(Ox, blur_size)
    Oy = low_pass_filter(Oy, blur_size)
    for i in range(0, row):
        for j in range(0, col):
            angle[i][j] = 0.5*math.atan(Oy[i][j]/Ox[i][j])#take the final orientation of all 17*17 blocks
    return angle





'''



cv2.waitKey(0)
cv2.destroyAllWindows()
