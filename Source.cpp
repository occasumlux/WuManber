#include "WuManber.hpp"
#include <iostream>
#include <fstream>

int main() {

    //File to open for searching, replace with any text file
    //As long as it does not have special characters
    std::ifstream file("alert");
    
    //Line buffer, we call the algorithm once per line
    std::string line;
    
    //Vector that stores the patterns to search
	std::vector<std::string> v;
	v.push_back("nmap XMAS");
	v.push_back("PING NMAP");
	v.push_back("Large ICMP");
	v.push_back("ancert");
	v.push_back("dnber");
    
    //Map holding the times that every pattern appeared
    std::unordered_map<std::string, std::size_t> counters;
    
    //Init of counters with value 0
    for (auto& pattern : v)
        counters[pattern] = 0;
    
    //List with the detail of every match found
    std::list<Occurrence> r;
    
    //Wu Manber -wrapper- class 
	WuManber wu;
    
    //Construction of the tables required by the algorithm
	wu.initialize(v);
    
    //Line counter, my Wu Manber algorithm does not have
    //sense of lines, only characters position
    std::size_t lineCounter = 1;
    
    if (file.is_open()) {
        //If the file was opened, start reading it
        while (std::getline(file, line)) {
            //For each line, call search and store the result
            auto r2 = wu.search(line, lineCounter);
            r.insert(r.end(), r2.begin(), r2.end());
            ++lineCounter;
        }
        file.close();
        
        //Print occurences data
        for (auto& res : r) {
            std::cout << res.index << " " << res.pattern << " " << res.line << std::endl;
            counters[res.pattern]++;
        }
        
        //Print counters data
        for (auto& counter : counters)
            std::cout << counter.first << " => " << counter.second << " times" << std::endl;
    }
	//system("PAUSE");
	return 0;
}
