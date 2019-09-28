import cv2
import numpy as np

img = cv2.imread('images.jpeg',0) # grayscale image
size = np.size(img)
skel = np.zeros(img.shape,np.uint8)

ret,img = cv2.threshold(img,127,255,0) #transform to binary image
element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3)) # We have to declare the structuring element we will use for our morphological operations, here we use a 3x3 cross-shaped structure element
done = False

while( not done):
    eroded = cv2.erode(img,element)
    temp = cv2.dilate(eroded,element)
    temp = cv2.subtract(img,temp)
    skel = cv2.bitwise_or(skel,temp)
    img = eroded.copy()

    zeros = size - cv2.countNonZero(img)
    if zeros==size:
        done = True

cv2.imshow("skel",skel)
cv2.waitKey(0)
cv2.destroyAllWindows()
