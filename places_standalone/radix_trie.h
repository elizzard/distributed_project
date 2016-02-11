/*
Liza Kadochnikova
radix_trie.h

Declarations for RadixTrie data structure
*/

#include <string>
#include <iostream>
using namespace std;

// alphabet + " " + "'"
#define NUM_AB_CHILDREN 28
#define SPACE_SPOT 26
#define APOSTROPHE_SPOT 27

struct Coord{
	float lat;
	float lon;
	Coord(float lt, float ln): lat(lt), lon(ln) {};
};

// trie node
struct TNode{
	TNode* parent;
	TNode* abchildren[NUM_AB_CHILDREN]; // where to branch
	// list for extra characters? or: skip it for now
	string key; // aka node-string
	Coord* data;	// pointer to coordinate data
	TNode(string key, TNode* parent);
	~TNode();
};

class RadixTrie{
public:
	RadixTrie();
	~RadixTrie();
	int insert(string name, float lat, float lon);
	// return 0 if found, -1 if not found, 1 if more than one answer for 
	int find(string name, float& lat, float& lon);
	void printTraverse();
private:
	TNode* root;
	void recursiveTNodeDelete(TNode* node);	
	void recursivePrintPreorder(TNode* node);
	TNode* match(string* word, int& word_pos, int& node_pos);
	int mapToBranch(char key);
};
