#!/usr/bin/env  python3
from PIL import Image
import numpy as np

# this script is not complete
# version as i decided to directly solve by hand

img1 = Image.open("logic-lock.png")
img2 = Image.open("xor-gate.png")

# test code for comparing two matrix 
# indexing as raw array  in memory
m1 = img1.size[0]
n1 = img1.size[1]

m2 = img2.size[0]
n2 = img2.size[1]

raw_matrix1 = [ i for i in img1.getdata() ]
raw_matrix2 = [ i for i in img2.getdata() ]


# takes matrix 1 data and compare with matrix 2
def compar(idx1, idy1, m1, n1, raw_matrix1, m2, n2, raw_matrix2):
    for idy2 in range(0, m2):
        for idx2 in range(0, n2):
            if raw_matrix1[(idy1 + idy2) * n1 + (idx1 + idx2)] != raw_matrix2[idy2 * n2 + idx2]:
                return False
    return True

# for matrix m x n
# task is to find a small matrix in large matrix and it can be done with

#       n1
#   m1  -------------->
#    |  -----------------------------
#    |  | 1 | 2 | 3 | 4 | 5 | 6 | 7 |       n2
#    |  -----------------------------       --------->
#    |  | 8 | 9 |10 | 11| 12| 13|.. |       -------------
#    v  -----------------------------   m2| | 5 | 6 | 7 |   <--------- search for smaller
#       | ..|.. |   |   |   |   |   |     | -------------              matrix in large matrix
#       -----------------------------     v | 12| 13|14 |
#       |   |   |   |   |   |   |   |       -------------
#       -----------------------------
#       |   |   |   |   |   |   |   |
#       -----------------------------
#
flag = False
for idy1 in range(0, m1 - m2):
    for idx1 in range(0, n1 - n2):
        for idy2 in range(0, m2):
            for idx2 in range(0, n2):
                if raw_matrix1[(idy1 + idy2) * n1 + (idx1 + idx2)] != raw_matrix2[idy2 * n2 + idx2]:
                
                

img1.close()
img2.close()



