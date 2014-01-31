#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "message.h"
#include "key.h"
#include "language_data.h"
#include "solver.h"

using namespace std;

#ifdef EXECUTABLE
int main(int argc, char **argv) {
	if (argc != 2)
	{
		cout << "Input file name required!" << endl;
		return 1;
	}
	std::ifstream t(argv[1]);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string str = buffer.str();
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	std::cout << str << std::endl;
	std::cout << str.size() << std::endl;
	
	LanguageData::Initialize();
	
	Message msg(str);// = new Message(str);
	Key key;
	key.Init(msg);
	cout << LanguageData::GetLetterFrequency('A') << endl;
	
	Solver solver(msg, key);
	solver.SetScoreLimit(44000);
	solver.SetTimeLimit(5);
	solver.Start();
	
    return 0;
}
#endif
