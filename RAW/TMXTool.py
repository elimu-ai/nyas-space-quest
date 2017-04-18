import xml.etree.ElementTree as ET
import sys
import os

if len(sys.argv) < 4:
	print 'Usage: python TMXTool.py input scale output'
	quit()
#kScaleFactor = 0.5
#kFileName = "testLevel.tmx"
#kFileOutName = "../test.tmx"
kFileName = sys.argv[1]
kScaleFactor = float(sys.argv[2])
kFileOutName = sys.argv[3]

tree = ET.parse(kFileName)
root = tree.getroot()

#this gets ALL the elements with the pattern (all children)
objects = root.findall('.//object')
tilesets = root.findall('.//tileset')
images = root.findall('.//image')

def scaleAtt(element, attName, scale):
	if (element.get(attName)): # check if the element ha that attribute, else dont care
		element.set(attName, str(int(round(float(element.get(attName))*scale))))
	return

#adjust map scale
print 'Map:', kFileName, kScaleFactor
scaleAtt(root, "tilewidth", kScaleFactor)
scaleAtt(root, "tileheight", kScaleFactor)

#tilesets:
print 'tilesets:',
for tileset in tilesets:
	print '#',
	scaleAtt(tileset, "tilewidth", kScaleFactor)
	scaleAtt(tileset, "tileheight", kScaleFactor)

#images:
print ' '
print 'images:',
for image in images:
	print '#',
	scaleAtt(image, "width", kScaleFactor)
	scaleAtt(image, "height", kScaleFactor)

#objects:
print ' '
print 'objects:',
for obj in objects:
	print '#',
	scaleAtt(obj, "x", kScaleFactor)
	scaleAtt(obj, "y", kScaleFactor)
	scaleAtt(obj, "width", kScaleFactor)
	scaleAtt(obj, "height", kScaleFactor)

print ' '

#delete old file and save new one
if os.path.exists(kFileOutName):
	os.remove(kFileOutName)
tree.write(kFileOutName)

print 'Done!'