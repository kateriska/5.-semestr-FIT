import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

img = cv2.imread("fakefig.png",0) # uint8 image in grayscale
fin_img	= cv2.ximgproc.thinning(img, cv2.ximgproc.THINNING_GUOHALL	)
cv2.imshow("Thinning using library",fin_img)

cv2.waitKey(0)
cv2.destroyAllWindows()
