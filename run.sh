#!/bin/bash

clear

make -s clean
make -s

./p

convert scene.ppm abc.png
