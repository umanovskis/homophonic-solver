#include "key.h"
#include "message.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>

Key::Key() : map_(MapType()), length_(0), key_(std::unordered_map<char, int>())
{}

Key::Key(const Key& other) : map_(other.map_), length_(other.length_)
{ }

Key& Key::operator=(const Key& other)
{
	map_ = MapType(other.map_);
	length_ = other.length_;
	
	return *this;
}

void Key::Init(const Message* message)
{
	int i = 0;
	std::map<char, int> letterHomophones;
	for (char c = 'A'; c <= 'Z'; c++)
	{
		letterHomophones.insert(std::pair<char, int> (c, message->GuessNumberOfHomophones(c)));
	}
	length_ = message->GetFrequencyMap().size();
	for (auto pair : message->GetFrequencyMap())
	{
		char guess = 0;
		for (char c = 'A'; c <= 'Z'; c++)
		{
			if (letterHomophones[c] > 0)
			{
				guess = c;
				letterHomophones[c]--;
				break;
			}
		}
		map_[pair.first] = guess;
		int numericalGuess = static_cast<int>(guess) - 'A';
		key_[pair.first] = numericalGuess;
		i++;
	}
	//length_ = map_.size();
}

int Key::GetPlainSymbol(const char cipherSymbol) const
{
	return key_.find(cipherSymbol)->second;
}

void Key::SetMapSymbol(const char cipherSymbol, int plainSymbol)
{
	key_[cipherSymbol] = plainSymbol;
}

std::string Key::AsPlainText()
{
	std::string s = "";
	for (auto pair : key_)
	{
		s += std::to_string(pair.second);
	}
	return s;
}

void Key::PrintKey() const
{
	for (auto pair : key_)
	{
		std::cout << pair.first << " : " << pair.second << " " << static_cast<char>(pair.second + 'A') << std::endl;
	}
	std::cout << std::endl;
}

int Key::GetLength() const
{
	return length_;
}

bool Key::Swap(int p1, int p2)
{
	MapType::iterator it = map_.begin();
	MapType::iterator it2 = map_.begin();
	if (p1 == p2) {
		return false;
	}
	for (int i = 0; i < p1; i++, it++);
	for (int i = 0; i < p2; i++, it2++);
	//std::cout << "Swapping " << (*it).second.second << " and " << (*it2).second.second << std::endl;
	if ((*it).second == (*it2).second) {
		return false;
	}
	std::swap((*it).second, (*it2).second);
	return true;
}

void Key::RandomShuffle(int times)
{
	for (int i = 0; i < times; i++)
	{
		int p1 = rand() % GetLength();
		int p2 = rand() % GetLength();
		
		Swap(p1, p2);
	}
}

