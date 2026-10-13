#include "Cinema.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include "Utils.h"

Cinema::Cinema(const std::string& dataDir)
    : dataDir_(dataDir), nextMovieId_(1), nextShowId_(1), nextBookingId_(1) {
    load();
    if (movies_.empty() && shows_.empty() && bookings_.empty()) {
        seedDefaults();
        save();
    }
}

// ---------------------------------------------------------------- persistence

void Cinema::load() {
    std::string line;

    std::ifstream movieIn(moviesFile().c_str());
    while (std::getline(movieIn, line)) {
        Movie m;
        if (Movie::deserialize(line, m)) {
            movies_.push_back(m);
            nextMovieId_ = std::max(nextMovieId_, m.getId() + 1);
        }
    }

    std::ifstream showIn(showsFile().c_str());
    while (std::getline(showIn, line)) {
        Show s;
        if (Show::deserialize(line, s)) {
            shows_.push_back(s);
            nextShowId_ = std::max(nextShowId_, s.getId() + 1);
        }
    }

    std::ifstream bookingIn(bookingsFile().c_str());
    while (std::getline(bookingIn, line)) {
        Booking b;
        if (Booking::deserialize(line, b)) {
            bookings_.push_back(b);
            nextBookingId_ = std::max(nextBookingId_, b.getId() + 1);
        }
    }
}

void Cinema::save() const {
    std::ofstream movieOut(moviesFile().c_str());
    for (const Movie& m : movies_) movieOut << m.serialize() << "\n";

    std::ofstream showOut(showsFile().c_str());
    for (const Show& s : shows_) showOut << s.serialize() << "\n";

    std::ofstream bookingOut(bookingsFile().c_str());
    for (const Booking& b : bookings_) bookingOut << b.serialize() << "\n";

    if (!movieOut || !showOut || !bookingOut) {
        std::cout << "  Warning: could not write data files to '" << dataDir_ << "'.\n";
    }
}

void Cinema::seedDefaults() {
    movies_.push_back(Movie(nextMovieId_++, "The Last Horizon", "Sci-Fi", 142, "English", "UA"));
    movies_.push_back(Movie(nextMovieId_++, "Monsoon Melodies", "Romance", 128, "Hindi", "U"));
    movies_.push_back(Movie(nextMovieId_++, "Midnight Heist", "Thriller", 117, "English", "A"));

    shows_.push_back(Show(nextShowId_++, 1, "Hall 1", "2026-10-20 18:00", 250.0, 6, 8));
    shows_.push_back(Show(nextShowId_++, 2, "Hall 2", "2026-10-20 20:30", 200.0, 5, 8));
    shows_.push_back(Show(nextShowId_++, 3, "Hall 1", "2026-10-21 21:00", 300.0, 6, 8));
}

// -------------------------------------------------------------------- lookups

Movie* Cinema::findMovie(int id) {
    for (Movie& m : movies_) if (m.getId() == id) return &m;
    return nullptr;
}

const Movie* Cinema::findMovie(int id) const {
    for (const Movie& m : movies_) if (m.getId() == id) return &m;
    return nullptr;
}

Show* Cinema::findShow(int id) {
    for (Show& s : shows_) if (s.getId() == id) return &s;
    return nullptr;
}

const Show* Cinema::findShow(int id) const {
    for (const Show& s : shows_) if (s.getId() == id) return &s;
    return nullptr;
}

Booking* Cinema::findBooking(int id) {
    for (Booking& b : bookings_) if (b.getId() == id) return &b;
    return nullptr;
}

std::string Cinema::movieTitle(int movieId) const {
    const Movie* m = findMovie(movieId);
    return m ? m->getTitle() : "(removed movie)";
}

bool Cinema::movieHasActiveBookings(int movieId) const {
    for (const Booking& b : bookings_) {
        if (b.isCancelled()) continue;
        const Show* s = findShow(b.getShowId());
        if (s && s->getMovieId() == movieId) return true;
    }
    return false;
}

// ------------------------------------------------------------ movie management

void Cinema::addMovie() {
    Utils::printTitle("ADD MOVIE");
    std::string title = Utils::readLine("Title      : ");
    std::string genre = Utils::readLine("Genre      : ");
    int duration = Utils::readInt("Duration (minutes, 1-400): ", 1, 400);
    std::string language = Utils::readLine("Language   : ");
    std::string rating = Utils::readLine("Rating (U / UA / A): ");

    movies_.push_back(Movie(nextMovieId_, title, genre, duration, language, rating));
    std::cout << "\n  Movie added with ID " << nextMovieId_ << ".\n";
    ++nextMovieId_;
    save();
}

void Cinema::listMovies() const {
    Utils::printTitle("NOW SHOWING");
    if (movies_.empty()) {
        std::cout << "  No movies available.\n";
        return;
    }
    Movie::displayHeader();
    for (const Movie& m : movies_) m.display();
}

void Cinema::searchMovie() const {
    Utils::printTitle("SEARCH MOVIE");
    std::string key = Utils::readLine("Enter title or genre to search: ");
    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    bool found = false;
    for (const Movie& m : movies_) {
        std::string hay = m.getTitle() + " " + m.getGenre();
        std::transform(hay.begin(), hay.end(), hay.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (hay.find(lowerKey) != std::string::npos) {
            if (!found) {
                Movie::displayHeader();
                found = true;
            }
            m.display();
        }
    }
    if (!found) std::cout << "  No movie matches \"" << key << "\".\n";
}

void Cinema::removeMovie() {
    Utils::printTitle("REMOVE MOVIE");
    if (movies_.empty()) {
        std::cout << "  No movies to remove.\n";
        return;
    }
    listMovies();
    int id = Utils::readInt("\nEnter movie ID to remove (0 to go back): ", 0, 100000);
    if (id == 0) return;

    if (!findMovie(id)) {
        std::cout << "  No movie with ID " << id << ".\n";
        return;
    }
    if (movieHasActiveBookings(id)) {
        std::cout << "  Cannot remove: this movie has active bookings. Cancel them first.\n";
        return;
    }
    if (!Utils::confirm("Remove \"" + movieTitle(id) + "\" and all of its shows?")) return;

    movies_.erase(std::remove_if(movies_.begin(), movies_.end(),
                                 [id](const Movie& m) { return m.getId() == id; }),
                  movies_.end());
    shows_.erase(std::remove_if(shows_.begin(), shows_.end(),
                                [id](const Show& s) { return s.getMovieId() == id; }),
                 shows_.end());
    std::cout << "  Movie removed.\n";
    save();
}

// ------------------------------------------------------------- show management

void Cinema::addShow() {
    Utils::printTitle("SCHEDULE SHOW");
    if (movies_.empty()) {
        std::cout << "  Add a movie first.\n";
        return;
    }
    listMovies();
    int movieId = Utils::readInt("\nMovie ID (0 to go back): ", 0, 100000);
    if (movieId == 0) return;
    if (!findMovie(movieId)) {
        std::cout << "  No movie with ID " << movieId << ".\n";
        return;
    }
    std::string hall = Utils::readLine("Hall name (e.g. Hall 1): ");
    std::string time = Utils::readLine("Date & time (e.g. 2026-10-25 19:30): ");
    double price = Utils::readDouble("Ticket price (Rs.): ", 1.0, 5000.0);
    int rows = Utils::readInt("Number of seat rows (1-10): ", 1, 10);
    int cols = Utils::readInt("Seats per row (1-15): ", 1, 15);

    shows_.push_back(Show(nextShowId_, movieId, hall, time, price, rows, cols));
    std::cout << "\n  Show scheduled with ID " << nextShowId_ << ".\n";
    ++nextShowId_;
    save();
}

void Cinema::listShows() const {
    Utils::printTitle("SHOW SCHEDULE");
    if (shows_.empty()) {
        std::cout << "  No shows scheduled.\n";
        return;
    }
    Show::displayHeader();
    for (const Show& s : shows_) s.displaySummary(movieTitle(s.getMovieId()));
}

// ------------------------------------------------------------- ticket booking

bool Cinema::parseSeatList(const std::string& line, const Show& show,
                           std::vector<Booking::Seat>& seats) const {
    std::istringstream in(line);
    std::string token;
    std::set<Booking::Seat> seen;
    while (in >> token) {
        int r = 0, c = 0;
        if (!Utils::parseSeat(token, r, c) || !show.isValidSeat(r, c)) {
            std::cout << "  \"" << token << "\" is not a valid seat for this hall.\n";
            return false;
        }
        if (!show.isAvailable(r, c)) {
            std::cout << "  Seat " << Utils::seatLabel(r, c) << " is already booked.\n";
            return false;
        }
        if (!seen.insert(Booking::Seat(r, c)).second) {
            std::cout << "  Seat " << Utils::seatLabel(r, c) << " was entered twice.\n";
            return false;
        }
        seats.push_back(Booking::Seat(r, c));
    }
    if (seats.empty()) {
        std::cout << "  No seats entered.\n";
        return false;
    }
    if (seats.size() > 10) {
        std::cout << "  You can book at most 10 seats at a time.\n";
        return false;
    }
    return true;
}

void Cinema::bookTickets() {
    Utils::printTitle("BOOK TICKETS");
    if (shows_.empty()) {
        std::cout << "  No shows available for booking.\n";
        return;
    }
    listShows();
    int showId = Utils::readInt("\nEnter show ID (0 to go back): ", 0, 100000);
    if (showId == 0) return;
    Show* show = findShow(showId);
    if (!show) {
        std::cout << "  No show with ID " << showId << ".\n";
        return;
    }
    if (show->availableSeats() == 0) {
        std::cout << "  Sorry, this show is sold out.\n";
        return;
    }

    std::cout << "\n  " << movieTitle(show->getMovieId()) << " | " << show->getHall()
              << " | " << show->getTime() << " | " << Utils::money(show->getPrice())
              << " per seat\n";
    show->displaySeatMap();

    std::string name = Utils::readLine("\nCustomer name: ");
    std::string phone;
    while (true) {
        phone = Utils::readLine("Phone number (10 digits): ");
        bool ok = phone.size() == 10 &&
                  std::all_of(phone.begin(), phone.end(),
                              [](unsigned char c) { return std::isdigit(c) != 0; });
        if (ok) break;
        std::cout << "  Phone number must be exactly 10 digits.\n";
    }

    std::vector<Booking::Seat> seats;
    while (true) {
        seats.clear();
        std::string line = Utils::readLine("Seats to book, separated by spaces (e.g. A1 A2), or 0 to cancel: ");
        if (line == "0") return;
        if (parseSeatList(line, *show, seats)) break;
    }

    double total = show->getPrice() * static_cast<double>(seats.size());
    Booking booking(nextBookingId_, showId, name, phone, seats, total);

    std::cout << "\n  Seats : " << booking.seatList() << "\n"
              << "  Total : " << Utils::money(total) << "\n";
    if (!Utils::confirm("Confirm booking?")) {
        std::cout << "  Booking discarded.\n";
        return;
    }

    for (const Booking::Seat& s : seats) show->bookSeat(s.first, s.second);
    bookings_.push_back(booking);
    ++nextBookingId_;
    save();

    Utils::printTitle("BOOKING CONFIRMED");
    booking.display(movieTitle(show->getMovieId()), show->getTime());
}

void Cinema::cancelBooking() {
    Utils::printTitle("CANCEL BOOKING");
    int id = Utils::readInt("Enter booking ID (0 to go back): ", 0, 1000000);
    if (id == 0) return;

    Booking* booking = findBooking(id);
    if (!booking) {
        std::cout << "  No booking with ID " << id << ".\n";
        return;
    }
    if (booking->isCancelled()) {
        std::cout << "  Booking #" << id << " is already cancelled.\n";
        return;
    }
    Show* show = findShow(booking->getShowId());
    booking->display(show ? movieTitle(show->getMovieId()) : "(removed)",
                     show ? show->getTime() : "-");
    if (!Utils::confirm("Cancel this booking and refund " + Utils::money(booking->getTotal()) + "?")) {
        return;
    }

    if (show) {
        for (const Booking::Seat& s : booking->getSeats()) show->releaseSeat(s.first, s.second);
    }
    booking->cancel();
    save();
    std::cout << "  Booking cancelled. Refund of " << Utils::money(booking->getTotal())
              << " issued.\n";
}

void Cinema::viewBookings() const {
    Utils::printTitle("ALL BOOKINGS");
    if (bookings_.empty()) {
        std::cout << "  No bookings yet.\n";
        return;
    }
    for (const Booking& b : bookings_) {
        const Show* s = findShow(b.getShowId());
        b.display(s ? movieTitle(s->getMovieId()) : "(removed)", s ? s->getTime() : "-");
        std::cout << "\n";
    }
}

// --------------------------------------------------------------------- reports

void Cinema::revenueReport() const {
    Utils::printTitle("REVENUE REPORT");
    double grand = 0.0;
    int tickets = 0;
    int cancelled = 0;

    for (const Show& s : shows_) {
        double revenue = 0.0;
        int sold = 0;
        for (const Booking& b : bookings_) {
            if (b.getShowId() != s.getId() || b.isCancelled()) continue;
            revenue += b.getTotal();
            sold += static_cast<int>(b.getSeats().size());
        }
        std::cout << "  Show #" << s.getId() << "  " << movieTitle(s.getMovieId())
                  << " (" << s.getTime() << ")\n"
                  << "     Tickets sold: " << sold << " / " << s.totalSeats()
                  << "   Revenue: " << Utils::money(revenue) << "\n";
        grand += revenue;
        tickets += sold;
    }
    for (const Booking& b : bookings_) if (b.isCancelled()) ++cancelled;

    Utils::printLine();
    std::cout << "  Total tickets sold : " << tickets << "\n"
              << "  Cancelled bookings : " << cancelled << "\n"
              << "  Total revenue      : " << Utils::money(grand) << "\n";
}
