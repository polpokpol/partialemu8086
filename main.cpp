#include <cstddef>
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

string byte_or_word(bool wide){if(wide){return "word";} return "byte";}
string _nmemonics_result_get(u8 input, u8 input_modifier=256){
    string result = nmemonics::umap.at(input) + " ";
    if(input_modifier == 0b101){
        string result = nmemonics::umap.at(0b001010) + " ";
        return result;
    }
    return result;
}

// temporary as we will have u16 and negative later
string _transform_int_to_string(u16 data0, u16 data1 = 0)
{   
    string result = "";
    result += std::to_string(data0);
    return result;
}

string _bracketer(string data_string){
    data_string = "[" + data_string +']';
    return data_string;
}


// per nmenemonic_number add another switch of reg r/m field
string left_and_right_encoding(u8 champiArray[], u8 counter[])
{
    u16 index = u16(counter[0]);
    // put it in a function
    u8 input = 0 + champiArray[index];
    // printf("saddsaads %d\n", champiArray[index]);
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
                    if(wide){
                        s_reg += nmemonics::umap11_wide.at(input_reg); 
                    }else{
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                    }
                    counter[0] +=2;
                    string stringified_integer_data = "";
                    if( champiArray[index + 2] != 0){
                        u16 the_0b10_integer_summation = (champiArray[index + 3] << 8) ^ champiArray[index + 2];
                        stringified_integer_data = " + " +  _transform_int_to_string(the_0b10_integer_summation);
                    }

                    s_rm += nmemonics::umap_rm_wide.at(input_rm);
                    if (destination) {
                        return nmemonics::umap.at(input) + " " + s_reg + ", " + _bracketer((s_rm +  stringified_integer_data));
                    }
                    return nmemonics::umap.at(input) + " " + _bracketer((s_rm +  stringified_integer_data)) + ", " + s_reg;
                } break;
                // ------------------
                case 0b01: {
                    // counter[1] = 3; // this is for debug only cause i did not configure vscode debug yet
                    if(wide){
                        s_reg += nmemonics::umap11_wide.at(input_reg); 
                        // counter[1] = 122;
                    }else{
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                        counter[1] = 122;
                    }
                    counter[0] +=1;
                    string stringified_integer_data = "";
                    if( champiArray[index + 2] != 0){ // CHECK IF RAW INT IS ZERO
                        stringified_integer_data = " + " +  _transform_int_to_string(champiArray[index + 2]);
                    }

                    s_rm += nmemonics::umap_rm_wide.at(input_rm);
                    if (destination) {
                        // nmemonics::umap.at(input) + " " + s_reg + ", " + s_rm + " + " + stringified_integer_data
                        // counter[1] = 122;
                        return nmemonics::umap.at(input) + " " + s_reg + ", " + _bracketer((s_rm +  stringified_integer_data));
                    }
                    // return nmemonics::umap.at(input) + " " + s_rm + ", " + s_reg;
                    return nmemonics::umap.at(input) + " " + _bracketer((s_rm +  stringified_integer_data)) + ", " + s_reg;
                } break;
                //------------------------
                default: {
                    if(wide){
                        // reuses umap11. i will change the name later not umap11
                        s_reg += nmemonics::umap11_wide.at(input_reg); 
                    }else{
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                    }
                    s_rm += nmemonics::umap_rm_wide.at(input_rm);

                    string nmemonics_result = nmemonics::umap.at(input) + " ";
                    string s_reg_result = s_reg + " ";
                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";
                    if (destination) {
                        result_0b00 = nmemonics_result + s_reg_result + "," + _bracketer( s_rm_result);
                        return result_0b00;
                    }
                    result_0b00 = nmemonics_result + _bracketer( s_rm_result) + ", " + s_reg_result;
                    return result_0b00;
                } break;
            }
        } break;
        case 0b1100011: {
            // left encoding
            bool wide = (0b00000001 & champiArray[index]);

            // right encoding
            u8 input2 = champiArray[index + 1];
            u8 input_mod = (0b11000000 & input2) >> 6; // not yet implemented
            u8 input_rm = (0b00000111 & input2);
            string s_reg = "";
            string s_rm = "";
            switch (input_mod) {
                case 0b11: {
                    // if (wide) {
                    //     s_rm += nmemonics::umap11_wide.at(input_rm);
                    // }else {
                    //     s_rm += nmemonics::umap11_not_wide.at(input_rm);
                    // }
                    // // ----
                    // return nmemonics::umap.at(input) + " " + s_reg + ", " + s_rm;
                }break;
                case 0b10: {
                    // 0b10 data first before another data
                    // i am not sure about the counter, because i am assuming that it is always +2 because it
                    // requires 16 bits. the wide option is only applicable to data1 not data 0.
                    // and not sure again, 16 bits if you input 8bit number it will automatically be
                    // 0x00 the rest of the unfilled 16bit space. like 0xb2, 0x00
                    string stringified_integer_data0 = "";
                    // if(wide){
                    u16 integer_summation = (champiArray[index + 3] << 8) ^ champiArray[index + 2];
                    stringified_integer_data0 = _transform_int_to_string(integer_summation);
                    counter[0] +=2;
                    // }else{
                    //     stringified_integer_data0 = "byte " + _transform_int_to_string(champiArray[index + 2]);
                    //     counter[0] +=1;
                    // }

                    string stringified_integer_data1 = "";
                    if(wide){
                        u16 integer_summation = (champiArray[index + 5] << 8) ^ champiArray[index + 4];
                        stringified_integer_data1 = "word " + _transform_int_to_string(integer_summation);
                        counter[0] +=2;
                    }else{
                        stringified_integer_data1 = "byte " + _transform_int_to_string(champiArray[index + 4]);
                        counter[0] +=1;
                    }

                    s_rm += nmemonics::umap_rm_wide.at(input_rm) + " + " + stringified_integer_data0;

                    string nmemonics_result = nmemonics::umap.at(input) + " ";
                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";
                    result_0b00 = nmemonics_result +_bracketer( s_rm_result) + ", " + stringified_integer_data1;
                    return result_0b00;
                }break;
                // ------------------
                case 0b01: {
                    // counter[1] = 3; // this is for debug only cause i did not configure vscode debug yet
                    // if(wide){
                    //     s_reg += nmemonics::umap11_wide.at(input_reg); 
                    //     // counter[1] = 122;
                    // }else{
                    //     s_reg += nmemonics::umap11_not_wide.at(input_reg);
                    //     counter[1] = 122;
                    // }
                    // counter[0] +=1;
                    // string stringified_integer_data = "";
                    // if( champiArray[index + 2] != 0){ // CHECK IF RAW INT IS ZERO
                    //     stringified_integer_data = " + " +  _transform_int_to_string(champiArray[index + 2]);
                    // }

                    // s_rm += nmemonics::umap_rm_wide.at(input_rm);
                    // return nmemonics::umap.at(input) + " " + s_reg + ", " + _bracketer((s_rm +  stringified_integer_data));
                } break;
                //------------------------
                default: {
                    counter[0] +=1;
                    string stringified_integer_data = "";
                    if(wide){
                        u16 integer_summation = (champiArray[index + 3] << 8) ^ champiArray[index + 2];
                        stringified_integer_data = "word " + _transform_int_to_string(integer_summation);
                        counter[0] +=1;
                    }else{
                        stringified_integer_data = "byte " + _transform_int_to_string(champiArray[index + 2]);
                    }

                    s_rm += nmemonics::umap_rm_wide.at(input_rm);

                    string nmemonics_result = nmemonics::umap.at(input) + " ";
                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";
                    result_0b00 = nmemonics_result +_bracketer( s_rm_result) + ", " + stringified_integer_data;
                    return result_0b00;
                } break;
            }
            string foo = "foofoo";
            return foo;
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
                counter[0]+=1;
                return  nmemonics::umap.at(input) + " " + s_reg + ", " + data;
            }
            s_reg += nmemonics::umap11_not_wide.at(input_reg);
            data += std::to_string(champiArray[index+1]);
            return  nmemonics::umap.at(input) + " " + s_reg + ", " + data;
        } break;
        case 0b000000: {
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
                case 0b01: {
                    // counter[1] = 3; // this is for debug only cause i did not configure vscode debug yet
                    if(wide){
                        s_reg += nmemonics::umap11_wide.at(input_reg); 
                    }else{
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                    }
                    string stringified_integer_data = "";
                    if( champiArray[index + 2] != 0){ // CHECK IF RAW INT IS ZERO
                        stringified_integer_data = " + " +  _transform_int_to_string(champiArray[index + 2]);
                    }

                    s_rm += nmemonics::umap_rm_wide.at(input_rm);
                    counter[0] +=1;
                    if (destination) {
                        return nmemonics::umap.at(input) + " " + s_reg + ", " + _bracketer((s_rm +  stringified_integer_data));
                    }
                    return nmemonics::umap.at(input)+ " " + byte_or_word(wide) + " " + _bracketer((s_rm +  stringified_integer_data)) + ", " + s_reg;
                } break;
                default: {
                    if(wide){
                        // reuses umap11. i will change the name later not umap11
                        s_reg += nmemonics::umap11_wide.at(input_reg); 
                    }else{
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                    }
                    s_rm += nmemonics::umap_rm_wide.at(input_rm);
                    string nmemonics_result = nmemonics::umap.at(input) + " ";
                    string s_reg_result = s_reg;
                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";
                    if (destination) {
                        result_0b00 = nmemonics_result + s_reg_result + ", " + _bracketer( s_rm_result);
                        return result_0b00;
                    }
                    result_0b00 = nmemonics_result + byte_or_word(wide)+ " " + _bracketer( s_rm_result) + ", " + s_reg_result;
                    return result_0b00;
                }break;
                
            } ;
        } break;
        case 0b100000: {
            // string gago = "gago\n";
            // return gago;
            // left encoding
            bool wide = (0b00000001 & champiArray[index]);
            bool destination = (0b00000010 & champiArray[index]) << 1;

            // right encoding
            u8 input2 = champiArray[index + 1];
            u8 input_modifier = (0b00111000 & input2) >> 3; // not reg. this is for add, sub and others
            u8 input_mod = (0b11000000 & input2) >> 6; // not yet implemented
            u8 input_rm = (0b00000111 & input2);
            string nmemonics_result = _nmemonics_result_get(input, input_modifier);
            string s_reg = "";
            string s_rm = "";
            switch (input_mod) {
                case 0b11: {
                    string stringified_integer_data0 = "";
                    u16 integer_summation = champiArray[index + 2];
                    stringified_integer_data0 = _transform_int_to_string(integer_summation);
                    s_rm += nmemonics::umap11_wide.at(input_rm) + ", " + stringified_integer_data0;

                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";
                    result_0b00 = nmemonics_result + s_rm_result;
                    counter[0] += 1;
                    return result_0b00;
                }break;
                case 0b10: {
                    string stringified_integer_data0 = "";
                    // if(wide){
                    u16 integer_summation = (champiArray[index + 3] << 8) ^ champiArray[index + 2];
                    stringified_integer_data0 = _transform_int_to_string(integer_summation);
                    counter[0] +=2;
                    // }else{
                    //     stringified_integer_data0 = "byte " + _transform_int_to_string(champiArray[index + 2]);
                    //     counter[0] +=1;
                    // }

                    string stringified_integer_data1 = "";
                    if(wide == 1 && destination == 0){
                        u16 integer_summation = (champiArray[index + 5] << 8) ^ champiArray[index + 4];
                        stringified_integer_data1 = _transform_int_to_string(integer_summation);
                        counter[0] +=2;
                    }else{
                        stringified_integer_data1 = _transform_int_to_string(champiArray[index + 4]);
                        counter[0] +=1;
                    }

                    s_rm += nmemonics::umap_rm_wide.at(input_rm) + " + " + stringified_integer_data0;

                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";
                    result_0b00 = nmemonics_result + "word " +_bracketer( s_rm_result)  + ", " + stringified_integer_data1;
                    return result_0b00;
                }break;
                default: {
                    string stringified_integer_data = "";
                    // temp solution for wides because i got confused in double wide
                    if(destination == false && wide){
                        wide = true;
                    }else{wide=false;}
                    // temp solution for wides because i got confused in double wide
                    if(wide){
                        u16 integer_summation = (champiArray[index + 3] << 8) ^ champiArray[index + 2];
                        stringified_integer_data = "word " + _transform_int_to_string(integer_summation);
                        counter[0] +=1;
                    }else{
                        stringified_integer_data =  _transform_int_to_string(champiArray[index + 2]);
                    }
                    counter[0] +=1;

                    s_rm += nmemonics::umap_rm_wide.at(input_rm);

                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";

                    if(destination){
                        result_0b00 = nmemonics_result +_bracketer( s_rm_result) + ", " + stringified_integer_data;
                        return result_0b00;
                    }

                    result_0b00 = nmemonics_result + "byte " + _bracketer( s_rm_result) + ", " + stringified_integer_data;
                    return result_0b00;
                }break;
            }
        }break;
        case 0b0000010: {
            bool wide = (0b00000001 & champiArray[index]);
            string stringified_integer_data = "";
            string result_string = nmemonics::umap.at(input) + " ";
            if(wide){
                counter[0] +=1;
                u16 integer_summation = (champiArray[index + 2] << 8) ^ champiArray[index + 1];
                stringified_integer_data = _transform_int_to_string(integer_summation);
                result_string += (nmemonics::umap11_wide.at(000) + ", " + stringified_integer_data);
                return result_string;
            }
            u8 integer_summation = champiArray[index + 1];
            stringified_integer_data = _transform_int_to_string(integer_summation);
            result_string += (nmemonics::umap11_not_wide.at(000) + ", " + stringified_integer_data);
            return result_string;
        }break;
        case 0b001010 | 0b001110: {
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
                case 0b01: {
                    // counter[1] = 3; // this is for debug only cause i did not configure vscode debug yet
                    if(wide){
                        s_reg += nmemonics::umap11_wide.at(input_reg); 
                    }else{
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                    }
                    string stringified_integer_data = "";
                    if( champiArray[index + 2] != 0){ // CHECK IF RAW INT IS ZERO
                        stringified_integer_data = " + " +  _transform_int_to_string(champiArray[index + 2]);
                    }

                    s_rm += nmemonics::umap_rm_wide.at(input_rm);
                    counter[0] +=1;
                    if (destination) {
                        return nmemonics::umap.at(input) + " " + s_reg + ", " + _bracketer((s_rm +  stringified_integer_data));
                    }
                    return nmemonics::umap.at(input)+ " " + byte_or_word(wide) + " " + _bracketer((s_rm +  stringified_integer_data)) + ", " + s_reg;
                } break;
                default: {
                    if(wide){
                        // reuses umap11. i will change the name later not umap11
                        s_reg += nmemonics::umap11_wide.at(input_reg); 
                    }else{
                        s_reg += nmemonics::umap11_not_wide.at(input_reg);
                    }
                    s_rm += nmemonics::umap_rm_wide.at(input_rm);
                    string nmemonics_result = nmemonics::umap.at(input) + " ";
                    string s_reg_result = s_reg;
                    string s_rm_result = s_rm;// we may add if needed later
                    string result_0b00 = "";
                    if (destination) {
                        result_0b00 = nmemonics_result + s_reg_result + ", " + _bracketer( s_rm_result);
                        return result_0b00;
                    }
                    result_0b00 = nmemonics_result + byte_or_word(wide)+ " " + _bracketer( s_rm_result) + ", " + s_reg_result;
                    return result_0b00;
                }break;
                
            } ;
        }break;
        case 0b0010110: {
            bool wide = (0b00000001 & champiArray[index]);
            string stringified_integer_data = "";
            string result_string = nmemonics::umap.at(input) + " ";
            if(wide){
                counter[0] +=1;
                u16 integer_summation = (champiArray[index + 2] << 8) ^ champiArray[index + 1];
                stringified_integer_data = _transform_int_to_string(integer_summation);
                result_string += (nmemonics::umap11_wide.at(000) + ", " + stringified_integer_data);
                return result_string;
            }
            u8 integer_summation = champiArray[index + 1];
            stringified_integer_data = _transform_int_to_string(integer_summation);
            result_string += (nmemonics::umap11_not_wide.at(000) + ", " + stringified_integer_data);
            return result_string;
        }break;
        default: {
            string chupapi = "chumami1\n";
            return chupapi;
        }break;

    };


}


int main(int argc, char **argv)
{
    // u8 champiHex[] = {0b10001001, 0b11011110, 0b10001000, 0b11000110, 0b10110001, 0b1100, 0b10110101,
    //     0b11110100, 0b10111001, 0b1100, 0b0, 0b10111001, 0b11110100, 0b11111111, 0b10111010,
    //     0b1101100, 0b1111, 0b10111010, 0b10010100, 0b11110000, 0b10001010, 0b0, 0b10001011, 0b11011,
    //     0b10001011, 0b1010110, 0b0, 0b10001010, 0b1100000, 0b100, 0b10001010, 0b10000000, 0b10000111,
    //     0b10011, 0b10001001, 0b1001, 0b10001000, 0b1010, 0b10001000, 0b1101110, 0b0
    // }; // temporary2, we will use external file later

    // u8 champiHex[] = {0b10001001, 0b11011110,0b10001000,0b11000110,0b10110001, 0b1100, 0xb5, 0xf4, 0xb9, 0x0c, 0x0,
    // 0b10111001, 0b11110100, 0b11111111, 0b10111010, 0b01101100, 0b1111, 0xba, 0x94, 0xf0, 0x8a, 0b0, 0x8b, 0x1b,
    // 0x8b, 0x56, 0x00, 0x8a,0x60,0x04,0x8a,0x80,0x87,0x13, 0x88, 0x6e, 0x00, 0x8b, 0x41, 0xdb,0x89,0x8c,0xd4,0xfe,
    // 0x8b,0x57,0xe0,0xc6,0x03,0x07,0xc7,0x85,0x85,0x03,0x5b,0x01,0b00000011, 0b11000,0x03,0x5e,0x00,0x83,0xc6,0x02,
    // 0x83,0xc5,0x02,0x83,0xc1,0x08,0x03,0x5e,0x00,0x03,0x4f,0x02,0x02,0x7a,0x04,0x03,0x7b,0x06,0x01,0x18,0x01,0x5e,0x00,
    // 0x01,0x4f,0x02,0x00,0x7a,0x04,0x01,0x7b,0x06,0x80,0x07,0x22,0x83,0x82,0xe8,0x03,0x1d,0x03,0x46,0x00,0x02,0x00,  
    // 0x01,0xd8,0x00,0xe0,0x05,0xe8,0x03,0x04,0xe2,0x04,0x09,0x2b,0x18,0x2b,0x5e,0x00,0x83,0xee,0x02,0x83,0xed,0x02,0x83,
    // 0xe9,0x08,0x2b,0x5e,0x00,0x2b,0x4f,0x02,0x2a,0x7a,0x04,0x2b,0x7b,0x06,0x29,0x18,0x29,0x5e,0x00,0x29,0x5e,0x00,0x29,
    // 0x4f,0x02,0x28,0x7a,0x04,0x29,0x7b,0x06,0x80,0x2f,0x22,0x83,0x29,0x1d,0x2b,0x46,0x00,0x2a,0x00,0x29,0xd8,0x28,0xe0,
    // 0x2d,0xe8,0x03,0x2c,0xe2,0x2c,0x09};
    // 0x3b,0x18,0x3b,0x5e,0x00,0x83,0xfe,0x02,0x83,0xfd,0x02,0x83,0xf9,0x08,0x3b,0x5e,0x00,0x3b,0x4f,0x02,0x3a,0x7a,0x04,0x3b,0x7b,0x06,0x39,0x18,0x39,0x5e,0x00,0x39,0x5e,0x00,0x39,0x4f,0x02,0x38,0x7a,0x04,0x39,0x7b,0x06,0x80,0x3f,0x22,0x83,0x3e,0xe2,0x12,0x1d,0x3b,0x46,0x00,0x3a,0x00,0x39,0xd8,0x38,0xe0,0x3d,0xe8,0x03,0x3c,0xe2,0x3c,0x09,0x75,0x02,0x75,0xfc,0x75,0xfa,0x75,0xfc,0x74,0xfe,0x7c,0xfc,0x7e,0xfa,0x72,0xf8,0x76,0xf6,0x7a,0xf4,0x70,0xf2,0x78,0xf0,0x75,0xee,0x7d,0xec,0x7f,0xea,0x73,0xe8,0x77,0xe6,0x7b,0xe4,0x71,0xe2,0x79,0xe0,0xe2,0xde,0xe1,0xdc,0xe0,0xda,0xe3,0xd8
    // u8 champiHex[] = {0b11000111,0b10000101,0x85,0x03,0x5b,0x01};
    u8 champiHex[] = {0x3b,0x18};

    string result = "";
    u8 kopal[] = {0, 0}; // it is an array as we will store many more later here
    while(kopal[0] < ArrayCount(champiHex))
    {
        result = left_and_right_encoding(champiHex, kopal);
        kopal[0]+=2; // minimum i think
        std::cout << result << std::endl;
    }
    printf("lalala counter0: %d\n", kopal[0]);
    printf("lalala ArrayCountChampi: %d\n", ArrayCount(champiHex));
    printf("lalala counter1: %d\n", kopal[1]);
    // printf("kapiling ka: %d\n", champiHex[0]);
    // u16 asdasd = (0b00010011 << 8) ^ 0b10000111;
    // string loti = word_or_byte(false);
    // cout << loti << "\n";
    // printf("asdsdaasdsda\n");
    

    return 0;

}
