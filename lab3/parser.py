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
		self.print_name_of_functions = False
		self.print_lexem = False

	def read_program_from_file(filename: str):
		with open(filename) as file:
			data = file.read()
			return Parser(string_of_lexems = data)

	def next(self):
		self.number_of_current_lexem += 1

		self.current_lexem = self.lexems[self.number_of_current_lexem]
		if self.print_lexem:
			print(f'Currently examining lexem  "{self.current_lexem}"')

	def print_lexems(self):
		print(f'Lexems: {self.lexems}')

	def __is_identificator(self, lexem):
		return lexem == 'x' or lexem == 'y' or lexem == 'z'

	def __is_operation_of_relation(self, lexem):
		return lexem == '==' or lexem == '>' or lexem == '>=' or lexem == '<>' or lexem == '<' or lexem == '<='

	def __is_operation_of_addition(self, lexem):
		return lexem == '+' or lexem == '-'

	def __is_operation_of_multiplication(self, lexem):
		return lexem == '*' or lexem == '/'

	def __is_constant(self, lexem):
		#print(f'in range? {lexem} {lexem in range(10)}')
		return lexem in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')

	def analyse(self):
		self.program()
		if self.current_lexem != '_':
			raise ParserSyntaxError('EOF expected')
		print(f'Your program is syntactically correct')

	def program(self):
		if self.print_name_of_functions:
			print(f'In program')
		self.block()

	def block(self):
		if self.print_name_of_functions:
			print(f'In block')
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
		if self.print_name_of_functions:
			print(f'In list_of_operators')
		if self.__is_identificator(self.current_lexem) or self.current_lexem == '{':
			self.operator()
			if self.current_lexem == ';':
				self.tail()
		else:
			raise ParserSyntaxError('Operator expected')

	def operator(self):
		if self.print_name_of_functions:
			print(f'In operator')
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
		if self.print_name_of_functions:
			print(f'In tail')
		if self.current_lexem == ';':
			self.next()
			self.operator()
			if self.current_lexem == ';':
				self.tail()
		else:
			raise ParserSyntaxError('Tail expected')

	def expression(self):
		if self.print_name_of_functions:
			print(f'In expression')
		self.arithmetic_expression()
		if self.__is_operation_of_relation(self.current_lexem):
			self.operation_of_relation()
			self.arithmetic_expression()

	def arithmetic_expression(self):
		if self.print_name_of_functions:
			print(f'In arithmetic_expression')
		self.term()
		if self.__is_operation_of_addition(self.current_lexem):
			self.arithmetic_expression_()

	def arithmetic_expression_(self):
		if self.print_name_of_functions:
			print(f'In arithmetic_expression_')
		self.operation_of_addition()
		self.term()
		if self.__is_operation_of_addition(self.current_lexem):
			self.arithmetic_expression_()

	def term(self):
		if self.print_name_of_functions:
			print(f'In term')
		self.factor()
		if self.__is_operation_of_multiplication(self.current_lexem):
			self.term_()

	def term_(self):
		if self.print_name_of_functions:
			print(f'In term_')
		self.operation_of_multiplication()
		self.factor()
		if self.__is_operation_of_multiplication(self.current_lexem):
			self.term_()

	def factor(self):
		if self.print_name_of_functions:
			print(f'In factor')
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
		if self.print_name_of_functions:
			print(f'In operation_of_relation')
		if self.__is_operation_of_relation(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Operation of relation expected')

	def operation_of_addition(self):
		if self.print_name_of_functions:
			print(f'In operation_of_addition')
		if self.__is_operation_of_addition(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Operation of addition expected')

	def operation_of_multiplication(self):
		if self.print_name_of_functions:
			print(f'In operation_of_multiplication')
		if self.__is_operation_of_multiplication(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Operation of multiplication expected')

	def constant(self):
		if self.print_name_of_functions:
			print(f'In constant')
		if self.__is_constant(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Constant expected')

	def identificator(self):
		if self.print_name_of_functions:
			print(f'In identificator')
		if self.__is_identificator(self.current_lexem):
			self.next()
		else:
			raise ParserSyntaxError('Identificator expected')





















