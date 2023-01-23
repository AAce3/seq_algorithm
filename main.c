#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "algorithm.h"

int main()
{
    char str[] = "ATCGTGG asdfasdfasdfasd ATTGTGG";
    size_s best_alignment = 0;
    float best_identity = 0.0f;
    align(str, strlen(str), 1, 29, 10, &best_alignment, &best_identity);
    printf("(%d, %f)", best_alignment, best_identity);
    return 0;
}
