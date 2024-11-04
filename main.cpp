#include <stdio.h>
#include <string.h>

typedef char unsigned u8;
typedef short unsigned u16;
typedef int unsigned u32;
typedef long long unsigned u64;

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;


#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

void *nmemonic(char haplan[], int corBin)
{
    // strcat(haplan, "dd");
    corBin = corBin >> 2;

    switch(corBin)
    {
        case 0b100010:
        {
            strcpy(haplan, "MOV");
        }break;
        default:
            return 0;
            break;
    };

    return 0;
}


int main(int argc, char **argv) {


    u8 champiHex[] = {0b10001001, 0b11011001};

    char lolo[4];
    nmemonic(lolo, champiHex[0]);
    // extract first

    // find the mov in array or hashmap?
    //code like case
        // after choosing the nmemonic you append it to
        // sub result
        // dont touch the last bit muna next tiem na yan
        // then append again the rest to sub result
        // after everything is over append it to result

    printf("Testo: %d %s \n", champiHex[0], lolo);
    return 0;
}
