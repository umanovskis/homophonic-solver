#ifndef _KEY_H
#define _KEY_H

#include <unordered_map>
#include <string>

class Message;

class Key
{
private:
	typedef std::unordered_map<char,char> MapType;
	MapType map_;
	int length_;
	std::unordered_map<char, int> key_;
public:
	void Init(const Message* message);
	void SetMapSymbol(const char cipherSymbol, int plainSymbol);
	int GetPlainSymbol(const char cipherSymbol) const;
	void PrintKey() const;
	int GetLength() const;
    bool Swap(int p1, int p2);
	std::string AsPlainText();
	void RandomShuffle(int times);
	
	Key();
	Key(const Key& other);
	
	Key& operator=(const Key& other);
};

#endif