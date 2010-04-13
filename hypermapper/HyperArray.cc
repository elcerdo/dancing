#include "HyperArray.h"
#include <cassert>
#include <string>

HyperArray::HyperArray() {
	for (int i=0; i<HEIGHT; i++) for (int j=0; j<WIDTH; j++) { data[i][j] = 0; }
}

int HyperArray::get_value(int row, const char *column) {
	std::string stdcolumn(column);
	if (stdcolumn == "from")  return row+1;
	if (stdcolumn == "kick")  return data[row][0];
	if (stdcolumn == "snare") return data[row][1];
	if (stdcolumn == "high")  return data[row][2];
	if (stdcolumn == "low")   return data[row][3];
	return -1;
}

bool HyperArray::set_value(int row, const char *column, int value) {
	if (value<1 or value>16) return false;
	std::string stdcolumn(column);
	if (stdcolumn == "kick")  { data[row][0] = value; return true; }
	if (stdcolumn == "snare") { data[row][1] = value; return true; }
	if (stdcolumn == "high")  { data[row][2] = value; return true; }
	if (stdcolumn == "low")   { data[row][3] = value; return true; }
	return false;
}
