#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../language_data.h"
#include "../message.h"
#include "../key.h"
#include "../solver.h"

TEST_CASE("Catch is working") {
	REQUIRE ( (2 * 2) == 4);
	REQUIRE ( (10 * 10) > 50);
}

TEST_CASE("Language data gets initialized") {
	LanguageData::Initialize();
	
	REQUIRE(LanguageData::GetTri(0) == 10);
	REQUIRE(LanguageData::GetTri(17574) == 27);
}


TEST_CASE("Z408 gets solved") {
	std::ifstream t("z408.txt");
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string str = buffer.str();
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());	
	LanguageData::Initialize();
	
	Message msg(str);
	Key key;
	key.Init(msg);
	
	Solver solver(msg, key);
	solver.SetScoreLimit(38000);
	solver.SetTimeLimit(12);
	int result = solver.Start();
	
	REQUIRE(result > 35000);
}