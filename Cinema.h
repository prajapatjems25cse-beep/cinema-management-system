#ifndef CINEMA_H
#define CINEMA_H

#include <string>
#include <vector>

#include "Booking.h"
#include "Movie.h"
#include "Show.h"

// Central manager: owns all movies, shows and bookings, implements every
// menu operation and saves / loads data from plain text files.
class Cinema {
public:
    explicit Cinema(const std::string& dataDir = ".");

    // Movie management
    void addMovie();
    void listMovies() const;
    void searchMovie() const;
    void removeMovie();

    // Show management
    void addShow();
    void listShows() const;

    // Ticket booking
    void bookTickets();
    void cancelBooking();
    void viewBookings() const;

    // Reports
    void revenueReport() const;

private:
    std::string moviesFile() const { return dataDir_ + "/movies.txt"; }
    std::string showsFile() const { return dataDir_ + "/shows.txt"; }
    std::string bookingsFile() const { return dataDir_ + "/bookings.txt"; }

    void load();
    void save() const;
    void seedDefaults();

    Movie* findMovie(int id);
    const Movie* findMovie(int id) const;
    Show* findShow(int id);
    const Show* findShow(int id) const;
    Booking* findBooking(int id);

    std::string movieTitle(int movieId) const;
    bool movieHasActiveBookings(int movieId) const;
    bool parseSeatList(const std::string& line, const Show& show,
                       std::vector<Booking::Seat>& seats) const;

    std::string dataDir_;
    std::vector<Movie> movies_;
    std::vector<Show> shows_;
    std::vector<Booking> bookings_;
    int nextMovieId_;
    int nextShowId_;
    int nextBookingId_;
};

#endif // CINEMA_H
