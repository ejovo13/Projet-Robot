#include "piece.h"
/**========================================================================
 *!                           Define functions to access each byte
 *========================================================================**/
// const uint8_t g_B0 = 0b10000000;
// const uint8_t g_B1 = 0b01000000;
// const uint8_t g_B2 = 0b00100000;
// const uint8_t g_B3 = 0b00010000;
// const uint8_t g_B4 = 0b00001000;
// const uint8_t g_B5 = 0b00000100;
// const uint8_t g_B6 = 0b00000010;
// const uint8_t g_B7 = 0b00000001;

#define B0 0b10000000
#define B1 0b01000000
#define B2 0b00100000
#define B3 0b00010000
#define B4 0b00001000
#define B5 0b00000100
#define B6 0b00000010
#define B7 0b00000001

// organize the bits from left to right
const uint8_t g_BITS[8] = {B0, B1, B2, B3, B4, B5, B6, B7};

bool nth_bit(uint8_t __case, size_t __n) {
    if (__n > 7) return false; // if the __bit value is not between 0 and 7
    return (__case & g_BITS[__n]) == g_BITS[__n]; // return true if the nth bit is 1
}

// set bit to on (true) or off (false). To set a bit, I actually need a pointer to the object in question
bool set_bit(uint8_t *__case, size_t __n, bool __val) {
    *__case |= g_BITS[__n]; // assure that the nth bit of __case is on
    if (!__val) { // if we want to set the bit to off (0, false) then we perform a bit flip with bitwise XOR
        *__case ^= g_BITS[__n];
    }
    return nth_bit(*__case, __n);
}

// Print matrix as piece, expand every single uint8_t horizontally so that a 40x8 uint8_t matrix becomes a 40x40 bit matrix
void print_piece(const Matrix *__piece) {

    uint8_t case_ij = 0;

    for (size_t i = 0; i < __piece->nrows; i++) {
        printf("| ");
        for (size_t j = 0; j < __piece->ncols; j++) {
            // loop through every bit position
            printf("- ");
            case_ij = Matrix_at(__piece, i, j);

            for (size_t b = 0; b < 8; b++) {
                printf("%d ", nth_bit(case_ij, b));
            }

        }

        printf("|\n");
    }
}

/**========================================================================
 *!                           Piece object functions
 *========================================================================**/
// So a piece is really just a matrix, but instead of a 40 x 8 matrix, I want to think of
// the piece as a 40 x 40 bitmap.

// THE VALUE WE PASS MUST BE DIVISIBLE BY 8
Piece *Piece_new(size_t __ncases) {

    Piece *p = NULL;

    // check to make sure that __ncases is divisible by 8.
    if (__ncases % 8 == 0) {
        p = Matrix_new(__ncases, __ncases/8);
    } else {
        perror("__ncases must be divisible by 8");
    }

    return p;
}

void Piece_print(const Piece *__p) {
    print_piece(__p);
}

void Piece_free(Piece *__p) {
    Matrix_free(__p);
}

// set a specific cases du piece and return what value is retained there
bool Piece_set(Piece *__p, size_t __i, size_t __j, bool __val) {

    // the __i element is the same

    // for __j, we have to find the offset

    // first thing we figure out is how many uint8_ts to move over, and then we select the offset.

    size_t mat_j = (size_t) __j / 8; // this just works
    size_t offset = __j % 8; // number of bits that are offset

    uint8_t *u = Matrix_access(__p, __i, mat_j); // access the parent uint8_t
    return set_bit(u, offset, __val);

}









