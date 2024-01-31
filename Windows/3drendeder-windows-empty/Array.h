#ifndef ARRAY_H
#define ARRAY_H

#define ArrayPush(array, value)                                              \
    do {                                                                      \
        (array) = ArrayHold((array), 1, sizeof(*(array)));                   \
        (array)[ArrayLength(array) - 1] = (value);                           \
    } while (0);

void* ArrayHold(void* array, int count, int itemSize);
int ArrayLength(void* array);
void ArrayFree(void* array);

#endif