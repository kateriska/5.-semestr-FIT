import cv2
import numpy as np

img = cv2.imread("gabor_segmented.tif", 0) # uint8 image
shape_img = img.shape
size_img = img.size

skeleton = np.zeros(shape_img, dtype=np.uint8)

ret, img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU) # transform to binary image
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
cv2.imwrite('thinned_gabor.tif', skeleton)
cv2.waitKey(0)
cv2.destroyAllWindows()

# segmentation, thinning, orientation field estimation
