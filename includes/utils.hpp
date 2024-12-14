#ifndef UTILS_HPP
#define UTILS_HPP

# include "includes.hpp"

# define SET 0
# define GET 1

std::vector<std::string>	split(std::string s, std::string delimiter);
std::string					generateFilename(std::string baseName);
const std::string			&getMessageCode(const int &code);
std::string					trim(const std::string& str);
void						trimn(std::string& str);
void						formatSlash(std::string &str);

#endif