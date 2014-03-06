//#define NDEBUG

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include "FibHeap.h"


const int LARGE = 2000000000;
const int SMALL = 2000;

const int arr_size = 30;
const double denominator = 2.7818281828459;

using namespace std;

int main(int argc,char** argv) {
    srand(static_cast<unsigned int>(time(0)));
    int count = 0;

    // construction tests
    // definitions
    double dbl_arr[arr_size];
    for ( int i = 0; i < arr_size; i++ ) {
        dbl_arr[i] = rand()/denominator;
    }

    FibHeap<int> H1;
    FibHeap<double> H2(dbl_arr, arr_size);
    FibHeap<double> H3(H2);
    // construction was successful
    assert(H1.size() == 0 && H2.size() == arr_size && H3.size() == arr_size);
    count++;
    // there are no External Nodes in the heap
    assert(H1.en_count() == 0 && H2.en_count() == 0 && H1.en_count() == 0);
    count++;
    // the copy constructor does not transform values and the heap produces an
    // identical sequence
    for ( int i = 0; i < arr_size; i++ ) {
        assert( H2.extractMin() == H3.extractMin() );
    }
    count++;

    cout << count << " tests passed!" << endl;
    cin.get();
    return 0;
}
