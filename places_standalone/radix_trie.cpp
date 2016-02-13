/*
Liza Kadochnikova
radix_trie.cpp

Describes the Radix Trie data structure to store coodrinates,
keyed by a prefix string (which represents location)
V2.0
permits special characters (any)
V2.1
case-insensitive compare
*/

#include "radix_trie.h"
using namespace std;

// TNode constructor
TNode::TNode(string k, TNode* pt){
	key = k;
	data = NULL;
	parent = pt;
	for (int i=0; i<NUM_AB_CHILDREN; i++){
		abchildren[i] = NULL;
	}
	schildren =  NULL;
}

// TNode destructor. Only destroy self, not children.
TNode::~TNode(){
	if (data) { delete data; }
	if (schildren) { delete schildren; }
}

void TNode::assignChild(char key, TNode* child){
	int i = mapToBranch(key);
	
	if (i >= 0) {// in array
		abchildren[i] = child;
		return;
	}
	else if (schildren){  // special characters in list
		// check if exists
		for (list<SpecChild>::iterator it=schildren->begin(); it!=schildren->end(); ++it){
			if (key == it->id) { it->node = child; return;}
		}
		// does not exist, add
		schildren->push_front(SpecChild(key,child));
		return;
	}
	// initialize list with one element
	schildren = new list<SpecChild>();
	schildren->push_front(SpecChild(key,child));
}

TNode* TNode::getChild(char key){
	int i = mapToBranch(key);
	
	if (i >= 0) {// in array
		return abchildren[i];
	}
	else if (schildren){  // special characters in list
		for (list<SpecChild>::iterator it=schildren->begin(); it!=schildren->end(); ++it){
			if (key == it->id) { return it->node;}
		}
	}
	return NULL;
}

// Maps character to branch on onto the index in abchildren[]
// assumes ASCII encoding
int TNode::mapToBranch(char key){
	if( key >='a' && key <= 'z'){ return (int)(key - 'a');}
	if( key >='A' && key <= 'Z'){ return (int)(key - 'A');}
	if( key == ' '){ return SPACE_SPOT; }
	return -1;
}

// RadixTrie constructor. Makes an empty trie.
RadixTrie::RadixTrie(){
	root = new TNode("", NULL); // empty string at root
}

// RadixTrie destructor. Destroys root and all subtrees recursively.
RadixTrie::~RadixTrie(){
	recursiveTNodeDelete(root);
}
// Takes care of the recursive tree deletion
void RadixTrie::recursiveTNodeDelete(TNode* n){
	for (int i=0; i<NUM_AB_CHILDREN; i++){
		if(n->abchildren[i]) {recursiveTNodeDelete(n->abchildren[i]);}
	}
	if (n->schildren){  // special characters 
		for (list<TNode::SpecChild>::iterator it=n->schildren->begin(); it!=n->schildren->end(); ++it){
			recursiveTNodeDelete(it->node);
		}
	}
	delete n;
}	
	
int RadixTrie::insert(string word, float lat, float lon){
	int word_pos, node_pos;
	// find insert position
	TNode* target = match(&word, root, word_pos, node_pos);
	
	// case 1: full match at this node (entry exists, or is a prefix)
	if (node_pos < 0 && word_pos < 0) {
		// just update value?
		if (target->data) {
			target->data->lat = lat;
			target->data->lon = lon;
		} else {
			target->data = new Coord(lat,lon);
		}
	}
	// case 2: prefix match, but extra characters in word left over
	else if(node_pos < 0 && word_pos >= 0) {
		// make new child node with remaining name substring
		TNode* child = new TNode(word.substr(word_pos), target);
		target->assignChild(word[word_pos], child);
		child->data = new Coord(lat,lon);
	}
	// case 3: match ends mid-node-string, new branch required
	else { // node_pos >= 0
		// 1. new fork node gets to keep part of taret's key[0.. node_pos -1]
		// and gets completely new children (2,3)
		TNode* new_fork = new TNode(target->key.substr(0, node_pos), target->parent);
		(new_fork->parent)->assignChild(new_fork->key[0],new_fork);

		// 2. this node becomes a subtree child(branch@key[node_pos])
		// with remaining sub-node-string, and keeps all the original children
		target->key = target->key.substr(node_pos);
		new_fork->assignChild(target->key[0],target);
		target->parent = new_fork;
		
		// 3. the new word 
		// 3a. word is a perfect match (no more substring tail)
		if (word_pos < 0) {
			new_fork->data = new Coord(lat,lon);
		}
		// 3b. there is a substring (new leaf, branch@word[word_pos])
		else {
			TNode* new_child = new TNode(word.substr(word_pos), new_fork);
			new_fork->assignChild(new_child->key[0], new_child);
			new_child->data = new Coord(lat,lon);
		}
	}
	return 0;
}

// return 	 0 if found
// 			 1 if unique prefix
// 			-1 if not found
// 			-2 if prefix to more than one 
// word		updated to full key if (1), unchanged otherwise
// result 	is meaningful only on positive return value
int RadixTrie::find(string& word, Coord& result){
	int word_pos, node_pos;
	// find insert position
	TNode* node = match(&word, root, word_pos, node_pos);
	
	// case 1: full match at this node (entry exists, or is a prefix)
	if (node_pos < 0 && word_pos < 0) {
		if (node->data){ // match
			result.lat = node->data->lat;
			result.lon = node->data->lon;
			return 0; 
		} else { // prefix to more than one
			return -2;
		}
	}
	// case 2: word found, but is a prefix ending mid-node-string
	else if (node_pos >= 0 && word_pos < 0) { 
		if (node->data){
			result.lat = node->data->lat;
			result.lon = node->data->lon;
			word.append(node->key.substr(node_pos));
			return 1; 
		} else {// prefix to more than one
			return -2;
		}
	}
	return -1;	// not found
}

// finds the longest prefix match of word to the contents of this trie
// (used to find insertion position and conditions)
// ?? if a position is set to negative value, that means we ran off the edge 
// of the string (full match) ???? 
//  - word_pos position in the string, at which the prefix match is broken [-1,0..size-1]
//  - node_pos corresponding position in the sequense of current node-string [-1,0..size-1]
//  returns pointer to the last node in the matching sequence
TNode* RadixTrie::match(string* word, TNode* start, int& word_pos, int& node_pos){
	bool done = false; // termination condition
	int wi = 0; 	// word position index
	int ni = 0;		// node-key position index
	TNode* curr = start;
	int wmax = word->length();
	
	while(!done){
		// explore this node's key
		int nmax = curr->key.length();
		while(ni<nmax && wi <wmax) {
			if(!noCaseEquals(curr->key[ni],(*word)[wi])){ // mismatch found
				done = true;
				break;
			}
			ni++; wi++;
		}
		
		if (done){ break;}
		if(ni == nmax && wi == wmax) {
			// prefect match, we're done
			wi = -1; ni = -1; break;
		} else if(wi == wmax){
			// word ended first, we're done
			wi = -1; break;
		} else {
			// key ended first, explore child
			TNode* child = curr->getChild((*word)[wi]);
			if(child){ // child exists, continue
				curr = child;
				ni = 0;
			} else { // nothing more to explore
				ni = -1; break;
			}
		}
	}
	
	word_pos = wi;
	node_pos = ni;
	return curr;
}

bool RadixTrie::noCaseEquals(char x, char y){
	return tolower(x) == tolower(y);
}

void RadixTrie::printTraverse(){
	recursivePrintPreorder(root);
	cout << endl;
}

void RadixTrie::recursivePrintPreorder(TNode* n){
	cout << "[" << n->key ;
	if (n->data) {
		cout << "->("<< n->data->lat <<","<< n->data->lon <<")";
	}
	for (int i=0; i<NUM_AB_CHILDREN; i++){
		if(n->abchildren[i]) {recursivePrintPreorder(n->abchildren[i]);}
	}
	if (n->schildren){  // special characters 
		for (list<TNode::SpecChild>::iterator it=n->schildren->begin(); it!=n->schildren->end(); ++it){
			recursivePrintPreorder(it->node);
		}
	}
	cout << "]";
}

// test program
int main(){
	RadixTrie r = RadixTrie();
	r.printTraverse();
	r.insert("amnesty", 1,0);
	r.printTraverse();
	r.insert("international", 0,1);
	r.printTraverse();
	r.insert("internship", 0,2);
	r.printTraverse();
	r.insert("i'm fine", 0,3);
	r.printTraverse();
	r.insert("allez", 2,0);
	r.printTraverse();
	r.insert("all", 3,0);
	r.printTraverse();
	r.insert("all*bright", 4,0.5);
	r.printTraverse();
	
	Coord xy(360,360); 
	string words[6] =  {"boom", "I'm Fine", "all", "intern", "all*", "internshipdom"};
	for (int i = 0; i<6; i++){
		cout << "find("<< words[i]<<") : ";
		int res = r.find(words[i], xy);
		cout<< res << " "<<words[i]<<" ["<<xy.lat << "," << xy.lon <<"]"<<endl;	
	}
}