// This is a program that will read a matrix in from a file and display the piece that is encoded in this matrix

#include "ejovo_matrix.h"
#include "piece.h"
#include <stdio.h>
#include <stdint.h>


// we know that the matrix we are trying to read is 80 x 10

// then we want to print that to screen

int main(int argc, char **argv) {

    // printf("First argument: %s\n", argv[0]);
    // printf("Second argument: %s\n", argv[1]);

    // char *f_name = argv[1];

    // FILE *file = fopen(f_name, "r");
    // char line[256] = {0};

    Matrix *m = Matrix_new(80, 10);
    int x = 0;
    int count = 0;
    size_t i = 0;
    size_t j = 0;

    while(scanf("%d", &x) == 1) {

        i = count / 10;
        j = count % 10;

        Matrix_set(m, i, j, x);

        // printf("count: %d ", count);
        // printf("Matrix_set(m, %u, %u, %u)\n", i, j, x);
        count = count + 1;

    }

    // Matrix_print_int(m);

    // now we want to print the matrix in the terminal as a laberynth

    // fclose(file);

    uint8_t case_ij = 0;

    for (size_t i = 0; i < m->nrows; i++) {
        // printf("| ");
        for (size_t j = 0; j < m->ncols; j++) {
            // loop through every bit position
            // printf("- ");
            case_ij = Matrix_at(m, i, j);

            for (size_t b = 0; b < 8; b++) {
                if ( nth_bit(case_ij, b) ) {
                    printf("\u2588");
                } else {
                    printf(" ");
                }
            }
        }
        // printf("|\n");
        printf("\n");
    }

    Matrix_free(m);

    return 0;
}