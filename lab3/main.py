from parser import Parser

if __name__ == "__main__":
	filename = 'program.txt'
	parser = Parser.read_program_from_file(filename)
	parser.print_lexems()
	parser.analyse()
	