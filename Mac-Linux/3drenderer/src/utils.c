#include <stdio.h>
#include "utils.h"
#include "array.h"

void print_array_faces(face_t* toPrint)
{
    for (int i = 0; i < array_length(toPrint); i++)
    {
        printf("FACE %i: (%d, %d, %d)\n", i, toPrint[i].a, toPrint[i].b, toPrint[i].c);
    }
}