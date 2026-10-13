#include "Utils.h"

#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace Utils {

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> parts;
    std::string item;
    std::istringstream stream(s);
    while (std::getline(stream, item, delim)) {
        parts.push_back(item);
    }
    // getline drops a trailing empty field, so restore it
    if (!s.empty() && s.back() == delim) {
        parts.push_back("");
    }
    return parts;
}

std::string trim(const std::string& s) {
    std::size_t start = 0;
    std::size_t end = s.size();
    while (start < end && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    return s.substr(start, end - start);
}

std::string sanitize(const std::string& s) {
    std::string out = s;
    for (char& c : out) {
        if (c == '|') c = '/';
    }
    return out;
}

std::string readLine(const std::string& prompt, bool allowEmpty) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cout << "\nInput closed. Exiting.\n";
            std::exit(0);
        }
        line = trim(line);
        if (!line.empty() || allowEmpty) {
            return sanitize(line);
        }
        std::cout << "  Input cannot be empty. Try again.\n";
    }
}

int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            std::size_t used = 0;
            int value = std::stoi(line, &used);
            if (used == line.size() && value >= lo && value <= hi) {
                return value;
            }
        } catch (...) {
        }
        std::cout << "  Please enter a whole number between " << lo << " and " << hi << ".\n";
    }
}

double readDouble(const std::string& prompt, double lo, double hi) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            std::size_t used = 0;
            double value = std::stod(line, &used);
            if (used == line.size() && value >= lo && value <= hi) {
                return value;
            }
        } catch (...) {
        }
        std::cout << "  Please enter a number between " << lo << " and " << hi << ".\n";
    }
}

bool confirm(const std::string& prompt) {
    while (true) {
        std::string line = readLine(prompt + " (y/n): ");
        char c = static_cast<char>(std::tolower(static_cast<unsigned char>(line[0])));
        if (c == 'y') return true;
        if (c == 'n') return false;
        std::cout << "  Please answer y or n.\n";
    }
}

std::string money(double value) {
    std::ostringstream out;
    out << "Rs. " << std::fixed << std::setprecision(2) << value;
    return out.str();
}

std::string seatLabel(int row, int col) {
    return std::string(1, static_cast<char>('A' + row)) + std::to_string(col + 1);
}

bool parseSeat(const std::string& token, int& row, int& col) {
    if (token.size() < 2) return false;
    if (!std::isalpha(static_cast<unsigned char>(token[0]))) return false;
    row = std::toupper(static_cast<unsigned char>(token[0])) - 'A';
    std::string number = token.substr(1);
    for (char c : number) {
        if (!std::isdigit(static_cast<unsigned char>(c))) return false;
    }
    if (number.size() > 3) return false;
    col = std::stoi(number) - 1;
    return col >= 0;
}

void printLine(char ch, int width) {
    std::cout << std::string(width, ch) << "\n";
}

void printTitle(const std::string& title) {
    std::cout << "\n";
    printLine('=');
    std::cout << "  " << title << "\n";
    printLine('=');
}

} // namespace Utils
