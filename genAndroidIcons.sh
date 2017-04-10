#!/bin/bash
#Generates the icons for android app (requires imagemagick)
IMAGE=icon.png
BGCOLOR="#1F3A69"
# Directories
# -------------------------------------------
ICONDIR=proj.android/res
echo $ICONDIR
eval mkdir -p "$ICONDIR"
# Show progress
set -x
# -------------------------------------------
# App Icons
# -------------------------------------------
c='convert -background NONE'
$c "$IMAGE" -resize 96x96   "$ICONDIR/drawable-xhdpi/icon.png"
$c "$IMAGE" -resize 72x72   "$ICONDIR/drawable-hdpi/icon.png"
$c "$IMAGE" -resize 36x36   "$ICONDIR/drawable-ldpi/icon.png"
$c "$IMAGE" -resize 48x48   "$ICONDIR/drawable-mdpi/icon.png"
