#pragma once

#include <istream>
#include <vector>
#include <map>
#include <queue>

namespace Day14 {
    using Chem = std::string;
    using Quant = long long int;
    using ChemQuant = std::pair<Quant, Chem>;
    using RecipeReq = std::pair<Quant, std::vector<ChemQuant>>;

    std::map<Chem, RecipeReq> readRecipes(std::istream& input);
    ChemQuant parseChemQuant(std::string str);
    std::vector<ChemQuant> parseChemQuantList(std::string str);

    Quant computeRequiredOre(const std::map<Chem, RecipeReq>& recipes, Quant fuel);

    Quant solve1(std::istream& input);
    Quant solve2(std::istream& input);
}
