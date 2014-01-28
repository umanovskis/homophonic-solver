#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <map>
#include <vector>

class Key;

class Message
{
private:
	typedef std::map<char,int> FreqMap;
	std::string ciphertext_;
	int num_symbols_;
	int uniform_;
	FreqMap freqmap_;
	void Init();
public:
	Message(const std::string ciphertext);
	const std::string GetCiphertext() const;
	const FreqMap GetFrequencyMap() const;
	void PrintFrequencyMap() const;
	int GuessNumberOfHomophones(char c) const;
	int GetUniformNumber() const;
	const std::string DecryptAsString(Key &key) const;
	std::vector<int> DecryptInt(Key &key) const;
};

#endif