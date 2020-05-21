from parser import Parser

if __name__ == "__main__":
	parser = Parser()

	test1 = '( 1 + 2 ) * 4 > 7 * ( 2 - 5 / 2 )' # 1 2 + 4 * 7 2 5 2 / - * > 
	test2 = ') 1 * 7' # Right bracket is not balanced
	test3 = '( 3 * 4 ) / 5 * (' # Right bracket is missed
	test4 = '( 3 * 4 ) / 5 4' # Operator is missed
	test5 = '3 + 4 + 7 * ( 9 - 3 / ( 3 + 8 ) )' # 3 4 + 7 9 3 3 8 + / - * + 

	test = test5
	parser.read_test(test)
	parser.analyze()
	