#include "Movie.h"

#include <iomanip>
#include <iostream>
#include <vector>

#include "Utils.h"

Movie::Movie() : id_(0), durationMin_(0) {}

Movie::Movie(int id, const std::string& title, const std::string& genre,
             int durationMin, const std::string& language, const std::string& rating)
    : id_(id), title_(title), genre_(genre), durationMin_(durationMin),
      language_(language), rating_(rating) {}

void Movie::displayHeader() {
    std::cout << std::left << std::setw(5) << "ID" << std::setw(26) << "Title"
              << std::setw(12) << "Genre" << std::setw(9) << "Minutes"
              << std::setw(10) << "Language" << std::setw(7) << "Rating" << "\n";
    Utils::printLine('-', 69);
}

void Movie::display() const {
    std::cout << std::left << std::setw(5) << id_ << std::setw(26) << title_.substr(0, 24)
              << std::setw(12) << genre_.substr(0, 10) << std::setw(9) << durationMin_
              << std::setw(10) << language_.substr(0, 9) << std::setw(7) << rating_ << "\n";
}

std::string Movie::serialize() const {
    return std::to_string(id_) + "|" + title_ + "|" + genre_ + "|" +
           std::to_string(durationMin_) + "|" + language_ + "|" + rating_;
}

bool Movie::deserialize(const std::string& line, Movie& out) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 6) return false;
    try {
        out = Movie(std::stoi(f[0]), f[1], f[2], std::stoi(f[3]), f[4], f[5]);
    } catch (...) {
        return false;
    }
    return true;
}
