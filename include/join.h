#pragma once

#include <sstream>
#include <string>


template <typename Iterable>
auto join(const Iterable &iterable, const std::string &sep = "\t") -> decltype(
    iterable.begin(), iterable.end(), std::string{}) {
    std::stringstream ss;
    int i = 0;
    for (const auto &elem : iterable) {
        if (i > 0)
            ss << sep;
        ss << elem.first << " " << elem.second;
        ++i;
    }
    return ss.str();
}
