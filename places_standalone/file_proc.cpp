/*
Liza Kadochnikova
file_proc.cpp

Provides the function that parses places2k.txt into an array
of structures
*/

#include "file_proc.h"

RadixTrie* parseFile(){
	
	string fileName = PLACES_F_NAME;
	ifstream data;
	RadixTrie* db = new RadixTrie();
			
	try {
		// open file
		data.open(fileName);
		string dataLine;
		
		// read a line at a time		
		while(getline(data, dataLine)){
			// extract and parse relevant fields, push to db
			process_record(dataLine, db);
		}
		
		// close file
		data.close();
		
		return db;
	}
	catch (std::exception e) {
		cout << "! error processing file" << endl;
		delete db;
	}
	return NULL;
}

// name field needs major processing, may spawn more than one record per line
void process_record(string& dataLine, RadixTrie* db) {
	int pos;
	const string WHITESPACE = " \t\f\v\n\r"; 
	string name, state, lon, lat;
			
	state = dataLine.substr(STATE_POS,STATE_SIZE);
	name = dataLine.substr(NAME_POS,NAME_SIZE);			
	lat = dataLine.substr(LAT_POS,LAT_SIZE);
	lon = dataLine.substr(LONG_POS,LONG_SIZE);
			
	// name needs LOTS of formatting
	// need to erase trailing whitespace
	pos = name.find_last_not_of(WHITESPACE);
	// deal with random "(balance)" suffix
	if (name[pos] == ')'){
		pos = name.find_last_of('(', pos);
		pos = name.find_last_not_of(WHITESPACE,pos-1); // skip to next word
	}
	name.erase(pos+1);
		
	//split on "-" or "(...)"
	int paren_pos = name.find_first_of('(');
	
	int dash_pos = name.find_first_of('-');
	
	if(paren_pos >= name.length() && dash_pos >= name.length()){
		// make all lowercase? - yes
		make_lowercase(name);
		// append to state
		state.append(name);
		// push to database
		db.insert(state, stof(lat), stof(lon));
		return;
	}
	
	// else parse multiple entries
	// mark designation start
	pos = name.find_last_of(WHITESPACE);
	
		// keep place designation(town, city, CDP, village, borough, etc.)
		string designation = name.substr(pos);
	
	// make all lowercase? - yes
	
	
	// append to state
	state.append(name);
	// push to database
	db.insert(state, stof(lat), stof(lon));
}		

void make_lowercase(string& s){
	
}

int main(){
	
	return 0;
}