#include <string>
#include <iostream>
#include <ctime>
#include <array>
#include <cmath>

#include "solver.h"
#include "message.h"
#include "key.h"
#include "language_data.h"

static inline float fastlog (float x);

Solver::Solver(const Message& message, Key &key) : message_(message), key_(key), bestKey_(Key()),
											tempTabu_(std::unordered_set<std::string>()), 
											optimalTabu_((std::unordered_set<std::string>())),
											iteration_limit_(-1), score_limit_(-1), time_limit_(-1)
{
	srand(time(nullptr));
}

void Solver::SetKey(Key &key)
{
	key_ = key;
}

int Solver::Start()
{
	int lastScore;
	int bestScore;
	int currentBestScore;
	unsigned int iterations = 0;
	const int maxTolerance = 40;
	const int endIterationShuffles = 5;
	const int tempClearProbability = 80;
	const int iterationsBeforeReset = 300;
	
	int tolerance = 0;
	int currentTolerance = 0;
	int unimprovedIterations = 0;
	
	Key bestKey(key_);
	Key currentBestKey(key_);

	currentBestScore = bestScore = lastScore = CalculateScore(message_.DecryptInt(key_));
	
	clock_t beginTime = clock();
	
	//while (bestScore < 42000 && iterations < 35)
	while ((iteration_limit_ < 0 || iteration_limit_ > iterations) && (score_limit_ < 0 || score_limit_ > bestScore) &&
	  (time_limit_ < 0 || time_limit_ > TimeSince(beginTime)))
	{
		bool improved = false;
		for (int p1 = 0; p1 < key_.GetLength(); p1++)
		{
			for (int p2 = 0; p2 < key_.GetLength(); p2++)
			{
				if (!key_.Swap(p1, p2)) continue;
				int score = CalculateScore(message_.DecryptIntCached(key_));
				if (tempTabu_.find(key_.AsPlainText()) != std::end(tempTabu_) ||
				  false)
					//optimalTabu_.find(key_.AsPlainText()) != std::end(optimalTabu_))
				{
					//std::cout << "Blacklisted " << std::endl;
					score = -10000;
				}
				else
				{
				}
				tempTabu_.insert(key_.AsPlainText());
				
				tolerance = rand() % (maxTolerance - currentTolerance + 1);
				
				if (score < lastScore - tolerance)
				{
					key_.Swap(p1, p2); //undo last swap
					message_.RestoreCache(); //restores previous cache
				}
				else
				{
					lastScore = score;
					if (lastScore > bestScore)
					{
						improved = true;
						bestScore = lastScore;
						bestKey = key_;
					}
					
					if (score > currentBestScore)
					{
						currentBestScore = score;
						currentBestKey = key_;
					}
				}
			}
		}
		
		if (!improved)
		{
			if (++currentTolerance >= maxTolerance)
			{
				currentTolerance = 0;
			}
			
			unimprovedIterations++;
			if (unimprovedIterations >= iterationsBeforeReset)
			{
				std::cout << "The plateau's clean, no dirt to be seen" << std::endl;
				optimalTabu_.insert(currentBestKey.AsPlainText());
				currentBestScore = -10000;
				
				if (rand() % 2)
				{
					std::cout << "Restart w/ random key" << std::endl;
					key_.RandomShuffle(100);
				}
				else
				{
					std::cout << "Back to best key" << std::endl;
					key_ = bestKey;
				}
				unimprovedIterations = 0;
			}
		}
		else
		{
			currentTolerance = 0;
			unimprovedIterations = 0;
		}
		key_.RandomShuffle(endIterationShuffles);
		if (tempTabu_.find(key_.AsPlainText()) != std::end(tempTabu_) ||
			false)
			//optimalTabu_.find(key_.AsPlainText()) != std::end(optimalTabu_))
		{
			lastScore = -10000;
		}
		else
		{
			lastScore = CalculateScore(message_.DecryptInt(key_));
		}
		
		if ((rand() % 100) < tempClearProbability) { //CLEAR_TABU_PROB
			tempTabu_.clear();
		}
		
		iterations++;
#ifdef EXECUTABLE
		std::cout << "Iteration " << iterations - 1 << " done with score " << currentBestScore << ", best is " << bestScore << std::endl;
#endif
	} //end of hill climber loop

	std::cout << iterations << " iterations completed" << std::endl;
	std::cout << "Best key is " << bestKey.AsPlainText() << std::endl;
	std::cout << "Best score is " << bestScore << std::endl;
	std::cout << "And it decrypts to" << std::endl;
	std::cout << message_.DecryptAsString(bestKey);
	
	bestKey_ = bestKey;
	return bestScore;
}

int Solver::CalculateScore(const std::vector<int>& plaintext)
{
	unsigned int score = 0;
	
	unsigned int biscore, triscore, tetrascore, pentascore;
	biscore = triscore = tetrascore = pentascore = 0;

	auto it = std::begin(plaintext);
	
	size_t remaining = plaintext.size();
	while (it != std::end(plaintext))
	{
		int gram = *(it++); //1st
		auto it2 = it;
		if (remaining > 1 || it2 != std::end(plaintext)) {
			gram *= 26;
			gram += *(it2++);
		}
		
		biscore += LanguageData::GetBigramFrequency(gram);
		
		if (remaining > 2 || it2 != std::end(plaintext)) {
			gram *= 26;
			gram += *(it2++);
			triscore += LanguageData::GetTri(gram);
		}
		
		if (remaining > 3 || it2 != std::end(plaintext)) {
			gram *= 26;
			gram += *(it2++);
			tetrascore += LanguageData::GetTetra(gram);
		}
		
		if (remaining > 4 || it2 != std::end(plaintext)) {
			gram *= 26;
			gram += *(it2++);
			pentascore += LanguageData::GetPenta(gram); //hehe, pentagram
		}
		remaining--;
	}
	score = pentascore + (tetrascore >> 1) + (triscore >> 2) + (biscore >> 3);
	
	//update statistics
	
//This is a perfectly normal way to value-initialize an array in C++11
//GCC is sadly being too paranoid about this
#pragma GCC diagnostic push	
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	std::array<int, 26> freqs {};
#pragma GCC diagnostic pop
	int unique = 0;
	for (size_t i = 0; i < plaintext.size(); i++)
	{
		int idx = plaintext[i];
		if (!freqs[idx])
		{
			unique++;
		}
		freqs[idx]++;
	}
	double probMass = (double)plaintext.size() / unique;
	float entropy = 0.0;
	float entropyProb = 0.0;
	double ioc = 0.0;
	double chi2 = 0.0;
	double chiTemp = 0.0;
	for (int i = 0; i < 26; i++)
	{
		if (freqs[i] > 1)
		{
			ioc += freqs[i] * (freqs[i] - 1);
		}
		
		if (freqs[i])
		{
			entropyProb = freqs[i] / plaintext.size();
			entropy += entropyProb * (fastlog(entropyProb) / std::log(2));

			chiTemp = freqs[i] - probMass;
			chiTemp *= chiTemp;
			chiTemp /= probMass;
			chi2 += chiTemp;
		}
	}
			
	ioc /= plaintext.size() * (plaintext.size() - 1);
	chi2 /= plaintext.size();
	entropy *= -1;
	
	double dioc = 0.0; //GetDIoC(plaintext);
	
	
	double multiplier = 1.0;
	multiplier *= 1.05 - (5 * std::abs(ioc - LanguageData::EnglishIoC));
	//multiplier *= 1.05 - ((5 >> 1) * std::abs(dioc - LanguageData::EnglishDIoC));
	multiplier *= 1.05 - (5 * std::abs(chi2 - LanguageData::EnglishChi2)) / 60.0;
	multiplier *= 1.05 - (5 * std::abs(entropy - LanguageData::EnglishEntropy)) / 150.0;
	
	return int(score * multiplier);
}

double Solver::GetDIoC(std::vector<int>& plaintext)
{
	#pragma GCC diagnostic push	
	#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
	std::array<int, 676> freqs {};
	#pragma GCC diagnostic pop
	
	double dioc = 0;
	int count = 0;
	
	for (size_t i = 0; i < plaintext.size() - 1; i += 2)
	{
		int idx1 = plaintext[i];
		int idx2 = plaintext[i + 1];
		freqs[idx1 * 26 + idx2]++;
		count++;
	}
	for (size_t i = 0; i < freqs.size(); i++)
	{
		if(freqs[i] > 1)
		{
			dioc += freqs[i] * (freqs[i] - 1);
		}
	}
	
	dioc /= count * (count - 1);
	std::cout << "DIOC count is " << count << std::endl;
	return dioc;
}

void Solver::SetIterationLimit(int limit)
{
	iteration_limit_ = limit;
}

void Solver::SetScoreLimit(int limit)
{
	score_limit_ = limit;
}

void Solver::SetTimeLimit(int limit)
{
	time_limit_ = limit;
}

int Solver::TimeSince(clock_t &begin)
{
	clock_t now = clock();	
	return (now - begin) / CLOCKS_PER_SEC;
}

// Using this over std::log does on average save about 0.05s per run
static inline float fastlog (float x)
{  
  union { float f; uint32_t i; } vx = { x };
  float y = vx.i;
  y *= 8.2629582881927490e-8f;
  return y - 87.989971088f;
}
