#include "ejovo_matrix.h"
#include <string.h>

// perform literally 0 checks, just allocate the space for a new matrix
Matrix *matalloc(size_t __nrows, size_t __ncols) {

    Matrix *x = (Matrix *) malloc(sizeof(Matrix));
    MATRIX_TYPE *data = (MATRIX_TYPE *) malloc(sizeof(MATRIX_TYPE) * (__nrows * __ncols));
    x->data = data;
    x->nrows = __nrows;
    x->ncols = __ncols;

    return x;

}

Matrix * Matrix_new(int __nrows, int __ncols) {

    Matrix *x = (Matrix *) malloc(sizeof(Matrix));

    if(x) {

        if ( __nrows > 0 && __ncols > 0) {
            MATRIX_TYPE *data = (MATRIX_TYPE *) calloc(__nrows * __ncols, sizeof(MATRIX_TYPE));
            x->data = data;
            if (data) { // if the data is allocated properly
                x->nrows = __nrows;
                x->ncols = __ncols;
            } else {
                x->nrows = 0;
                x->ncols = 0;
            }
        } else {
            x->data = NULL;
            x->nrows = 0;
            x->ncols = 0;
        }

        return x;

    } else {

        return x;
    }
}

// Free the memory associated with the matrix and then free the pointer itself
void Matrix_free(Matrix *__A) {
    if (__A) {
        if (__A->data) {
            free(__A->data);
        }
        __A->ncols = 0;
        __A->nrows = 0;
        free(__A);
    }
}

bool Matrix_valid_bounds(const Matrix *__m, size_t __i, size_t __j) {
    return (__i < __m->nrows && __j < __m->ncols);
}


// return the value of the element at __m(__i, __j) [zero indexed]
// return -1 if bounds are not respected and prints an error to the screen
MATRIX_TYPE Matrix_at(const Matrix *__m, size_t __i, size_t __j) {

    if (Matrix_valid_bounds(__m, __i, __j)) {

        return __m->data[__i * __m->ncols + __j];

    } else {
        fprintf(stderr, "**WARNING** Trying to access array element out of bounds. (at)\n");

        // #if MATRIX_TYPE == Complex
            // return Complex_zero();
        // #else
        return -1;
        // #endif
    }
}

// Return element at __m[__i][__j] without checking bounds
MATRIX_TYPE matat(const Matrix *__m, size_t __i, size_t __j) {
    return __m->data[__i * __m->ncols + __j];
}

// set value of the element at __m(__i, __j) [zero indexed]
// return 0 if the bounds were respected, -1 elsewise
int Matrix_set(Matrix * __m, size_t __i, size_t __j, MATRIX_TYPE __value) {

    if (Matrix_valid_bounds(__m, __i, __j)) {
        __m->data[__i*__m->ncols + __j] = __value;
        return 0;
    } else {
        fprintf(stderr, "**WARNING** Trying to access array element out of bounds. (set)\n");
        return 1;
    }
}

// set value of the element at __m(__i, __j) without checking the indices
void matset(Matrix *__m, size_t __i, size_t __j, MATRIX_TYPE __value) {
    __m->data[__i * __m->ncols + __j] = __value;
}

// return a pointer to the element at __m(__i, __j) [zero indexed]
MATRIX_TYPE *Matrix_access(const Matrix * __m, size_t __i, size_t __j) {
    return matacc_check(__m, __i, __j);
}

// return a pointer to the element at __m(__i, __j) without checking the indices
MATRIX_TYPE *matacc(const Matrix *__m, size_t __i, size_t __j) {
    return __m->data + (__i * __m->ncols + __j);
}

// return a pointer to the element at __m(__i, __j) checking the indices
// returns null if the bounds are not respected
MATRIX_TYPE *matacc_check(const Matrix *__m, size_t __i, size_t __j) {

    if (Matrix_valid_bounds(__m, __i, __j)) {
        return __m->data + (__i*__m->ncols + __j);
    } else {
        fprintf(stderr, "**WARNING** Trying to access array element out of bounds. (access)\n");
        return NULL;
    }
}

void matprint(const Matrix *__m) {

    Matrix_summary(__m);
    for (size_t i = 0; i < __m->nrows; i++) {
        printf("| ");
        for (size_t j = 0; j < __m->ncols; j++) {
            printf("%4.4lf ", matat(__m, i, j));
        }

        printf("|\n");

    }

}

void Matrix_print_int(const Matrix *__m) {

    Matrix_summary(__m);
    for (size_t i = 0; i < __m->nrows; i++) {
        printf("| ");
        for (size_t j = 0; j < __m->ncols; j++) {
            printf("%u ", Matrix_at(__m, i, j));
        }

        printf("|\n");
    }
}

void Matrix_print(const Matrix *__m) {

    Matrix_summary(__m);
    for (size_t i = 0; i < __m->nrows; i++) {
        printf("| ");
        for (size_t j = 0; j < __m->ncols; j++) {
            printf("%4.4lf ", Matrix_at(__m, i, j));
        }

        printf("|\n");
    }
}

void Matrix_summary(const Matrix *__m) {
    printf("%lu x %lu matrix\n", __m->nrows, __m->ncols);
}

// return true if __A and __B have the same size and all of the elements are identical
bool matcmp(const Matrix *__A, const Matrix *__B) {

    if ( __A->nrows != __B->nrows || __A->ncols != __B->ncols) {
        return false;
    }
    return matcmp_bytes(__A, __B);
}

// compare the bytes of the data using memcmp
bool matcmp_bytes(const Matrix *__A, const Matrix *__B) {
    return memcmp((void *) __A->data, (void *) __B->data, sizeof(MATRIX_TYPE) * (__A->nrows * __A->ncols)) == 0;
}



// Copy the bytes
// this is a utility function and should not be used by the end user
static bool matcpy(Matrix * __dest, const Matrix * __src) {

    // Copy the bytes of __src->data into __dest->data
    memcpy(__dest->data, __src->data, sizeof(MATRIX_TYPE)*(__src->nrows * __src->ncols));
    __dest->ncols = __src->ncols;
    __dest->nrows = __src->nrows;
    if(__dest && __src && __dest->data) { // if all the pointers are not null, return true
        return  true;
    } else {
        return false;
    }
}


// copy the contents of matrix __src into __dest
Matrix * matclone(const Matrix * __src) {

    Matrix * clone = NULL;

    clone = Matrix_new(__src->nrows, __src->ncols);
    if (clone) {
        matcpy(clone, __src);
    }

    return clone;
}

Matrix * Matrix_clone(const Matrix * __src) {
    return matclone(__src);
}


void matfill(Matrix *__A, const MATRIX_TYPE __value) {

    for (size_t i = 0; i < __A->nrows; i++) {
        for (size_t j = 0; j < __A->ncols; j++) {
            matset(__A, i, j, __value);
        }
    }
}

// fill matrix with a single value
void Matrix_fill(Matrix * __A, const MATRIX_TYPE value) {

    for (size_t i = 0; i < __A->nrows; i++) {
        for (size_t j = 0; j < __A->ncols; j++) {
            Matrix_set(__A, i, j, value);
        }
    }
}


// matrix of all ones
Matrix * Matrix_ones(size_t __nrows, size_t __ncols) {

    Matrix * m = Matrix_new(__nrows, __ncols);
    matfill(m, 1);

    return m;

}


Matrix * Matrix_ij(size_t __nrows, size_t __ncols) {

    Matrix * m = Matrix_new(__nrows, __ncols);
    if(m) {
        for (size_t i = 0; i < __nrows; i++) {
            for (size_t j = 0; j < __ncols; j++) {
                matset(m, i, j, i + j + 1);
            }
        }
    }

    return m;

}

Matrix * Matrix_value(size_t __nrows, size_t __ncols, MATRIX_TYPE __value) {

    Matrix * m = Matrix_new(__nrows, __ncols);
    matfill(m, __value);

    return m;
}


size_t Matrix_size(const Matrix *__A) {
    return __A->nrows * __A->ncols;
}

bool Matrix_is_square(const Matrix *__A) {
    return __A->nrows == __A->ncols;
}



// Copy the matrix __src into the submatrix of __dest prescribed by the start and end indices
int matcpyele(Matrix * __dest, size_t __istart, size_t __iend, size_t __jstart, size_t __jend, Matrix * __src) {

    // If the submatrix is not contained in __des
    if (__iend < __istart || __jend < __jstart || __iend >= __dest->nrows || __jend >= __dest->ncols) {
        perror("Selected submatrix is not contained within Parent Matrix\n");
        return -1;
    }

    // If the __src is not the same size as the selected submatrix
    if (__src->nrows != (__iend - __istart + 1) || __src->ncols != (__jend - __jstart + 1)) {
        perror("Selected submatrix not the same size as the src matrix to copy\n");
        return -2;
    }

    matcpyele_unsafe(__dest, __istart, __iend, __jstart, __jend, __src);


    return 0;
}

// Copy the elements of __src into the submatrix of __dest prescribed by the start and end indices WITHOUT CHECKING THE BOUNDS
void matcpyele_unsafe(Matrix *__dest, size_t __istart, size_t __iend, size_t __jstart, size_t __jend, Matrix *__src) {

    for (size_t i = __istart, irow = 0; i <= __iend; i++, irow++) {
        for (size_t j = __jstart, jcol = 0; j <= __jend; j++, jcol++) {
            Matrix_set(__dest, i, j, Matrix_at(__src, irow, jcol));
        }
    }
    // optimized for row-major access

}

