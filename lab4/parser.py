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
	def __init__(self):
		pass

	def read_test(self, string_of_symbols: str):
		self.symbols = string_of_symbols.split()
		self.symbols.append('$')
		self.number_of_current_symbol = 0
		self.length_of_list = len(self.symbols)
		self.current_symbol = self.symbols[self.number_of_current_symbol]
		self.stack = ['$']
		self.rpn = ''

	def read_program_from_file(filename: str):
		with open(filename) as file:
			data = file.read()
			return Parser(string_of_symbols = data)

	def next(self):
		self.number_of_current_symbol += 1
		self.current_symbol = self.symbol[self.number_of_current_symbol]
		
	def print_symbols(self):
		print(f'Symbols: {self.symbols}')

	def symbol_to_rpn(self, c: str):
		if c not in ('(', ')'):
			self.rpn = self.rpn + c + ' '

	def __get_precedence(self, a: str, b: str):
		constant = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
		operation_of_multiplication = ['*', '/']
		operation_of_addition = ['+', '-']
		operation_of_relation = ['<', '<=', '==', '<>', '>', '>=']
	
		if a == ')': 
			if b == '(' or b in constant:
				raise ParserSyntaxError('Operator is missed')
			if 	b in operation_of_multiplication or b in operation_of_addition or b ==')' or b == '$' \
				or b in operation_of_relation:
				return '>'

		if a in constant: 
			if b == '(' or b in constant:
				raise ParserSyntaxError('Operator is missed')
			if 	b in operation_of_multiplication or b in operation_of_addition or b ==')' or b == '$' \
				or b in operation_of_relation:
				return '>'

		if a in operation_of_multiplication:
			if b == '(' or b in constant:
				return '<'
			if 	b in operation_of_multiplication or b in operation_of_addition or b ==')' or b == '$' \
			or b in operation_of_relation:
				return '>'

		if a in operation_of_addition:
			if b == '(' or b in constant or b in operation_of_multiplication:
				return '<'
			if 	b in operation_of_addition or b ==')' or b == '$' or b in operation_of_relation:
				return '>'

		if a == '(':
			if b == '(' or b in constant or b in operation_of_multiplication or b in operation_of_addition \
			or b in operation_of_relation:
				return '<'
			if b == ')':
				return '='
			if b == '$':
				raise ParserSyntaxError('Right bracket is missed')

		if a == '$':
			if b == '(' or b in constant or b in operation_of_multiplication or b in operation_of_addition \
			or b in operation_of_relation:
				return '<'
			if b == ')':
				raise ParserSyntaxError('Right bracket is not balanced')
			if b == '$':
				raise ParserSyntaxError('Operand is missed')

		if a in operation_of_relation:
			if b == '(' or b in constant or b in operation_of_multiplication or b in operation_of_addition:
				return '<'
			if b == ')' or b == '$':
				return '>'
			if b in operation_of_relation:
				raise ParserSyntaxError('Two comparisons in one expression are forbidden')


	def analyze(self):
		if self.stack[-1] == '$' and self.symbols[0] == '$':
			raise ParserSyntaxError('Operand is missed')
		while 100:
			if self.stack[-1] == '$' and self.symbols[0] == '$':
				break
			precedence = self.__get_precedence(self.stack[-1], self.symbols[0])
			if precedence == '<' or precedence == '=':
				self.stack.append(self.symbols[0])
				self.symbols = self.symbols[1:]
			elif precedence == '>':
				while True:
					pop_stack = self.stack[-1]
					self.stack = self.stack[:-1]
					self.symbol_to_rpn(pop_stack)
					if self.__get_precedence(self.stack[-1], pop_stack) == '<':
						break
			print(f'stack: {self.stack}\nsymbols: {self.symbols}')

		print(f'rpn: {self.rpn}')






















