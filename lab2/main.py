from grammar import Grammar

'''
test_delete_recursion1.json - нахождение эпсилон правил
test_delete_recursion2.json - находение эпсилон правил
test_delete_recursion3.json - удаление произвольной левой рекурсии
test_delete_recursion4.json
test_useless_symbols1.json - нахождение бесполезных терминалов (здесь D непорождающий, С недостижимый)
test_useless_symbols2.json - нахождение бесполезных символовов (остается только одно правило)
test_useless_symbols3.json - всё остается, как было
'''


if __name__ == "__main__":
	testfile1 = 'test_delete_recursion1.json'
	grammar1 = Grammar.read_grammar_from_file(testfile1)
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
	grammar1.write_grammar_to_file(testfile1)

	testfile2 = 'test_useless_symbols1.json'
	grammar2 = Grammar.read_grammar_from_file(testfile2)
	print(f'\n\n\n----------------------\nYour grammar:')
	grammar2.print_grammar()
	print(f'Deleting useless symbols...')
	grammar2.delete_useless_symbols()
	print(f'Grammar without useless symbols:')
	grammar2.print_grammar()
	grammar2.write_grammar_to_file(testfile2)