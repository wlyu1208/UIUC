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

files = ['ghostPiece'];


for filename in files:
    new_w = 16
    new_h = 16
    palette_hex = ['0x6A6A6A','0x404040','0x7D7D7D','0x505050','0x2A2A2A','0x9B9B9B', '0xFFFFFF', '0x999999', '0x696969', '0x000000', '0x7F7F7F']
    palette_rgb = [hex_to_rgb(color) for color in palette_hex]

    pixel_tree = KDTree(palette_rgb)
    im = Image.open(filename+ ".png") #Can be many different formats.
    im = im.convert("RGBA")
    im = im.resize((new_w, new_h),Image.ANTIALIAS) # regular resize
    pix = im.load()
    pix_freqs = Counter([pix[x, y] for x in range(im.size[0]) for y in range(im.size[1])])
    pix_freqs_sorted = sorted(pix_freqs.items(), key=lambda x: x[1])
    pix_freqs_sorted.reverse()
    print(pix)
    outImg = Image.new('RGB', im.size, color='white')
    outFile = open(filename + '.txt', 'w')
    i = 0
    for y in range(im.size[1]):
        for x in range(im.size[0]):
            pixel = im.getpixel((x,y))
            print(pixel)
            if(pixel[3] < 200):
                outImg.putpixel((x,y), palette_rgb[0])
                outFile.write("%x\n" % (0))
                print(i)
            else:
                index = pixel_tree.query(pixel[:3])[1]
                outImg.putpixel((x,y), palette_rgb[index])
                outFile.write("%x\n" % (index))
            i += 1
    outFile.close()
    outImg.save(filename + "16x16.png")
