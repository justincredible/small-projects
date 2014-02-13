#include <new>
#include "polynomial.h"

#define NULL 0

// create the zero polynomial
Polynomial::Polynomial() {
	degree = -1;
	coefficients = NULL;
}

// create a polynomial of degree deg
Polynomial::Polynomial(const int deg) {
	if ( deg >= 0 ) {
		degree = deg;
		try {
			coefficients = new double[degree+1];
		}
		catch( std::bad_alloc ) {
			throw NoMemory();
		}
		// initialize array. degree-th coefficient is 1 so that this polynomial
		// is not identically the zero polynomial
		for ( int i = 0; i < degree; i++ ) {
			coefficients[i] = 0;
		}
		coefficients[degree] = 1;
	}
}

// create a polynomial of degree deg with predetermined coefficients
Polynomial::Polynomial(const int deg, double *arr) {
	if ( deg >= 0 ) {
		degree = deg;
		try {
			coefficients = new double[degree+1];
		}
		catch( std::bad_alloc ) {
			throw NoMemory();
		}
		// set array to equal the first deg+1 elements of arr
		for ( int i = 0; i <= degree; i++ ) {
			coefficients[i] = arr[i];
		}
	}
}

// create a polynomial identical to one that is already instantiated
Polynomial::Polynomial(const Polynomial &original) {
	degree = original.degree;
	try {
		coefficients = new double[degree+1];
	}
	catch( std::bad_alloc ) {
		throw NoMemory();
	}
	for ( int i = 0; i <= degree; i++ ) {
		coefficients[i] = original.coefficients[i];
	}
}

// deallocate memory for coefficients
Polynomial::~Polynomial() {
	delete [] coefficients;
}

// assigns a polynomial to another one
Polynomial Polynomial::operator=(const Polynomial &right) {
	if ( this->degree >= 0 ) {
		delete [] this->coefficients;
	}
	this->degree = right.degree;
	try {
		this->coefficients = new double[this->degree+1];
	}
	catch( std::bad_alloc ) {
		throw NoMemory();
	}
	for ( int i = 0; i <= this->degree; i++ ) {
		this->coefficients[i] = right.coefficients[i];
	}
	return *this;
}

// adds two polynomials
Polynomial Polynomial::operator+(const Polynomial &right) {
	if ( this->degree == -1 ) {
		Polynomial result(right);
		return result;
	}
	if ( right.degree == -1 ) {
		Polynomial result(*this);
		return result;
	}
	if ( this->degree < right.degree ) {
		Polynomial result(right.degree);
		for ( int i = 0; i <= this->degree; i++ ) {
			result[i] = this->coefficients[i] + right.coefficients[i];
		}
		for ( int i = this->degree+1; i <= right.degree; i++ ) {
			result[i] = right.coefficients[i];
		}
		return result;
	}
	else {
		int index = this->degree;
		Polynomial result(index);
		for ( int i = 0; i <= right.degree; i++ ) {
			result[i] = this->coefficients[i] + right.coefficients[i];
		}
		for ( int i = right.degree+1; i <= index; i++ ) {
			result[i] = this->coefficients[i];
		}
		if ( result.getCoefficient(index) == 0 ) {
			do {
					index--;
			} while ( index >= 0 && result.getCoefficient(index) == 0 );
			result.setDegree(index);
		}
		return result;
	}
}

// multiplies two polynomials
Polynomial Polynomial::operator*(const Polynomial &right) {
	if ( this->degree == -1 || right.degree == -1 ) {
		Polynomial result;
		return result;
	}
	int index = this->degree + right.degree;
	Polynomial result(index);
	double total;
	// outer loop controls assignment to coefficients of result
	for ( int i = 0; i <= index; i++ ) {
		total = 0;
		// inner loop adds the coefficients of all terms with degree i. 
		for ( int j = 0; j <= i; j++ ) {
			if ( this->degree >=j && right.degree >= i-j ) {
				total += (*this)[j] * right.coefficients[i-j];
			}
		}
		result[i] = total;
	}
	return result;
}

// fetches coefficients without calling the accessor
double& Polynomial::operator[](int index) {
	if ( index >= 0 && index <= degree ) {
		return coefficients[index];
	}
	else {
		throw OutOfRange();
	}
}

// changes the degree and adjusts the coefficients accordingly
void Polynomial::setDegree(int deg) {
	// test that deg is valid and will actually change degree
	if ( deg != degree && deg >= 0 ) {
		if ( degree >= 0 ) {
			double* temp = coefficients;
			try {
				coefficients = new double[deg+1];
			}
			catch( std::bad_alloc ) {
				throw NoMemory();
			}
			if ( deg < degree ) {
				for ( int i = 0; i <= deg; i++ ) {
					coefficients[i] = temp[i];
				}
				if ( coefficients[degree] == 0 ) {
					coefficients[degree] = 1;
				}
				delete [] temp;
			}
			else {
				for ( int i = 0; i <= degree; i++ ) {
					coefficients[i] = temp[i];
				}
				for ( int i = degree+1; i < deg; i++ ) {
					coefficients[i] = 0;
				}
				coefficients[deg] = 1;
				delete [] temp;
			}
		}
		else {
			degree = deg;
			try {
				coefficients = new double[degree+1];
			}
			catch( std::bad_alloc ) {
				throw NoMemory();
			}
			for ( int i = 0; i < degree; i++ ) {
				coefficients[i] = 0;
			}
			coefficients[degree] = 1;
		}
	}
}

// sets coefficient of the term with degree index to value
void Polynomial::setCoefficient(int index, double value) {
	if ( index >= 0 && index <= degree ) {
		coefficients[index] = value;
		if ( index == degree && value == 0 ) {
			do {
				index--;
			} while ( index >= 0 && coefficients[index] == 0 );
			this->setDegree(index);
		}
	}
}

// fetches the degree of the polynomial
int Polynomial::getDegree() {
	return degree;
}

// fetches coefficent of the term with degree index
double Polynomial::getCoefficient(int index) {
	if ( index >= 0 ) {
		if ( index <= degree ) {
			return coefficients[index];
		}
		else {
			return 0;
		}
	}
}
