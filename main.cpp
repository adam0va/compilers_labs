#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <unordered_set>
#include <set>
#include <queue>
#include <map>
#include <utility>

// STEP 1: insert concatenation operator to regular expression
void addConcatSymbol(std::string &s) {
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '(' || s[i] == '|')
			continue;

		if (i < s.length() - 1) {
			if (s[i + 1] == '*' || s[i + 1] == '+' || s[i + 1] == '*' || s[i + 1] == '|' || s[i + 1] == ')') 
				continue; 
			else {
				s.insert(i + 1, ".");
				i++;
			}

		}
	}
}

// STEP 2: convert regular expression to postfix form
bool isOperator(char c) {
	return c == '.' || c == '*' || c =='|' || c == '+';
}

int getPrecedence(char c) {
	if (c == '|')
		return 0;
	else if (c == '.')
		return 1;
	else if (c == '*' || c == '+')
		return 2;
	else return -1;
}

std::string makePostfixForm(std::string &s, std::set<char> &alfabet) {
	std::stack<char> operatorStack;
	std::string outputQueue;
	int len = s.length();
	
	for (int i = 0; i < len; i++) {
		if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) {
			outputQueue.push_back(s[i]);
			alfabet.insert(s[i]);
		}
		else if (isOperator(s[i])) {
			while (	not(operatorStack.empty()) && 
					isOperator(operatorStack.top()) && 
					(getPrecedence(operatorStack.top()) >= getPrecedence(s[i]))) {
				outputQueue.push_back(operatorStack.top());
				operatorStack.pop();
			}
			operatorStack.push(s[i]);
		} else if (s[i] == '(') {
			operatorStack.push(s[i]);
		} else if (s[i] == ')') {
			while (operatorStack.top() != '(') {
				outputQueue.push_back(operatorStack.top());
				operatorStack.pop();
			}
			if (operatorStack.top() == '(')
				operatorStack.pop();
		}
	}
	while (not(operatorStack.empty())) {
		outputQueue.push_back(operatorStack.top());
		operatorStack.pop();
	}
	return outputQueue;
}

// STEP 3: build NFA from regular expression
struct trasition {
	int fromState;
	int toState;
	char symbol;
};

struct epsilonTrasition {
	int fromState;
	int toState;
};

struct state {
	int state;
	std::vector<trasition*> trasitions;
	std::vector<epsilonTrasition*> epsilonTrasitions;
	bool isFinal;
};

state* createState(int n, bool isFinal_) {
	state *newState = new state;
	newState->state = n;
	newState->isFinal = isFinal_;
	return newState;
}

trasition* createTransition(int fromState_, int toState_, char symbol_) {
	trasition *newTransition = new trasition;
	newTransition->fromState = fromState_;
	newTransition->toState = toState_;
	newTransition->symbol = symbol_;
	return newTransition;
}

epsilonTrasition* createEpsilonTransition(int fromState_, int toState_) {
	epsilonTrasition *newTransition = new epsilonTrasition;
	newTransition->fromState = fromState_;
	newTransition->toState = toState_;
	return newTransition;
}

void printTransition(trasition *transition_) {
	std::cout << "from: " << transition_->fromState << " to: " << transition_->toState << " symbol: " << transition_->symbol << '\n';
}

void printEpsilonTransition(epsilonTrasition *transition_) {
	std::cout << "from: " << transition_->fromState << " to: " << transition_->toState << " symbol: eps\n" << std::endl;;
}

void printState(state *state_) {
	std::cout << "state " << state_->state << " is final? " << state_->isFinal << std::endl;
	if (state_->isFinal) return;
	std::cout << "trasitions: ";
	for (int i = 0; i < state_->trasitions.size(); i++) {
		printTransition(state_->trasitions[i]);
	}
	for (int i = 0; i < state_->epsilonTrasitions.size(); i++) {
		printEpsilonTransition(state_->epsilonTrasitions[i]);
	}
}

class NFA {
public:
	std::map<int, state*> states;
	state *start, *end;

	NFA() {
		start = NULL;
		end = NULL;
	}

	~NFA() {}

	void addState(state *newState, bool isStart, bool isEnd) {
		states.insert(std::pair<int, state*>(newState->state, newState));
		if (isStart) start = newState;
		if (isEnd) end = newState;
	}

	void statesUnion(std::map<int, state*> &newStates) {
		states.insert(newStates.begin(), newStates.end());
	}

	void printNFA() {
		std::cout << "\nNFA: start " << start->state << " end " << end->state << std::endl;
		std::map<int, state*>::iterator it;
		for (it = states.begin(); it != states.end(); ++it) {
			printState(it->second);
			std::cout << "---------------" << std::endl;
		}
	}
};

NFA postfixToNFA(std::string &postfix) {
	std::stack<NFA> automataStack;
	int len = postfix.length();
	int stateCounter = 0;

	for (int i = 0; i < len; i++) {
		switch (postfix[i]) {
			case '|': {
				NFA nfa1, nfa2;
				nfa2 = automataStack.top();
				automataStack.pop();
				nfa1 = automataStack.top();
				automataStack.pop();
				state *stateStart = createState(stateCounter, false);
				stateCounter++;
				state *stateEnd = createState(stateCounter, true);
				std::cout << "end state: " << stateEnd->state << std::endl;
				stateCounter++;
				nfa1.end->isFinal = false;
				nfa2.end->isFinal = false;
				stateStart->epsilonTrasitions.push_back(createEpsilonTransition(stateStart->state, nfa1.start->state));
				stateStart->epsilonTrasitions.push_back(createEpsilonTransition(stateStart->state, nfa2.start->state));
				nfa1.end->epsilonTrasitions.push_back(createEpsilonTransition(nfa1.end->state, stateEnd->state));
				nfa2.end->epsilonTrasitions.push_back(createEpsilonTransition(nfa2.end->state, stateEnd->state));
				nfa1.statesUnion(nfa2.states);
				nfa1.states.insert(std::pair<int, state*>(stateStart->state, stateStart));
				nfa1.states.insert(std::pair<int, state*>(stateEnd->state, stateEnd));
				nfa1.start = stateStart;
				nfa1.end = stateEnd;
				nfa1.printNFA();
				automataStack.push(nfa1);
				break;
			}
			case '*': {
				NFA nfa;
				nfa = automataStack.top();
				automataStack.pop();
				state *stateStart = createState(stateCounter, false);
				stateCounter++;
				state *stateEnd = createState(stateCounter, true);
				stateCounter++;
				nfa.end->isFinal = false;
				stateStart->epsilonTrasitions.push_back(createEpsilonTransition(stateStart->state, nfa.start->state));
				nfa.end->epsilonTrasitions.push_back(createEpsilonTransition(nfa.end->state, stateEnd->state));
				nfa.end->epsilonTrasitions.push_back(createEpsilonTransition(nfa.end->state, nfa.start->state));
				stateStart->epsilonTrasitions.push_back(createEpsilonTransition(stateStart->state, stateEnd->state));
				nfa.start = stateStart;
				nfa.end = stateEnd;
				nfa.states.insert(std::pair<int, state*>(stateStart->state, stateStart));
				nfa.states.insert(std::pair<int, state*>(stateEnd->state, stateEnd));
				nfa.printNFA();
				automataStack.push(nfa);
				break;
			}
			case '.': {
				NFA nfa1, nfa2;
				nfa2 = automataStack.top();
				automataStack.pop();
				nfa1 = automataStack.top();
				automataStack.pop();
				nfa1.end->epsilonTrasitions.push_back(createEpsilonTransition(nfa1.end->state, nfa2.start->state));
				nfa1.end->isFinal = false;
				nfa1.end = nfa2.end;
				nfa1.statesUnion(nfa2.states);
				nfa1.printNFA();
				automataStack.push(nfa1);
				break;
			}	
			case '+': {
				NFA nfa;
				nfa = automataStack.top();
				automataStack.pop();
				state *stateStart = createState(stateCounter, false);
				stateCounter++;
				state *stateEnd = createState(stateCounter, true);
				stateCounter++;
				nfa.end->isFinal = false;
				stateStart->epsilonTrasitions.push_back(createEpsilonTransition(stateStart->state, nfa.start->state));
				nfa.end->epsilonTrasitions.push_back(createEpsilonTransition(nfa.end->state, stateEnd->state));
				nfa.end->epsilonTrasitions.push_back(createEpsilonTransition(nfa.end->state, nfa.start->state));
				nfa.start = stateStart;
				nfa.end = stateEnd;
				nfa.states.insert(std::pair<int, state*>(stateStart->state, stateStart));
				nfa.states.insert(std::pair<int, state*>(stateEnd->state, stateEnd));
				nfa.printNFA();
				automataStack.push(nfa);
				break;
			}
			default: {	// letters
				state *state1 = createState(stateCounter, false);
				stateCounter++;
				state *state2 = createState(stateCounter, true);
				stateCounter++;
				state1->trasitions.push_back(createTransition(state1->state, state2->state, postfix[i]));
				NFA newNFA;
				newNFA.addState(state1, true, false);
				newNFA.addState(state2, false, true);
				newNFA.printNFA();
				automataStack.push(newNFA);
				break;
			}	
		}
	}

	return automataStack.top();
}

// STEP 4: building DFA from NFA
struct DFAState
{
	int state;
	std::set<int> fromNFAstates;
	std::map<char, int> transitions;
	bool isFinal;
};

DFAState *createDFAState(int state_, std::set<int> fromStates, bool isFinal_) {
	DFAState *newState = new DFAState;
	newState->state = state_;
	newState->fromNFAstates = fromStates;
	newState->isFinal = isFinal_;
	return newState;
}

void printDFAState(DFAState *state) {
	std::cout << "state " << state->state << " is final? " << (state->isFinal == true) << std::endl;
	std::cout << "from NFA states: ";
	for (std::set<int>::iterator itr = state->fromNFAstates.begin(); itr != state->fromNFAstates.end(); ++itr) {
		std::cout << (*itr) << " ";
	}
	std::cout << "\ntrasitions: ";
	if (state->transitions.empty()) {
		std::cout << "\n";
		return;
	}
	for (std::map<char, int>::iterator itr = state->transitions.begin(); itr != state->transitions.end(); ++itr) {
		std::cout << "to: " << (*itr).second << " symbol: " << (*itr).first << '\n';
	}
	std::cout << "\n";
}

class DFA
{
public:
	std::map<int, DFAState*> states;
	DFAState *start, *end;
	DFA() {
		start = NULL;
		end = NULL;
	}
	~DFA() {}
	void addDFAState(DFAState *newState, bool isStart, bool isFinal) {
		states.insert(std::pair<int, DFAState*>(newState->state, newState));
		if (isStart) start = newState;
		if (isFinal) end = newState;
	}
	void printDFA() {
		std::cout << "DFA: ";
		for (std::map<int, DFAState*>::iterator itr = states.begin(); itr != states.end(); ++itr) {
			printDFAState((*itr).second);
		}
	}

};

std::set<int> findTransitionsByLetter(std::vector<trasition*> trasitions, char letter) {
	std::set<int> trasitionsTo;

	for (std::vector<trasition*>::iterator itr = trasitions.begin(); itr < trasitions.end(); ++itr) {
		if ((*itr)->symbol == letter) {
			trasitionsTo.insert((*itr)->toState);
		}
	}
	return trasitionsTo;
}

std::set<int> epsilonClosure(NFA nfa, state *curState) {
	std::set<int> curEpsilonClosure;
	curEpsilonClosure.insert(curState->state);
	int epsilonTrasitionsNumber = curState->epsilonTrasitions.size();
	for (int i = 0; i < epsilonTrasitionsNumber; i++) {
		std::set<int> newSet;
		int from = curState->epsilonTrasitions[i]->toState;
		newSet = epsilonClosure(nfa, nfa.states[from]);
		curEpsilonClosure.insert(newSet.begin(), newSet.end());
	}
	std::set<int>::iterator itr;
	std::cout << "epsilonClosure " << curState->state << ": "; 
	for (itr = curEpsilonClosure.begin(); itr != curEpsilonClosure.end(); ++itr) 
        std::cout << (*itr) << ' ';
    std::cout << std::endl;
	return curEpsilonClosure;
}

bool isFinal(NFA nfa, std::set<int> states) {
	for (std::set<int>::iterator itr = states.begin(); itr != states.end(); ++itr) 
		if (nfa.end->state == (*itr)) 
			return true;
	return false;
}

int stateExcists(DFA &dfa, std::vector<std::pair<int, std::set<int> > > queue, std::set<int> set) {
	// std::cout << "\ncheck the set: ";
	// for (std::set<int>::iterator itr = set.begin(); itr != set.end(); ++itr) {
	// 	std::cout << (*itr) <<  ' ';
	// }
	std::cout << '\n';
	for (std::vector<std::pair<int, std::set<int> > >::iterator queueItr = queue.begin(); 
			queueItr != queue.end(); ++queueItr) {
		if (set == (*queueItr).second) {
			std::cout << "state excists" << std::endl;
			return (*queueItr).first;
		}
	}
	std::map<int, DFAState*> &states = dfa.states;
	for (std::map<int, DFAState*>::iterator itr = states.begin(); itr != states.end(); ++itr) {
		if ((*itr).second->fromNFAstates == set) {
			std::cout << "state excists" << std::endl;
			return (*itr).first;
		}
	}
	std::cout << "state does not excist" << std::endl;
	return -1;
}

DFA NFAtoDFA(NFA nfa, std::set<char> alfabet) {
	std::set<int> startState;
	std::vector<state> DFAStates;
	std::vector<std::pair<int, std::set<int> > > stateQueue; // поменять очередь на вектор для норм проверки
	int stateCounter = 0;
	DFA dfa;

	startState = epsilonClosure(nfa, nfa.start);
	stateQueue.push_back(std::make_pair(stateCounter, startState));
	stateCounter++;

	while (not(stateQueue.empty())) { // в каждом состоянии из множества найти все переходы по каждой букве из алфавита
		std::set<int> currentState = stateQueue.front().second;
		int nState = stateQueue.front().first;
		bool isFinal_ = isFinal(nfa, stateQueue.front().second);
		dfa.addDFAState(createDFAState(stateQueue.front().first, stateQueue.front().second, isFinal_), nState == 0, isFinal_);

		// for each letter in the alfabet
		for (std::set<char>::iterator alfabetItr = alfabet.begin(); alfabetItr != alfabet.end(); ++alfabetItr) {
			std::set<int> newState;
			// for each state of nfa from the set
			for (std::set<int>::iterator stateItr = currentState.begin(); stateItr != currentState.end(); ++stateItr) {
				int nfaState = (*stateItr);
				// get transitions by the letter
				std::set<int> state_ = findTransitionsByLetter(nfa.states[nfaState]->trasitions, (*alfabetItr));
				// create new state where all the transitions go
				newState.insert(state_.begin(), state_.end());
			}

			if (not(newState.empty())) {
				// find epsilon closure of these transitions
				std::set<int> finalSetOfStates;
				for (std::set<int>::iterator itr = newState.begin(); itr != newState.end(); ++itr) {
					std::set<int> fromEpsClosure = epsilonClosure(nfa, nfa.states[(*itr)]);
					finalSetOfStates.insert(fromEpsClosure.begin(), fromEpsClosure.end());
				}
				// add new set of NFA states to queue if they haven't been there
				int n = stateExcists(dfa, stateQueue, finalSetOfStates);
				if (n >= 0) {
					dfa.states[nState]->transitions.insert(std::make_pair((*alfabetItr), n));
				} else {
					stateQueue.push_back(std::make_pair(stateCounter, finalSetOfStates));
					dfa.states[nState]->transitions.insert(std::make_pair((*alfabetItr), stateCounter++));
				}
			}
			
		}
		stateQueue.erase(stateQueue.begin());
	}
	dfa.printDFA();
	return dfa;
}


int main() {
	std::string regex;
	std::string postfix;
	std::set<char> alfabet;
	NFA nfa;
	DFA dfa;

	std::cout << "Enter your regex in infix form: ";
	std::cin >> regex;
	addConcatSymbol(regex);
	std::cout << "Postfix form of your regex: " << regex << std::endl;
	postfix = makePostfixForm(regex, alfabet);
	std::cout << postfix << std::endl;
	std::set<char>::iterator itr;
	std::cout << "alfabet: "; 
	for (itr = alfabet.begin(); itr != alfabet.end(); itr++) 
        std::cout << (*itr) << ' ';
    std::cout << std::endl; 
	nfa = postfixToNFA(postfix);
	//epsilonClosure(nfa, nfa.start);
	NFAtoDFA(nfa, alfabet);
}






