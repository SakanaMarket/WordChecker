// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <string>
#include <algorithm>
#include <iostream>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
	// returns provided set method contains
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	// vector to return
	std::vector<std::string> iSuggestAcceptingCthulhu;

	// needed for insertions and replacements
	std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	
	// swap using std::swap
	for (int swap = 0; swap < word.size(); ++swap)
	{
		std::string place_holder = word;
		std::swap(place_holder[swap], place_holder[swap+1]);
		if (wordExists(place_holder) && !(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), place_holder) != iSuggestAcceptingCthulhu.end()))
		{
			iSuggestAcceptingCthulhu.push_back(place_holder);
		}
	}

	// insertions using string.insert
	for (int index = 0; index < word.size(); ++index)
	{
		for (int alpha = 0; alpha < 26; ++alpha)
		{
			std::string place_holder = word;
			std::string to_insert = alphabet.substr(alpha, 1);
			place_holder.insert(index, to_insert);
			if (wordExists(place_holder) && !(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), place_holder) != iSuggestAcceptingCthulhu.end()))
			{
				iSuggestAcceptingCthulhu.push_back(place_holder);
			}

			// checking for inserts before and after the word
			std::string insert_before = word;
			std::string insert_after = word;
			insert_after.append(to_insert);
			to_insert.append(insert_before);
			
			if (wordExists(insert_after) && !(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), insert_after) != iSuggestAcceptingCthulhu.end()))
			{
				iSuggestAcceptingCthulhu.push_back(insert_after);
			}
			if (wordExists(to_insert) && !(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), to_insert) != iSuggestAcceptingCthulhu.end()))
			{
				iSuggestAcceptingCthulhu.push_back(to_insert);
			}
		}
	}



	// deletion using string.erase
	for (int del_index = 0; del_index < word.size(); ++del_index)
	{
		std::string place_holder = word;
		place_holder.erase(del_index, 1);
		if (wordExists(place_holder) && !(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), place_holder) != iSuggestAcceptingCthulhu.end()))
		{
			iSuggestAcceptingCthulhu.push_back(place_holder);
		}
	}

	// replacement using string.replace
	for (int r_index = 0; r_index < word.size(); ++r_index)
	{
		for (int beta = 0; beta < 26; ++beta)
		{
			std::string place_holder = word;
			std::string replacement = alphabet.substr(beta, 1);
			place_holder.replace(r_index, 1, replacement);
			if (wordExists(place_holder) && !(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), place_holder) != iSuggestAcceptingCthulhu.end()))
			{
				iSuggestAcceptingCthulhu.push_back(place_holder);
			}
		}
	}

	// splitting using string.substr
	for (int split_index = 0; split_index < word.size(); ++split_index)
	{
		std::string place_holder = word;
		std::string s1 = place_holder.substr(0, split_index+1);
		std::string s2 = "";
		for (int remaining = split_index+1; remaining < word.size(); ++remaining)
		{
			s2 += place_holder.substr(remaining, 1);
		}
		if (wordExists(s1) && wordExists(s2))
		{
			if (!(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), s1) != iSuggestAcceptingCthulhu.end()) 
				&& !(std::find(iSuggestAcceptingCthulhu.begin(), 
				iSuggestAcceptingCthulhu.end(), s2) != iSuggestAcceptingCthulhu.end()))
			{
				// returns both words as 1 line with space in between
				std::string s3 = s1 + " " + s2;
				iSuggestAcceptingCthulhu.push_back(s3);
			}
		}
	}

	return iSuggestAcceptingCthulhu;
}

