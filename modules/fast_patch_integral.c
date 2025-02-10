#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
 * Builds the integral image from a 2D grayscale image.
 * 
 * Input:
 *   - in     : pointer to the input image data (width*height elements)
 *   - width  : width of the image
 *   - height : height of the image
 * 
 * Output:
 *   - integral: pointer to the integral image (must be allocated externally)
 *
 * The integral image at (x, y) contains the sum of all pixels
 * in the rectangle from (0,0) to (x,y) inclusive:
 * 
 *    integral[y * width + x] =
 *       in[y * width + x]
 *     + integral[(y-1) * width + x]
 *     + integral[y * width + (x-1)]
 *     - integral[(y-1) * width + (x-1)]
 */
void buildIntegralImage(const uint8_t *in, int width, int height, unsigned long *integral)
{
    // First row
    unsigned long runningSum = 0;
    for (int x = 0; x < width; x++) {
        runningSum += in[x];
        integral[x] = runningSum;
    }

    // Subsequent rows
    for (int y = 1; y < height; y++) {
        runningSum = 0;
        for (int x = 0; x < width; x++) {
            runningSum += in[y * width + x];
            // integral above plus current row sum
            integral[y * width + x] = integral[(y - 1) * width + x] + runningSum;
        }
    }
}

/*
 * Helper function to get the sum of a sub-rectangle from (x1, y1) to (x2, y2)
 * inclusive using the integral image. Make sure x1, y1, x2, y2 are within
 * bounds of the image.
 * 
 * integral sum of region:
 *   sum = II(x2, y2)
 *       - II(x1-1, y2)
 *       - II(x2, y1-1)
 *       + II(x1-1, y1-1)
 */
unsigned long getRegionSum(const unsigned long *integral,
                           int width, int height,
                           int x1, int y1, int x2, int y2)
{
    // Clamp to boundaries, just in case
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= width)  x2 = width - 1;
    if (y2 >= height) y2 = height - 1;

    unsigned long A = 0, B = 0, C = 0, D = 0;

    D = integral[y2 * width + x2];
    if (x1 > 0) {
        C = integral[y2 * width + (x1 - 1)];
    }
    if (y1 > 0) {
        B = integral[(y1 - 1) * width + x2];
    }
    if (x1 > 0 && y1 > 0) {
        A = integral[(y1 - 1) * width + (x1 - 1)];
    }

    return (D - C - B + A);
}

/*
 * Box blur using the integral image.
 * 
 * Inputs:
 *   - in       : original grayscale image
 *   - out      : blurred output image (must be allocated)
 *   - width    : image width
 *   - height   : image height
 *   - integral : integral image (same width, height) already computed
 *   - radius   : blur "radius" determines the bounding box side.
 *                The actual box side is (2*radius + 1).
 */
void boxBlur(const uint8_t *in, uint8_t *out, 
             int width, int height,
             const unsigned long *integral,
             int radius)
{
    int diameter = 2 * radius + 1;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            // Compute bounding box corners
            int x1 = x - radius;
            int y1 = y - radius;
            int x2 = x + radius;
            int y2 = y + radius;

            // Get region sum using the integral image
            unsigned long regionSum = getRegionSum(integral, width, height,
                                                   x1, y1, x2, y2);
            // Compute the number of valid pixels in that region
            // We can clamp the boundaries or recalc the actual bounding box
            // that was used.
            if (x1 < 0) x1 = 0;
            if (y1 < 0) y1 = 0;
            if (x2 > width  - 1) x2 = width  - 1;
            if (y2 > height - 1) y2 = height - 1;

            int rectWidth  = (x2 - x1 + 1);
            int rectHeight = (y2 - y1 + 1);
            int area = rectWidth * rectHeight;

            // Average
            uint8_t val = (uint8_t)(regionSum / area);

            out[y * width + x] = val;
        }
    }
}

int main(void)
{
    // Example dimensions
    int width = 8;
    int height = 6;
    int radius = 1;  // adjust as needed

    // Allocate memory for images and integral
    uint8_t *input = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    uint8_t *output = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    unsigned long *integral = (unsigned long*)malloc(width * height * sizeof(unsigned long));

    if (!input || !output || !integral) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // Fill input with some sample data (e.g., random)
    // In practice, you'd read an actual image here.
    srand(42);
    for (int i = 0; i < width * height; i++) {
        input[i] = rand() % 256; // Random pixel [0..255]
    }

    // Build the integral image
    buildIntegralImage(input, width, height, integral);

    // Perform the box blur
    boxBlur(input, output, width, height, integral, radius);

    // Print input and output for demonstration
    // (row by row for clarity)
    printf("Input Image:\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%3d ", input[y * width + x]);
        }
        printf("\n");
    }

    printf("\nBlurred Image:\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            printf("%3d ", output[y * width + x]);
        }
        printf("\n");
    }

    // Cleanup
    free(input);
    free(output);
    free(integral);

    return 0;
}
