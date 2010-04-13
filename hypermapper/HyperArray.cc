#include "HyperArray.h"
#include <cassert>
#include <string>
#include <iostream>

using std::endl;
using std::cout;

HyperArray::HyperArray() {
	for (int i=0; i<HEIGHT; i++) for (int j=0; j<WIDTH; j++) { data[i][j] = 0; }
}

HyperArray::HyperArray(const HyperArray &orig) {
	for (int i=0; i<HEIGHT; i++) for (int j=0; j<WIDTH; j++) { data[i][j] = orig.data[i][j]; }
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

HyperThreadParams::HyperThreadParams(const HyperArray &array) : array(array), finished(false) {
	pthread_mutex_init(&finished_mutex,NULL);
	pthread_create(&thread,NULL,HyperThreadParams::loop,static_cast<void*>(this));
}

HyperThreadParams::~HyperThreadParams() {
	pthread_mutex_destroy(&finished_mutex);
}

bool HyperThreadParams::is_finished() {
	pthread_mutex_lock(&finished_mutex);
	bool cache = finished;
	pthread_mutex_unlock(&finished_mutex);
	return cache;
}

void *HyperThreadParams::loop(void *arg) {
	HyperThreadParams *self = static_cast<HyperThreadParams*>(arg);
	cout << "started thread" << endl;
	cout.flush();

	

	pthread_mutex_lock(&self->finished_mutex);
	self->finished = true;
	pthread_mutex_unlock(&self->finished_mutex);
	
	return NULL;
}
