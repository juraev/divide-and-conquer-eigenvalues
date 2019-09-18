#include "matrix.h"

#include <stdio.h>

#define MAT_VAL "%5.2f"

#define MAT_VAL_LEN 7

void matrix_print_part(const matrix_type_t mat, unsigned h_min, unsigned w_min, unsigned h_max, unsigned w_max)
{
    unsigned i, j;
    for (i = h_min; i < h_max; i++) {
        printf("|");
        for (j = w_min; j < w_max - 1; j++) {
            printf(MAT_VAL" ", matrix_get(mat, i, j));
        }
        printf(MAT_VAL"|\n", matrix_get(mat, i, w_max - 1));
    }    
}

void matrix_print(const matrix_type_t mat)
{
    matrix_print_part(mat, 0, 0, matrix_height(mat), matrix_width(mat));
}

matrix_type_t read_matrix_from_file(const char*fname)
{
    matrix_type_t mat;
    FILE*f;
    unsigned h, w;
    unsigned i, j;
    
    f = fopen(fname, "rb");
    if (f == NULL) {
        goto err0;
    }

    if (fscanf(f, "%u %u\n", &h, &w) != 2) {
        goto err1;
    }

    mat = matrix_create(h, w, 0.0);
    if (mat == NULL) {
        goto err1;
    }

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            double cell;
            if (fscanf(f, "%lf", &cell) != 1) {
                goto err2;
            }
            matrix_set(mat, i, j, cell);
        }
    }

    fclose(f);

    return mat;

 err2:
    matrix_free(mat);
 err1:
    fclose(f);
    /* remove(fname); */
 err0:
    return NULL;
}

int save_matrix_to_file(const matrix_type_t mat, const char*fname)
{
    int r;
    FILE*f;
    unsigned i, j;

    f = fopen(fname, "wb");
    if (f == NULL) {
        r = -1;
        goto err0;
    }

    fprintf(f, "%u %u\n", matrix_height(mat), matrix_width(mat));

    for (i = 0; i < matrix_height(mat); i++) {
        for (j = 0; j < matrix_width(mat) - 1; j++) {
            fprintf(f, "%lf ", matrix_get(mat, i, j));
        }
        fprintf(f, "%lf\n", matrix_get(mat, i, matrix_width(mat) - 1));
    }

    return 0;

 err0:
    return r;
}