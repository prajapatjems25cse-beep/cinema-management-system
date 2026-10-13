#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// Small helper functions shared by every class in the project:
// string handling, safe console input and formatting.
namespace Utils {

std::vector<std::string> split(const std::string& s, char delim);
std::string trim(const std::string& s);
std::string sanitize(const std::string& s);   // removes the '|' file delimiter

std::string readLine(const std::string& prompt, bool allowEmpty = false);
int readInt(const std::string& prompt, int lo, int hi);
double readDouble(const std::string& prompt, double lo, double hi);
bool confirm(const std::string& prompt);

std::string money(double value);              // e.g. "Rs. 250.00"
std::string seatLabel(int row, int col);      // (0,0) -> "A1"
bool parseSeat(const std::string& token, int& row, int& col); // "A1" -> (0,0)

void printLine(char ch = '-', int width = 60);
void printTitle(const std::string& title);

} // namespace Utils

#endif // UTILS_H
