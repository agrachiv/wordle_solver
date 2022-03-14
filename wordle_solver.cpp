#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include <regex>

enum character_state { GRAY = 0, GREEN = 1, YELLOW = 2, OVERFLOW = 3};

int ctoi( std::string letter)
{
	if ( letter == "а") return 0;
	if ( letter == "б") return 1;
	if ( letter == "в") return 2;
	if ( letter == "г") return 3;
	if ( letter == "д") return 4;
	if ( letter == "е") return 5;
	if ( letter == "ё") return 6;
	if ( letter == "ж") return 7;
	if ( letter == "з") return 8;
	if ( letter == "и") return 9;
	if ( letter == "й") return 10;
	if ( letter == "к") return 11;
	if ( letter == "л") return 12;
	if ( letter == "м") return 13;
	if ( letter == "н") return 14;
	if ( letter == "о") return 15;
	if ( letter == "п") return 16;
	if ( letter == "р") return 17;
	if ( letter == "с") return 18;
	if ( letter == "т") return 19;
	if ( letter == "у") return 20;
	if ( letter == "ф") return 21;
	if ( letter == "х") return 22;
	if ( letter == "ц") return 23;
	if ( letter == "ч") return 24;
	if ( letter == "ш") return 25;
	if ( letter == "щ") return 26;
	if ( letter == "ъ") return 27;
	if ( letter == "ы") return 28;
	if ( letter == "ь") return 29;
	if ( letter == "э") return 30;
	if ( letter == "ю") return 31;
	if ( letter == "я") return 32;
	
	assert( false && "Wrong letter");

}

std::string itoc( int i)
{
	switch ( i)
	{
		case 0  : return "а";
		case 1  : return "б";
		case 2  : return "в";
                case 3  : return "г";
		case 4  : return "д";
		case 5  : return "е";
		case 6  : return "ё";
		case 7  : return "ж";
		case 8  : return "з";
		case 9  : return "и";
		case 10 : return "й";
		case 11 : return "к";
		case 12 : return "л";
		case 13 : return "м";
		case 14 : return "н";
		case 15 : return "о";
		case 16 : return "п";
		case 17 : return "р";
		case 18 : return "с";
		case 19 : return "т";
		case 20 : return "у";
 		case 21 : return "ф";
		case 22 : return "х";
		case 23 : return "ц";
		case 24 : return "ч";
		case 25 : return "ш";
		case 26 : return "щ";
		case 27 : return "ъ";
		case 28 : return "ы";
		case 29 : return "ь";
		case 30 : return "э";
		case 31 : return "ю";
		case 32 : return "я";

		default: assert( false && "Wrong integer");
	}
}

struct Mask
{
        std::vector<std::string>& dictionary;
	std::vector<std::vector<int>>& frequency;
	std::vector<int>& global_frequency;
        
	std::vector<int> mask;
        std::string query;

        Mask( std::vector<std::vector<int>>& frequency, std::vector<int>& global_frequency, std::vector<std::string>& dictionary, std::string query);
        Mask operator++( int);

        double calculate_probability();
        int calculate_stipping_size();
};

struct Solver
{
	std::vector<std::string> dictionary;
	std::vector<std::vector<int>> frequency;
	std::vector<int> global_frequency;

	Solver();
	bool matches_mask( std::string word);
	void find_best_candidate();
	void calculate_letters_frequency();
	void filter_dictionary( std::string regular_expresiion);
	double calculate_expected_strip( std::string candidate);
};

Solver::Solver() : frequency( 5, std::vector<int>( 33, 0)), global_frequency( 33, 0)
{
	std::ifstream dictionary_stream;
	dictionary_stream.open ( "stripped_dictionary.txt");
	assert( dictionary_stream.is_open() == true && "Missing stipped_dictionary.txt");

	std::string word;
	while ( dictionary_stream >> word)
		dictionary.push_back( word);

	dictionary_stream.close();

	std::cout << "Current dictionary contains " << dictionary.size() << " five letter words" << std::endl;

	calculate_letters_frequency();
	find_best_candidate();
}

void Solver::find_best_candidate()
{
	double highscore = 0;
	std::string best_candidate;

        for ( auto const& candidate: dictionary)
	{
		double score = calculate_expected_strip( candidate);

		//int green_score = 0;
		//int yellow_score = 0;
		/*
		for (int pos = 0; pos < 5; pos++)
		
			int letter_code = ctoi( candidate.substr( pos*2, 2));
			green_score += frequency[pos][letter_code];
			//yellow_score += global_frequency[letter_code];
			global_frequency[letter_code] = 0;
		}

		score = green_score;
		*/

		if (score >= highscore)
		{
			std::cout << "New best candidate: " << candidate << " Score: " << score << std::endl;
			best_candidate = candidate;
			highscore = score;
		}
	}
}

double Solver::calculate_expected_strip( std::string candidate)
{
	double expected_strip = 0;
	double probability = 0;
	Mask mask( frequency, global_frequency, dictionary, candidate);
	for ( int i = 0; i < 243; i++)
	{
		double mask_probability = mask.calculate_probability();
		probability += mask_probability;
		expected_strip += mask_probability * mask.calculate_stipping_size();
		mask++;
	}
	//std::cout << "Expected strip of: " << candidate << " = " << expected_strip << std::endl;
	//std::cout << "Probability = " << probability << std::endl;
	return expected_strip;
}

bool Solver::matches_mask( std::string word)
{
	/*
	for ( int i = 0; i < 5; i++)
		if ( mask[i] != word[i] && mask[i] != '*')
			return false;
	*/
	return true;	
}

void Solver::calculate_letters_frequency()
{
	for ( auto const& word: dictionary)
	{
		for( int pos = 0; pos < 5; pos++)
		{
			std::string letter = word.substr( 2*pos, 2);
			for( int i = 0; i < 33; i++)
			{
				if( letter == itoc( i))
					frequency[pos][i]++;
			}	
		}
	}
	
	for( int i = 0; i < 33; i++)
		for ( auto const& word: dictionary)
			if ( word.find( itoc( i)) != std::string::npos)
				global_frequency[i]++;

	int g_f = 0;
	int l_f = 0;
	for ( int i = 0; i < 33; i++)
	{
		g_f += global_frequency[i];
		for( int j = 0; j < 5; j++)
			l_f += frequency[j][i];
	}
	std::cout << "Dictionary size: " << dictionary.size() << " gf_summ: " << g_f << "lf_summ: " << l_f << std::endl;
}

void Solver::filter_dictionary( std::string regular_expresiion)
{
	
}

int Mask::calculate_stipping_size()
{
        int stripping_size = 0;
        for ( auto const& candidate: dictionary)
        {
                for ( int i = 0; i < 5; i++)
                {
			std::string candidate_letter = candidate.substr( 2 * i, 2);
			std::string query_letter = query.substr( 2 * i, 2);

                        if( mask[i] == GREEN && query_letter != candidate_letter)
                        {
                                stripping_size++;
                                break;
                        }
                        else if ( mask[i] == YELLOW)
                        {
                                if ( candidate.find( query_letter) == std::string::npos)
                                {
                                        stripping_size++;
                                        break;
                                }
                        }
                }
        }
        return stripping_size;
}

double Mask::calculate_probability()
{
	double probability = 1;
	for ( int i = 0; i < 5; i++)
	{
		int letter_code = ctoi( query.substr( 2 * i, 2));
		std::vector<bool> calculated_yellows( 33, false);

		if (mask[i] == GRAY)
			probability *= 1 - ( double)( global_frequency[letter_code]) / ( double)( dictionary.size());
		if (mask[i] == YELLOW)
		{
			if ( calculated_yellows[i] == false)
			{
				probability *= ( double)( global_frequency[letter_code]) / ( double)( dictionary.size());
				calculated_yellows[i] == true;
			}
		}
		if (mask[i] == GREEN)
			probability *= ( double)( frequency[i][letter_code]) / ( double)dictionary.size();
		else
			assert( true && "Broken mask");

	}
	return probability;
}

Mask::Mask( std::vector<std::vector<int>>& frequency, std::vector<int>& global_frequency, std::vector<std::string>& dictionary, std::string query) 
	: mask( 5, GRAY), dictionary( dictionary), query( query), frequency( frequency), global_frequency( global_frequency) {}

Mask Mask::operator++( int)
{
        for ( int i = 4; i >= 0; i--)
        {
                mask[i]++;
                if( mask[i] != OVERFLOW)
                {
                        break;
                }
                else
                {
                        mask[i] = 0;
                }
        }
	return *this;
}

int main()
{
	Solver solver;
	/*
		for ( int i = 0; i < 33; i++)
	       		std::cout << itoc(i) << " " << solver.global_frequency[i] << std::endl;
	*/
	return 0;
}	
