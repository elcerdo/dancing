#ifndef __HYPERARRAY_H__
#define __HYPERARRAY_H__

#include <pthread.h>
#include <map>
#include <vector>

class HyperArray {
public:
	static const int HEIGHT = 16;
	static const int WIDTH  = 4;
	HyperArray();
	HyperArray(const HyperArray &orig);
	
	bool load_file(const char *filename);
	bool save_file(const char *filename);

	int get_value_input(int row, int column) const;
	int get_value_output(int row, int column, int solution) const;
	int get_value(int row, const char *column, int solution) const;
	
	bool set_value_input(int row, int column, int value);
	bool set_value(int row, const char *column, int value);
	
	bool unset_value_input(int row, int column);
	bool unset_value(int row, const char *column);
	
	typedef std::pair<int,int> Coord;
	typedef std::map<Coord,int> Data;
	typedef std::vector<Data> Solutions;
	
	Solutions solutions;
protected:
	Data data;
};

class HyperThreadParams {
public:
	HyperThreadParams(const HyperArray &array);
	~HyperThreadParams();
	bool is_finished();
	const int constraints, moves;
	HyperArray local_array;
protected:
	static void *loop(void *);
	pthread_t thread;
	bool finished;

	pthread_mutex_t finished_mutex;
};

#endif