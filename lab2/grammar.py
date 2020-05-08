import json
import re

class Grammar:
	def __init__(self, startNonTerminal_, terminals_: str, nonTerminals_: str, rules_):
		self.startNonTerminal = startNonTerminal_
		self.terminals = terminals_
		self.nonTerminals = nonTerminals_
		self.rules = rules_

	def print_grammar(self):
		print(f'nonterminals: {self.nonTerminals}\nterminals: {self.terminals}\nstart nonterminal: {self.startNonTerminal}')
		print(f'rules: {json.dumps(self.rules,indent=4)}\n')

	def read_grammar_from_file(filename: str):
		with open(filename) as json_file:
			jsonGrammar = json.load(json_file)
			return Grammar(startNonTerminal_ = jsonGrammar['startNonTerminal'], terminals_ = jsonGrammar['terminals'],
				nonTerminals_ = jsonGrammar['nonTerminals'], rules_ = jsonGrammar['rules'])

	def find_eps_nonterms(self):
		eps_nonterms = []
		for key, value in self.rules.items():
			for alt in value:
				if "e" in alt:
					eps_nonterms.append(key)

		while 1:
			new_nonterms = []
			# составляем регулярное выражение из найденных эпсилон-нетерминалов
			eps_nonterms_regex = '[' + ''.join(x for x in eps_nonterms) + ']+'
			regex = re.compile(eps_nonterms_regex)
			#print(f'epsilon-nonterms regex: {eps_nonterms_regex}')
			
			# в каждом правиле проверям
			for key, value in self.rules.items():
				if not(key in eps_nonterms):
					# каждую альтернативу: если она целиком состоит из найденных эпсилон-нетерминалов
					# то добавляем в множество новый нетерминал
					for alt in value:
						alt_string = ''.join(x for x in alt)
						#print(f'string to check: {alt_string}')
						match_res = regex.fullmatch(alt_string)
						if match_res:
							eps_nonterms += key
							new_nonterms += key

			if new_nonterms == []:
				break
						
		print(f'epsilon-nonterms: {eps_nonterms}')
		return eps_nonterms

	def delete_epsilon_rules(self):
		if 'e' not in self.terminals:
			return
		eps_nonterms = self.find_eps_nonterms()			
		rules_ = self.rules
		# 1) 	удаляем эпсилон-правила
		# 2)	для каждой альтернативы с эпсилон-нетерминалом добавить
		#    	альтернативу без него
		for key, value in self.rules.items():
			if ["e"] in value:
				value.remove(["e"])
			for alternative in value:
				current_alt = []
				current_rest = alternative
				for symbol in alternative:
					current_alt += symbol
					current_rest = current_rest[1:]
					if symbol in eps_nonterms:
						new_alternative = current_alt[:-1] + current_rest
						if new_alternative not in rules_[key] and new_alternative != []:
							rules_[key] += [new_alternative]

		# если из начального нетерминала выводится эпсилон,
		# добавляем новое начальное правило
		#print(f'eps_nonterms: {eps_nonterms}')
		if self.startNonTerminal in eps_nonterms:
			rules_["S_"] = [self.startNonTerminal] + ["e"]
			self.startNonTerminal = "S_"
		self.rules = rules_

	def __handle_direct_left_recursive_rule(self, nonterm_from: str, nonterm_to: str):
		left_rec_alt =[]
		other_alt = []
		for alt in self.rules[nonterm_from]:
			if alt[0] == nonterm_to:
				left_rec_alt += [alt]
			else:
				other_alt += [alt]
		if left_rec_alt == []:
			return
		new_rule_for_nonterm = []
		new_nonterm = nonterm_from + "_"
		self.nonTerminals.append(new_nonterm)
		for alt in other_alt:
			if alt == ["e"]:
				alt = [new_nonterm]
			else:
				alt.append(new_nonterm)
		self.rules[nonterm_from] = other_alt
		rule_for_new_nonterm = []
		for alt in left_rec_alt:
			alt = alt[1:]
			alt.append(new_nonterm)
			rule_for_new_nonterm.append(alt)
		rule_for_new_nonterm.append(["e"])
		if "e" not in self.terminals:
			self.terminals.append("e")
		self.rules[new_nonterm] = rule_for_new_nonterm

		

	def __handle_common_left_recursive_rule(self, nonterm_from: str, nonterm_to: str):
		new_rule = []
		numbers_of_found_alts = []
		found_alts = []

		for alt_number in range(len(self.rules[nonterm_from])):
			if self.rules[nonterm_from][alt_number][0] == nonterm_to:
				found_alts.append(self.rules[nonterm_from][alt_number])
				numbers_of_found_alts.append(alt_number)
		if found_alts == []:
			return
		# удаляем найденные альтернативы
		for number in numbers_of_found_alts[::-1]:
			del self.rules[nonterm_from][number]
		insert_from = self.rules[nonterm_to]
		# заменяем их на новые
		for alt in found_alts:
			for to_insert in insert_from:
				if to_insert == ["e"]:
					new_alt = alt[1:]
				else:
					new_alt = to_insert + alt[1:]
				self.rules[nonterm_from].append(new_alt)


	def delete_left_recursion(self):
		for i in range(0, len(self.nonTerminals)):
			for j in range(0, i+1):
				if self.nonTerminals[i] == self.nonTerminals[j]:
					self.__handle_direct_left_recursive_rule(self.nonTerminals[i], self.nonTerminals[j])
				else:
					self.__handle_common_left_recursive_rule(self.nonTerminals[i], self.nonTerminals[j])

	def __find_productive_symbols(self):
		productive_symbols = []
		terminals_regex = '[' + ''.join(x for x in self.terminals) + ']+'
		print(terminals_regex)
		regex = re.compile(terminals_regex)

		for key, value in self.rules.items():
			for alternative in value:
				alt_string = ''.join(x for x in alternative)
				isProductive = regex.fullmatch(alt_string)
				if isProductive:
					productive_symbols.append(key)

		while 1: 
			new_symbols = []
			for key, value in self.rules.items():
				if key not in productive_symbols:
					for alternative in value:
						isProductive = True
						for symbol in alternative:
							if symbol not in productive_symbols and symbol not in self.terminals:
								isProductive = False
						if isProductive:
							if key not in productive_symbols:
								productive_symbols.append(key)
								new_symbols.append(key)
			if new_symbols == []:
				break

		print(productive_symbols)

	def delete_useless_symbols(self):
		self.__find_productive_symbols()

















