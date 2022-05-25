#!/bin/bash

INPUT="$1"
OUTPUT="$2"

if [ -z "$INPUT" ];then
    echo "Error: invalid arguments"
    echo "usage: ./makeplot.sh INPUT [OUTPUT]"
    exit 1
fi

if [ -z "$OUTPUT" ];then
    gnuplot -e "filename='$INPUT'" plot_result.plg
else
    gnuplot -e "filename='$INPUT'" -e "out='$OUTPUT.png'" plot_result.plg
fi
