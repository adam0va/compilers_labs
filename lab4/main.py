from parser import Parser

if __name__ == "__main__":
	test = '( 1 + 2 ) * 4 > 7 * ( 2 - 5 / 2 )'
	parser = Parser(test)
	parser.analyze()