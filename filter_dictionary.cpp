#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>

int main()
{
	std::ifstream dictionary;
	std::ofstream stripped_dictionary;

	dictionary.open ( "dictionary.txt");
	stripped_dictionary.open ( "stripped_dictionary.txt");

	assert( dictionary.is_open() == true); //Missing dictionary.txt
	assert( stripped_dictionary.is_open() == true); //Missing stripped_dictionary.txt
	std::string word;
	while ( dictionary >> word)
	{
		std::cout << word.size() << std::endl;
		if ( word.size() == 10)
		{
			stripped_dictionary << word << std::endl;
			std::cout << word << std::endl;
		}
	}
	return 0;

