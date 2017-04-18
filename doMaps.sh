#!/bin/zsh
echo Generating scaled maps
maps=(RAW/*.tmx)
for map in ${maps[@]}; do
	echo "->" $map
	python RAW/TMXTool.py $map 0.5 Resources/SD/${map##*/}
	python RAW/TMXTool.py $map 2.0 Resources/HDR/${map##*/}
	cp $map Resources/HD/${map##*/}
done

exit 0