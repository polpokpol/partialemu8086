#include <iostream>
#include <string>
#include <stdio.h>
#include "nmemonics.h"

using std::string;

typedef char unsigned u8;
typedef short unsigned u16;
typedef int unsigned u32;
typedef long long unsigned u64;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

// void nmemonics(Operation op, s8 *string_input) {
//     switch (op) {
//         case Operation::OP_MOV: {
//             printf("adsad\n");
//         } break;
//         default:
//             break;
//     };
// }

struct opcode_dw {
    string nmemonic;
    bool destination;
    bool wide;
};

void extractor(u8 input, opcode_dw *instruction) {
    instruction->destination = (0b00000010 & input);
    instruction->wide = (0b00000001 & input);
    input = input >> 2;
    switch (input) {
        case 0b100010: {
            instruction->nmemonic = nmemonics::umap.at(input);
        }
        default: break;
    };
}


int main(int argc, char **argv)
{
    u8 champiHex[] = {0b10001001, 0b11011001, 0b10001001, 0b11011001, 0b10001001, 0b11011001};
    string result = "";
    // result = nmemonics::umap.at(champiHex[0]);
    int i = 0;

    // while(i < ArrayCount(champiHex))
    // {
    //     // std::string sub_result = extractor(champiHex[i]);
    //     string sub_result = nmemonics::umap.at(champiHex[i]);
    //
    //     result = result + sub_result + "\n";
    //     i+=2; // temporary as it depends
    // }
    opcode_dw op0;
    extractor(champiHex[0], &op0);

    // nmemonics(Operation::OP_MOV, extractor(champiHex[0]));
    // std::cout << result << std::endl;
    // std::cout << (0b00000010 & champiHex[0]) << std::endl;
    std::cout << "test: " << (op0.wide) << std::endl;
    return 0;

}
