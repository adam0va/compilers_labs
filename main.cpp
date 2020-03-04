#include <string>
#include <iostream>
#include <stack>
#include <string>

void addConcatSymbol(std::string &s) {
	// *, (, ), |

	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '(' || s[i] == '|')
			continue;

		if (i < s.length() - 1) {
			if (s[i + 1] == '*' || s[i + 1] == '*' || s[i + 1] == '|' || s[i + 1] == ')') 
				continue; 
			else {
				s.insert(i + 1, ".");
				i++;
			}

		}
	}
}

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

std::string makePostfixForm(std::string &s) {
	std::stack<char> operatorStack;
	std::string outputQueue;
	int len = s.length();
	
	for (int i = 0; i < len; i++) {
		if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) // добавить проверку, что нет других символов
			outputQueue.push_back(s[i]);
		else if (isOperator(s[i])) {
			while (	not(operatorStack.empty()) && 
					isOperator(operatorStack.top()) && 
					(getPrecedence(operatorStack.top() >= getPrecedence(s[i])))) {
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

int main() {
	std::string regex;
	std::string postfix;

	std::cout << "Enter your regex: ";
	std::cin >> regex;
	addConcatSymbol(regex);
	std::cout << regex << std::endl;
	postfix = makePostfixForm(regex);
	std::cout << postfix << std::endl;
}






