#include "HyperArray.h"
#include <cassert>
#include <string>
#include <iostream>
#include "andrew.h"

using std::endl;
using std::cout;

HyperArray::HyperArray() {}

HyperArray::HyperArray(const HyperArray &orig) { 
	data = Data(orig.data);
	solutions = Solutions(orig.solutions);
}	

static const uint32_t magic_number = 171827664;
static const int header_size = 32;
static const int array_header_size = 3;

static const int array_normal = 0;
static const int array_solution = 1;

bool HyperArray::load_file(const char *filename) {
	FILE *handle = fopen(filename,"rb");
	if (!handle) return false;

	int nsolution = 0;
	{ // loading header
		uint32_t header[header_size];
		for (int k=0; k<header_size; k++) { header[k] = 0; }
		fread(header, sizeof(uint32_t), header_size, handle);
		if (ferror(handle) || header[0] != magic_number || header[1] != HEIGHT || header[2] != WIDTH) {
			fclose(handle);
			return false;
		}
		nsolution = header[3];
	}
	printf("good header %d solutions\n", nsolution);

	Data local_data;
	{ // loading left array
		uint32_t array_header[array_header_size];
		for (int k=0; k<array_header_size; k++) { array_header[k] = 0; }
		fread(array_header, sizeof(uint32_t), array_header_size, handle);
		if (ferror(handle) || array_header[0] != array_normal) {
			fclose(handle);
			return false;
		}
		int ncell = array_header[1];
		while (ncell) {
			uint32_t cell[3];
			for (int k=0; k<3; k++) { cell[k] = 0; }
			fread(cell, sizeof(uint32_t), 3, handle);
			if (ferror(handle) || cell[0] >= HEIGHT || cell[1] >= WIDTH || cell[2] >= HEIGHT+1 || cell[2] == 0) {
				fclose(handle);
				return false;
			}
			Coord coord = std::make_pair<int,int>(cell[0], cell[1]);
			local_data[coord] = cell[2];
			ncell--;
		}
	}
	printf("found data %d cells\n", local_data.size());
	
	Solutions local_solutions;
	while (nsolution) {
		Data local_solution;
		{ // loading left array
			uint32_t array_header[array_header_size];
			for (int k=0; k<array_header_size; k++) { array_header[k] = 0; }
			fread(array_header, sizeof(uint32_t), array_header_size, handle);
			if (ferror(handle) || array_header[0] != array_solution) {
				fclose(handle);
				return false;
			}
			int ncell = array_header[1];
			while (ncell) {
				uint32_t cell[3];
				for (int k=0; k<3; k++) { cell[k] = 0; }
				fread(cell, sizeof(uint32_t), 3, handle);
				if (ferror(handle) || cell[0] >= HEIGHT || cell[1] >= WIDTH || cell[2] >= HEIGHT+1 || cell[2] == 0) {
					fclose(handle);
					return false;
				}
				printf("found cell %d in solution %d\n", ncell, nsolution);
				Coord coord = std::make_pair<int,int>(cell[0], cell[1]);
				local_solution[coord] = cell[2];
				ncell--;
			}
		}
		printf("found solution %d %d cells\n", nsolution, local_solution.size());
		local_solutions.push_back(local_solution);
		nsolution--;
	}
	
	data = local_data;
	solutions = local_solutions;
	
	return true;
}
	
bool HyperArray::save_file(const char *filename) {
	FILE *handle = fopen(filename,"wb");
	if (!handle) return false;
	
	{ // saving header
		uint32_t header[header_size];
		for (int k=0; k<header_size; k++) { header[k] = 0; }
		header[0] = magic_number;
		header[1] = HEIGHT;
		header[2] = WIDTH;
		header[3] = solutions.size();
		fwrite(header, sizeof(uint32_t), header_size, handle);
	}
	
	{ // saving left array
		uint32_t array_header[array_header_size];
		for (int k=0; k<array_header_size; k++) { array_header[k] = 0; }
		array_header[0] = array_normal;
		array_header[1] = data.size();
		fwrite(array_header, sizeof(uint32_t), array_header_size, handle);
		for (Data::const_iterator i=data.begin(); i!= data.end(); i++) {
			uint32_t cell[3];
			cell[0] = i->first.first;
			cell[1] = i->first.second;
			cell[2] = i->second;
			fwrite(cell, sizeof(uint32_t), 3, handle);
		}
	}
	
	// saving solutions
	for (Solutions::const_iterator i=solutions.begin(); i!=solutions.end(); i++) {
		const Data &solution = *i;
		uint32_t array_header[array_header_size];
		for (int k=0; k<array_header_size; k++) { array_header[k] = 0; }
		array_header[0] = array_solution;
		array_header[1] = solution.size();
		fwrite(array_header, sizeof(uint32_t), array_header_size, handle);
		for (Data::const_iterator i=solution.begin(); i!= solution.end(); i++) {
			uint32_t cell[3];
			cell[0] = i->first.first;
			cell[1] = i->first.second;
			cell[2] = i->second;
			fwrite(cell, sizeof(uint32_t), 3, handle);
		}
	}
		
	fclose(handle);
	
	return true;
}

int HyperArray::get_value(int row, const char *column, int solution) const {
	std::string stdcolumn(column);
	if (stdcolumn == "from")  return row+1;
	if (stdcolumn == "kick_input")  return get_value_input(row,0);
	if (stdcolumn == "snare_input") return get_value_input(row,1);
	if (stdcolumn == "high_input")  return get_value_input(row,2);
	if (stdcolumn == "low_input")   return get_value_input(row,3);
	if (stdcolumn == "kick_output")  return get_value_output(row,0,solution);
	if (stdcolumn == "snare_output") return get_value_output(row,1,solution);
	if (stdcolumn == "high_output")  return get_value_output(row,2,solution);
	if (stdcolumn == "low_output")   return get_value_output(row,3,solution);
	return 0;
}

int HyperArray::get_value_output(int i, int j, int s) const {
	assert(i>=0 and i<HEIGHT and j>=0 and j<WIDTH);
	if (s<0 or s>=solutions.size()) return 0;
	const Data &solution = solutions[s];
	Coord coord = std::make_pair(i,j);
	Data::const_iterator value = solution.find(coord);
	assert(value != solution.end());
	return value->second;
}
	
int HyperArray::get_value_input(int i, int j) const {
	assert(i>=0 and i<HEIGHT and j>=0 and j<WIDTH);
	Coord coord = std::make_pair(i,j);
	Data::const_iterator value = data.find(coord);
	if (value == data.end()) return 0;
	return value->second;
}

bool HyperArray::set_value(int row, const char *column, int value) {
	std::string stdcolumn(column);
	if (stdcolumn == "kick_input")  { return set_value_input(row,0,value); }
	if (stdcolumn == "snare_input") { return set_value_input(row,1,value); }
	if (stdcolumn == "high_input")  { return set_value_input(row,2,value); }
	if (stdcolumn == "low_input")   { return set_value_input(row,3,value); }
	return false;
}

bool HyperArray::set_value_input(int i, int j, int value) {
	if (value<1 or value>16) return false;
	assert(i>=0 and i<HEIGHT and j>=0 and j<WIDTH);
	Coord coord = std::make_pair(i,j);
	data[coord] = value;
	return true;
}

bool HyperArray::unset_value(int row,const char *column) {
	std::string stdcolumn(column);
	if (stdcolumn == "kick_input")  { return unset_value_input(row,0); }
	if (stdcolumn == "snare_input") { return unset_value_input(row,1); }
	if (stdcolumn == "high_input")  { return unset_value_input(row,2); }
	if (stdcolumn == "low_input")   { return unset_value_input(row,3); }
	return false;
}

bool HyperArray::unset_value_input(int i, int j) {
	assert(i>=0 and i<HEIGHT and j>=0 and j<WIDTH);
	Coord coord = std::make_pair(i,j);
	data.erase(coord);
	return true;
}

HyperThreadParams::HyperThreadParams(const HyperArray &array) : local_array(array), finished(false), constraints(0), moves(0) {
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

	Node::Collector collector;
	Node *root = build_structure(collector);
    cout << "collector has " << collector.size() << " nodes" << endl;
    Node::print_root(root,cout,false);
	cout.flush();
	
    // playing move
    SolveParams params(root,20);
    cout << "filling cells" << endl;
	for (int i=0; i<HEIGHT; i++) for (int j=0; j<WIDTH; j++) {
		int value = self->local_array.get_value_input(i,j); 
		if (value) { fill_cell(params,i,j,value); }
	}
    Node::print_root(root,cout,false);
	cout.flush();
	const_cast<int&>(self->moves) = Node::get_number_of_moves(root);
	const_cast<int&>(self->constraints) = Node::get_number_of_constraints(root);
	
    // solving
    cout << "looking for " << params.max_solution << " solution(s)" << endl;
    params.solve(cout,false);
    cout << "found " << params.solutions.size() << " solution(s)" << endl;
    int count=0;
    for (SolveParams::Solutions::iterator isolution=params.solutions.begin(); isolution!=params.solutions.end(); isolution++) {
        assert(isolution->size() == WIDTH*HEIGHT);
		HyperArray::Data solution;
		
        printf("%d\n",count++);
        printf("  |  K  S  H  L\n");
        printf("--+------------\n");
        for (int i=0; i<HEIGHT; i++) {
            printf("%2d|",i+1);
            for (int j=0; j<WIDTH; j++) {
                int coup;
                for (SolveParams::Solution::const_iterator imove=isolution->begin(); imove!=isolution->end(); imove++) {
                    Node *move = *imove;
                    int im,jm;
                    int ret = sscanf(move->get_id().c_str(),"%d in (%d,%d)",&coup,&im,&jm);
                    assert(ret != EOF);
                    if (im == i and jm == j) break;
                }
                printf(" %2d",coup);
				
				solution[std::make_pair(i,j)] = coup;
            }
            printf("\n");
        }
        printf("\n");
		
		self->local_array.solutions.push_back(solution);
		
    }
	
	delete_collector(collector);
	
	pthread_mutex_lock(&self->finished_mutex);
	self->finished = true;
	pthread_mutex_unlock(&self->finished_mutex);
	
	return NULL;
}
