#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <image_path>\n", argv[0]);
        return -1;
    }

    // Variables to hold image data
    int width, height, channels;
    const char* image_path = argv[1];

    // Load the image
    unsigned char* img_data = stbi_load(image_path, &width, &height, &channels, 0);
    if (!img_data) {
        printf("Error: Could not load image %s\n", image_path);
        return -1;
    }

    // Print image information
    printf("Image loaded successfully!\n");
    printf("Width: %d, Height: %d, Channels: %d\n", width, height, channels);

    // Free image memory
    stbi_image_free(img_data);

    return 0;
}
