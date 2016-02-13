/*
Liza Kadochnikova
file_proc.cpp

Provides the function that parses places2k.txt into an array
of structures

NOTE: use <g++ -std=c++11 *.cpp>
*/

#include "file_proc.h"
#include <iostream>
#include <fstream>

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
	size_t pos;
	const string WHITESPACE = " \t\f\v\n\r"; 
	string name, state, num;
	float lon, lat;
			
	state = dataLine.substr(STATE_POS,STATE_SIZE);
	name = dataLine.substr(NAME_POS,NAME_SIZE);			
	lat = stof(dataLine.substr(LAT_POS,LAT_SIZE));
	lon = stof(dataLine.substr(LONG_POS,LONG_SIZE));
			
	// name needs LOTS of formatting
	// need to erase trailing whitespace
	pos = name.find_last_not_of(WHITESPACE);
	// deal with random "(balance)" suffix
	if (name[pos] == ')'){
		pos = name.find_last_of('(', pos);
		pos = name.find_last_not_of(WHITESPACE,pos-1); // skip to next word
	}
	name.erase(pos+1);
	stripDesignation(name);
		
	//split on "-" or "(...)"
	size_t paren_pos = name.find_last_of(')');
	size_t dash_pos = name.find_first_of('-');
	
	if(string::npos== paren_pos && string::npos== dash_pos){
		// append to state
		state.append(name);
		// push to database
		db->insert(state, lat, lon);
		return;
	}
	// else parse multiple entries
	if(string::npos != paren_pos){
		size_t begin_paren = name.find_last_of('(');
		string sub_name = name.substr(begin_paren+1,paren_pos - begin_paren -1);
		
		string st = state;   // copy, b/c don't want to mutate state
		st.append(sub_name);
		db->insert(st, lat, lon);
		
		// trim string
		name.replace(begin_paren-1, paren_pos - begin_paren +2,"");
		
		// if no dash, push remaining name to db
		if (string::npos == dash_pos){
			state.append(name);
			db->insert(state, lat, lon);
			return;
		}
	}
	
	// if there's more than one dash before first space, or the length of
	// left split string is too small (1-2 characters), or word after dash
	// starts with a lowercase, then it is probably all one name
	if(string::npos != dash_pos){
		size_t dash_pos_next = name.find_first_of('-', dash_pos+1);
		// all one hyphenated word
		if(string::npos != dash_pos_next || 
			name[dash_pos+1] == tolower(name[dash_pos+1]) ||
			dash_pos < 2){ 
			state.append(name);
			db->insert(state, lat, lon);
			return;
		}
		// else need to split
		string st = state;   // copy prefix for 2nd name entry
		st.append(name.substr(0,dash_pos));
		db->insert(st, lat, lon);
		
		state.append(name.substr(dash_pos+1));
		db->insert(state, lat, lon);
	}
}		

// if the last word in the string is a designation identifier, erase it
bool stripDesignation(string& s){
	const int D_SIZE = 9;
	const string DESIGNATIONS[D_SIZE] = {"town", "city", "CDP", "County",
		"village", "borough", "municipality", "comunidad", "urbana"};
	const string WHITESPACE = " \t\f\v\n\r"; 
	
	size_t pos = s.find_last_of(WHITESPACE);
	string tail = s.substr(pos+1);
	
	for (int i=0; i<D_SIZE; i++){
		if ( DESIGNATIONS[i]== tail) { 
			s.erase(pos);
			if (i == D_SIZE-1){ // need to delete preceding "zona" as well
				pos = s.find_last_of(WHITESPACE);
				s.erase(pos);
			}
			return true;
		}
	}
	return false;
}
	
/*	-----------------------------------------------------------------
	TEST!  
	This main will need to be either commented out, 
	or something in makefile needs to tell linker to ignore this main 
	( idk how, easier to just comment it out)
*/
int main(){
/* small test */ 
//	RadixTrie* db = parseFile(TEST_F_NAME);
//	db->printTraverse();
	
	/*  -------------------------------------------------------------
		This goes into *_svc.c file (which says do not modify)
		right into main(), as early in the function as possible.
		Don't forget to #include "file_proc.h" (or add this line to .h generated from .x)
	*/
	RadixTrie* db = parseFile(PLACES_F_NAME);	// Maybe global variable?
	
	
	/* -------------------------------------------------------------
		Something like this pseudocode-ish goes  into *_server.c
		whatever is the equivalent of readdir_1_svc from HW1
		Don't forget to #include "file_proc.h" (or add this line to .h generated from .x)
	*/
	// may need to xdr_free on any leftover linked list received from airports
	// get name (from client = fxn argument list)
	string s = "FLSt. Augustine";				
	// strip designation
	stripDesignation(s);	// shoudld do nothing
	// do search
	Coord xy(360,360); 		// this is where coordinate values will be returned
	int res = db->find(s, xy);	
	// value of res explains the result:
	// if not found (res <0), return NULL
	// else if found (res >=0), call airports fxn ...
	cout<< res << " "<<s<<" ["<<xy.lat << "," << xy.lon <<"]"<<endl;
	// return-fwd KNN results
	
	/* -------------------------------------------------------------
		this delete also goes into *_svc.c file (which says do not modify)
		at the very end of main().
	*/
	delete db;
	
	return 0;
}