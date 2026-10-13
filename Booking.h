#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <utility>
#include <vector>

// A ticket booking made by a customer for one or more seats of a show.
class Booking {
public:
    typedef std::pair<int, int> Seat; // (row, column), both zero-based

    Booking();
    Booking(int id, int showId, const std::string& customerName,
            const std::string& phone, const std::vector<Seat>& seats, double total);

    int getId() const { return id_; }
    int getShowId() const { return showId_; }
    const std::string& getCustomerName() const { return customerName_; }
    const std::string& getPhone() const { return phone_; }
    const std::vector<Seat>& getSeats() const { return seats_; }
    double getTotal() const { return total_; }
    bool isCancelled() const { return cancelled_; }

    void cancel() { cancelled_ = true; }

    std::string seatList() const;         // "A1, A2, B5"
    void display(const std::string& movieTitle, const std::string& showTime) const;

    // "id|showId|name|phone|total|cancelled|r:c,r:c"
    std::string serialize() const;
    static bool deserialize(const std::string& line, Booking& out);

private:
    int id_;
    int showId_;
    std::string customerName_;
    std::string phone_;
    std::vector<Seat> seats_;
    double total_;
    bool cancelled_;
};

#endif // BOOKING_H
