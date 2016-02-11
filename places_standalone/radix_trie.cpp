/*
Liza Kadochnikova
radix_trie.cpp

Describes the Radix Trie data structure to store coodrinates,
keyed by a prefix string (location)

*/

#include "radix_trie.h"

// TNode constructor
TNode::TNode(string k, TNode* pt){
	key = k;
	data = NULL;
	parent = pt;
	for (int i=0; i<NUM_AB_CHILDREN; i++){
		abchildren[i] = NULL;
	}
}

// TNode destructor. Only destroy self, not children.
TNode::~TNode(){
	if (data) { delete data; }
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
void RadixTrie::recursiveTNodeDelete(TNode* node){
	for (int i=0; i<NUM_AB_CHILDREN; i++){
		if(node->abchildren[i]) {recursiveTNodeDelete(node->abchildren[i]);}
	}
	delete node;
}	
	
int RadixTrie::insert(string word, float lat, float lon){
	int word_pos, node_pos;
	// find insert position
	TNode* parent = match(&word, word_pos, node_pos);
	
	// case 1: full match at this node (entry exists, or is a prefix)
	if (node_pos < 0 && word_pos < 0) {
		// just update value?
		if (parent->data) {
			parent->data->lat = lat;
			parent->data->lon = lon;
		} else {
			parent->data = new Coord(lat,lon);
		}
	}
	// case 2: prefix match, but extra characters in word left over
	else if(node_pos < 0 && word_pos >= 0) {
		// make new child node with remaining name substring
		int i = mapToBranch(word[word_pos]);
		parent->abchildren[i] = new TNode(word.substr(word_pos), parent);
		parent->abchildren[i]->data = new Coord(lat,lon);
	}
	// case 3: match ends mid-node-string, new branch required
	else { // node_pos >= 0
		// 1. new parent node gets to keep key-part of this one [0.. node_pos -1]
		// and gets completely new children (2,3)
		TNode* new_parent = new TNode(parent->key.substr(0, node_pos), parent->parent);
		int i = mapToBranch(new_parent->key[0]);
		new_parent->parent->abchildren[i] = new_parent;

		// 2. this node becomes a subtree child(branch@key[node_pos])
		// with remaining sub-node-string 
		parent->key = parent->key.substr(node_pos);
		i = mapToBranch(parent->key[0]);
		new_parent->abchildren[i] = parent;
		parent->parent = new_parent;
		
		// 3. the new word 
		// word is a perfect match (no more substring tail)
		if (word_pos < 0) {
			new_parent->data = new Coord(lat,lon);
		}
		// there is a substring (new leaf, branch@word[word_pos])
		else {
			TNode* new_child = new TNode(word.substr(word_pos), new_parent);
			i = mapToBranch(new_child->key[0]);
			new_parent->abchildren[i] = new_child;
			new_child->data = new Coord(lat,lon);
		}
	}
	return 0;
}

// return 0 if found, -1 if not found, 1 if more than one answer for (it's a prefix) 
int RadixTrie::find(string word, float& lat, float& lon){
	int word_pos, node_pos;
	// find insert position
	TNode* node = match(&word, word_pos, node_pos);
	
	// case 1: full match at this node (entry exists, or is a prefix)
	if (node_pos < 0 && word_pos < 0) {
		if (node->data){ // match
			lat = node->data->lat;
			lon = node->data->lon;
			return 0; 
		} else { // prefix
			return 1;
		}
	}
	// word not found, but is a prefix ending mid-node-string
	else if (node_pos >= 0 && word_pos < 0) { return 1; }
	
	return -1;
}

// finds the longest prefix match of word to the contents of this trie
// (used to find insertion position and conditions)
// ?? if a position is set to negative value, that means we ran off the edge 
// of the string (full match) ???? 
//  - word_pos position in the string, at which the prefix match is broken [-1,0..size-1]
//  - node_pos corresponding position in the sequense of current node-string [-1,0..size-1]
//  returns pointer to the last node in the matching sequence
TNode* RadixTrie::match(string* word, int& word_pos, int& node_pos){
	bool done = false; // termination condition
	int wi = 0; 	// word position index
	int ni = 0;		// node-key position index
	TNode* curr = root;
	int wmax = word->length();
	
	while(!done){
		// explore this node's key
		int nmax = curr->key.length();
		while(ni<nmax && wi <wmax) {
			if(curr->key[ni] != (*word)[wi]){ // mismatch found
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
			TNode* child = curr->abchildren[mapToBranch((*word)[wi])];
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

// Maps character to branch on onto the index in abchildren[]
// assumes ASCII encoding
int RadixTrie::mapToBranch(char key){
	
	if( key >='a' && key <= 'z'){ return (int)(key - 'a');}
	if( key >='A' && key <= 'Z'){ return (int)(key - 'A');}
	if( key == ' '){ return SPACE_SPOT; }
	if( key == '\''){ return APOSTROPHE_SPOT;}
	return -1;
}

void RadixTrie::printTraverse(){
	recursivePrintPreorder(root);
	cout << endl;
}

void RadixTrie::recursivePrintPreorder(TNode* node){
	cout << "[" << node->key ;
	if (node->data) {
		cout << "->("<< node->data->lat <<","<< node->data->lon <<")";
	}
	for (int i=0; i<NUM_AB_CHILDREN; i++){
		if(node->abchildren[i]) {recursivePrintPreorder(node->abchildren[i]);}
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
	
	float lt = 0;
	float ln = 0;
	int res = r.find("i'm fine", lt, ln);
	cout << "find(i'm fine) : " << res << " ["<<lt << "," << ln <<"]"<<endl;
	res = r.find("all", lt, ln);
	cout << "find(all) : " << res << " ["<<lt << "," << ln <<"]"<<endl;
	res = r.find("intern", lt, ln);
	cout << "find(intern) : " << res << " ["<<lt << "," << ln <<"]"<<endl;
	res = r.find("boom", lt, ln);
	cout << "find(boom) : " << res << " ["<<lt << "," << ln <<"]"<<endl;
	res = r.find("internshipdom", lt, ln);
	cout << "find(internshipdom) : " << res << " ["<<lt << "," << ln <<"]"<<endl;
}