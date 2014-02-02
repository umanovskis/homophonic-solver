#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Catch is working", "[catch]") {
	REQUIRE ( (2 * 2) == 4);
	REQUIRE ( (10 * 10) > 50);
}