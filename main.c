#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

// Define a struct to hold RGB values
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RGB;

int main() {
    FILE *file = fopen("data/car.jpg", "rb");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Initialize JPEG decompression object
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    // Specify the source of the compressed data (the file)
    jpeg_stdio_src(&cinfo, file);

    // Read the JPEG header
    jpeg_read_header(&cinfo, TRUE);

    // Start decompression
    jpeg_start_decompress(&cinfo);

    // Get image dimensions
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int num_channels = cinfo.output_components; // Should be 3 for RGB

    // Allocate memory for the RGB matrix
    RGB **image_matrix = (RGB **)malloc(height * sizeof(RGB *));
    for (int i = 0; i < height; i++) {
        image_matrix[i] = (RGB *)malloc(width * sizeof(RGB));
    }

    unsigned char *row_pointer = (unsigned char *)malloc(width * num_channels);

    // Read scanlines one at a time
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, &row_pointer, 1);
        for (int x = 0; x < width; x++) {
            int index = x * num_channels;
            image_matrix[cinfo.output_scanline - 1][x].r = row_pointer[index];
            image_matrix[cinfo.output_scanline - 1][x].g = row_pointer[index + 1];
            image_matrix[cinfo.output_scanline - 1][x].b = row_pointer[index + 2];
        }
    }

    // Finish decompression
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    // Print the RGB content in a 2D table format
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("(%d, %d, %d) ", image_matrix[y][x].r, image_matrix[y][x].g, image_matrix[y][x].b);
        }
        printf("\n");
    }

    // Free the allocated memory
    for (int i = 0; i < height; i++) {
        free(image_matrix[i]);
    }
    free(image_matrix);
    free(row_pointer);

    fclose(file);
    return EXIT_SUCCESS;
}