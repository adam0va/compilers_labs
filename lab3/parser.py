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
		self.number_of_current_lexem = 0
		self.current_lexem = self.lexems[self.number_of_current_lexem]

	def read_program_from_file(filename: str):
		with open(filename) as file:
			data = file.read()
			return Parser(string_of_lexems = data)

	def next(self):
		self.number_of_current_lexem += 1
		self.current_lexem = self.lexems[self.number_of_current_lexem]

	def print_lexems(self):
		print(self.lexems)

	def __is_identificator(self, lexem):
		return lexem == 'x' or lexem == 'y' or lexem == 'z'

	def __is_operation_of_realtion(self, lexem):
		return lexem == '==' or lexem == '>' or lexem == '>=' or lexem == '<>' or lexem == '<' or lexem == '<='

	def __is_operation_of_addition(self, lexem):
		return lexem == '+' or lexem == '-'

	def analyse(self):
		self.program()
		print(f'start')
		if self.current_lexem:
			raise ParserSyntaxError('EOF expected')
		print(f'good')

	def program(self):
		self.block()

	def block(self):
		print(f'block')
		if self.lexems[self.number_of_current_lexem] == '{':
			self.next()
			self.list_of_operators()
			if self.lexems[self.number_of_current_lexem] == '}':
				return
			else:
				raise ParserSyntaxError('} expected')
		else:
			raise ParserSyntaxError('{ expected')

	def list_of_operators(self):
		if self.__is_identificator(self.current_lexem) or self.current_lexem = '{':
			self.operator()
			self.next()
			if self.current_lexem == ';':
				self.tail()
				self.next()
		else:
			raise ParserSyntaxError('Operator expected')

	def operator(self):
		if self.current_lexem == '{':
			self.next()
			self.list_of_operators()
			if self.current_lexem == '}':
				pass
			else:
				raise ParserSyntaxError('} expected at the end of block')
		elif self.__is_identificator(self.current_lexem):
			self.identificator()
			self.next()
			if  self.current_lexem == '=':
				self.next()
				self.expression()

	def tail(self):
		if self.current_lexem == ';':
			self.next()
			self.operator()
			self.next()
			if self.current_lexem == ';':
				self.tail()

	def identificator(self):
		if self.__is_identificator(self.current_lexem):
			pass

	def expression(self):
		self.arithmetic_expression()
		if __is_operation_of_realtion(self.current_lexem):
			self.operation_of_relation()
			self.arithmetic_expression()

	def arithmetic_expression(self):
		self.term()

		self.arithmetic_expression_()

	def arithmetic_expression_(self):
		self.operation_of_addition()
		self.term()
		self.arithmetic_expression_()



















