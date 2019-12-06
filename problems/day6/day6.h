#pragma once

#include <istream>
#include <map>
#include <vector>

namespace Day6 {
    using Object = std::string;

    std::pair<std::multimap<Object, Object>, std::map<Object, Object>> readInput(std::istream& input);

    int countTotalOrbits(std::multimap<Object, Object>& toChildren, const Day6::Object& object, int indirectOrbits);
    std::vector<Object> getAncestors(std::map<Object, Object>& toParents, Object object);

    int solve1(std::istream& input);
    int solve2(std::istream& input);
}
