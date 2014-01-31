#include "message.h"
#include "language_data.h"
#include "key.h"

#include <iostream>
#include <vector>

using namespace std;

Message::Message(const string& ciphertext) : ciphertext_(ciphertext), num_symbols_(), 
											uniform_(), freqmap_(), cachedPlaintext(ciphertext_.size()),
											oldCache(ciphertext_.size())
{
	Init();
	//printFrequencyMap();
}

void Message::Init()
{
	for (char c : ciphertext_)
	{
		auto bound = freqmap_.lower_bound(c);
		if (bound != end(freqmap_) && !(freqmap_.key_comp()(c, bound->first))) //found
		{
			bound->second++;
		}
		else
		{
			freqmap_.insert(bound, FreqMap::value_type(c, 1)); //not found
		}			
	}
	num_symbols_ = freqmap_.size();
	uniform_ = ciphertext_.size() / num_symbols_;
}

const string Message::GetCiphertext() const
{
	return ciphertext_;
}

const Message::FreqMap Message::GetFrequencyMap() const
{
	return freqmap_;
}

int Message::GuessNumberOfHomophones(char c) const
{
	int expectedInText = LanguageData::GetLetterFrequency(c) * ciphertext_.size();
	if (expectedInText == 0) expectedInText = 1;
	return max(expectedInText / uniform_, 1);
}

const std::string Message::DecryptAsString(Key &key) const
{
	std::string s = "";
	for (auto el : DecryptInt(key))
	{
		char c = (char)el + 'A';
		s += c;
	}
	return s;
}

std::vector<int> Message::DecryptInt(Key &key) const
{
	cachedPlaintext.clear();
	for (const char &c : ciphertext_)
	{
		cachedPlaintext.push_back(key.GetPlainSymbol(c));
	}
	return cachedPlaintext;
}

const std::vector<int>& Message::DecryptIntCached(Key &key) const
{
	oldCache = cachedPlaintext;
	for (size_t i = 0; i < ciphertext_.size(); i++)
	{
		char c = ciphertext_[i];
		if (c == key.GetCached()[0] || c == key.GetCached()[1])
		{
		  cachedPlaintext[i] = key.GetPlainSymbol(c);
		}
	}
	return cachedPlaintext;
}

void Message::RestoreCache() const
{
  cachedPlaintext = std::move(oldCache);
}
