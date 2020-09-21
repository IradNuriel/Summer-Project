# import
import os
import PIL
import os.path
from PIL import Image

# camera ID
camID = int(input())

# path to directory
path = "C:\\_Documents\\photos8\\{}\\men".format(camID)

# target size for the images
res = (960,720)

# target aspect ratio
ratio = 960.0 / 720.0

# for every photo in the directory (sorted alphanumerically), resize it and give it a proper name
for i,file in enumerate(os.listdir(path)):
	# open the image
	f_img = path + "\\" + file
	img = Image.open(f_img)
	
	# resize the image
	width, height = img.size
	img = img.crop((0, 0, height * ratio, height))
	img = img.resize(res)
	
	# save the image under a new name
	img.save(path + "\\{}_{}.jpeg".format(camID,i))