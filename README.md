# Fast Patch Integral

This project implements a fast patch integral image computation and a box blur filter using the integral image technique. The integral image is a powerful tool in computer vision that allows for quick summation of pixel values over rectangular regions, which is particularly useful for tasks like image filtering and feature extraction.

## Features

- **Integral Image Computation**: Efficiently computes the integral image from a 2D grayscale image, allowing for rapid summation of pixel values over arbitrary rectangular regions.
- **Box Blur Filter**: Applies a box blur to an image using the precomputed integral image, providing a fast and efficient way to blur images with a specified radius.

## Usage

The main program demonstrates the use of the integral image and box blur filter on a randomly generated grayscale image. The dimensions of the image and the blur radius can be adjusted in the `main` function.

## Requirements

- C compiler (e.g., GCC)
- Standard C library

## Compilation

To compile the program, use a C compiler like GCC:
