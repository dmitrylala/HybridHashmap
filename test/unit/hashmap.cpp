#include <vector>
#include <catch2/catch.hpp>

#include "hashmap.h"


TEST_CASE( "empty hashmap", "[hashmap]" ) {
    Hashmap<std::string, std::string> h;

    REQUIRE( h.size() == 0 );
    REQUIRE( h.empty() == true );
}

TEST_CASE( "sample hashmap", "[hashmap]" ) {
    Hashmap<std::string, std::string> h;

    std::vector< std::pair<std::string, std::string> > colors = {
        {"BLACK", "#000000"},
        {"WHITE", "#FFFFFF"},
        {"PURPLE", "#A020F0"}
    };
    for (auto color : colors)
        h.add(color);

    REQUIRE( h.size() == 3 );
    REQUIRE( h.empty() == false );
    for (auto color : colors) {
        REQUIRE( h.contains(color.first) == true );
        REQUIRE( h[color.first] == color.second );
    }

    std::pair<std::string, std::string> orange = { "ORANGE", "#FFA500" };
    REQUIRE( h.contains(orange.first) == false );
    REQUIRE_THROWS_WITH( h[orange.first], NO_KEY_MSG );
}
