#include "Booking.h"

#include <iostream>

#include "Utils.h"

Booking::Booking() : id_(0), showId_(0), total_(0.0), cancelled_(false) {}

Booking::Booking(int id, int showId, const std::string& customerName,
                 const std::string& phone, const std::vector<Seat>& seats, double total)
    : id_(id), showId_(showId), customerName_(customerName), phone_(phone),
      seats_(seats), total_(total), cancelled_(false) {}

std::string Booking::seatList() const {
    std::string out;
    for (std::size_t i = 0; i < seats_.size(); ++i) {
        if (i > 0) out += ", ";
        out += Utils::seatLabel(seats_[i].first, seats_[i].second);
    }
    return out;
}

void Booking::display(const std::string& movieTitle, const std::string& showTime) const {
    std::cout << "  Booking #" << id_ << (cancelled_ ? "  [CANCELLED]" : "") << "\n"
              << "    Customer : " << customerName_ << " (" << phone_ << ")\n"
              << "    Movie    : " << movieTitle << "\n"
              << "    Show     : #" << showId_ << " at " << showTime << "\n"
              << "    Seats    : " << seatList() << "\n"
              << "    Total    : " << Utils::money(total_) << "\n";
}

std::string Booking::serialize() const {
    std::string seats;
    for (std::size_t i = 0; i < seats_.size(); ++i) {
        if (i > 0) seats += ",";
        seats += std::to_string(seats_[i].first) + ":" + std::to_string(seats_[i].second);
    }
    return std::to_string(id_) + "|" + std::to_string(showId_) + "|" + customerName_ + "|" +
           phone_ + "|" + std::to_string(total_) + "|" + (cancelled_ ? "1" : "0") + "|" + seats;
}

bool Booking::deserialize(const std::string& line, Booking& out) {
    std::vector<std::string> f = Utils::split(line, '|');
    if (f.size() != 7) return false;
    try {
        std::vector<Seat> seats;
        for (const std::string& token : Utils::split(f[6], ',')) {
            if (token.empty()) continue;
            std::vector<std::string> rc = Utils::split(token, ':');
            if (rc.size() != 2) return false;
            seats.push_back(Seat(std::stoi(rc[0]), std::stoi(rc[1])));
        }
        out = Booking(std::stoi(f[0]), std::stoi(f[1]), f[2], f[3], seats, std::stod(f[4]));
        if (f[5] == "1") out.cancel();
    } catch (...) {
        return false;
    }
    return true;
}
