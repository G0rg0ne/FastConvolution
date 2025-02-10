#include <stdio.h>
#include <stdlib.h>

void load_raw_image(const char* file_name, int width, int height) {
    FILE* fp = fopen(file_name, "rb");
    if (!fp) {
        printf("Error: Could not open file %s\n", file_name);
        exit(1);
    }

    // Allocate memory for RGB data
    unsigned char* buffer = (unsigned char*)malloc(3 * width * height);
    if (!buffer) {
        printf("Error: Memory allocation failed\n");
        fclose(fp);
        exit(1);
    }

    // Read the raw data
    fread(buffer, 3, width * height, fp);
    fclose(fp);

    printf("Raw image loaded successfully (Width: %d, Height: %d)\n", width, height);

    // Free memory
    free(buffer);
}

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Usage: %s <image_path> <width> <height>\n", argv[0]);
        return -1;
    }

    const char* image_path = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);

    load_raw_image(image_path, width, height);
    return 0;
}
