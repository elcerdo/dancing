#ifndef __HYPERARRAY_H__
#define __HYPERARRAY_H__

class HyperArray {
public:
	static const int HEIGHT = 16;
	static const int WIDTH  = 4;
	HyperArray();
	int get_value(int row, const char *column);
	bool set_value(int row, const char *column, int value);
protected:
	int data[HEIGHT][WIDTH];
};

#endif