#include <string>
#include <iostream>

void addConcatSymbol(std::string &s) {
	// *, (, ), |

	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '(' || s[i] == '|')
			continue;

		if (i < s.length() - 1) {
			if (s[i + 1] == '*' || s[i + 1] == '|' || s[i + 1] == ')') 
				continue; 
			else {
				s.insert(i + 1, ".");
				i++;
			}

		}
	}
}

int getPrecedence(char c) {
	if (c == '|')
		return 0;
	else if (c == '.')
		return 1;
	else if (c == '*')
		return 2;
	else return -1;
}

void makePostfixForm(std::string &s) {

}

int main() {
	std::string regex;

	std::cin >> regex;
	addConcatSymbol(regex);
	std::cout << regex << std::endl;
}