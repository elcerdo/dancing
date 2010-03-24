#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <istream>

struct Array {
	Array(int width, int height);
	~Array();

	bool parse(std::istream &is); 
    int get_number_of_ones() const;
	bool get_value(int i, int j) const;
	void set_value(int i, int j, bool value);

	const int width, height, size;
protected:
	bool *data;
};

std::ostream &operator<<(std::ostream &os,const Array &array);

#endif
