#include <stdio.h>
#include <stdlib.h>
// The format of a HPEG file : [0xFFC0] [Length (2 bytes)] [Precision (1 byte)] [Height (2 bytes)] [Width (2 bytes)]
int main() {
    FILE *file = fopen("data/car.jpg", "rb");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    unsigned char buffer[2];
    unsigned char marker[2];
    int width = 0, height = 0;

    // Read the first two bytes to check for the JPEG SOI marker
    fread(buffer, 1, 2, file);
    if (buffer[0] != 0xFF || buffer[1] != 0xD8) {
        printf("Not a valid JPEG file\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Read through the file to find the SOF marker
    while (fread(marker, 1, 2, file) == 2) {
        if (marker[0] != 0xFF) {
            printf("Invalid marker\n");
            break;
        }

        if (marker[1] >= 0xC0 && marker[1] <= 0xC3) {
            // Found the SOF marker
            fseek(file, 3, SEEK_CUR); // Skip length and precision
            fread(buffer, 1, 2, file);
            printf("height0 %d : ,height1 %d\n ",buffer[0],buffer[1]);
            height = (buffer[0] << 8) + buffer[1];
            fread(buffer, 1, 2, file);
            width = (buffer[0] << 8) + buffer[1];
            break;
        } else {
            // Skip the segment
            fread(buffer, 1, 2, file);
            int length = (buffer[0] << 8) + buffer[1];
            fseek(file, length - 2, SEEK_CUR);
        }
    }

    if (width > 0 && height > 0) {
        printf("Width: %d, Height: %d\n", width, height);
    } else {
        printf("Could not find image dimensions\n");
    }

    fclose(file);
    return EXIT_SUCCESS;
}