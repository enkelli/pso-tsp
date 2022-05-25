///
/// FIT BUT, SFC, 1. project
/// Pavol Plaskon, xplask00
///

#include <cctype>

#include "utils.hpp"


///
/// Converts string to number.
///
/// @returns -1 when input is not positive number.
///
int str_to_pint(const std::string &str)
{
	if (str.empty()) {
		return -1;
	}
    for (const auto &c: str)
    {
        if (!isdigit(c))
        {
            return -1;
        }
    }
    return std::stoi(str);
}
