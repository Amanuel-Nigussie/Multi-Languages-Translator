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
#include <fstream>

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

HashTable::HashTable(int capacity)
{
	this->buckets = new Entry * [capacity];
	this->size = 0;
	this->capacity = capacity;
	this->collisions = 0;
	for (int i = 0; i < capacity; i++) {
		this->buckets[i] = nullptr;
	}
}
//============================================================================

unsigned long HashTable::hashCode(string word)
{
	unsigned long hash = 0;
	int shift = 5;

	for (int i = 0; i < word.length(); i++) {
		hash = (hash << shift) | (hash >> (32 - shift));
		hash += (unsigned long) word[i];
	}
	return hash % this->capacity;
}
//============================================================================

unsigned int HashTable::getSize() 
{
	return this->size;
}
//============================================================================

unsigned int HashTable::getCollisions()
{
	return this->collisions;
}
//============================================================================

void HashTable::import(string path)
{
	ifstream zfile(path);
	if (!zfile.is_open()) {
		cerr << "File not found";
		exit (1);
	}
	string line, zlanguage, zword, zmeanings;
	getline(zfile, zlanguage);
	int count = 0;
	while (getline(zfile, line)) {
		stringstream s(line);
		getline(s, zword, ':');
		getline(s, zmeanings);
		this->insert(zword, zmeanings, zlanguage);
		count++;
	}
	zfile.close();
	cout << count << " " << zlanguage << " words have been imported successfully." << endl;
}
//============================================================================

void HashTable::addWord(string word, string meanings, string language)
{
	this->insert(word, meanings, language);
}
//============================================================================

void HashTable::delWord(string word)
{
	unsigned long index = this->hashCode(word);
	if (this->buckets[index] == nullptr) {
		cout << word << " not found in the Dictionary." << endl;
	}
	else {
		do {
			if (this->buckets[index]->word == word) {
				if (buckets[index]->deleted) {
					cout << word << " not found in the Dictionary." << endl;
					return;
				}
				else {
					this->buckets[index]->deleted = true;
					this->size--;
					cout << word << " has been successfully deleted from the Dictionary." << endl;
					return;
				}
			}
			index = (index + 1) % capacity;
		} while (this->buckets[index] != nullptr);
		cout << word << " not found in the Dictionary." << endl;
	}
}
//============================================================================

void HashTable::insert(string word, string meanings, string language)
{
	for (int i = 0; i < word.length(); i++) {
		if (word[i] >= 'A' && word[i] <= 'Z') {
			word[i] = word[i] + 32;
		}
	}
	unsigned long index = this->hashCode(word);
	if (this->buckets[index] == nullptr) {
		this->buckets[index] = new Entry(word, meanings, language);
		this->size++;
	}
	else {
		do {      
			if (buckets[index]->word == word) { 
				if (buckets[index]->deleted) {
					buckets[index] = new Entry(word, meanings, language);
				}
				else {
					buckets[index]->addTranslation(meanings, language);
					return;
				}
			}
			index = (index + 1) % capacity;     
			collisions++;      
		} while (buckets[index] != nullptr && !buckets[index]->deleted);  
		buckets[index] = new Entry(word, meanings, language);
		size++;      
	}
}
//============================================================================

void HashTable::delTranslation(string word, string language)
{
	unsigned long index = this->hashCode(word);
	if (this->buckets[index] == nullptr) {
		cout << word << " not found in the Dictionary." << endl;
	}
	else {
		do {
			if (this->buckets[index]->word == word) {
				if (buckets[index]->deleted) {
					cout << word << " not found in the Dictionary." << endl;
					return;
				}
				else {
					for (int i = 0; i < this->buckets[index]->translations.size(); i++) {
						if (this->buckets[index]->translations[i].language == language) {
							this->buckets[index]->translations.erase(this->buckets[index]->translations.begin() + i);
							cout << "Translation of " << word << " in " << language << " has been successfully deleted from the Dictionary." << endl;
							return;
						}
					}
					cout << "Translation of " << word << " in " << language << " not found." << endl;
					return;
				}
			}
			index = (index + 1) % capacity;
		} while (this->buckets[index] != nullptr);
		cout << word << " not found in the Dictionary." << endl;
	}
}
//============================================================================

void HashTable::delMeaning(string word, string meaning, string language)
{
	unsigned long index = this->hashCode(word);
	if (this->buckets[index] == nullptr) {
		cout << word << " not found in the Dictionary." << endl;
	}
	else {
		do {
			if (this->buckets[index]->word == word) {
				if (buckets[index]->deleted) {
					cout << word << " not found in the Dictionary." << endl;
					return;
				}
				else {
					for (int i = 0; i < this->buckets[index]->translations.size(); i++) {
						if (this->buckets[index]->translations[i].language == language) {
							for (int j = 0; j < this->buckets[index]->translations[i].meanings.size(); j++) {
								if (this->buckets[index]->translations[i].meanings[j] == meaning) {
									this->buckets[index]->translations[i].meanings.erase(this->buckets[index]->translations[i].meanings.begin() + j);
									cout << "Meaning of " << word << " in " << language << " has been successfully deleted from the Dictionary." << endl;
									return;
								}
							}
							cout << "Meaning of " << word << " in " << language << " not found." << endl;
							return;
						}
					}
					cout << "Translation of " << word << " in " << language << " not found." << endl;
					return;
				}
			}
			index = (index + 1) % capacity;
		} while (this->buckets[index] != nullptr);
		cout << word << " not found in the Dictionary." << endl;
	}
}
//============================================================================

void HashTable::exportData(string language, string filePath)
{
	ofstream zfile(filePath);
	int count = 0;
	if (!zfile.is_open()) {
		cerr << "File not found";
		exit(1);
	}
	zfile << language << endl;
	for (int i = 0; i < this->capacity; i++) {
		if (this->buckets[i] != nullptr && !this->buckets[i]->deleted) {
			zfile << this->buckets[i]->word << ":";
			count++;
			for (int j = 0; j < this->buckets[i]->translations.size(); j++) {
				if (this->buckets[i]->translations[j].language == language) {
					for (int k = 0; k < this->buckets[i]->translations[j].meanings.size(); k++) {
						zfile << this->buckets[i]->translations[j].meanings[k];
						if (k < this->buckets[i]->translations[j].meanings.size() - 1) {
							zfile << ";";
						}
					}
					break;
				}
			}
			zfile << endl;
		}
	}
	zfile.close();
	cout << count << " records have been successfully exported to " << filePath << endl;
}
//============================================================================

void HashTable::find(string word)
{
	string lword = word;
	for (int i = 0; i < word.length(); i++) {
		if (word[i] >= 'A' && word[i] <= 'Z') {
			lword[i] = word[i] + 32;
		}
	}
	unsigned long index = this->hashCode(lword);
	int count = 1;
	if (this->buckets[index] == nullptr) {
		cout << word << " not found in the Dictionary." << endl;
	}
	else {
		do {
			if (this->buckets[index]->word == lword) {
				if (buckets[index]->deleted) {
					cout << word << " not found in the Dictionary." << endl;
					return;
				}
				else {
					cout << word << " found in the Dictionary after " << count << " comparisons." << endl;
					this->buckets[index]->print();
					return;
				}
			}
			index = (index + 1) % capacity;
			count++;
		} while (this->buckets[index] != nullptr);
		cout << word << " not found in the Dictionary." << endl;
	}
}
//============================================================================

HashTable::~HashTable()
{
	for (int i = 0; i < capacity; i++) {
		if (this->buckets[i] != nullptr) {
			delete this->buckets[i];
		}
	}
	delete[] this->buckets;
}
//============================================================================
