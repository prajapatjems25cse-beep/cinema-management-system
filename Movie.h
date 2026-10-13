#ifndef MOVIE_H
#define MOVIE_H

#include <string>

// A movie that the cinema can screen.
class Movie {
public:
    Movie();
    Movie(int id, const std::string& title, const std::string& genre,
          int durationMin, const std::string& language, const std::string& rating);

    int getId() const { return id_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getGenre() const { return genre_; }
    int getDuration() const { return durationMin_; }
    const std::string& getLanguage() const { return language_; }
    const std::string& getRating() const { return rating_; }

    void display() const;                 // one table row
    static void displayHeader();          // table header for display()

    std::string serialize() const;        // "id|title|genre|duration|language|rating"
    static bool deserialize(const std::string& line, Movie& out);

private:
    int id_;
    std::string title_;
    std::string genre_;
    int durationMin_;
    std::string language_;
    std::string rating_;
};

#endif // MOVIE_H
