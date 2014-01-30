#ifndef _SOLVER_H
#define _SOLVER_H

#include <string>
#include <unordered_set>
#include <memory>
#include <vector>
#include "key.h"
#include "message.h"

//class Message;
//class Key;

class Solver
{
private:
	Message message_;
	Key key_;
	Key bestKey_;
	int CalculateScore(const std::vector<int>& plaintext);
	double GetDIoC(std::vector<int>& plaintext);
	std::unordered_set<std::string> tempTabu_;
	std::unordered_set<std::string> optimalTabu_;
	
	Solver(const Solver&) = delete;
	Solver& operator=(const Solver&) = delete;
public:
	Solver();
	Solver(const Message& message, Key &key);
	void SetKey(Key &key);
	int Start();
};

#endif