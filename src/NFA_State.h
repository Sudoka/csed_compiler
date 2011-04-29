#ifndef NFA_STATE_H_
#define NFA_STATE_H_

#define INPUT_CHAR signed short
#define EPSILON -1

#include <string>
#include <vector>
#include <set>
#include <queue>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm>
using namespace std;

class NFA_State {

private:
	int id;
	string accepting_pattern;
	bool is_accepting;
	int token_id;

	multimap<INPUT_CHAR, NFA_State*> *transitions;
	multimap<INPUT_CHAR, NFA_State*>::iterator iterate;
	pair<multimap<INPUT_CHAR, NFA_State*>::iterator, multimap<INPUT_CHAR,
			NFA_State*>::iterator> return_it;

	//helper method to get_transition in case of epsilon input
	vector<NFA_State*>* epislon_closure(NFA_State*);

	//get transition helper with specified input
	vector<NFA_State *>* get_transitions_helper(INPUT_CHAR);

	//remove duplicates helper function
	inline void remove_duplicates(std::vector<INPUT_CHAR> *vec) {
		std::sort(vec->begin(), vec->end());
		vec->erase(std::unique(vec->begin(), vec->end()), vec->end());
	}

public:
	NFA_State();

	//add transition with specified input
	void add_transition(INPUT_CHAR, NFA_State *);

	//get transition with specified input
	vector<NFA_State *>* get_transitions(INPUT_CHAR);

	//returns state information
	string get_description();

	void set_id(int);
	int get_id();
	void set_token_id(int);
	bool is_accepting_state();
	int get_token_id();
	void set_accepting_pattern(string);
	string get_accepting_pattern();
	vector<INPUT_CHAR>* get_transitions_inputs();
	virtual ~NFA_State();
};

#endif /* NFA_STATE_H_ */
