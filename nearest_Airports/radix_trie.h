/*
Liza Kadochnikova
radix_trie.h

Declarations for RadixTrie data structure
V2.0
permits special characters (any)
V2.1
case-insensitive compare
*/

#ifndef _RADIX_TRIE_H_RPCGEN
#define _RADIX_TRIE_H_RPCGEN

#endif
#include <string>
#include <iostream>
#include <list>

// alphabet + " " + "'"
#define NUM_AB_CHILDREN 27
#define SPACE_SPOT 26

struct Coord{
	float lat;
	float lon;
	Coord(float lt, float ln): lat(lt), lon(ln) {};
};

// trie node
class TNode{
public:	
	TNode* parent;
	std::string key; // aka node-string
	Coord* data;	// pointer to coordinate data

	struct SpecChild{
		char id;	// branch identifying character
		TNode* node; // pointer to subtree
		SpecChild(char c, TNode* p): id(c), node(p){};
	};
	std::list<SpecChild>* schildren; // branch on special characters
	TNode* abchildren[NUM_AB_CHILDREN];      // branch on common characters

	TNode(std::string key, TNode* parent);
	~TNode();
	void assignChild(char key, TNode* child);
	TNode* getChild(char key);  // NULL if DNE
private:
	int mapToBranch(char key);
};

class RadixTrie{
public:
	RadixTrie();
	~RadixTrie();
	int insert(std::string name, float lat, float lon);
	// return 0 if found, -1 if not found, 1 if more than one answer for 
	int find(std::string& name, Coord& result);
	void printTraverse();
	
private:
	TNode* root;
	void recursiveTNodeDelete(TNode* node);	
	void recursivePrintPreorder(TNode* node);
	TNode* match(std::string* word, TNode* start, int& word_pos, int& node_pos);
	bool noCaseEquals(char x, char y);
};
