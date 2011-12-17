#!/bin/sh
#
# Generates a header file that encodes the image resources into
# C-friendly byte arrays. This allows for the resources to be
# "compiled into" the final executable.
#
# This will only work where the xxd tool is available:
#
cd `dirname $0`

image_files=$(ls *.png)
header_file=fvw_resources.h
header_comments="// this file auto-generated on `date`"

echo "Generating $header_file for images..."
echo $header_comments > $header_file

for image_file in $image_files
do
    echo "// resource for $image_file" >> $header_file
    xxd -i $image_file 1>> $header_file
    echo >> $header_file
    echo " ... processed $image_file"
done

echo "Finished generating $header_file"
