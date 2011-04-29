/*
 * NFA_State.cpp
 *
 *  Created on: Apr 22, 2011
 *      Author: ahmed
 */

#include "NFA_State.h"

map<int, bool> visited;

NFA_State::NFA_State() {
	//the id will be used as an identifier in the conversion to DFA state
	id = 0;
	//EPSILON is considered the accepting pattern for a new state
	accepting_pattern = "";

	//state is not considered (Token Acceptor) at creation
	is_accepting = false;

	//Accepting Token ID
	token_id = -1;

	//create map transition
	transitions = new multimap<INPUT_CHAR, NFA_State*> ();
}

bool NFA_State::is_accepting_state(){
	return is_accepting;
}

int NFA_State::get_token_id(){
	return token_id;
}

void NFA_State::add_transition(INPUT_CHAR input, NFA_State* state) {

	multimap<INPUT_CHAR, NFA_State*>::iterator it = transitions->find(input);
	//transition already found
	if (it != transitions->end() && it->second == state)
		return;

	transitions->insert(pair<INPUT_CHAR, NFA_State*> (input, state));

}

vector<NFA_State *>* NFA_State::get_transitions(INPUT_CHAR input) {

	//check epsilon input
	if (input == EPSILON) {
		visited.clear();
		return epislon_closure(this);
	} else
		return get_transitions_helper(input); //normal case
}

vector<NFA_State *>* NFA_State::get_transitions_helper(INPUT_CHAR input) {

	vector<NFA_State *> *result;
	result = new vector<NFA_State *> ();
	return_it = transitions->equal_range(input);
	//iterate over the input range
	for (iterate = return_it.first; iterate != return_it.second; ++iterate) {
		NFA_State *s = (*iterate).second;
		result->push_back(s);
	}
	return result;
}

vector<NFA_State *>* NFA_State::epislon_closure(NFA_State* s) {
	//visited states
	set<NFA_State *> visited;
	visited.insert(s);
	//bfs queue
	queue<NFA_State*> queue;
	queue.push(s);

	NFA_State * current_state;

	while (!queue.empty()){
		current_state = queue.front();
		queue.pop();
		//Neighbors
		vector <NFA_State *> * transitions = current_state->get_transitions_helper(EPSILON);
		for(unsigned int i = 0; i<transitions->size();i++){
			if (visited.find(transitions->at(i)) == visited.end()){
				//a new state was found
				visited.insert(transitions->at(i));
				queue.push(transitions->at(i));
			}
		}
		delete transitions;
	}
	//Why not return set instead of having to copy the set into a vector !!!
	vector <NFA_State *> * result = new vector<NFA_State *>();
    set<NFA_State *>::iterator it;
    for (it = visited.begin(); it != visited.end(); it++)
        result->push_back(*(it));
    return result;
}

void NFA_State::set_accepting_pattern(string accepting_pattern) {
	this->accepting_pattern = accepting_pattern;
}

void NFA_State::set_id(int new_id) {
	id = new_id;
}

int NFA_State::get_id(){
	return this->id;
}
void NFA_State::set_token_id(int token_id){
	//marks this state as accepting state with the given token id
	this->is_accepting = true;
	this->token_id = token_id;
}

string NFA_State::get_accepting_pattern() {
	return this->accepting_pattern;
}

vector<INPUT_CHAR>* NFA_State::get_transitions_inputs() {
	vector<INPUT_CHAR> *vec;
	vec = new vector<INPUT_CHAR> ();
	for (iterate = transitions->begin(); iterate != transitions->end(); ++iterate) {
		vec->push_back(iterate->first);
	}
	remove_duplicates(vec);
	return vec;
}

//returns state information
string NFA_State::get_description() {
	stringstream info;
	info << "----------------------------------------------\n";
	info << "State ID = " << (id >= 0 ? id : (int) this)  << "\n";
	info << "Accepting Pattern = " << accepting_pattern << "\n";
	if (is_accepting)
		info << "State Type = ACCEPTING Token with ID = " << token_id << "\n";
	else
		info << "State Type = Normal\n";

	if (transitions->size() > 0){
		info << "Transitions :\n";
		info << "=============\n";
		string input;
		for (iterate = transitions->begin(); iterate != transitions->end(); ++iterate) {
			if (iterate->first >= 0)
				input =(char)iterate->first;
			else
				input = "EPSILON";
			info << "[" << input << "  =>  " << iterate->second->id << "] , ";
		}
		info << "\n";
	}
	return info.str();
}

NFA_State::~NFA_State() {
	for (iterate = transitions->begin(); iterate != transitions->end(); ++iterate) {
		delete iterate->second;
		iterate->second = 0;
	}
	transitions->clear();
	delete transitions;
}

//int main() {
//	NFA_State *s;
//	NFA_State *v;
//	NFA_State *w;
//	NFA_State *q;
//
//	v = new NFA_State();
//	s = new NFA_State();
//	w = new NFA_State();
//	q = new NFA_State();
//
//	s->set_id(1);
//	v->set_id(2);
//	w->set_id(3);
//	q->set_id(4);
//
//	s->add_transition(EPSILON, v);
//	s->add_transition(EPSILON, q);
//	 s->add_transition('a', v);
//	 s->add_transition('a', v);
//	 s->add_transition('a', v);
//	 s->add_transition('b', q);
//	 s->add_transition('b', q);
//	 s->add_transition('b', q);
//	 s->add_transition('c', q);
//	 v->add_transition(EPSILON, s);
//	 //s->add_transition(EPSILON, s);
//	//s->show() ;
//	 v->add_transition(EPSILON, w);
//	//v->set_accepting_pattern("ssss");
//	//v->show();
//
//
//	//vector <INPUT_CHAR> *v2 = v->get_transitions_inputs();
//
//	vector<NFA_State*> *vec = s->get_transitions(EPSILON);
//
//	//cout << "returned" << endl;
//	//vector<INPUT_CHAR>::iterator iterate2;
//	vector<NFA_State*>::iterator iterate2;
//
//	/*for (iterate2 = v2->begin(); iterate2 != v2->end(); ++iterate2) {
//	 INPUT_CHAR c = (*iterate2);
//	 if(c == -1)
//	 cout << "EPSILON"<<" , ";
//	 else
//	 cout << char(c)<<" , ";
//	 }*/
//	cout <<"result"<<endl;
//	for (iterate2 = vec->begin(); iterate2 != vec->end(); ++iterate2) {
//		NFA_State *s = (*iterate2);
//		cout << s->get_description();
//		//cout << s->show()<<endl;
//	}
//
//	vec = s->get_transitions(EPSILON);
//	//cout <<"result2"<<endl;
//	for (iterate2 = vec->begin(); iterate2 != vec->end(); ++iterate2) {
//			NFA_State *s = (*iterate2);
//	//		cout << s->get_description();
//			//cout << s->show()<<endl;
//		}
//
//
//	return 0;
//}

