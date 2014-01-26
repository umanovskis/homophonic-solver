#ifndef _LANGUAGE_DATA_H
#define _LANGUAGE_DATA_H

#include <unordered_map>
#include <string>
#include <array>

namespace LanguageData
{	
	typedef std::unordered_map<char, double> CharToDoubleMap;
	typedef std::unordered_map<std::string, int> StringToIntMap;
	double GetLetterFrequency(char c);
	int GetBigramFrequency(int gram);
	int GetTrigramFrequency(std::string trigram);
	int GetTetragramFrequency(std::string tetragram);
	int GetPentagramFrequency(std::string pentagram);
	void Initialize();
	int GetTri(int gram);
	int GetTetra(int gram);
	int GetPenta(int gram);
	extern double EnglishIoC;
	extern double EnglishDIoC;
	extern double EnglishChi2;
	extern double EnglishEntropy;
}

#endif