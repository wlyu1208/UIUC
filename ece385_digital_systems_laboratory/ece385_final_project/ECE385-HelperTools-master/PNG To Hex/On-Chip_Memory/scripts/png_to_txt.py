from PIL import Image
from collections import Counter
from scipy.spatial import KDTree
import numpy as np
def hex_to_rgb(num):
    h = str(num)
    return int(h[0:4], 16), int(('0x' + h[4:6]), 16), int(('0x' + h[6:8]), 16)
def rgb_to_hex(num):
    h = str(num)
    return int(h[0:4], 16), int(('0x' + h[4:6]), 16), int(('0x' + h[6:8]), 16)

filenames = ["ZBlockPiece","SBlockPiece","OBlockPiece","TBlockPiece","LBlockPiece","JBlockPiece","IBlockPiece","IBlockPieceSized", "JBlockPieceSized", "LBlockPieceSized", "OBlockPieceSized", "SBlockPieceSized", "TBlockPieceSized", "ZBlockPieceSized"]


for filename in filenames:
    im = Image.open("../sprite_originals/" + filename + '.png')
    pix = im.load()

    outFile = open("../sprite_bytes/" + filename + '.txt', 'w')

    for y in range(im.size[1]):
        for x in range(im.size[0]):
            print(pix[x,y])
            r = format(pix[x,y][0], '02x')
            g = format(pix[x,y][1], '02x')
            b = format(pix[x,y][2], '02x')

            outFile.write(r+g+b+'\n')
            im.save("../sprite_originals/" + filename + '.png')
    outFile.close()
