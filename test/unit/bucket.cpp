#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "bucket.h"
#include "join.h"


TEST_CASE( "empty bucket", "[bucket]" ) {
    Bucket<int, int> b;

    REQUIRE( b.size() == 0 );
    REQUIRE( b.capacity() == DEFAULT_CACHE_SIZE );
    REQUIRE( join(b.data()) == "" );
    REQUIRE( join(b.cache()) == "" );
    REQUIRE( join(b.list()) == "" );
    REQUIRE( b.empty() == true );
    REQUIRE( b.contains(20) == false );
}

TEST_CASE( "small bucket", "[bucket]" ) {
    Bucket<int, int> b;
    for (int i = 1; i <= 5; ++i)
        b.add(std::pair<int, int>(i, i + 10));

    REQUIRE( b.size() == 5 );
    REQUIRE( b.capacity() == DEFAULT_CACHE_SIZE );
    REQUIRE( b.empty() == false );

    std::string content = "1 11\t2 12\t3 13\t4 14\t5 15";
    REQUIRE( join(b.data()) == content );
    REQUIRE( join(b.cache()) == content );
    REQUIRE( join(b.list()) == "" );
}

TEST_CASE( "bigger bucket", "[bucket]" ) {
    Bucket<int, int> b;
    for (int i = 1; i <= 15; ++i)
        b.add(std::pair<int, int>(i, i + 1));

    REQUIRE( b.size() == 15 );
    REQUIRE( b.capacity() == DEFAULT_CACHE_SIZE );
    REQUIRE( b.empty() == false );

    std::string cache = "1 2\t2 3\t3 4\t4 5\t5 6\t6 7\t7 8\t8 9\t9 10\t10 11";
    std::string list = "11 12\t12 13\t13 14\t14 15\t15 16";
    REQUIRE( join(b.data()) == cache + "\t" + list );
    REQUIRE( join(b.cache()) == cache );
    REQUIRE( join(b.list()) == list );

    SECTION( "checking if an element exists" ) {
        REQUIRE( b.contains(1) == true );
        REQUIRE( b.contains(15) == true );
        REQUIRE( b.contains(20) == false );
    }

    SECTION( "removing elements from cache" ) {
        b.remove(5);
        b.remove(6);

        std::string new_cache = "1 2\t2 3\t3 4\t4 5\t7 8\t8 9\t9 10\t10 11\t11 12\t12 13";
        std::string new_list = "13 14\t14 15\t15 16";

        REQUIRE( b.size() == 13 );
        REQUIRE( join(b.data()) == new_cache + "\t" + new_list );
        REQUIRE( join(b.cache()) == new_cache );
        REQUIRE( join(b.list()) == new_list );
    }

    SECTION( "removing elements from list" ) {
        b.remove(12);
        b.remove(13);

        std::string new_list = "11 12\t14 15\t15 16";

        REQUIRE( b.size() == 13 );
        REQUIRE( join(b.data()) == cache + "\t" + new_list );
        REQUIRE( join(b.cache()) == cache );
        REQUIRE( join(b.list()) == new_list );
    }

    SECTION( "clear bucket" ) {
        b.clear();

        REQUIRE( b.size() == 0 );
        REQUIRE( b.capacity() == DEFAULT_CACHE_SIZE );
        REQUIRE( join(b.data()) == "" );
        REQUIRE( join(b.cache()) == "" );
        REQUIRE( join(b.list()) == "" );
    }
}

TEST_CASE( "copying buckets", "[bucket]") {
    Bucket<int, int> b;
    for (int i = 1; i <= 15; ++i)
        b.add(std::pair<int, int>(i, i + 1));

    std::string cache = "1 2\t2 3\t3 4\t4 5\t5 6\t6 7\t7 8\t8 9\t9 10\t10 11";
    std::string list = "11 12\t12 13\t13 14\t14 15\t15 16";

    Bucket<int, int> c(b);

    auto verify = [&cache, &list](const Bucket<int, int> &bucket) {
        REQUIRE( bucket.size() == 15 );
        REQUIRE( bucket.capacity() == DEFAULT_CACHE_SIZE );
        REQUIRE( bucket.empty() == false );
        REQUIRE( join(bucket.data()) == cache + "\t" + list );
        REQUIRE( join(bucket.cache()) == cache );
        REQUIRE( join(bucket.list()) == list );
    };

    verify(b);
    verify(c);

    Bucket<int, int> d;
    d = b;

    verify(b);
    verify(d);
}
