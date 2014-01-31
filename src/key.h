#ifndef _KEY_H
#define _KEY_H

#include <unordered_map>
#include <string>

class Message;

class Key
{
private:
	typedef std::unordered_map<char, int> MapType;
	int length_;
	std::unordered_map<char, int> key_;
	char cached[2];
public:
	void Init(const Message& message);
	void SetMapSymbol(const char cipherSymbol, int plainSymbol);
	int GetPlainSymbol(const char cipherSymbol) const;
	void PrintKey() const;
	int GetLength() const;
    bool Swap(size_t p1, size_t p2);
	std::string AsPlainText() const;
	void RandomShuffle(int times);
	char* getCached();
	
	Key();
	Key(const Key& other);
	
	Key& operator=(const Key& other);
};

#endif
