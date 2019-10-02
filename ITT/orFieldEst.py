import numpy as np
import cv2
from matplotlib import pyplot as plt
from PIL import Image, ImageDraw

img = cv2.imread("nwmPa.png", cv2.IMREAD_COLOR)  # uint8 image
#norm_image = cv2.normalize(image, None, alpha=0, beta=1, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_32F)
norm_image = cv2.normalize(img,None,0,255,cv2.NORM_MINMAX)
cv2.imshow('Result', norm_image)

height = np.size(norm_image, 0)
width = np.size(norm_image, 1)
print(height)
print(width)

step_count = 16
image = Image.new(mode='L', size=(height, width), color=255)

# Draw some lines
draw = ImageDraw.Draw(image)
y_start = 0
step_size = int(image.width / step_count)

for x in range(0, image.width, step_size):
    line = ((x, y_start), (x, height))
    draw.line(line, fill=128)

x_start = 0

for y in range(0, image.height, step_size):
    line = ((x_start, y), (width, y))
    draw.line(line, fill=128)

del draw

image.show()
result = cv2.add(image, img)
cv2.imshow('Result', result)






cv2.waitKey(0)
cv2.destroyAllWindows()
