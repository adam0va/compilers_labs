import json
import re

class Grammar:
	def __init__(self, startNonTerminal_, terminals_, nonTerminals_, rules_):
		self.startNonTerminal = startNonTerminal_
		self.terminals = terminals_
		self.nonTerminals = nonTerminals_
		self.rules = rules_

	def print_grammar(self):
		print(f'nonterminals: {self.nonTerminals}\nterminals: {self.terminals}\nstart nonterminal: {self.startNonTerminal}\n')
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

					#[eps_terms]+
		while 1:
			new_nonterms = []
			# составляем регулярное выражение из найденных эпсилон-нетерминалов
			eps_nonterms_regex = '[' + ''.join(x for x in eps_nonterms) + ']+'
			regex = re.compile(eps_nonterms_regex)
			print(f'epsilon-nonterms regex: {eps_nonterms_regex}')
			# в каждом правиле проверям
			for key, value in self.rules.items():
				if not(key in eps_nonterms):
					# каждую альтернативу: если она целиком состоит из найденных эпсилон-нетерминалов
					# то добавляем в множество новый нетерминал
					for alt in value:
						alt_string = ''.join(x for x in alt)
						print(f'string to check: {alt_string}')
						match_res = regex.fullmatch(alt_string)
						if match_res:
							eps_nonterms += key
							new_nonterms += key

			if new_nonterms == []:
				break
						
		print(f'epsilon-nonterms: {eps_nonterms}')
		return eps_nonterms

	def delete_epsilon_rules(self):
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
		if self.startNonTerminal in eps_nonterms:
			rules_["S_"] = [self.startNonTerminal] + ["e"]
			self.startNonTerminal = "S_"
		self.print_grammar()





















