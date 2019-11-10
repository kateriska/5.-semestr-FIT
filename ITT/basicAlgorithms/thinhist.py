import numpy as np
import cv2
from matplotlib import pyplot as plt

orig_img = cv2.imread("segmented_img.tif",0)
shape_img = orig_img.shape
size_img = orig_img.size
type_img = orig_img.dtype
print(type_img)

skeleton = np.zeros(shape_img, dtype=type_img)
ret, img = cv2.threshold(orig_img, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU) # transform to binary image

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
alpha = 0.2
beta = ( 1.0 - alpha )
adding = cv2.addWeighted( orig_img, alpha, skeleton, beta, 0.0)
cv2.imshow("Thinned image", skeleton)
cv2.imshow("adding", adding)

print(adding)
plt.hist(adding.ravel(),256,[0,256]); plt.show()
#hist_lbp = cv2.calcHist([adding], [0], None, [256], [0, 256])

'''
figure = plt.figure()
current_plot = figure.add_subplot(1, 1, 1)
current_plot.plot(hist_lbp, color = "black")
#current_plot.set_xlim([0,260])
#current_plot.set_xlim([0,250])
#current_plot.set_ylim([0,4000])
current_plot.set_title("LBP histogram")
current_plot.set_xlabel("Bins")
current_plot.set_ylabel("Number of pixels")
ytick_list = [int(i) for i in current_plot.get_yticks()]
current_plot.set_yticklabels(ytick_list,rotation = 90)
plt.show()
'''


cv2.waitKey(0)
cv2.destroyAllWindows()
