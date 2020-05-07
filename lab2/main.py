from grammar import Grammar

'''
test.json - нахождение эпсилон правил
test2.json - находение эпсилон правил
test_delete_recursion.json - удаление произвольной левой рекурсии
'''


if __name__ == "__main__":
	grammar = Grammar.read_grammar_from_file('test_delete_recursion2.json')
	print(f'Your grammar:')
	grammar.print_grammar()
	print(f'Deleting epsilon rules...')
	grammar.delete_epsilon_rules()
	print(f'\nGrammar without epsilon rules:')
	grammar.print_grammar()
	print(f'Deleting left recursion...')
	grammar.delete_left_recursion()
	print(f'\nGrammar without left recursion:')
	grammar.print_grammar()