#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define WIDTH 1200
#define HEIGHT 1200

int main(int argc, char** argv) {
    clock_t start = clock();

    // Remove MPI initialization
    double real_min = -1.0;
    double real_max = 1.0;
    double imag_min = -1.0;
    double imag_max = 1.0;
    int max_counter = 1000;

    int sub_region = HEIGHT; 
    int start_row = 0;
    int end_row = HEIGHT;

    int* row = (int*)malloc(sizeof(int) * WIDTH);
    int* data = (int*)malloc(sizeof(int) * WIDTH * HEIGHT);

    for (int y = start_row; y < end_row; y++) {
        for (int x = 0; x < WIDTH; x++) {
            double c_real = real_min + (real_max - real_min) * x / WIDTH;
            double c_imag = imag_min + (imag_max - imag_min) * y / HEIGHT;
            double z_real = 0.0;
            double z_imag = 0.0;

            int counter = 0;
            while (z_real * z_real + z_imag * z_imag < 4.0 && counter < max_counter) {
                double next_z_real = z_real * z_real - z_imag * z_imag + c_real;
                double next_z_imag = 2.0 * z_real * z_imag + c_imag;
                z_real = next_z_real;
                z_imag = next_z_imag;
                counter++;
            }

            if (counter == max_counter) {
                row[x] = 0;
            } else {
                row[x] = counter % 256;
            }
        }

        int r_index = (y - start_row) * WIDTH;

        for (int x = 0; x < WIDTH; x++) {
            data[r_index + x] = row[x];
        }
    }

    free(row);

    FILE* fp = fopen("mandelbrot.pgm", "wb");
    fprintf(fp, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        fputc(data[i], fp);
    }
    fclose(fp);
    free(data);

    clock_t end = clock();
    double elapsed =(double)(end - start);
    printf("Elapsed time: %f seconds\n", elapsed);

    return 0;
}