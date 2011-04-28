//============================================================================
// Name        : csed_compiler.cpp
// Author      : Ahmed Kotb
// Version     :
// Copyright   : All Rights Reserved KotbCorp
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "NFA.h"
#include "DFA.h"
using namespace std;

int main() {
	cout << "The Best Compiler isA" << endl;

	//testing NFA

	NFA nfa1('x', 'z');
	NFA nfa2('b');

	cout << "===================================================" << endl;
	//Testing Simple NFA operations (ORing and concatenation)
	nfa1.join(&nfa2, NFA_JOIN_OR);
	NFA nfa3('c');
	nfa1.join(&nfa3, NFA_JOIN_CONCATENATE);
	nfa1.finalize_NFA(0);
	//nfa accept the pattern (x | y | z | b)c
	// nfa1.debug();

	cout << "===================================================" << endl;
	//Testing combined NFA
	NFA nfaA('a');
	nfaA.finalize_NFA(0);
	NFA nfaB('b');
	nfaB.finalize_NFA(1);
	vector<NFA *> vector;
	vector.push_back(&nfaA);
	vector.push_back(&nfaB);
	NFA * combined = NFA::create_combined_NFA(&vector);
	//combined->debug();

	cout << "===================================================" << endl;
	//Testing NFA to DFA conversion
	NFA nfa4('x', 'z');
	DFA* dfa = new DFA(&nfa4);
	dfa->debug();
	cout << "===================================================" << endl;
	//Testing NFA to DFA conversion
	// Example in compilers book page 155
	// (a|b)*abb
	// the dfa is found at page 156
	NFA nfa5('a');
	nfa5.join(new NFA('b'),NFA_JOIN_OR);
	nfa5.apply_star_closure();
	nfa5.join(new NFA("abb"),NFA_JOIN_CONCATENATE);
	nfa5.finalize_NFA(0);
	//nfa5.debug();
	DFA * result = new DFA(&nfa5);
	result->debug();
	return 0;
}
