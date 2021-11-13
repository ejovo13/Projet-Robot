#include "ejovo_matrix.h"
#include "piece.h"

int main()
{
    Piece *p = Piece_new(8);
    Piece_set(p,2,3,true);
    Piece_print(p);
    printf("%d",Piece_get(p,2,3));
}