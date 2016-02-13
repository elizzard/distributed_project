/*
Liza Kadochnikova
file_proc.cpp

Provides the function that parses places2k.txt into an array
of structures
*/

#include "file_proc.h"

RadixTrie* parseFile(string fileName){
	
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
	int paren_pos = name.find_last_of(')');
	
	int dash_pos = name.find_last_of('-');
	
	if(string::npos== paren_pos && string::npos== dash_pos){
		// append to state
		state.append(name);
		// push to database
		db.insert(state, stof(lat), stof(lon));
		return;
	}
	// else parse multiple entries
	
	// keep place designation(town, city, CDP, village, borough, municipality, comunidad, zona urbana ...)
	pos = name.find_last_of(WHITESPACE);
	string designation = name.substr(pos);
	name.erase(pos);
	
	while(string::npos != paren_pos){
		int begin_paren = name.find_last_of('(');
		string sub_name = name.substr(begin_paren+1,paren_pos - begin_paren -1);
		
		sub_name.append(designation);
		string st = state;   // copy, b/c don't want to mutate state
		st.append(sub_name);
		db.insert(st, stof(lat), stof(lon));
		
		name.replace(begin_paren, paren_pos - begin_paren +1,"");
		paren_pos = name.find_last_of(')');
	}
	
	// if there's more than one dash, or the length of
	// left split string is too small (1-2 characters), it is probably all one name
	while(string::npos != dash_pos){
		
	}
}		

int main(){
	RadixTrie* db = parseFile(TEST_F_NAME);
	
	
	
	delete db;
	return 0;
}