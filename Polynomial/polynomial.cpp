#include <new>
#include <cmath>
#include <algorithm>
#include <limits>
#include <iostream>
#include "polynomial.h"

#define NULL 0


/*********************
 * {con,de}structor(s)
 *********************/

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
	else {
		degree = -1;
		coefficients = NULL;
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
	else {
		degree = -1;
		coefficients = NULL;
	}
}

// create a polynomial identical to one that is already instantiated
Polynomial::Polynomial(const Polynomial &original) {
	degree = original.degree;
	if ( degree >= 0 ) {
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
	else {
		coefficients = NULL;
	}
}

// deallocate memory for coefficients
Polynomial::~Polynomial() {
	if ( coefficients != NULL ) {
		delete [] coefficients;
		coefficients = NULL;
	}
}


/**********************
 * overloaded operators
 **********************/

// assigns a polynomial to another one
Polynomial Polynomial::operator=(const Polynomial &right) {
	if ( this->degree >= 0 ) {
		delete [] this->coefficients;
	}
	this->degree = right.degree;
	if ( this->degree >= 0 ) {
		try {
			this->coefficients = new double[this->degree+1];
		}
		catch( std::bad_alloc ) {
			throw NoMemory();
		}
		for ( int i = 0; i <= this->degree; i++ ) {
			this->coefficients[i] = right.coefficients[i];
		}
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
	double sum;
	if ( this->degree < right.degree ) {
		Polynomial result(right.degree);
		for ( int i = 0; i <= this->degree; i++ ) {
			sum = this->coefficients[i] + right.coefficients[i];
			// essentially equal to 0 test
			// source: http://stackoverflow.com/questions/17333/most-effective-way-for-float-and-double-comparison
			if ( fabs(sum) <= 
				 ( fabs(this->coefficients[i]) > fabs(right.coefficients[i]) ?
				 fabs(right.coefficients[i]) : fabs(this->coefficients[i]) ) *
				 std::numeric_limits<double>::epsilon() ) {
				result[i] = 0;
			}
			else {
				result[i] = sum;
			}
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
			sum = this->coefficients[i] + right.coefficients[i];
			if ( fabs(sum) <= 
				 ( fabs(this->coefficients[i]) > fabs(right.coefficients[i]) ?
				 fabs(right.coefficients[i]) : fabs(this->coefficients[i]) ) *
				 std::numeric_limits<double>::epsilon() ) {
				result[i] = 0;
			}
			else {
				result[i] = sum;
			}
		}
		for ( int i = right.degree+1; i <= index; i++ ) {
			result[i] = this->coefficients[i];
		}
		if ( result[index] == 0 ) {
			result.clean();
		}
		return result;
	}
}

// subtracts two polynomials
Polynomial Polynomial::operator-(const Polynomial &right) {
	if ( this->degree == -1 ) {
		Polynomial result(right);
		return result;
	}
	if ( right.degree == -1 ) {
		Polynomial result(*this);
		return result;
	}
	double diff;
	if ( this->degree < right.degree ) {
		Polynomial result(right.degree);
		for ( int i = 0; i <= this->degree; i++ ) {
			diff = this->coefficients[i] - right.coefficients[i];
			if ( fabs(diff) <= 
				 ( fabs(this->coefficients[i]) > fabs(right.coefficients[i]) ?
				 fabs(right.coefficients[i]) : fabs(this->coefficients[i]) ) *
				 std::numeric_limits<double>::epsilon() ) {
				result[i] = 0;
			}
			else {
				result[i] = diff;
			}
		}
		for ( int i = this->degree+1; i <= right.degree; i++ ) {
			result[i] = -right.coefficients[i];
		}
		return result;
	}
	else {
		int index = this->degree;
		Polynomial result(index);
		for ( int i = 0; i <= right.degree; i++ ) {
			diff = this->coefficients[i] - right.coefficients[i];
			if ( fabs(diff) <= 
				 ( fabs(this->coefficients[i]) > fabs(right.coefficients[i]) ?
				 fabs(right.coefficients[i]) : fabs(this->coefficients[i]) ) *
				 std::numeric_limits<double>::epsilon() ) {
				result[i] = 0;
			}
			else {
				result[i] = diff;
			}
		}
		for ( int i = right.degree+1; i <= index; i++ ) {
			result[i] = this->coefficients[i];
		}
		if ( result[index] == 0 ) {
			result.clean();
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
		for ( int j = std::max(i-right.degree, 0);
			  j <= std::min(this->degree, i);
			  j++ ) {
			total += (*this)[j] * right.coefficients[i-j];
		}
		result[i] = total;
	}
	return result;
}

// divides two polynomials and returns the quotient
Polynomial Polynomial::operator/(const Polynomial &right) {
	if ( this->degree == -1 ) {
		return *this;
	}
	if ( right.degree == -1 ) {
		throw DivideByZero();
	}
	EuclidPair result = EuclideanDivision(*this, right);
	return result.quotient;
}

// divides two polynomials and returns the remainder
Polynomial Polynomial::operator%(const Polynomial &right) {
	if ( this->degree == -1 ) {
		return *this;
	}
	if ( right.degree == -1 ) {
		throw DivideByZero();
	}
	EuclidPair result = EuclideanDivision(*this, right);
	return result.remainder;
}

// accesses and/or mutates coefficient of degree index
double& Polynomial::operator[](int index) {
	if ( index >= 0 && index <= degree ) {
		return coefficients[index];
	}
	else {
		throw OutOfRange();
	}
}

// determines whether two polynomials have the same degrees and coefficients
bool Polynomial::operator==(const Polynomial &right) {
	if ( this->degree != right.degree ) {
		return false;
	}
	for ( int i = 0; i <= this->degree; i++ ) {
		if ( (*this)[i] != right.coefficients[i] ) {
			return false;
		}
	}
	return true;
}

// determines whether two polynomials have different degrees or coefficients
bool Polynomial::operator!=(const Polynomial &right) {
	if ( this->degree != right.degree ) {
		return true;
	}
	for ( int i = 0; i <= this->degree; i++ ) {
		if ( (*this)[i] != right.coefficients[i] ) {
			return true;
		}
	}
	return false;
}

/************************
 * mutators and accessors
 ************************/

// changes the degree and adjusts the coefficients accordingly
void Polynomial::setDegree(int deg) {
	if ( deg != degree ) {
		if ( deg >= 0 ) {
			if ( degree >= 0 ) {
				double* temp = coefficients;
				try {
					coefficients = new double[deg+1];
				}
				catch( std::bad_alloc ) {
					throw NoMemory();
				}
				if ( deg < degree ) {
					degree = deg;
					for ( int i = 0; i <= degree; i++ ) {
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
					degree = deg;
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
		else {
			if ( degree >= 0 ) {
				delete [] coefficients;
			}
			degree = -1;
			coefficients = NULL;
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


/*************************************
 * miscellaneous and private functions
 *************************************/

// evaluate polynomial at a point via Horner's method
double Polynomial::evaluate(const double point) {
	double result = (*this)[this->degree];
	for ( int i = this->degree - 1; i >= 0; i-- ) {
		result *= point;
		result += (*this)[i];
	}
	return result;
}

// divides two polynomials to obtain a Euclid pair
EuclidPair Polynomial::EuclideanDivision(const Polynomial &left,
										 const Polynomial &right) {
	EuclidPair result;
	if ( left.degree < right.degree ) {
		result.remainder = left;
		return result;
	}
	else {
		int index = left.degree - right.degree;
		result.quotient.setDegree(index);
		Polynomial dividend(left), divisor(right);
		while ( dividend.degree >= divisor.degree ) {
			result.quotient[index] = 
				dividend[dividend.degree]/divisor[divisor.degree];
			divisor = result.quotient.subterm(index) * divisor;
			dividend = dividend - divisor;
			if ( dividend.degree == index + right.degree ) {
				dividend[dividend.degree] = 0;
				dividend.clean();
			}
			divisor = right;
			// if ( dividend.degree - divisor.degree == index ) error;
			index = dividend.degree - divisor.degree;
		}
		result.remainder = dividend;
		return result;
	}
}

// creates a polynomial identical to a single term of the caller
Polynomial Polynomial::subterm(int degree) {
	Polynomial result;
	result.setDegree(degree);
	result[degree] = (*this)[degree];
	return result;
}

// resets the degree if leading coefficients are 0
void Polynomial::clean() {
	int index = this->degree;
	while ( index >= 0 && (*this)[index] == 0  ) {
		index--;
	}
	this->setDegree(index);
}


/*********
 * friends
 *********/

// outputs a polynomial as a line: degree tab_char coefficients 0 to degree
std::ostream& operator<<(std::ostream &out, Polynomial &poly) {
	out << poly.degree << "\t";
	for ( int i = 0; i <= poly.degree; i++ ) {
		out << " " << poly[i];
	}
	out << std::endl;
	return out;
}