#include "solve.h"
#include <iostream>
#include <sstream>

namespace Day14 {

    Quant solve1(std::istream& input) {
        const auto recipes = readRecipes(input);
        return computeRequiredOre(recipes, 1);
    }

    Quant solve2(std::istream& input) {
        const auto recipes = readRecipes(input);
        const Quant maxOre = 1e12;

        // Exponential search
        int maxFuel = 1;
        while (computeRequiredOre(recipes, maxFuel) <= maxOre) {
            maxFuel *= 2;
        }
        int minFuel = maxFuel / 2;

        // Binary search
        while (minFuel <= maxFuel) {
            int fuel = (minFuel + maxFuel) / 2;
            auto ore = computeRequiredOre(recipes, fuel);
            if (ore < maxOre) {
                minFuel = fuel + 1;
            } else if (ore > maxOre) {
                maxFuel = fuel - 1;
            } else {
                return fuel; // found exact solution (unlikely)
            }
        }

        // We have minFuel > maxFuel, ore(minFuel) > maxOre, and ore(maxFuel) < maxOre
        return maxFuel;
    }

    ChemQuant parseChemQuant(std::string str) {
        std::stringstream ss(str);
        int amount;
        std::string name;
        ss >> amount;
        ss >> name;
        return { amount, name };
    }

    std::vector<ChemQuant> parseChemQuantList(std::string str) {
        std::stringstream ss(str);
        std::string line;
        std::vector<ChemQuant> list;

        while (std::getline(ss, line, ',')) {
            list.push_back(parseChemQuant(line));
        }

        return list;
    }

    std::map<Chem, RecipeReq> readRecipes(std::istream& input) {
        std::map<Chem, RecipeReq> recipes;

        std::string line;
        while (std::getline(input, line)) {
            std::stringstream ss(line);
            std::string requireStr;
            std::string produceStr;

            std::getline(ss, requireStr, '=');
            ss.ignore(2);
            std::getline(ss, produceStr);

            auto produces = parseChemQuant(produceStr);
            auto requires = parseChemQuantList(requireStr);

            recipes[produces.second] = { produces.first, requires };
        }

        return recipes;
    }

    Quant computeRequiredOre(const std::map<Chem, RecipeReq>& recipes, Quant fuel) {
        std::queue<ChemQuant> todo;
        std::map<Chem, Quant> leftover;

        todo.emplace(fuel, "FUEL");

        Quant totalOre = 0;

        while (!todo.empty()) {
            Quant required;
            Chem chem;
            std::tie(required, chem) = todo.front();
            todo.pop();

            if (chem == "ORE") {
                totalOre += required;
                continue;
            }

            // First take from leftover chemicals
            if (leftover[chem] >= required) {
                leftover[chem] -= required;
                continue;
            }
            required -= leftover[chem];
            leftover[chem] = 0;

            // Then produce more
            auto recipe = recipes.at(chem);

            // How many times should we do the recipe?
            Quant mult = required / recipe.first;
            if (required % recipe.first != 0) {
                mult += 1;
            }

            leftover[chem] += (mult * recipe.first) - required;
            required = 0;

            // Add chemicals to be produced to queue
            for (const auto& req : recipe.second) {
                todo.emplace(req.first * mult, req.second);
            }
        }

        return totalOre;
    }

}
