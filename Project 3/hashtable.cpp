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

Translation::Translation(string meanings, string language)     // Constructor
{
	this->language = language;      // Assign the language to the object
	stringstream s(meanings);      // Create a stringstream object
	string meaning;			   // Create a string to store the meanings
	while (getline(s, meaning, ';'))      // Loop through the stringstream object
	{
		bool found = false;      // Create a boolean variable to check if the meaning is already in the vector
		for (int i = 0; i < this->meanings.size(); i++) {       // Loop through the meanings vector
			if (this->meanings[i] == meaning) {      // Check if the meaning is already in the vector
				found = true;      // If the meaning is found, set the found variable to true
				break;      // Break the loop
			}
		}
		if (!found) {      // If the meaning is not found
			this->meanings.push_back(meaning);      // Add the meaning to the vector
		}
	}
}
//============================================================================

void Translation::addMeaning(string newMeanings)      // Add a new meaning to the vector
{
	stringstream s(newMeanings);      // Create a stringstream object
	string meaning;       // Create a string to store the meanings
	while (getline(s, meaning, ';'))       // Loop through the stringstream object
	{
		bool found = false;      // Create a boolean variable to check if the meaning is already in the vector
		for (int i = 0; i < this->meanings.size(); i++) {     // Loop through the meanings vector
			if (this->meanings[i] == meaning) {      // Check if the meaning is already in the vector
				found = true;      // If the meaning is found, set the found variable to true
				break;      // Break the loop
			}
		}
		if (!found) {      // If the meaning is not found
			this->meanings.push_back(meaning);      // Add the meaning to the vector
		}
	}
}
//============================================================================

void Translation::display()      // Display the language and meanings
{
	cout << setw(10) << left << this->language << ":  ";      // Display the language
	for (int i = 0; i < this->meanings.size(); i++) {      // Loop through the meanings vector
		cout << this->meanings[i];      // Display the meanings
		if (i < this->meanings.size() - 1) {      // Check if the meaning is not the last one
			cout << "; ";      // Display a semicolon and a space
		}
	}
	cout << endl;      // Display a new line
}
//============================================================================

Entry::Entry(string word, string meanings, string language)      // Constructor
{
	this->word = word;      // Assign the word to the object
	Translation t(meanings, language);       // Create a Translation object
	this->translations.push_back(t);       // Add the Translation object to the translations vector
	this->deleted = false;      // Set the deleted variable to false
}
//============================================================================

void Entry::addTranslation(string newMeanings, string language)       // Add a new translation to the vector
{
	for (int i = 0; i < this->translations.size(); i++) {       // Loop through the translations vector
		if (this->translations[i].language == language) {       // Check if the language is already in the vector
			this->translations[i].addMeaning(newMeanings);      // Add the new meanings to the vector
			return;      // Return
		}
	}
	Translation t(newMeanings, language);       // Create a Translation object
	this->translations.push_back(t);       // Add the Translation object to the translations vector
}
//============================================================================

void Entry::print()     // Print the word and its translations
{
	for (int i = 0; i < this->translations.size(); i++) {      // Loop through the translations vector
		this->translations[i].display();       // Display the language and meanings
	}
}
//============================================================================

HashTable::HashTable(int capacity)       // Constructor
{
	this->buckets = new Entry * [capacity];       // Create an array of Entry pointers
	this->size = 0;       // Set the size to 0
	this->capacity = capacity;       // Set the capacity to the given capacity
	this->collisions = 0;       // Set the collisions to 0
	for (int i = 0; i < capacity; i++) {       // Loop through the array
		this->buckets[i] = nullptr;       // Set each element to nullptr
	}
}
//============================================================================

unsigned long HashTable::hashCode(string word)      // Hash function
{
	unsigned long hash = 0;      // Initialize the hash to 0
	int shift = 5;      // Set the shift to 5

	for (int i = 0; i < word.length(); i++) {     // Loop through the word
		hash = (hash << shift) | (hash >> (32 - shift));      // Shift the hash
		hash += (unsigned long)word[i];       // Add the ASCII value of the character to the hash
	}
	return hash % this->capacity;      // Return the hash modulo the capacity
}
//============================================================================

unsigned int HashTable::getSize()      // Get the size of the HashTable
{
	return this->size;      // Return the size
}
//============================================================================

unsigned int HashTable::getCollisions()      // Get the number of collisions
{
	return this->collisions;       // Return the collisions
}
//============================================================================

void HashTable::import(string path)       // Import a dictionary file
{
	ifstream zfile(path);      // Open the file
	if (!zfile.is_open()) {      // Check if the file is open
		cerr << "File not found";       // Display an error message
		exit(1);     // Exit the program
	}
	string line, zlanguage, zword, zmeanings;      // Create strings to store the line, language, word, and meanings
	getline(zfile, zlanguage);      // Get the language
	int count = 0;      // Initialize the count to 0
	while (getline(zfile, line)) {      // Loop through the file
		stringstream s(line);     // Create a stringstream object
		getline(s, zword, ':');     // Get the word
		getline(s, zmeanings);      // Get the meanings
		this->insert(zword, zmeanings, zlanguage);     // Insert the word, meanings, and language
		count++;     // Increment the count
	} 
	zfile.close();      // Close the file
	cout << count << " " << zlanguage << " words have been imported successfully." << endl;      // Display the number of words imported
}
//============================================================================

void HashTable::addWord(string word, string meanings, string language)     // Add a word to the dictionary
{
	this->insert(word, meanings, language);      // Insert the word, meanings, and language
	cout << word << " has been successfully added to the Dictionary." << endl;      // Display a success message
}
//============================================================================

void HashTable::delWord(string word)      // Delete a word from the dictionary
{
	string lword = word;      // Create a string to store the lowercase word
	for (int i = 0; i < word.length(); i++) {      // Loop through the word
		if (word[i] >= 'A' && word[i] <= 'Z') {     // Check if the character is uppercase
			lword[i] = word[i] + 32;      // Convert the character to lowercase
		}
	}
	unsigned long index = this->hashCode(lword);	  // Get the hash code of the lowercase word
	if (this->buckets[index] == nullptr) {       // Check if the bucket is empty
		cout << word << " not found in the Dictionary." << endl;       // Display a message
	}
	else {      // If the bucket is not empty
		do {      // Do while loop
			if (this->buckets[index]->word == lword) {      // Check if the word is found
				if (buckets[index]->deleted) {      // Check if the bucket is deleted
					cout << word << " not found in the Dictionary." << endl;      // Display a message
					return;      // Return
				}
				else {      // If the bucket is not deleted
					this->buckets[index]->deleted = true;      // Set the deleted variable to true
					this->size--;      // Decrement the size
					cout << word << " has been successfully deleted from the Dictionary." << endl;      // Display a message
					return;      // Return
				}
			}
			index = (index + 1) % capacity;      // Increment the index
		} while (this->buckets[index] != nullptr);      // Loop while the bucket is not empty
		cout << word << " not found in the Dictionary." << endl;     // Display a message
	}
}
//============================================================================

void HashTable::insert(string word, string meanings, string language)      // Insert a word to the dictionary
{
	for (int i = 0; i < word.length(); i++) {      // Loop through the word
		if (word[i] >= 'A' && word[i] <= 'Z') {      // Check if the character is uppercase
			word[i] = word[i] + 32;      // Convert the character to lowercase
		}
	}
	unsigned long index = this->hashCode(word);      // Get the hash code of the word
	if (this->buckets[index] == nullptr) {      // Check if the bucket is empty
		this->buckets[index] = new Entry(word, meanings, language);     // Create a new Entry object
		this->size++;      // Increment the size
	}
	else {     // If the bucket is not empty
		do {      // Do while loop
			if (buckets[index]->word == word) {      // Check if the word is found
				if (buckets[index]->deleted) {      // Check if the bucket is deleted
					buckets[index] = new Entry(word, meanings, language);      // Create a new Entry object
				}
				else {      // If the bucket is not deleted
					buckets[index]->addTranslation(meanings, language);      // Add the new meanings to the vector
					return;     // Return
				}
			}
			index = (index + 1) % capacity;       // Increment the index    
			collisions++;      // Increment the collisions
		} while (buckets[index] != nullptr);      // Loop while the bucket is not empty
		buckets[index] = new Entry(word, meanings, language);     // Create a new Entry object
		size++;      // Increment the size
	}
}
//============================================================================

void HashTable::delTranslation(string word, string language)      // Delete a translation from the dictionary
{
	string lword = word;      // Create a string to store the lowercase word
	for (int i = 0; i < word.length(); i++) {      // Loop through the word
		if (word[i] >= 'A' && word[i] <= 'Z') {       // Check if the character is uppercase
			lword[i] = word[i] + 32;       // Convert the character to lowercase
		}
	}
	unsigned long index = this->hashCode(lword);      // Get the hash code of the lowercase word
	if (this->buckets[index] == nullptr) {       // Check if the bucket is empty
		cout << word << " not found in the Dictionary." << endl;       // Display a message
	}
	else {      // If the bucket is not empty
		do {      // Do while loop
			if (this->buckets[index]->word == lword) {      // Check if the word is found
				if (buckets[index]->deleted) {      // Check if the bucket is deleted
					cout << word << " not found in the Dictionary." << endl;     // Display a message
					return;      // Return
				}
				else {      // If the bucket is not deleted
					for (int i = 0; i < this->buckets[index]->translations.size(); i++) {      // Loop through the translations vector
						if (this->buckets[index]->translations[i].language == language) {      // Check if the language is found
							this->buckets[index]->translations.erase(this->buckets[index]->translations.begin() + i);     // Delete the translation
							cout << "Translation of " << word << " in " << language << " has been successfully deleted from the Dictionary." << endl;      // Display a message
							return;      // Return
						}
					}
					cout << "Translation of " << word << " in " << language << " not found." << endl;     // Display a message
					return;     // Return
				}
			}
			index = (index + 1) % capacity;     // Increment the index
		} while (this->buckets[index] != nullptr);      // Loop while the bucket is not empty
		cout << word << " not found in the Dictionary." << endl;     // Display a message
	}
}
//============================================================================

void HashTable::delMeaning(string word, string meaning, string language)      // Delete a meaning from the dictionary
{
	string lword = word;     // Create a string to store the lowercase word
	for (int i = 0; i < word.length(); i++) {      // Loop through the word
		if (word[i] >= 'A' && word[i] <= 'Z') {      // Check if the character is uppercase
			lword[i] = word[i] + 32;      // Convert the character to lowercase
		}
	}
	unsigned long index = this->hashCode(lword);      // Get the hash code of the lowercase word
	if (this->buckets[index] == nullptr) {      // Check if the bucket is empty
		cout << word << " not found in the Dictionary." << endl;      // Display a message
	}
	else {     // If the bucket is not empty
		do {      // Do while loop
			if (this->buckets[index]->word == lword) {      // Check if the word is found
				if (buckets[index]->deleted) {      // Check if the bucket is deleted
					cout << word << " not found in the Dictionary." << endl;      // Display a message
					return;     // Return
				}
				else {      // If the bucket is not deleted
					for (int i = 0; i < this->buckets[index]->translations.size(); i++) {      // Loop through the translations vector
						if (this->buckets[index]->translations[i].language == language) {      // Check if the language is found
							for (int j = 0; j < this->buckets[index]->translations[i].meanings.size(); j++) {      // Loop through the meanings vector
								if (this->buckets[index]->translations[i].meanings[j] == meaning) {      // Check if the meaning is found
									this->buckets[index]->translations[i].meanings.erase(this->buckets[index]->translations[i].meanings.begin() + j);      // Delete the meaning
									cout << "Meaning of " << word << " in " << language << " has been successfully deleted from the Dictionary." << endl;      // Display a message
									return;      // Return
								}
							}
							cout << "Meaning of " << word << " in " << language << " not found." << endl;      // Display a message
							return;     // Return
						}
					}
					cout << "Translation of " << word << " in " << language << " not found." << endl;     // Display a message
					return;      // Return
				}
			}
			index = (index + 1) % capacity;      // Increment the index
		} while (this->buckets[index] != nullptr);      // Loop while the bucket is not empty
		cout << word << " not found in the Dictionary." << endl;      // Display a message
	}
}
//============================================================================

void HashTable::exportData(string language, string filePath)     // Export a dictionary to a file
{
	ofstream zfile(filePath);      // Open the file
	int count = 0;      // Initialize the count to 0
	if (!zfile.is_open()) {     // Check if the file is open
		cerr << "File not found";     // Display an error message
		exit(1);     // Exit the program
	}
	zfile << language << endl;     // Write the language to the file
	for (int i = 0; i < this->capacity; i++) {      // Loop through the array
		if (this->buckets[i] != nullptr && !this->buckets[i]->deleted) {      // Check if the bucket is not empty
			zfile << this->buckets[i]->word << ":";     // Write the word to the file
			count++;     // Increment the count
			for (int j = 0; j < this->buckets[i]->translations.size(); j++) {     // Loop through the translations vector
				if (this->buckets[i]->translations[j].language == language) {     // Check if the language is found
					for (int k = 0; k < this->buckets[i]->translations[j].meanings.size(); k++) {      // Loop through the meanings vector
						zfile << this->buckets[i]->translations[j].meanings[k];     // Write the meanings to the file
						if (k < this->buckets[i]->translations[j].meanings.size() - 1) {     // Check if the meaning is not the last one
							zfile << ";";      // Write a semicolon to the file
						}
					}
					break;     // Break the loop
				}
			}
			zfile << endl;     // Write a new line to the file
		}
	}
	zfile.close();     // Close the file
	cout << count << " records have been successfully exported to " << filePath << endl;     // Display a message
}
//============================================================================

void HashTable::find(string word)     // Find a word in the dictionary
{
	string lword = word;     // Create a string to store the lowercase word
	for (int i = 0; i < word.length(); i++) {     // Loop through the word
		if (word[i] >= 'A' && word[i] <= 'Z') {      // Check if the character is uppercase
			lword[i] = word[i] + 32;      // Convert the character to lowercase
		}     
	}
	unsigned long index = this->hashCode(lword);     // Get the hash code of the lowercase word
	int count = 1;     // Initialize the count to 1
	if (this->buckets[index] == nullptr) {     // Check if the bucket is empty
		cout << word << " not found in the Dictionary." << endl;     // Display a message
	}  
	else {     // If the bucket is not empty
		do {     // Do while loop
			if (this->buckets[index]->word == lword) {      // Check if the word is found
				if (buckets[index]->deleted) {     // Check if the bucket is deleted
					cout << word << " not found in the Dictionary." << endl;     // Display a message
					return;     // Return
				}
				else {
					cout << word << " found in the Dictionary after " << count << " comparisons." << endl;     // Display a message
					this->buckets[index]->print();      // Print the word and its translations
					return;     // Return
				}
			}
			index = (index + 1) % capacity;      // Increment the index
			count++;      // Increment the count
		} while (this->buckets[index] != nullptr);      // Loop while the bucket is not empty
		cout << word << " not found in the Dictionary." << endl;     // Display a message
	}
}
//============================================================================

HashTable::~HashTable()      // Destructor
{
	for (int i = 0; i < capacity; i++) {     // Loop through the array
		if (this->buckets[i] != nullptr) {     // Check if the bucket is not empty
			delete this->buckets[i];     // Delete the bucket
		}
	}
	delete[] this->buckets;     // Delete the array
}
//============================================================================
