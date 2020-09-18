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

files = ["IBlockPiece", "LBlockPiece", "JBlockPiece", "TBlockPiece", "SBlockPiece", "SBlockPiece", "ZBlockPiece", "OBlockPiece"];


for filename in files:
    new_w = 16
    new_h = 16
    palette_hex = ['0x07A3A2','0x0BEDEC','0x056768','0x078184','0x31F5F1','0xE45F00','0xFF9847','0x903C00','0xB44A00','0xFFB884','0xC114C5','0xEA50EC','0x790D7C','0x98119B','0xED7FF2','0x017EDA','0x3DACFF','0x005087','0x0163AC','0x75C4FD','0x81AD00','0xB9F902','0x516c01','0x658700','0xC6FE29','0xE60000','0xFF4F4F','0x900000','0xB60000','0xFF8383','0xCDAB01','0x826A00','0xA18700','0xFFE260','0xFFDD2B','0x44FFFF', '0x000000', '0x3F48CC', '0xFF9115', '0xFFFF44', '0x44FF44', '0xFF44FF', '0xED1C24']
    palette_rgb = [hex_to_rgb(color) for color in palette_hex]

    pixel_tree = KDTree(palette_rgb)
    im = Image.open("../sprite_originals/" + filename+ ".png") #Can be many different formats.
    im = im.convert("RGBA")
    im = im.resize((new_w, new_h),Image.ANTIALIAS) # regular resize
    pix = im.load()
    pix_freqs = Counter([pix[x, y] for x in range(im.size[0]) for y in range(im.size[1])])
    pix_freqs_sorted = sorted(pix_freqs.items(), key=lambda x: x[1])
    pix_freqs_sorted.reverse()
    print(pix)
    outImg = Image.new('RGB', im.size, color='white')
    outFile = open("../sprite_bytes/" + filename + '.txt', 'w')
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
    outImg.save("../sprite_originals/" + filename + "Sized.png")

filenames = ["IBlockPieceSized", "JBlockPieceSized", "LBlockPieceSized", "OBlockPieceSized", "SBlockPieceSized", "TBlockPieceSized", "ZBlockPieceSized"]




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



