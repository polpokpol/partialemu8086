#include <iostream>
#include <string>
#include <stdio.h>
#include "nmemonics.h"
#include <format>

using std::string;
using std::cout;
using std::endl;

typedef char unsigned u8;
typedef short unsigned u16;
typedef int unsigned u32;
typedef long long unsigned u64;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))



string left_encoding_extractor(u8 input) {
    for (int i = 0; i < 8; ++i) {
        if(nmemonics::umap.count(input)) {
            break;
        }
        input = input >> 1;
    }
    string result = "";
    switch (input) {
        case 0b100010: {
            result += nmemonics::umap.at(input) + " ";
            instruction->nmemonic_number = input;
            instruction->destination = (0b00000010 & input) << 1;
            instruction->wide = (0b00000001 & input);
        } break;
        case 0b1011: {
            instruction->nmemonic = nmemonics::umap.at(input);
            instruction->nmemonic_number = input;
            instruction->destination = true; // always 1 or true
            instruction->wide = (0b00001 & input) << 3;
        } break;
        default: break;

    };
}

// per nmenemonic_number add another switch of reg r/m field
string left_and_right_encoding(u8 *champiArray, u8 index) {
    u8 input = champiArray[index];
    for (int i = 0; i < 8; ++i) {
        if(nmemonics::umap.count(input)) {
            break;
        }
        input = input >> 1;
    }
    switch (input) {
        case 0b100010: {
            // left encoding
            bool destination = (0b00000010 & input) << 1;
            bool wide = (0b00000001 & input);

            // right encoding
            u8 input_mod = (0b11000000 & input) >> 6; // not yet implemented
            u8 input_reg = (0b00111000 & input) >> 3;
            u8 input_rm = (0b00000111 & input);
            string s_reg = "";
            string s_rm = "";
            switch (input_mod) {
                case 0b11: {
                    if (instruction->wide) {
                        s_reg += nmemonics::umap11_wide.at(input_reg);
                        s_rm += nmemonics::umap11_wide.at(input_rm);
                    }else {
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                        s_rm += nmemonics::umap11_not_wide.at(input_rm);
                    }
                    // ----
                    if (instruction->destination == true) {
                        return s_reg + ", " + s_rm;
                    }
                    return s_rm + ", " + s_reg;
                } break;
                case 0b10: {

                } break;
                case 0b01: {

                } break;
                default: {

                } break;
            }
        } break;
        case 0b1011: {
            // if(instruction->wide) { } // not yet implemented. // we need an array of input
            string data = std::to_string(input);
            u8 input_reg = (0b00111000 & input) >> 3;
            string s_reg = "";
            return data;
        } break;
        default: break;

    };


}


int main(int argc, char **argv)
{
    // u8 champiHex[] = {0b10001001, 0b11011001, 0x88, 0xe5,
    // 0x89, 0xda, 0x89, 0xde, 0x89, 0xfb, 0x88, 0xc8, 0x88, 0xed, 0x89, 0xc3,
    //     0x89, 0xf3, 0x89, 0xfc, 0x89, 0xc5 }; // temporary

    // u8 champiHex[] = {0b10001001, 0b11011110, 0b10001000, 0b11000110, 0b10110001, 0b1100, 0b10110101,
    //     0b11110100, 0b10111001, 0b1100, 0b0, 0b10111001, 0b11110100, 0b11111111, 0b10111010,
    //     0b1101100, 0b1111, 0b10111010, 0b10010100, 0b11110000, 0b10001010, 0b0, 0b10001011, 0b11011,
    //     0b10001011, 0b1010110, 0b0, 0b10001010, 0b1100000, 0b100, 0b10001010, 0b10000000, 0b10000111,
    //     0b10011, 0b10001001, 0b1001, 0b10001000, 0b1010, 0b10001000, 0b1101110, 0b0
    // }; // temporary2, we will use external file later

    u8 champiHex[] = {0b10110001, 0b1100,};

    string result = "";
    int i = 0;
    while(i < ArrayCount(champiHex))
    {
        // string left_sub_result =  left_encoding_extractor(champiHex[i]);
        // string sub_result = op0.nmemonic + " ";
        string temp = right_encoding(&champiHex, i);
        sub_result = sub_result + temp;
        result = result + sub_result + "\n";
        i+=2; // temporary as it depends
    }
    std::cout << result << std::endl;
    // for(int i = 0; i < ArrayCount(champiHex); i++) {
    //     // cout << champiHex[i] << " ";
    //     printf("%d ", champiHex[i]);
    // }
    // printf("champiHex: %d\n\n", champiHex[1]);
    // for loop reversed test
    // u8 a = 0b11111111;
    // for (int i = 0; i < 8; ++i) {
    //     a = a >> 1;
    //     printf("champiHex: %d\n", a);
    // }
    // printf("lati: %d", nmemonics::umap.count(0b100010));

    return 0;

}
