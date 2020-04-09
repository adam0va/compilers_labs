#include <string>
#include <iostream>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include "stdio.h"

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
/*
	void makeTransitionsTable(std::set<char> &alfabet) {
		int numberOfStates = this.states.size();
		int maxLetterCode = findMax(alfabet); 
		std::vector<std::vector<std::vector<int> > > transitionsTable(numberOfStates+1, 
			std::vector<std::vector<int> >(maxLetterCode+1, std::vector<int>(0)));


		// просмотриваем каждое состояние автомата
		for (std::map<int, state*>::iterator stateItr = this.states.begin(); stateItr != this.states.end(); ++stateItr) {
			//и каждую букву алфавита (из каждого состояния есть переход по каждой букве)
			for (std::set<char>::iterator alfabetItr = alfabet.begin(); alfabetItr != alfabet.end(); ++alfabetItr) {
				// добавляем элемент таблицы по индексу [номер состояния, откуда переход][буква] 
				// куда переход
				//transitionsTable[stateItr->first][(*alfabetItr)].push_back(stateItr->second->transitions[(*alfabetItr)]);
			}
		}
		printf("Transitions table\n");
		printTable(transitionsTable, alfabet, dfa);


	}*/
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
				//nfa1.printNFA();
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
				//nfa.printNFA();
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
				//nfa1.printNFA();
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
				//nfa.printNFA();
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
				//newNFA.printNFA();
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

void printDFAState(DFAState *state, bool isStart) {
	std::cout << "state " << state->state << " is start? " << isStart << " is final? " << (state->isFinal == true) << std::endl;
	std::cout << "from states: ";
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
		printf("DFA: ");
		for (std::map<int, DFAState*>::iterator itr = states.begin(); itr != states.end(); ++itr) {
			printDFAState((*itr).second, this->start->state == itr->first);
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
	std::set<int>::iterator itr; /*
	std::cout << "epsilonClosure " << curState->state << ": "; 
	for (itr = curEpsilonClosure.begin(); itr != curEpsilonClosure.end(); ++itr) 
        std::cout << (*itr) << ' ';
    std::cout << std::endl;*/
	return curEpsilonClosure;
}

bool isFinal(NFA nfa, std::set<int> states) {
	for (std::set<int>::iterator itr = states.begin(); itr != states.end(); ++itr) 
		if (nfa.end->state == (*itr)) 
			return true;
	return false;
}

int stateExcists(DFA &dfa, std::vector<std::pair<int, std::set<int> > > queue, std::set<int> set) {
	for (std::vector<std::pair<int, std::set<int> > >::iterator queueItr = queue.begin(); 
			queueItr != queue.end(); ++queueItr) {
		if (set == (*queueItr).second) {
			return (*queueItr).first;
		}
	}
	std::map<int, DFAState*> &states = dfa.states;
	for (std::map<int, DFAState*>::iterator itr = states.begin(); itr != states.end(); ++itr) {
		if ((*itr).second->fromNFAstates == set) {
			return (*itr).first;
		}
	}
	return -1;
}

DFA NFAtoDFA(NFA nfa, std::set<char> alfabet) {
	std::set<int> startState;
	std::vector<state> DFAStates;
	std::vector<std::pair<int, std::set<int> > > stateQueue; // поменять очередь на вектор для норм проверки
	int stateCounter = 1;
	DFA dfa;

	startState = epsilonClosure(nfa, nfa.start);
	stateQueue.push_back(std::make_pair(stateCounter, startState));
	stateCounter++;

	while (not(stateQueue.empty())) { // в каждом состоянии из множества найти все переходы по каждой букве из алфавита
		std::set<int> currentState = stateQueue.front().second;
		int nState = stateQueue.front().first;
		bool isFinal_ = isFinal(nfa, stateQueue.front().second);
		dfa.addDFAState(createDFAState(stateQueue.front().first, stateQueue.front().second, isFinal_), nState == 1, isFinal_);

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
	//dfa.printDFA();
	return dfa;
}

// STEP 5: minimize DFA
void addDeadState(DFA &dfa, std::set<char> &alfabet) { // add dead state where non-excisting edges go
	std::set<int> emptySet;
	DFAState *deadState = createDFAState(0, emptySet, false);
	for (std::set<char>::iterator itr = alfabet.begin(); itr != alfabet.end(); ++itr) {
		deadState->transitions.insert(std::make_pair((*itr), 0));
		for (std::map<int, DFAState*>::iterator stateItr = dfa.states.begin(); stateItr != dfa.states.end(); ++stateItr) {
			if (stateItr->second->transitions.count((*itr)) < 1) {
				stateItr->second->transitions.insert(std::make_pair((*itr), 0));
			}
		}
	}
	dfa.addDFAState(deadState, false, false);	
}

int printIntVector(std::vector<int> &vector) {
	int len = 0;
	for (std::vector<int>::iterator itr = vector.begin(); itr != vector.end(); ++itr) {
		printf("%d ", (*itr));
		len += std::to_string(*itr).length() + 1;
	}
	return len;
}

void printPartition(std::vector<std::vector<int> > &partition) {
	printf("partition: \n");
	int length = partition.size();
	for (int i = 0; i < length; i++) {
		printf("state set %d: ", i);
		printIntVector(partition[i]);
		printf("\n");
	}
	printf("======\n");
}

void initialPartition(DFA &dfa, std::vector<std::vector<int> > &partition, std::vector<int> &classAttachment) {
	std::vector<int> final, notFinal;
	for (std::map<int, DFAState*>::iterator itr = dfa.states.begin(); itr != dfa.states.end(); ++itr) {
		if (itr->second->isFinal) {
			final.push_back(itr->second->state);
			classAttachment[itr->first] = 1;
		} else {
			notFinal.push_back(itr->second->state);
			classAttachment[itr->first] = 0;
		}
	}
	partition.push_back(notFinal);
	partition.push_back(final);
}

void printQueue(std::vector<std::pair<int, char> > queue) {
	int length = queue.size();
	printf("queue: \n");
	for (int i = 0; i < length; i++) {
		printf("%d %c\n", queue[i].first, queue[i].second);
	}
	printf("======\n");
}

int findMax(std::set<char> &alfabet) {
    int max = 0; 
    if (!alfabet.empty()) 
        max = *(alfabet.rbegin()); 
    return max; 
} 

void printTable(std::vector<std::vector<std::vector<int> > > &transitionsTable, std::set<char> &alfabet, DFA &dfa) {
	
	int width = dfa.states.size() * 1.3;
	int maxStateLen = std::to_string(dfa.states.size()).length();
	printf("%*c", maxStateLen + 2, '|');
	for (std::set<char>::iterator itr = alfabet.begin(); itr != alfabet.end(); ++itr) {
		printf("%-*c|", width, *itr);
	}
	
	printf("\n");
	for (std::map<int, DFAState*>::iterator stateItr = dfa.states.begin(); stateItr != dfa.states.end(); ++stateItr) {
		printf("%-*d|", maxStateLen + 1, stateItr->first);
		for (std::set<char>::iterator itr = alfabet.begin(); itr != alfabet.end(); ++itr) {
			int len = printIntVector(transitionsTable[stateItr->first][(*itr)]);
			printf("%*c", width - len + 1, '|');
		}
		printf("\n");
	}
}

void makeTransitionsTable(DFA &dfa, std::set<char> &alfabet) {
	int numberOfStates = dfa.states.size();
	int maxLetterCode = findMax(alfabet); 
	std::vector<std::vector<std::vector<int> > > transitionsTable(numberOfStates+1, 
		std::vector<std::vector<int> >(maxLetterCode+1, std::vector<int>(0)));
	// просмотриваем каждое состояние автомата
	for (std::map<int, DFAState*>::iterator stateItr = dfa.states.begin(); stateItr != dfa.states.end(); ++stateItr) {
		//и каждую букву алфавита (из каждого состояния есть переход по каждой букве)
		for (std::set<char>::iterator alfabetItr = alfabet.begin(); alfabetItr != alfabet.end(); ++alfabetItr) {
			// добавляем элемент таблицы по индексу [номер состояния, откуда переход][буква] 
			// куда переход
			transitionsTable[stateItr->first][(*alfabetItr)].push_back(stateItr->second->transitions[(*alfabetItr)]);
		}
	}
	printf("Transitions table\n");
	printTable(transitionsTable, alfabet, dfa);
}

void makeBackTransitionsTable(DFA &dfa, std::vector<std::vector<std::vector<int> > > &transitionsTable, std::set<char> &alfabet) {
	// просмотриваем каждое состояние автомата
	for (std::map<int, DFAState*>::iterator stateItr = dfa.states.begin(); stateItr != dfa.states.end(); ++stateItr) {
		//и каждую букву алфавита (из каждого состояния есть переход по каждой букве)
		for (std::set<char>::iterator alfabetItr = alfabet.begin(); alfabetItr != alfabet.end(); ++alfabetItr) {
			// добавляем элемент таблицы по индексу [куда переход по букве][буква] 
			// номер состояния, откуда переход
			transitionsTable[stateItr->second->transitions[(*alfabetItr)]][(*alfabetItr)].push_back(stateItr->first);
		}
	}

}

bool hasAllTransitions(DFA &dfa, int alfabetPower) {
	int statesPower = dfa.states.size();
	int transitionsPower = 0;

	for (std::map<int, DFAState*>::iterator itr = dfa.states.begin(); itr != dfa.states.end(); ++itr) 
		transitionsPower += itr->second->transitions.size();
	return statesPower * alfabetPower == transitionsPower;
}

DFA buildMinDFA(DFA &dfa, std::set<char> &alfabet, int numberOfStates, std::vector<std::vector<int> > &partition, 
	std::vector<int> &classAttachment) {
	DFA minDFA;
	int partitionPower = partition.size();

	for (int i = 0; i < partitionPower; i++) {
		std::set<int> fromStates;
		bool isFinal = false, isStart = false;
		std::map<char, int> transitions;
		DFAState *newState;
		for (int m = 0; m < partition[i].size(); m++) {
			fromStates.insert(partition[i][m]);
			isFinal = isFinal || dfa.states[partition[i][m]]->isFinal;
			isStart = isStart || (dfa.start->state == dfa.states[partition[i][m]]->state);
			for (std::set<char>::iterator itr = alfabet.begin(); itr != alfabet.end(); ++itr) {
				transitions[(*itr)] = classAttachment[dfa.states[partition[i][m]]->transitions[(*itr)]];
			}

		}
		newState = createDFAState(i, fromStates, isFinal);
		newState->transitions = transitions;
		minDFA.addDFAState(newState, isStart, isFinal);
	}
	return minDFA;
}

void makePartition(DFA &dfa, std::set<char> &alfabet, int numberOfStates, 
	std::vector<std::vector<int> > &partition, std::vector<int> &classAttachment) {
	int maxLetterCode = findMax(alfabet);

	printf("\n");
	std::vector<std::pair<int, char> > queue; // очередь
	std::vector<std::vector<std::vector<int> > > transitionsTable(numberOfStates+1, 
		std::vector<std::vector<int> >(maxLetterCode+1, std::vector<int>(0)));
	std::map<int, std::vector<int> > classConsistency; // какому номеру класса какие состояния соответствуют

	// начальное разбиение: допускающие и недопускающие состояния, заполнение вектора classAttachment
	initialPartition(dfa, partition, classAttachment);
	//printPartition(partition);

	// заполняем очередь парами: класс, буква алфавита
	for (int i = 0; i < partition.size(); i++) {
		for (std::set<char>::iterator alfabetItr = alfabet.begin(); alfabetItr != alfabet.end(); 
			++alfabetItr) {
			queue.push_back(std::make_pair(i, (*alfabetItr)));
		}
	}
	//printQueue(queue);

	// заполняем обратную таблицу переходов
	makeBackTransitionsTable(dfa, transitionsTable, alfabet);
	/*printf("Back trasitions table\n");
	printTable(transitionsTable, alfabet, dfa);*/

	while (not(queue.empty())) {
		std::pair<int, char> splitter = queue.front();
		std::vector<int> splitterClass = partition[splitter.first];
		char splitterLetter = splitter.second;
		std::map<int, DFAState*> dfaStates = dfa.states;

		classConsistency.clear();
		queue.erase(queue.begin());
		// для каждого состояния из класса в сплиттере
		for (int i = 0; i < splitterClass.size(); i++) {
			// для каждого состояния автомата с ребром в сплиттер
			std::vector<int> statesToSplitter = transitionsTable[splitterClass[i]][splitterLetter];
			int statesToSplitterPower = statesToSplitter.size();
			
			for (int r = 0; r < statesToSplitterPower; r++) {
				// из какого класса состояние с ребром в сплиттер?
				int fromClass = classAttachment[transitionsTable[splitterClass[i]][splitterLetter][r]];
				if (classConsistency.find(fromClass) == classConsistency.end()) {
					std::vector<int> v;
					classConsistency[fromClass] = v;
				}
				classConsistency[fromClass].push_back(transitionsTable[splitterClass[i]][splitterLetter][r]);
			}
		}

		// теперь обновить разбиение с учетом того, разделились ли состояния
		for (std::map<int, std::vector<int> >::iterator itr = classConsistency.begin(); 
			itr != classConsistency.end(); ++itr) {
			int fromClass = itr->first;
			// если не все состояния из класса переходят в сплиттер, то это состояния надо разделить на два
			if (classConsistency[fromClass].size() < partition[fromClass].size()) {
				// добавляем пустое состояние в разбиение
				std::vector<int> v;
				partition.push_back(v);
				int newClassNumber = partition.size() - 1;
				// каждое состояния в выделяемом классе
				for (int i = 0; i < classConsistency[fromClass].size(); i++) {
					// удаляем из старого класса
					partition[fromClass].erase(std::find(partition[fromClass].begin(), 
						partition[fromClass].end(), classConsistency[fromClass][i]));
					// добавляем в новый класс
					partition[newClassNumber].push_back(classConsistency[fromClass][i]);
				}
				if (partition[newClassNumber] > partition[fromClass])
					std::swap(partition[fromClass], partition[newClassNumber]);
				// меняем номера класса в массиве
				for (int i = 0; i < partition[newClassNumber].size(); i++)
					classAttachment[partition[newClassNumber][i]] = newClassNumber;
				// добавляем новые классы в очередь
				for (std::set<char>::iterator alfabetItr = alfabet.begin(); alfabetItr != alfabet.end(); 
					++alfabetItr)
					queue.push_back(std::make_pair(newClassNumber, (*alfabetItr)));
			}
		}
	}
	
	// если образовалось пустое состояние, удаляяем его
	for (std::vector<std::vector<int> >::iterator itr = partition.begin(); itr != partition.end(); ++itr) {
		if (itr->empty()) {
			partition.erase(itr);
		}
	}

	//printPartition(partition);
}

DFA minimizeDFA(DFA &dfa, std::set<char> &alfabet) {
	std::vector<std::vector<int> > partition; // разбиение

	// добавляем состояние, в которое ведут ребра из всех вершин по всем символам
	// если количество переходов != количество состояний * мощность алфавита
	if (not(hasAllTransitions(dfa, alfabet.size()))) {
		addDeadState(dfa, alfabet);
		dfa.printDFA();
	}
	int numberOfStates = dfa.states.size();
	std::vector<int> classAttachment(numberOfStates); // classAttachment[i] - какому классу разбиения принадлежит состояние i
	makePartition(dfa, alfabet, numberOfStates, partition, classAttachment);

	DFA minimizedDFA = buildMinDFA(dfa, alfabet, numberOfStates, partition, classAttachment);
	printf("min DFA ");
	//minimizedDFA.printDFA();
	makeTransitionsTable(minimizedDFA, alfabet);
	return minimizedDFA;
}

// STEP 6: string recognition

bool stringMatchesDFA(std::string word, DFA dfa) {
	int wordLength = word.size();
	DFAState *currentState = dfa.start;
	std::cout << "-" << currentState->state << "-> ";
	for (int i = 0; i < wordLength; i++) {
		char c = word[i];
		currentState = dfa.states[currentState->transitions[c]];
		std::cout << word.substr(i, wordLength) << " -" << currentState->state << "-> ";
	}
	std::cout << std::endl;
	return currentState->isFinal;
}

int main() {
	std::string regex, procRegex;
	std::string postfix, word;
	std::set<char> alfabet;
	NFA nfa;
	DFA dfa;

	std::cout << "Enter your regex in infix form: ";
	std::cin >> regex;
	procRegex = regex;
	addConcatSymbol(procRegex);
	std::cout << "Postfix form of your regex: " << procRegex << std::endl;
	postfix = makePostfixForm(procRegex, alfabet);
	std::cout << postfix << std::endl;
	std::set<char>::iterator itr;
	std::cout << "alfabet: "; 
	for (itr = alfabet.begin(); itr != alfabet.end(); itr++) 
		std::cout << (*itr) << ' ';
	std::cout << std::endl; 
	nfa = postfixToNFA(postfix);
	nfa.printNFA();
	dfa =  NFAtoDFA(nfa, alfabet);
	makeTransitionsTable(dfa, alfabet);
	DFA minDFA = minimizeDFA(dfa, alfabet);
	std::cout << "\nYour regex is: " << regex; 
	std::cout << "\nEnter the word to check if it matches your regular expression: ";
	std::cin >> word;
	std::cout << (stringMatchesDFA(word, minDFA) ? "Given word matches your regular expression\n" : 
		"Given word doesn't match your regular expression\n");
}






