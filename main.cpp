// Cinema Management System - console application
// Build: make        Run: ./cinema

#include <iostream>

#include "Cinema.h"
#include "Utils.h"

namespace {

void printMenu() {
    Utils::printTitle("CINEMA MANAGEMENT SYSTEM");
    std::cout << "   1. View movies\n"
              << "   2. Search movie\n"
              << "   3. Add movie\n"
              << "   4. Remove movie\n"
              << "   5. View show schedule\n"
              << "   6. Schedule new show\n"
              << "   7. Book tickets\n"
              << "   8. Cancel booking\n"
              << "   9. View all bookings\n"
              << "  10. Revenue report\n"
              << "   0. Exit\n";
    Utils::printLine();
}

} // namespace

int main() {
    Cinema cinema(".");

    while (true) {
        printMenu();
        int choice = Utils::readInt("Enter your choice: ", 0, 10);
        switch (choice) {
            case 1:  cinema.listMovies();    break;
            case 2:  cinema.searchMovie();   break;
            case 3:  cinema.addMovie();      break;
            case 4:  cinema.removeMovie();   break;
            case 5:  cinema.listShows();     break;
            case 6:  cinema.addShow();       break;
            case 7:  cinema.bookTickets();   break;
            case 8:  cinema.cancelBooking(); break;
            case 9:  cinema.viewBookings();  break;
            case 10: cinema.revenueReport(); break;
            case 0:
                std::cout << "\nThank you for using the Cinema Management System. Goodbye!\n";
                return 0;
        }
    }
}
