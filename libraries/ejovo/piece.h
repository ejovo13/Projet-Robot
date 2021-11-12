// Les fonctions qui gere la creation des pieces et l'acces aux elements
#pragma once

#ifndef PIECEH
#define PIECEH

#include "ejovo_matrix.h"
#include "stdint.h"
#include "stdbool.h"

// A piece is a 40 x 5 Array

#define PIECE_ROWS 40
#define PIECE_COLS (PIECE_ROWS/8)

// extern const uint8_t g_B0;
// extern const uint8_t g_B1;
// extern const uint8_t g_B2;
// extern const uint8_t g_B3;
// extern const uint8_t g_B4;
// extern const uint8_t g_B5;
// extern const uint8_t g_B6;
// extern const uint8_t g_B7;
// where g_B0 = 10000000, g_B1 = 01000000, etc.

extern bool nth_bit(uint8_t __case, size_t __n);

// set bit to on (true) or off (false). To set a bit, I actually need a pointer to the object in question
extern bool set_bit(uint8_t *__case, size_t __n, bool __val);

// Print matrix as piece, expand every single uint8_t horizontally so that a 40x8 uint8_t matrix becomes a 40x40 bit matrix
extern void print_piece(const Matrix *__piece);

/**========================================================================
 *!                           Piece functions
 *========================================================================**/
typedef Matrix Piece;

// THE VALUE WE PASS MUST BE DIVISIBLE BY 8
extern Piece *Piece_new(size_t __ncases);

extern void Piece_print(const Piece *__p);

extern void Piece_free(Piece *__p);

extern bool Piece_set(Piece *__p, size_t __i, size_t __j, bool __val);

extern bool Piece_get(const Piece *__p, size_t __i, size_t __j);




#endif