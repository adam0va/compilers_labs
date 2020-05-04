from grammar import Grammar

if __name__ == "__main__":
	grammar = Grammar.read_grammar_from_file('test.json')
	grammar.print_grammar()

	grammar.delete_epsilon_rules()