#ifndef __HYPERARRAY_H__
#define __HYPERARRAY_H__

#include <pthread.h>

class HyperArray {
public:
	static const int HEIGHT = 16;
	static const int WIDTH  = 4;
	HyperArray();
	HyperArray(const HyperArray &orig);
	int get_value(int row, const char *column);
	bool set_value(int row, const char *column, int value);
protected:
	int data[HEIGHT][WIDTH];
};

struct HyperThreadParams {
	HyperThreadParams(const HyperArray &array);
	~HyperThreadParams();
	bool is_finished();
	const HyperArray array;
protected:
	static void *loop(void *);
	pthread_t thread;
	bool finished;
	pthread_mutex_t finished_mutex;
};

#endif