#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <string>

class Location {

public:
	Location();
	~Location();
	Location(const Location &copy);
	Location &operator= (const Location &operand);

private:
	std::string pattern;

	// Для начала только root
	std::string root;

};

#endif