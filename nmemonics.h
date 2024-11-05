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

};



#endif //NMEMONICS_H
