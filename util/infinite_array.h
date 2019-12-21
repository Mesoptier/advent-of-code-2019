#include <unordered_map>

template<class value_t, class map_t = std::unordered_map<size_t, value_t>>
class infinite_array {
    map_t data;

public:
    value_t& at(size_t key) {
        if (!data.count(key)) {
            data.emplace(key, 0);
        }
        return data.at(key);
    }
    const value_t at(size_t key) const {
        if (!data.count(key)) {
            return 0;
        }
        return data.at(key);
    }
};