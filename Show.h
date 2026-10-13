#ifndef SHOW_H
#define SHOW_H

#include <string>
#include <vector>

// One screening of a movie in a hall at a given time, with its own seat map.
class Show {
public:
    Show();
    Show(int id, int movieId, const std::string& hall, const std::string& time,
         double price, int rows, int cols);

    int getId() const { return id_; }
    int getMovieId() const { return movieId_; }
    const std::string& getHall() const { return hall_; }
    const std::string& getTime() const { return time_; }
    double getPrice() const { return price_; }
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }

    bool isValidSeat(int row, int col) const;
    bool isAvailable(int row, int col) const;
    bool bookSeat(int row, int col);      // false if invalid or already taken
    bool releaseSeat(int row, int col);   // false if invalid or already free

    int totalSeats() const { return rows_ * cols_; }
    int availableSeats() const;

    void displaySeatMap() const;
    void displaySummary(const std::string& movieTitle) const; // one table row
    static void displayHeader();

    // "id|movieId|hall|time|price|rows|cols|seatString" (seatString is 0/1 per seat)
    std::string serialize() const;
    static bool deserialize(const std::string& line, Show& out);

private:
    int id_;
    int movieId_;
    std::string hall_;
    std::string time_;
    double price_;
    int rows_;
    int cols_;
    std::vector<std::vector<bool>> booked_;
};

#endif // SHOW_H
