import cv2
import numpy as np

img = cv2.imread('images.jpeg',0) # grayscale image
size = np.size(img)
skel = np.zeros(img.shape,np.uint8) # skeleton, in the beginning black picture

ret,img = cv2.threshold(img,127,255,0) #transform to binary image
element = cv2.getStructuringElement(cv2.MORPH_CROSS,(3,3)) # We have to declare the structuring element we will use for our morphological operations, here we use a 3x3 cross-shaped structure element
done = False

while( not done):
    temp = cv2.morphologyEx(img, cv2.MORPH_OPEN, element); # storing to temporary image
    temp = cv2.bitwise_not(temp, element);
    temp = cv2.bitwise_and(img, temp);
    skel = cv2.bitwise_or(skel, temp);
    img = cv2.erode(img, element);

    zeros = size - cv2.countNonZero(img)
    if zeros==size:
        done = True


cv2.imshow("skel",skel)
cv2.waitKey(0)
cv2.destroyAllWindows()

'''
bool done;
do
{
  cv::morphologyEx(img, temp, cv::MORPH_OPEN, element);
  cv::bitwise_not(temp, temp);
  cv::bitwise_and(img, temp, temp);
  cv::bitwise_or(skel, temp, skel);
  cv::erode(img, img, element);

  double max;
  cv::minMaxLoc(img, 0, &max);
  done = (max == 0);
} while (!done);
'''
