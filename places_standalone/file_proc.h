/*
Liza Kadochnikova
file_proc.h

Provides the function that parses places2k.txt into an array
of structures
*/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "radix_trie.h"

using namespace std;

//positions of relevant data in file 
#define STATE_POS 0
#define STATE_SIZE 2
#define NAME_POS 9
#define NAME_SIZE 64
#define LAT_POS 143
#define LAT_SIZE 10
#define LONG_POS 154
#define LONG_SIZE 10

// data already sorted, state first
#define PLACES_F_NAME "places2k.txt"

void process_record(string& dataLine, RadixTrie* db);
RadixTrie* parseFile();