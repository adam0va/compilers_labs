from grammar import Grammar

'''
test.json - нахождение эпсилон правил
test2.json - находение эпсилон правил
test_delete_recursion.json - удаление произвольной левой рекурсии
test_productive_symbols.json - нахождение порождающих терминалов (здесь D непорождающий)
test_delete_useless.json
'''


if __name__ == "__main__":
	grammar1 = Grammar.read_grammar_from_file('test_delete_recursion2.json')
	print(f'Your grammar:')
	grammar1.print_grammar()
	print(f'Deleting epsilon rules...')
	grammar1.delete_epsilon_rules()
	print(f'\nGrammar without epsilon rules:')
	grammar1.print_grammar()
	print(f'Deleting left recursion...')
	grammar1.delete_left_recursion()
	print(f'\nGrammar without left recursion:')
	grammar1.print_grammar()

	grammar2 = Grammar.read_grammar_from_file('test_productive_symbols.json')
	print(f'\n\n\n----------------------\nYour grammar:')
	grammar2.print_grammar()
	grammar2.delete_useless_symbols()