//============================================================================
// Name         : Multi-language dictionary
// Author       : Amanuel Nigussie Demeke
// Version      : 1.0
// Date Created : April 29, 2024
// Date Modified: 
// Description  : HashTable Implementation using C++
//============================================================================

#include "hashtable.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Translation::Translation(string meanings, string language)
{
	this->language = language;
	stringstream s(meanings);
	string meaning;
	while (getline(s, meaning, ';'))
	{
		bool found = false;
		for (int i = 0; i < this->meanings.size(); i++) {
			if (this->meanings[i] == meaning) {
				found = true;
				break;
			}
		}
		if (!found) {
			this->meanings.push_back(meaning);
		}
	}
}
//============================================================================

void Translation::addMeaning(string newMeanings)
{
	stringstream s(newMeanings);
	string meaning;
	while (getline(s, meaning, ';'))
	{
		bool found = false;
		for (int i = 0; i < this->meanings.size(); i++) {
			if (this->meanings[i] == meaning) {
				found = true;
				break;
			}
		}
		if (!found) {
			this->meanings.push_back(meaning);
		}
	}
}
//============================================================================

void Translation::display()
{
	cout << setw(10) << left << this->language << ":  ";
	for (int i = 0; i < this->meanings.size(); i++) {
		cout << this->meanings[i];
		if (i < this->meanings.size() - 1) {
			cout << "; ";
		}
	}
	cout << endl;
}
//============================================================================

Entry::Entry(string word, string meanings, string language)
{
	this->word = word;
	Translation t(meanings, language);
	this->translations.push_back(t);
	this->deleted = false;
}
//============================================================================

void Entry::addTranslation(string newMeanings, string language)
{
	for (int i = 0; i < this->translations.size(); i++) {
		if (this->translations[i].language == language) {
			this->translations[i].addMeaning(newMeanings);
			return;
		}
	}
	Translation t(newMeanings, language);
	this->translations.push_back(t);
}
//============================================================================

void Entry::print()
{
	for (int i = 0; i < this->translations.size(); i++) {
		this->translations[i].display();
	}
}
//============================================================================


