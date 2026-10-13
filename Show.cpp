#include "Show.h"

#include <iomanip>
#include <iostream>

#include "Utils.h"

Show::Show() : id_(0), movieId_(0), price_(0.0), rows_(0), cols_(0) {}

Show::Show(int id, int movieId, const std::string& hall, const std::string& time,
           double price, int rows, int cols)
    : id_(id), movieId_(movieId), hall_(hall), time_(time), price_(price),
      rows_(rows), cols_(cols),
      booked_(rows, std::vector<bool>(cols, false)) {}

bool Show::isValidSeat(int row, int col) const {
    return row >= 0 && row < rows_ && col >= 0 && col < cols_;
}

bool Show::isAvailable(int row, int col) const {
    return isValidSeat(row, col) && !booked_[row][col];
}

bool Show::bookSeat(int row, int col) {
    if (!isAvailable(row, col)) return false;
    booked_[row][col] = true;
    return true;
}

bool Show::releaseSeat(int row, int col) {
    if (!isValidSeat(row, col) || !booked_[row][col]) return false;
    booked_[row][col] = false;
    return true;
}

int Show::availableSeats() const {
    int count = 0;
    for (int r = 0; r < rows_; ++r)
        for (int c = 0; c < cols_; ++c)
            if (!booked_[r][c]) ++count;
    return count;
}

void Show::displaySeatMap() const {
    std::cout << "\n" << std::string(static_cast<std::size_t>(cols_) * 4 / 2, ' ')
              << "[ S C R E E N ]\n\n    ";
    for (int c = 0; c < cols_; ++c) {
        std::cout << std::setw(3) << (c + 1) << " ";
    }
    std::cout << "\n";
    for (int r = 0; r < rows_; ++r) {
        std::cout << "  " << static_cast<char>('A' + r) << " ";
        for (int c = 0; c < cols_; ++c) {
            std::cout << (booked_[r][c] ? "[X] " : "[ ] ");
        }
        std::cout << "\n";
    }
    std::cout << "\n  [ ] available   [X] booked   ("
              << availableSeats() << " of " << totalSeats() << " free)\n";
}

void Show::displayHeader() {
    std::cout << std::left << std::setw(5) << "ID" << std::setw(26) << "Movie"
              << std::setw(10) << "Hall" << std::setw(18) << "Time"
              << std::setw(12) << "Price" << std::setw(8) << "Free" << "\n";
    Utils::printLine('-', 79);
}

void Show::displaySummary(const std::string& movieTitle) const {
    std::cout << std::left << std::setw(5) << id_ << std::setw(26) << movieTitle.substr(0, 24)
              << std::setw(10) << hall_.substr(0, 9) << std::setw(18) << time_.substr(0, 17)
              << std::setw(12) << Utils::money(price_)
              << std::setw(8) << availableSeats() << "\n";
}

std::string Show::serialize() const {
    std::string seats;
    for (int r = 0; r < rows_; ++r)
        for (int c = 0; c < cols_; ++c)
            seats += booked_[r][c] ? '1' : '0';
    return std::to_string(id_) + "|" + std::to_string(movieId_) + "|" + hall_ + "|" +
           time_ + "|" + std::to_string(price_) + "|" + std::to_string(rows_) + "|" +
           std::to_string(cols_) + "|" + seats;
}

bool Show::deserialize(const std::string& line, Show& out) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 8) return false;
    try {
        int rows = std::stoi(f[5]);
        int cols = std::stoi(f[6]);
        if (rows <= 0 || cols <= 0 ||
            f[7].size() != static_cast<std::size_t>(rows) * static_cast<std::size_t>(cols)) {
            return false;
        }
        out = Show(std::stoi(f[0]), std::stoi(f[1]), f[2], f[3], std::stod(f[4]), rows, cols);
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                out.booked_[r][c] = (f[7][static_cast<std::size_t>(r) * cols + c] == '1');
    } catch (...) {
        return false;
    }
    return true;
}
