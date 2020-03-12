#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <unordered_set>
#include <set>
#include <queue>
#include <map>

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
		//states.push_back(newState);
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
	for (itr = curEpsilonClosure.begin(); itr != curEpsilonClosure.end(); itr++) 
        std::cout << (*itr) << ' ';
    std::cout << std::endl;
	return curEpsilonClosure;
}


int main() {
	std::string regex;
	std::string postfix;
	std::set<char> alfabet;
	NFA nfa;

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
	epsilonClosure(nfa, nfa.start);
}






