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

class HomophonicSolver
{
private:
	Key key_;
	Message msg_;
	Solver* solver_;
public:
	
	HomophonicSolver(std::string& ciphertext) : key_(), msg_(ciphertext), solver_(nullptr)
	{
		key_.Init(msg_);
	}
	
	void InitializeSolver()
	{
		LanguageData::Initialize();		
	}
	
	void SetTimeLimit(int limit)
	{
		solver_->SetTimeLimit(limit);
	}
	
	void SetScoreLimit(int limit)
	{
		solver_->SetScoreLimit(limit);
	}

	int Solve()
	{
		key_.Init(msg_);
		std::cout << "Inited" << std::endl;
		std::cout << msg_.GetCiphertext() << std::endl;
		std::cout << key_.AsPlainText() << std::endl;
		solver_ = new Solver(msg_, key_);
		solver_->SetTimeLimit(5);
		solver_->SetScoreLimit(42000);
		int res = solver_->Start();
		delete solver_;
		return res;
	}
};

HomophonicSolver* libInstance;

void Init(std::string& ciphertext)
{
	libInstance = new HomophonicSolver(ciphertext);
}

int Solve()
{
	return libInstance->Solve();
}

void Destroy()
{
	delete libInstance;
}