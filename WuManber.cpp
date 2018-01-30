#include "WuManber.hpp"
#include <iostream>

WuManber::WuManber():
	initialized(false),
	B(2),//Block size
	m(0),//Patterns minimum length
	patterns(),//Vector of patterns
	shift_table(),//SHIFT table, according to paper
	aux_shift_table(),//Auxiliary shift table (ASHIFT)
	hash_table()//HASH table
{}

WuManber::WuManber(const std::vector<std::string>& patterns_in) {
	WuManber();
	initialize(patterns_in);
}

WuManber::~WuManber(){}

void WuManber::initialize(std::vector<std::string> const & patterns_in) {

	patterns.assign(patterns_in.begin(), patterns_in.end());

	//Init of m
	for (auto iter = patterns.begin(); iter != patterns.end(); ++iter) {
		m = (m == 0) ? iter->length() : std::min(iter->length(), m);
	}

	std::cout << "m: " << m << std::endl;

	//Init of tables for each block in each pattern
	std::size_t pattern_index = 0;
	for (auto pattern_iter = patterns.begin();
		pattern_iter != patterns.end();
		++pattern_iter, ++pattern_index) {
		for (std::size_t char_index = 0; char_index < m - B + 1; ++char_index) {
			std::string block = pattern_iter->substr(char_index, B);
			std::size_t block_pos = m - char_index - B;
			//std::cout << "Char index: " << char_index << std::endl;
			std::size_t last_shift = (block_pos == 0) ? m - B + 1 : block_pos;

			//Init of SHIFT table
			if (shift_table.count(block)) {
				last_shift = shift_table[block];
				shift_table[block] = std::min(shift_table[block], block_pos);
			}
			else
				shift_table[block] = block_pos;

			//Init of HASH table
			if (block_pos == 0) {
				hash_table[block].push_back(pattern_index);
				if (last_shift != 0) {
					if (aux_shift_table.count(block))
						aux_shift_table[block] = std::min(aux_shift_table[block], last_shift);
					else
						aux_shift_table[block] = last_shift;
				}
			}
		}
	}

	std::cout << "\nShift table:\n";
	for (auto& shift : shift_table)
		std::cout << shift.first << "->" << shift.second << std::endl;

	std::cout << "\nAux Shift table:\n";
	for (auto& shift : aux_shift_table)
		std::cout << shift.first << "->" << shift.second << std::endl;
	std::cout << std::endl;

	initialized = true;
}

std::list<Occurrence> WuManber::search(const std::string& text, const std::size_t line) const {
	
	std::list<Occurrence> occurrences;
	if (initialized) {
		//std::cout << "Searching!\n" << std::endl;

		for (size_t char_index = m - B; char_index < text.length(); ++char_index) {
			std::string block = text.substr(char_index, B);
			//std::cout << "Block: " << block << std::endl;
			//std::cout << "Char index: " << char_index << std::endl;
			auto shift_value = shift_table.find(block);
			if (shift_value != shift_table.end()) {
				//std::cout << shift_value->second << std::endl;
				if (shift_value->second == 0) {//Possible match
					//std::cout << "Step 2!" << std::endl;
					auto possibles = hash_table.at(block);
					/*for (auto& p : possibles)
						std::cout << p << std::endl;*/
					for (auto pattern_iter = possibles.begin();
						pattern_iter != possibles.end();
						++pattern_iter) {
						std::string pattern = patterns[*pattern_iter];
						//std::cout << "Pattern: " << pattern << std::endl;
						std::size_t char_pos = char_index - m + B;
                        //std::cout << "Char pos: " << char_pos << std::endl;
						std::size_t i;
						//std::cout << "Step 3!\n";
						for (i = 0; i < pattern.length(); ++i) {
							//std::cout << "Pat: " << pattern.at(i) << " -- Tex: " << text.at(char_pos + i) << std::endl;
							if (pattern.at(i) != text.at(char_pos + i)) {
								break;
							}
						}
						if (i == pattern.length()) {
							Occurrence occurrence = {};
							occurrence.index = char_pos;
							occurrence.pattern = pattern;
                            occurrence.line = line;
							occurrences.push_back(occurrence);
							//std::cout << "Found " << pattern << std::endl;
						}
					}
					char_index += aux_shift_table.at(block) - 1;//Shift according to ASHIFT[block]
				}
				else
					char_index += shift_value->second - 1;//Shift according to SHIFT[block]
			}
			else {
				char_index += m - B;//Shift the maximum possible to the right
				//std::cout << "Max shift: " << block << std::endl;
			}
		}
	}
	return occurrences;
}
