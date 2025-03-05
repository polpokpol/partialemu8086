//
// Created by aa on 11/5/24.
//

#ifndef NMEMONICS_H
#define NMEMONICS_H
#include <string>
#include <unordered_map>


struct nmemonics {
    // instruction name
    static std::unordered_map<int, std::string> umap;
    //

    // modfield name 0b11, im lazy so lets go! reg field the same
    static std::unordered_map<int, std::string> umap11_not_wide;
    static std::unordered_map<int, std::string> umap11_wide;
    //

    // modfield name 0b00, 0b01, 0b10. All of them except 0b11
    // 0b11 is register/register, the rest with exception: is register/memory
    // static std::unordered_map<int, std::string> umap00_not_wide; i dont think there is not wide
    static std::unordered_map<int, std::string> umap_rm_wide;
    //
    static std::unordered_map<std::string, int> grp_map;

};



#endif //NMEMONICS_H
