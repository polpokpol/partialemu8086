#include <iostream>
#include <string>
#include <stdio.h>
#include "nmemonics.h"
// #include <format>
#include <bitset>

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





// per nmenemonic_number add another switch of reg r/m field
string left_and_right_encoding(u8 champiArray[], u8 index)
{
    // put it in a function
    u8 input = champiArray[index];
    for (int i = 0; i < 8; ++i) {
        if(nmemonics::umap.count(input)) {
            break;
        }
        input = input >> 1;
    }
    // put it in a function
    switch (input) {
        case 0b100010: {
            // left encoding
            bool destination = (0b00000010 & champiArray[index]) << 1;
            bool wide = (0b00000001 & champiArray[index]);

            // right encoding
            u8 input2 = champiArray[index + 1];
            u8 input_mod = (0b11000000 & input2) >> 6; // not yet implemented
            u8 input_reg = (0b00111000 & input2) >> 3;
            u8 input_rm = (0b00000111 & input2);
            string s_reg = "";
            string s_rm = "";
            switch (input_mod) {
                case 0b11: {
                    if (wide) {
                        s_reg += nmemonics::umap11_wide.at(input_reg);
                        s_rm += nmemonics::umap11_wide.at(input_rm);
                    }else {
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                        s_rm += nmemonics::umap11_not_wide.at(input_rm);
                    }
                    // ----
                    if (destination) {
                        return nmemonics::umap.at(input) + " " + s_reg + ", " + s_rm;
                    }
                    return nmemonics::umap.at(input) + " " + s_rm + ", " + s_reg;
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
            bool wide = (champiArray[index] & 0b00001000) >> 3;
            u8 input_reg = (champiArray[index] & 0b00000111);
            string data = "";
            string s_reg = "";
            if(wide) {
                s_reg += nmemonics::umap11_wide.at(input_reg);
                // wide data
                u16 the_data = (champiArray[index+2] << 8) + champiArray[index + 1];
                data += std::to_string(the_data);
                return  nmemonics::umap.at(input) + " " + s_reg + ", " + data;
            }
            s_reg += nmemonics::umap11_not_wide.at(input_reg);
            data += std::to_string(champiArray[index+1]);
            return  nmemonics::umap.at(input) + " " + s_reg + ", " + data;
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

    u8 champiHex[] = {0b10001001, 0b11011110, 0b10001000, 0b11000110, 0b10110001, 0b1100, 0xb5, 0xf4, 0xb9, 0x0c};

    string result = "";
    int i = 0;
    while(i < ArrayCount(champiHex))
    {
        result = left_and_right_encoding(champiHex, i);
        i+=2; // temporary as it depends
        std::cout << result << std::endl;
    }

    // string toli = "lala ";
    // toli = toli + std::to_string(10);
    // printf("lati: %d\n", nmemonics::umap.count(0b100010));
    // printf("champiHex: %d\n", champiHex[1]);
    // u8 chapli0 = 0b01;
    // u8 chapli1 = 0b10;
    // u16 latlat = chapli0 << 8;
    // latlat += chapli1;
    // cout << std::bitset<16>(latlat) << endl;

    return 0;

}
