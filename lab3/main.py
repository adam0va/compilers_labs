from parser import Parser

'''
Tests:
correct_program.txt
test_operator_expected.txt
test_bracket_expected.txt
test_eof_expected.txt
test_assignment_expected.txt
'''

if __name__ == "__main__":
	filename = 'test_assignment_expected.txt'
	parser = Parser.read_program_from_file(filename)
	parser.print_lexems()
	parser.analyse()
	