CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -O2
TARGET   = cinema
SRCS     = main.cpp Cinema.cpp Movie.cpp Show.cpp Booking.cpp Utils.cpp
OBJS     = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
