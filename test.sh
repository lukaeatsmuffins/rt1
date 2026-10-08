#!/bin/bash
cmake --build build
./build/raytracer > image.ppm
convert image.ppm image.png
