#ifndef FIBONACCIHEAP_H_
#define FIBONACCIHEAP_H_

#include <cmath>
#include <cstdlib>
#include <limits>
#include <deque>

template <class T>
class FibHeap {

protected:
    /*
     * Algorithm from: Cormen et al. (2009) "Fibonacci Heaps," in Introduction to Algorithms, 3rd ed. Cambridge: MIT Press, pp. 505-530.
     * consider all references as paraphrases
     *
     *
     */
    // DATA STRUCTURES
    template <class C>
    struct FibonacciNode
    {
	    C key;

	    bool mark;
	
	    FibonacciNode* p;
	    FibonacciNode* left;
	    FibonacciNode* right;
	    FibonacciNode* child;
	    unsigned int degree;

        FibonacciNode() : key() mark() degree() p(left(right(child()))) {}
        FibonacciNode(C value) : key(value), mark(), degree() {
            p = left = right = child = 0;
        }
    };

    template <class C>
    struct FibonacciHeap
    {	
	    unsigned int n;
	    FibonacciNode<C>* min;

        FibonacciHeap() : n(), min() {}
    };

    // OPERATIONS
    /* *heap = MakeHeap()
     * . . . allocates and returns the Fibonacci heap object H, where 
     * H.n = 0 and H.min = NULL;
     */
    FibonacciHeap<T>* MakeHeap()
    {
	    FibonacciHeap<T>* H;
	
	    H = new FibonacciHeap<T>;
	    H->n = 0;
	    H->min = NULL;

	    return H;
    }

    /* Insert(H,x)
     * 1. x.degree = 0
     * 2. x.p = NULL
     * 3. x.child = NULL
     * 4. x.mark = FALSE
     * 5. if H.min == NULL
     * 6. 	create a root list for H containing just x
     * 7. 	H.min = x
     * 8. else insert x into H's root list
     * 9. 	if x.key < H.min.key
     *10. 		H.min = x
     *11. H.n = H.n + 1
     */
    void Insert( FibonacciHeap<T>* H, FibonacciNode<T>* x )
    {
	    // 1
	    x->degree = 0;
	    // 2
	    x->p = NULL;
	    // 3
	    x->child = NULL;
	    // 4
	    x->mark = false;
	    // 5
	    if ( H->min == NULL )
	    {
		    // 6, 7
		    H->min = x->left = x->right = x;
	    }
	    else
	    {
		    // 8
		    H->min->left->right = x;
		    x->left = H->min->left;
		    H->min->left = x;
		    x->right = H->min;
		    // 9
		    if ( x->key < H->min->key )
		    {
			    // 10
			    H->min = x;
		    }
	    }
	    // 11
	    H->n++;
    }

    /* *node = Minimum(H)
     * The minimum node of a Fibonacci Heap H is given by the pointer H.min
     * . . .
     */
    FibonacciNode<T>* Minimum( FibonacciHeap<T>* H )
    {
	    return H->min;
    }

    /* *heap = Union(H1,H2)
     * 1. H = MAKE-FIB-HEAP()
     * 2. H.min = H1.min
     * 3. concatenate the root list of H2 with the root list of H
     * 4. if (H1.min == NULL) or (H2.min != NULL and H2.min.key < H1.min.key)
     * 5. 	H.min = H2.min
     * 6. H.n = H1.n + H2.n
     * 7. return H
     */
    FibonacciHeap<T>* FibHeap<T>::Union( FibonacciHeap<T>* H1, FibonacciHeap<T>* H2 )
    {
	    FibonacciHeap<T>* H;
	
	    // 1
	    H = MakeHeap();
	    // 2
	    H->min = H1->min;
	    // 3
	    if ( H->min != NULL && H2->min != NULL )
	    {
		    H->min->right->left = H2->min->left;
		    H2->min->left->right = H->min->right;
		    H->min->right = H2->min;
		    H2->min->left = H->min;
	    }
	    // 4
	    if ( H1->min == NULL || ( H2->min != NULL && H2->min->key < H1->min->key ) )
	    {
		    // 5
		    H->min = H2->min;
	    }
	    // 6
	    H->n = H1->n + H2->n;
	    // 7
	    delete H1;
	    delete H2;

	    return H;
    }

    /* *node = ExtractMin(H)
     * 1. z = H.min
     * 2. if z != NULL
     * 3. 	for each child x of z
     * 4. 		add x to the root list of H
     * 5. 		x.p = NULL
     * 6. 	remove z from the root list of H
     * 7.		if z == z.right
     * 8. 		H.min = NULL
     * 9. 	else H.min = z.right
     *10. 		CONSOLIDATE(H)
     *11. 	H.n = H.n - 1
     *12. return z
     */
    FibonacciNode<T>* ExtractMin( FibonacciHeap<T>* H )
    {
	    FibonacciNode<T>* z, * x, * next;
	    FibonacciNode<T>** childList;
	
	    // 1
	    z = H->min;
	    // 2
	    if ( z != NULL )
	    {
		    // 3
		    x = z->child;
		    if ( x != NULL )
		    {
			    childList = new FibonacciNode<T>*[z->degree];
			    next = x;
			    for ( int i = 0; i < (int)z->degree; i++ )
			    {
				    childList[i] = next;
				    next = next->right;
			    }
			    for ( int i = 0; i < (int)z->degree; i++ )
			    {
				    x = childList[i];
				    // 4
				    H->min->left->right = x;
				    x->left = H->min->left;
				    H->min->left = x;
				    x->right = H->min;
				    // 5
				    x->p = NULL;
			    }
			    delete [] childList;
		    }
		    // 6
		    z->left->right = z->right;
		    z->right->left = z->left;
		    // 7
		    if ( z == z->right )
		    {
			    // 8
			    H->min = NULL;
		    }
		    else
		    {
			    // 9
			    H->min = z->right;
			    // 10
			    Consolidate(H);
		    }
		    // 11
		    H->n--;
	    }
	    // 12
	    return z;
    }

    /* Consolidate(H)
     * 1. let A[0 . . D(H.n)] be a new array
     * 2. for i = 0 to D(H.n)
     * 3. 	A[i] = NULL
     * 4. for each node w in the root list of H
     * 5. 	x = w
     * 6. 	d = x.degree
     * 7. 	while A[d] != NULL
     * 8. 		y = A[d]
     * 9. 		if x.key > y.key
     *10.			exchange x with y
     *11. 		FIB-HEAP-LINK(H,y,x)
     *12. 		A[d] = NULL
     *13. 		d = d + 1
     *14. 	A[d] = x
     *15. H.min = NULL
     *16. for i = 0 to D(H.n)
     *17. 	if A[i] != NULL
     *18. 		if H.min == NULL
     *19. 			create a root list for H containing just A[i]
     *20. 			H.min = A[i]
     *21. 		else insert A[i] into H's root list
     *22. 			if A[i].key < H.min.key
     *23. 				H.min = A[i]
     */
    void Consolidate( FibonacciHeap<T>* H )
    {
	    FibonacciNode<T>* w, * next, * x, * y, * temp;
	    FibonacciNode<T>** A, ** rootList;
	    // Max degree <= log base golden ratio of n
	    unsigned int d, rootSize;
	    int max_degree = static_cast<int>(
            floor( log(static_cast<double>(H->n)) / 
                   log((1 + sqrt(static_cast<double>(5)))/2)
            )
        );
	
	    // 1
	    A = new FibonacciNode<T>*[max_degree+2]; // plus two both for indexing to max degree and also A[max_degree+1] == NULL
	    // 2
	    for ( int i = 0; i < max_degree+2; i++ )
	    {
		    // 3
		    A[i] = NULL;
	    }
	    // 4
	    w = H->min;
	    rootSize = 0;
	    next = w;
	    do
	    {
		    rootSize++;
		    next = next->right;
	    } while ( next != w );
	    rootList = new FibonacciNode<T>*[rootSize];
	    for ( int i = 0; i < static_cast<int>(rootSize); i++ )
	    {
		    rootList[i] = next;
		    next = next->right;
	    }
	    for ( int i = 0; i < static_cast<int>(rootSize); i++ )
	    {
		    w = rootList[i];
		    // 5
		    x = w;
		    // 6
		    d = x->degree;
		    // 7
		    while ( A[d] != NULL )
		    {
			    // 8
			    y = A[d];
			    // 9
			    if ( x->key > y->key )
			    {
				    // 10
				    temp = x;
				    x = y;
				    y = temp;
			    }
			    // 11
			    FibHeapLink(H,y,x);
			    // 12
			    A[d] = NULL;
			    // 13
			    d++;
		    }
		    // 14
		    A[d] = x;
	    }
	    delete [] rootList;
	    // 15
	    H->min = NULL;
	    // 16
	    for ( int i = 0; i < max_degree+2; i++ )
	    {
		    // 17
		    if ( A[i] != NULL )
		    {
			    // 18
			    if ( H->min == NULL )
			    {
				    // 19, 20
				    H->min = A[i]->left = A[i]->right = A[i];
			    }
			    else
			    {
				    // 21
				    H->min->left->right = A[i];
				    A[i]->left = H->min->left;
				    H->min->left = A[i];
				    A[i]->right = H->min;
				    // 22
				    if ( A[i]->key < H->min->key )
				    {
					    // 23
					    H->min = A[i];
				    }
			    }
		    }
	    }
	    delete [] A;
    }

    /* FibHeapLink(H,y,x)
     * 1. remove y from the root list of H
     * 2. make y a child of x, incrementing x.degree
     * 3. y.mark = FALSE
     */
    void FibHeapLink( FibonacciHeap<T>* H, FibonacciNode<T>* y, FibonacciNode<T>* x )
    {
	    // 1
	    y->left->right = y->right;
	    y->right->left = y->left;
	    // 2
	    if ( x->child != NULL )
	    {
		    x->child->left->right = y;
		    y->left = x->child->left;
		    x->child->left = y;
		    y->right = x->child;
	    }
	    else
	    {
		    x->child = y;
		    y->right = y;
		    y->left = y;
	    }
	    y->p = x;
	    x->degree++;
	    // 3
	    y->mark = false;
    }

    /* DecreaseKey(H,x,k)
     * 1. if k > x.key
     * 2. 	error "new key is greater than current key"
     * 3. x.key = k
     * 4. y = x.p
     * 5. if y != NULL and x.key < y.key
     * 6. 	CUT(H,x,y)
     * 7. 	CASCADING-CUT(H,y)
     * 8. if x.key < H.min.key
     * 9. 	H.min = x
     */
    void DecreaseKey( FibonacciHeap<T>* H, FibonacciNode<T>* x, T k )
    {
	    FibonacciNode<T>* y;
	
	    // 1
	    if ( x->key < k )
	    {
		    // 2
		    // error( "new key is greater than current key" );
		    return;
	    }
	    // 3
	    x->key = k;
	    // 4
	    y = x->p;
	    // 5
	    if ( y != NULL && x->key < y->key )
	    {
		    // 6
		    Cut(H,x,y);
		    // 7
		    CascadingCut(H,y);
	    }
	    // 8
	    if ( x->key < H->min->key )
	    {
		    // 9
		    H->min = x;
	    }
    }

    /* Cut(H,x,y)
     * 1. remove x from the child list of y, decrementing y.degree
     * 2. add x to the root list of H
     * 3. x.p = NULL
     * 4. x.mark = FALSE
     */
    void Cut( FibonacciHeap<T>* H, FibonacciNode<T>* x, FibonacciNode<T>* y )
    {
	    // 1
	    if ( x->right == x )
	    {
		    y->child = NULL;
	    }
	    else
	    {
		    x->right->left = x->left;
		    x->left->right = x->right;
		    if ( y->child == x )
		    {
			    y->child = x->right;
		    }
	    }
	    y->degree--;
	    // 2
	    H->min->right->left = x;
	    x->right = H->min->right;
	    H->min->right = x;
	    x->left = H->min;
	    // 3
	    x->p = NULL;
	    // 4
	    x->mark = false;
    }

     /* CascadingCut(H,y)
      * 1. z = y.p
      * 2. if z != NULL
      * 3. 	if y.mark == FALSE
      * 4. 		y.mark = TRUE
      * 5. 	else CUT(H,y,z)
      * 6. 		CASCADING-CUT(H,z)
      */
    void CascadingCut( FibonacciHeap<T>* H, FibonacciNode<T>* y )
    {
	    FibonacciNode<T>* z;
	
	    // 1
	    z = y->p;
	    // 2
	    if ( z != NULL )
	    {
		    // 3
		    if ( y->mark == false )
		    {
			    // 4
			    y->mark = true;
		    }
		    else
		    {
			    // 5
			    Cut(H,y,z);
			    // 6
			    CascadingCut(H,z);
		    }
	    }
    }

    /* Delete(H,x)
     * undefined behaviour for non-numeric types
     */
    void Delete( FibonacciHeap<T>* H, FibonacciNode<T>* x )
    {
	    DecreaseKey(H,x,numeric_limits<T>::lowest());
	    delete ExtractMin(H);
    }

private:
    // VARIABLES
    std::deque<FibonacciNode<T>*> nodes;
    FibonacciHeap<T>* heap;

public:
    // CONSTRUCTORS
    FibHeap() : heap(MakeHeap()) {}

    FibHeap(T* array, int size) : heap(MakeHeap()) {
        FibonacciNode<T>* node;
        for ( int i = 0; i < size; i++ ) {
            node = new FibonacciNode<T>(array[i]);
            Insert(heap,node);
        }
    }

    FibHeap(FibHeap &H) : heap(MakeHeap()) {
        FibonacciNode<T>* node;
        while ( H.size() > 0 ) {
            H.storeMin();
        }
        while ( H.en_count() > 0 ) {
            node = new FibonacciNode<T>(H.en_valueLast());
            Insert(heap,node);
            H.en_insertLast();
        }
    }

    // DESTRUCTOR
    FibHeap<T>::~FibHeap() {
        while ( heap->min ) {
            delete ExtractMin(heap);
        }
        delete heap;
    }

    int size() { return static_cast<int>(heap->n) >= 0 ? 
                        static_cast<int>(heap->n) :
                        numeric_limits<int>::max();
    }

    int en_count() { return nodes.size(); }

    T en_valueLast() { return nodes.back()->key; }

    void en_insertLast() { 
        Insert(heap,nodes.back());
        nodes.pop_back();
    }

    //
    void storeMin() { nodes.push_back(ExtractMin(heap)); }

    T extractMin() {
        T value;
        nodes.push_front(ExtractMin(heap));
        value = nodes.front()->key;
        delete nodes.front();
        nodes.pop_front();
        return value;
    }

};


#endif
