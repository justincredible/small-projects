//#define NDEBUG

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include "FibHeap.h"

const int ARR_SIZE = 20000;
const double denominator = 2.7818281828459;

using namespace std;

int main(int argc,char** argv) {
    srand(static_cast<unsigned int>(time(0)));
    int count = 0;

    // DOUBLE TESTS
    /*
     */
    // definitions
    double dbl_arr[ARR_SIZE];

    // CONSTRUCTOR tests
    for ( int i = 0; i < ARR_SIZE; i++ ) {
        dbl_arr[i] = rand()/denominator;
    }

    FibHeap<double> H1;
    FibHeap<double> H2(dbl_arr, ARR_SIZE);
    FibHeap<double> H3(H2);
    // construction was successful
    assert(H1.size() == 0 && H2.size() == ARR_SIZE && H3.size() == ARR_SIZE);
    count++;
    // there are no External Nodes in the heap
    assert(H1.en_count() == 0 && H2.en_count() == 0 && H1.en_count() == 0);
    count++;
    // the copy constructor does not transform values and the heap produces an
    // identical sequence
    while( H2.size() ) {
        assert( H2.extractMin() == H3.extractMin() );
    }
    count++;

    // INSERTing and STORing value tests
    for ( int i = 0; i < ARR_SIZE; i++ ) {
        dbl_arr[i] = rand()/denominator/denominator;
        H1.insert(dbl_arr[i]);
        H2.store(dbl_arr[i]);
        H3.store(dbl_arr[i]);
        H3.en_insertFirst();
    }

    // size and en_count matches number of values passed
    assert(H1.size() == ARR_SIZE && H2.en_count() == ARR_SIZE && H3.size() == ARR_SIZE);
    count++;

    while ( H2.en_count() ) {
        H2.en_insertLast();
        H3.storeMin();
    }

    // any method of putting nodes in the heap produces the same result
    for ( int i = 0; i < ARR_SIZE; i++ ) {
        assert(H1.extractMin() == H3[i] && H2.extractMin() == H3[i]);
    }
    count++;
    // [] provides access to the node's key
    for ( int i = 0; i < H3.en_count(); i++ ) {
        H1.insert(H3[i]);
    }
    H3[H3.en_count()-1] = 7;
    for ( int i = 0; i < ARR_SIZE; i++ ) {
        assert(H1.extractMin() == H3[0] || H3[0] == 7 );
        H3.en_removeFirst();
    }
    count++;
    // check the sizes are correct
    assert(H1.size() == 0 && H3.size() == 0 && H3.en_count() == 0);
    count++;

    // MERGing HEAPS

    cout << count << " tests passed!" << endl;
    cin.get();
    return 0;
}
