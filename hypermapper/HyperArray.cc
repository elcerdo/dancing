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
