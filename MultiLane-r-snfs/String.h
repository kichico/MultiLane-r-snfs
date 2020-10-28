#ifndef STRING_H
#define STRING_H
#include <string>
#include <sstream>
#include <string.h>
#include <iomanip>

std::string int_to_string(int I, int decimal);
std::string double_to_string(double D, int decimal);

std::string int_to_string(int I, int decimal) {
	std::stringstream ss;
	ss <<std::fixed;
	ss <<std::setprecision(decimal) <<double(I);
	return ss.str();
}

std::string double_to_string(double D, int decimal) {
	std::stringstream ss;
	ss <<std::fixed;
	ss <<std::setprecision(decimal) <<D;
	return ss.str();
}

#endif // STRING_H	
