#include "day6.h"
#include <regex>
#include <iostream>

int Day6::solve1(std::istream& input) {
    std::multimap<Object, Object> toChildren = readInput(input).first;
    return countTotalOrbits(toChildren, "COM", 0);
}

int Day6::solve2(std::istream& input) {
    std::map<Object, Object> toParents = readInput(input).second;

    // Lists of indirect orbits excluding OBJ in order of adjacency to OBJ
    std::vector<Object> youAncestors = getAncestors(toParents, "YOU");
    std::vector<Object> sanAncestors = getAncestors(toParents, "SAN");

    // Reverse the lists such that the heads align
    std::reverse(youAncestors.begin(), youAncestors.end());
    std::reverse(sanAncestors.begin(), sanAncestors.end());

    // Find size of longest and shortest branch to YOU/SAN from COM
    int max = std::max(youAncestors.size(), sanAncestors.size());
    int min = (int) std::min(youAncestors.size(), sanAncestors.size());

    // We don't need to look at the tail end of the longer branch, since the
    // lowest common ancestor cannot be found there. So we prune that part.
    int count = max - min;
    int i = min - 1;

    while (youAncestors.at(i) != sanAncestors.at(i)) {
        count += 2;
        i--;
    }

    return count;
}

std::pair<std::multimap<Day6::Object, Day6::Object>, std::map<Day6::Object, Day6::Object>>
Day6::readInput(std::istream& input) {
    // Given COM)AAA, COM)BBB ...
    std::multimap<Object, Object> toChildren; // Multi-maps COM -> AAA, COM -> BBB
    std::map<Object, Object> toParents; // Maps AAA -> COM, BBB -> COM

    std::string line;
    while (std::getline(input, line)) {
        std::regex e("^(.+)\\)(.+)$");
        std::smatch cm;
        std::regex_match(line, cm, e);

        Object a = cm[1];
        Object b = cm[2];
        toChildren.emplace(a, b);
        toParents.emplace(b, a);
    }

    return {toChildren, toParents};
}

int Day6::countTotalOrbits(std::multimap<Object, Object>& toChildren, const Day6::Object& object, int indirectOrbits) {
    auto range = toChildren.equal_range(object);
    int sum = indirectOrbits;
    for (auto i = range.first; i != range.second; ++i) {
        sum += countTotalOrbits(toChildren, i->second, indirectOrbits + 1);
    }
    return sum;
}

std::vector<Day6::Object> Day6::getAncestors(std::map<Object, Object>& toParents, Day6::Object object) {
    std::vector<Object> ancestors;
    while (object != "COM") {
        object = toParents.at(object);
        ancestors.push_back(object);
    }
    return ancestors;
}