class ParserSyntaxError(Exception):
	def __init__(self, *args):
		if args:
			self.message = args[0]
		else:
			self.message = None

	def __str__(self):
		if self.message:
			return f'ParserSyntaxError: {self.message}'
		else:
			return f'ParserSyntaxError was raised'

class Parser:
	def __init__(self, string_of_lexems: str):
		self.lexems = string_of_lexems.split()
		self.lexems.append('_')
		self.number_of_current_lexem = 0
		self.length_of_list = len(self.lexems)
		self.current_lexem = self.lexems[self.number_of_current_lexem]

	def read_program_from_file(filename: str):
		with open(filename) as file:
			data = file.read()
			return Parser(string_of_lexems = data)

	def next(self):
		self.number_of_current_lexem += 1

		self.current_lexem = self.lexems[self.number_of_current_lexem]
		print(f'current lexem is: {self.current_lexem}')

	def print_lexems(self):
		print(self.lexems)

	def __is_identificator(self, lexem):
		return lexem == 'x' or lexem == 'y' or lexem == 'z'

	def __is_operation_of_relation(self, lexem):
		return lexem == '==' or lexem == '>' or lexem == '>=' or lexem == '<>' or lexem == '<' or lexem == '<='

	def __is_operation_of_addition(self, lexem):
		return lexem == '+' or lexem == '-'

	def __is_operation_of_multiplication(self, lexem):
		return lexem == '*' or lexem == '/'

	def __is_constant(self, lexem):
		print(f'in range? {lexem} {lexem in range(10)}')
		return lexem in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')

	def analyse(self):
		print(f'start')
		self.program()
		if self.current_lexem != '_':
			raise ParserSyntaxError('EOF expected')
		print(f'good')

	def program(self):
		print(f'program')
		self.block()

	def block(self):
		print(f'block')
		if self.lexems[self.number_of_current_lexem] == '{':
			self.next()
			self.list_of_operators()
			if self.lexems[self.number_of_current_lexem] == '}':
				self.next()
			else:
				raise ParserSyntaxError('} expected in the end of the block')
		else:
			raise ParserSyntaxError('{ expected')

	def list_of_operators(self):
		print(f'list_of_operators')
		if self.__is_identificator(self.current_lexem) or self.current_lexem == '{':
			self.operator()
			if self.current_lexem == ';':
				self.tail()
		else:
			raise ParserSyntaxError('Operator expected')

	def operator(self):
		print(f'operator')
		if self.current_lexem == '{':
			self.next()
			self.list_of_operators()
			if self.current_lexem == '}':
				self.next()
			else:
				raise ParserSyntaxError('} expected at the end of block')
		elif self.__is_identificator(self.current_lexem):
			self.identificator()
			if  self.current_lexem == '=':
				self.next()
				self.expression()
		else:
			raise ParserSyntaxError('Operator expected')

	def tail(self):
		print(f'tail')
		if self.current_lexem == ';':
			self.next()
			self.operator()
			if self.current_lexem == ';':
				self.tail()
		else:
			raise ParserSyntaxError('Tail expected')

	def expression(self):
		print(f'expression')
		self.arithmetic_expression()
		if self.__is_operation_of_relation(self.current_lexem):
			self.operation_of_relation()
			self.arithmetic_expression()

	def arithmetic_expression(self):
		print(f'arithmetic_expression')
		self.term()
		if self.__is_operation_of_addition(self.current_lexem):
			self.arithmetic_expression_()

	def arithmetic_expression_(self):
		print(f'arithmetic_expression_')
		self.operation_of_addition()
		self.term()
		if self.__is_operation_of_addition(self.current_lexem):
			self.arithmetic_expression_()

	def term(self):
		print(f'term')
		self.factor()
		if self.__is_operation_of_multiplication(self.current_lexem):
			self.term_()

	def term_(self):
		print(f'term_')
		self.operation_of_multiplication()
		self.factor()
		if self.__is_operation_of_multiplication(self.current_lexem):
			self.term_()

	def factor(self):
		print(f'factor')
		if self.__is_identificator(self.current_lexem):
			self.identificator()
		elif self.__is_constant(self.current_lexem):
			self.constant()
		elif self.current_lexem == '(':
			self.next()
			self.arithmetic_expression()
			if self.current_lexem == ')':
				self.next()
			else:
				raise ParserSyntaxError(') expected')
		else:
			raise ParserSyntaxError('Identificator, constant or arithmetic_expression in brackets expected')


	def operation_of_relation(self):
		print(f'operation_of_relation')
		if self.__is_operation_of_realtion(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Operation of relation expected')

	def operation_of_addition(self):
		print(f'operation_of_addition')
		if self.__is_operation_of_addition(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Operation of addition expected')

	def operation_of_multiplication(self):
		print(f'operation_of_multiplication')
		if self.__is_operation_of_multiplication(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Operation of multiplication expected')

	def constant(self):
		print(f'constant')
		if self.__is_constant(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Constant expected')

	def identificator(self):
		print(f'identificator')
		if self.__is_identificator(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Identificator expected')





















