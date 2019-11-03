import numpy as np
import cv2
from matplotlib import pyplot as plt
import math

img = cv2.imread("thinned_img.tif",0)  # uint8 image

ret, tresh_img = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
ret, tresh_copy = cv2.threshold(img, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)
cv2.imshow("Inverse treshold", tresh_img)

rows = np.size(tresh_img, 0)
cols = np.size(tresh_img, 1)
a=0
b=0
c=0
d=0
e=0
f=0
g=0
h=0
i=0
ridge = 0
ridcheck = 0
bif = 0
bifcheck =0
ridge_point_list = list()
ridge_point_delete = list()

for x in range(0,rows-1):
    for y in range(0, cols-1):
        pix1 = 0
        pix2 = 0
        pix3 = 0
        pix4 = 0
        pix5 = 0
        pix6 = 0
        pix7 = 0
        pix8 = 0
        pix9 = 0

        pix1 = tresh_img[x-1][y-1]
        pix2 = tresh_img[x][y-1]
        pix3 = tresh_img[x+1][y-1]
        pix4 = tresh_img[x-1][y]
        pix5 = tresh_img[x][y]
        pix6 = tresh_img[x+1][y]
        pix7 = tresh_img[x-1][y+1]
        #print(pix5)
        pix8 = tresh_img[x][y+1]
        pix9 = tresh_img[x+1][y+1]

        if(pix1 == 255 and pix2 == 0 and pix3 == 0 and pix4 == 0 and pix5 == 255 and pix6 == 0 and pix7 == 0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

        elif(pix1==0 and pix2==255 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

        elif (pix1==0 and pix2==0 and pix3==255 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

        elif (pix1==0 and pix2==0 and pix3==0 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

        elif (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==0 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

        elif (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==255 and pix8==0 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

        elif (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==255 and pix9==0):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

        elif (pix1==0 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==255):
            ridge = ridge + 1
            ridcheck = ridcheck + 1

##################################################################################################################################
        elif (pix1==255 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==255 and pix8==0 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        elif (pix1==0 and pix2==255 and pix3==0 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==255):
            bif = bif + 1
            bifcheck = bifcheck + 1
        elif (pix1==255 and pix2==0 and pix3==255 and pix4==0 and pix5==255 and pix6==0 and pix7==0 and pix8==255 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        elif (pix1==0 and pix2==255 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==255 and pix8==0 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        elif (pix1==0 and pix2==0 and pix3==255 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==0 and pix9==255):
            bif = bif + 1
            bifcheck = bifcheck + 1
        elif (pix1==255 and pix2==0 and pix3==0 and pix4==0 and pix5==255 and pix6==255 and pix7==0 and pix8==255 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1
        elif (pix1==0 and pix2==255 and pix3==0 and pix4==0 and pix5==255 and pix6==0 and pix7==255 and pix8==0 and pix9==255):
            bif = bif + 1
            bifcheck = bifcheck + 1
        elif (pix1==0 and pix2==0 and pix3==255 and pix4==255 and pix5==255 and pix6==0 and pix7==0 and pix8==255 and pix9==0):
            bif = bif + 1
            bifcheck = bifcheck + 1

        if (ridge > 0):
            x_str = str(x)
            y_str = str(y)
            ridge_point_list.append(x_str + " "+ y_str)
            cv2.circle(tresh_img, (x,y), 2, (255,0,0),1)
            ridge = 0
        #if (bif > 0):
        #    cv2.rectangle(tresh_img, (x-2,y-2), (x+2,y+2), (255,0,0),2)
        #    bif = 0

i = 0
#for i in range(len(ridge_point_list)-1):
#    print(ridge_point_list[i])


point_before = ridge_point_list[0]
point_before_x_str = point_before.split(" ", 1)[0]
point_before_x = int (point_before_x_str)
point_before_y_str = point_before.split(" ", 1)[1]
point_before_y = int (point_before_y_str)

i = 1

for i in range(1, len(ridge_point_list)-1):
    #print(ridge_point_list[i])

    point = ridge_point_list[i]
    #print(point)
    point_x_str = point.split(" ", 1)[0]
    point_x = int (point_x_str)
    #print(point_x)
    point_y_str = point.split(" ", 1)[1]
    point_y = int (point_y_str)
    '''
    print("Point before:")
    print(point_before)
    print("X of point before:")
    print(point_before_x)
    print("Y of point before:")
    print(point_before_y)
    print("This point:")
    print(point)
    print("X of this point:")
    print(point_x)
    print("Y of this point:")
    print(point_y)
    '''


    '''
    if (point_before == point -1 or point_before == point +1) or (point_before == point -2 or point_before == point +2) or (point_before == point -3 or point_before == point +3):
        print("DELETING")
        ridge_point_delete.append(ridge_point_list[i])
        ridge_point_delete.append(point_before)
    '''
    if (point_before_x == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_y == point_y and (point_before_x == point_x -1 or point_before_x == point_x +1 or  point_before_x == point_x -2 or point_before_x == point_x +2 or point_before_x == point_x -3 or point_before_x == point_x +3 or point_before_x == point_x -4 or point_before_x == point_x +4 or point_before_x == point_x -5 or point_before_x == point_x +5 or point_before_x == point_x -6 or point_before_x == point_x +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x -6 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x -5 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x -4 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x -3 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x -2 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x -1 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x +1 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x +2 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x +3 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x +4 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x +5 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)

    if (point_before_x +6 == point_x and (point_before_y == point_y -1 or point_before_y == point_y +1 or  point_before_y == point_y -2 or point_before_y == point_y +2 or point_before_y == point_y -3 or point_before_y == point_y +3 or point_before_y == point_y -4 or point_before_y == point_y +4 or  point_before_y == point_y -5 or point_before_y == point_y +5 or point_before_y == point_y -6 or point_before_y == point_y +6)):
        print("DELETING")
        ridge_point_delete.append(point)
        ridge_point_delete.append(point_before)


    point_before = point
    point_before_x_str = point_before.split(" ", 1)[0]
    point_before_x = int (point_before_x_str)
    point_before_y_str = point_before.split(" ", 1)[1]
    point_before_y = int (point_before_y_str)


    i +=1


#for p in ridge_point_delete:
    #print(p)



for p in ridge_point_list:
    if p in ridge_point_delete:
        #print("Yes, delete")
        ridge_point_list.remove(p)

for p in ridge_point_list:
    print(p)

#print("Count of reduced ridges")
#print(len(ridge_point_list))
#print("==========================")
for i in range(len(ridge_point_list)):
    #print(ridge_point_list[i])

    point = ridge_point_list[i]
    point = ridge_point_list[i]
    point_x_str = point.split(" ", 1)[0]
    point_x = int (point_x_str)
    point_y_str = point.split(" ", 1)[1]
    point_y = int (point_y_str)

    cv2.circle(tresh_copy, (point_x,point_y), 2, (255,0,0),1)

print("Count of all ridges:")
print(ridcheck)
print("======================")
print(bifcheck)
sum = ridcheck + bifcheck
print("Sum of ridges and bif")
print(sum)
cv2.imshow("Ridges", tresh_img);
cv2.imshow("Ridges corrected", tresh_copy);

cv2.imwrite('ridges.tif', tresh_img)
cv2.waitKey(0)
cv2.destroyAllWindows()
