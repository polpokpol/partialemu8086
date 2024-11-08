//
// Created by aa on 11/5/24.
//

#include "nmemonics.h"

std::unordered_map<int, std::string> nmemonics::umap = {
    {0b100010, "mov"}
};

std::unordered_map<int, std::string> nmemonics::umap11_not_wide = {
    {0b000, "al"},
    {0b001, "cl"},
    {0b010, "dl"},
    {0b011, "bl"},
    // _________________ space for readability
    {0b100, "ah"},
    {0b101, "ch"},
    {0b110, "dh"},
    {0b111, "bh"},
};

std::unordered_map<int, std::string> nmemonics::umap11_wide = {
    {0b000, "ax"},
    {0b001, "cx"},
    {0b010, "dx"},
    {0b011, "bx"},
    // _________________ space for readability
    {0b100, "sp"},
    {0b101, "bp"},
    {0b110, "si"},
    {0b111, "di"},
};