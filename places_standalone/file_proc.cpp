/*
Liza Kadochnikova
file_proc.cpp

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

#define PLACES_F_NAME "places2k.txt"

// relevant data
struct Record {
	string name; // includes state
	float latitude;
	float longitude;
	Record(string n, float lt, float ln): name(n),
		latitude(lt), longitude(ln){}
	void swap( Record& x, Record& y ){}
	bool operator()( const Record& x, const Record& y )
	{return x.name_p < y.name_p;} // ascending order A..Z
} record_obj;	// object to use for sort comparison function

void process_name_field(string& name);

RadixTrie* parseFile(){
	
	string fileName = PLACES_F_NAME;
	ifstream data;
	vector<Record> records;
	
	try {
		// open file
		data.open(fileName);
		string dataLine;

		// read a line at a time		
		while(getline(data, dataLine)){
			// extract and parse relevant fields, push to records
			process_record(dataLine, records);
		}
		
		// close file
		data.close();
		
		// sort the vector
		sort(records.begin(), records.end(), record_obj);
		// insert contents of the vector into a Radix-Trie
		
		// for now, print things
		for (int i = 0; i < 10; i++){
			cout<< records[i].name << "," << records[i].state<< ": ";
			cout<< records[i].latitude <<","<< records[i].longitude <<endl; 
		}
		
		// stick each into a Trie
		RadixTrie* db = new RadixTrie();
		for( vector<Record>::iterator it=records.begin(); it!=records.end(); ++it ){
			db.insert(it->name, it->latitude, it->longitude);
		}
		return db;
	}
	catch (std::exception e) {
		cout << "! error processing file" << endl;
	}
	return NULL;
}

// name field needs major processing, may spawn more than one record per line
void process_record(string& dataLine, vector<Record>& records) {
	int pos;
	string allowed = "";
	
	string name, state, lon, lat;
			
	state = dataLine.substr(STATE_POS,STATE_SIZE);
	name = dataLine.substr(NAME_POS,NAME_SIZE);			
	lat = dataLine.substr(LAT_POS,LAT_SIZE);
	lon = dataLine.substr(LONG_POS,LONG_SIZE);
			
	// name needs lots of formatting
	// need to erase trailing whitespace
	pos = name.find_last_not_of(" \t\f\v\n\r");
	// and place designation(town, city, CDP)
	pos = name.find_last_of(" ");
	name.erase(pos);
	
	// deal with random parentheses
	// replace special characters with spaces
	
	// make all lowercase
	
	// put them into struct, push to vector
	Record r = Record(name,stof(lat),stof(lon));
	records.push_back(r);
}		

int main(){
	
	return 0;
}