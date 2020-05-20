from parser import Parser

if __name__ == "__main__":
	test = '( 1 + 2 ) * 4 >= 5 / 7'
	parser = Parser(test)
	parser.analyze()