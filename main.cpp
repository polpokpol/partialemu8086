#include <iostream>
#include <string>
#include <stdio.h>
#include "nmemonics.h"
#include <bitset>

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

string right_encoding(u8 input, opcode_dw *instruction) {
    u8 input_mod = (0b11000000 & input); // not yet implemented
    u8 input_reg = (0b00111000 & input) >> 3;
    u8 input_rm = (0b00000111 & input);
    string s_reg = "";
    string s_rm = "";

    // input mod, soon
    if (instruction->wide == true) {
        s_reg += nmemonics::umap11_wide.at(input_reg);
        s_rm += nmemonics::umap11_wide.at(input_rm);
    }else {
        s_reg += nmemonics::umap11_not_wide.at(input_reg);
        s_rm += nmemonics::umap11_not_wide.at(input_rm);
    }
    // ----
    if (instruction->destination == true) {
        return s_reg + " " + s_rm;
    }
    return s_rm + ", " + s_reg;

}

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
    int i = 0;
    while(i < ArrayCount(champiHex))
    {
        opcode_dw op0;
        extractor(champiHex[i], &op0);
        string sub_result = op0.nmemonic + " ";
        string temp = right_encoding(champiHex[i + 1], &op0);
        sub_result = sub_result + temp;
        result = result + sub_result + "\n";
        i+=2; // temporary as it depends
    }
    std::cout << result << std::endl;
    return 0;

}
