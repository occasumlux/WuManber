#pragma once
#include <string>
#include <unordered_map> //Hashing tables
#include <algorithm> //min() function
#include <list>
#include <vector>
#include <cstddef> //size_t

struct Occurrence {
	std::size_t index;
    std::size_t line;
	std::string pattern;
};

class WuManber {
public:
	WuManber();
	WuManber(const std::vector<std::string>& patterns);
	virtual ~WuManber();
	void initialize(const std::vector<std::string>& patterns);
	std::list<Occurrence> search(const std::string& text, const std::size_t line) const;

private:
	bool initialized;
	std::size_t B;
	std::size_t m;
	std::vector<std::string> patterns;
	std::unordered_map<std::string, std::size_t> shift_table;
	std::unordered_map<std::string, std::size_t> aux_shift_table;
	std::unordered_map<std::string, std::list<std::size_t> > hash_table;

};

