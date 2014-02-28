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
    memory_scale = 0;
    coefficients = NULL;
}

// create a polynomial of degree deg an int.
// throws NoMemory exception
Polynomial::Polynomial(const int deg) {
    if ( deg >= 0 ) {
        degree = deg;
        memory_scale = 1;
        while ( memory_scale*BASE < degree+1 ) {
            memory_scale *= 2;
        }
        try {
            coefficients = new double[memory_scale*BASE];
        }
        catch( std::bad_alloc ) {
            throw NoMemory();
        }
        // initialize array. degree-th coefficient is 1 so that this polynomial
        // is not identically the zero polynomial.
        // bytes beyond [degree] are garbage
        for ( int i = 0; i < degree; i++ ) {
            coefficients[i] = 0;
        }
        coefficients[degree] = 1;
    }
    else {
        degree = -1;
        memory_scale = 0;
        coefficients = NULL;
    }
}

// create a polynomial of degree deg with predetermined coefficients
// throws NoMemory, OutOfRange exceptions
Polynomial::Polynomial(const int deg, double *arr, int arr_size) {
    if ( arr_size > deg ) {
        if ( deg >= 0 ) {
            degree = deg;
            memory_scale = 1;
            while ( memory_scale*BASE < degree+1 ) {
                memory_scale *= 2;
            }
            try {
                coefficients = new double[memory_scale*BASE];
            }
            catch( std::bad_alloc ) {
                throw NoMemory();
            }
            // set coefficients to equal the first deg+1 elements of arr
            for ( int i = 0; i <= degree; i++ ) {
                coefficients[i] = arr[i];
            }
        }
        else {
            degree = -1;
            memory_scale = 0;
            coefficients = NULL;
        }
    }
    else {
        throw OutOfRange();
    }
}

// create a polynomial identical to one that is already instantiated
// throws NoMemory exception
Polynomial::Polynomial(const Polynomial &original) {
    degree = original.degree;
    memory_scale = original.memory_scale;
    if ( degree >= 0 ) {
        try {
            coefficients = new double[memory_scale*BASE];
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

// deallocate memory for coefficients when points to NULL
Polynomial::~Polynomial() {
    if ( !coefficients ) {
        delete [] coefficients;
        coefficients = NULL;
    }
}


/**********************
 * overloaded operators
 **********************/

// assigns a polynomial to another one.
// this function modifies the caller only if it is not equal to the argument
Polynomial& Polynomial::operator=(const Polynomial &right) {
    if ( *this != right ) {
        this->setDegree(right.degree);
        for ( int i = 0; i <= this->degree; i++ ) {
            this->coefficients[i] = right.coefficients[i];
        }
    }
    return *this;
}

// adds two polynomials and assigns value to the caller.
// if the caller is the zero polynomial the sum is equal to the argument;
// else if the argument is not the zero polynomial the sum must be computed
Polynomial& Polynomial::operator+=(const Polynomial &right) {
    if ( this->degree == -1 ) {
        *this = right;
    }
    else if ( right.degree != -1 ) {
        double sum;
        if ( this->degree < right.degree ) {
            int old_degree = this->degree;
            this->setDegree(right.degree);
            // add coefficients of the same degree and assign to caller
            for ( int i = 0; i <= old_degree; i++ ) {
                sum = this->coefficients[i] + right.coefficients[i];
                // essentially equal to 0 test
                // source: http://stackoverflow.com/questions/17333/most-effective-way-for-float-and-double-comparison
                if ( fabs(sum) <= (
                     fabs(this->coefficients[i]) > fabs(right.coefficients[i]) ?
                     fabs(right.coefficients[i]) : fabs(this->coefficients[i]) 
                     ) * std::numeric_limits<double>::epsilon() ) {
                    this->coefficients[i] = 0;
                }
                else {
                    this->coefficients[i] = sum;
                }
            }
            // since right.degree > this->degree the remaining sums are of the
            // form 0 + right.coefficients[i]
            for ( int i = old_degree+1; i <= right.degree; i++ ) {
                this->coefficients[i] = right.coefficients[i];
            }
        }
        else {
            // add coefficients of the same degree and assign to caller
            for ( int i = 0; i <= right.degree; i++ ) {
                sum = this->coefficients[i] + right.coefficients[i];
                if ( fabs(sum) <= (
                     fabs(this->coefficients[i]) > fabs(right.coefficients[i]) ?
                     fabs(right.coefficients[i]) : fabs(this->coefficients[i]) 
                     ) * std::numeric_limits<double>::epsilon() ) {
                    this->coefficients[i] = 0;
                }
                else {
                    this->coefficients[i] = sum;
                }
            }
            // any coefficient of degree > right.degree is of the form
            // this->coefficients[i] + 0 and no second loop is necessary.
            // the leading coefficent may be 0 when this->degree = right.degree
            this->simplify();
        }
    }
    return *this;
}

// multiplies two polynomials and assigns value to the caller
Polynomial& Polynomial::operator*=(const Polynomial &right) {
    // the zero polynomial dominates multiplication
    if ( this->degree == -1 || right.degree == -1 ) {
        this->setDegree(-1);
        return *this;
    }
    int old_degree = this->degree;
    this->setDegree(this->degree+right.degree);
    double total;
    // if a.i is the ith coefficient of the caller and b.i the ith coefficient
    // of the argument, then a.i = a.i*b.0 + a.i-1*b.1 + . . . + a.0*b.i for
    // each 0 <= i <= this->degree + right.degree.
    // the loop counts down so that old coefficients are not overwritten before
    // they are no longer needed
    for ( int i = this->degree; i >= 0; i-- ) {
        total = 0;
        // the max and min prevent attempts to access out of range values in
        // the arrays
        for ( int j = std::max(i-right.degree, 0);
              j <= std::min(old_degree, i);
              j++ ) {
            total += this->coefficients[j] * right.coefficients[i-j];
        }
        this->coefficients[i] = total;
    }
    return *this;
}

// subtracts two polynomials via addition with additive inverse and assigns
// value to caller.
// this can be improved by providing scalar multiplcation
Polynomial& Polynomial::operator-=(const Polynomial &right) {
    Polynomial negative(0);
    negative[0] = -1;
    return *this += negative *= right;
}

// divides two polynomials and assigns quotient to caller
// // throws DivideByZero exception
Polynomial& Polynomial::operator/=(const Polynomial &right) {
    // the zero polynomial dominates division
    if ( this->degree == -1 ) {
        return *this;
    }
    if ( right.degree == -1 ) {
        throw DivideByZero();
    }
    return *this = (EuclideanDivision(*this, right)).quotient;
}

// divides two polynomials and assigns remainder to caller
// throws DivideByZero exception
Polynomial& Polynomial::operator%=(const Polynomial &right) {
    if ( this->degree == -1 ) {
        return *this;
    }
    if ( right.degree == -1 ) {
        throw DivideByZero();
    }
    return *this = (EuclideanDivision(*this, right)).remainder;
}

// adds two polynomials
Polynomial Polynomial::operator+(const Polynomial &right) {
    Polynomial result(*this);
    return result += right;
}

// multiplies two polynomials
Polynomial Polynomial::operator*(const Polynomial &right) {
    Polynomial result(*this);
    return result *= right;
}

// subtracts two polynomials
Polynomial Polynomial::operator-(const Polynomial &right) {
    Polynomial result(*this);
    return result -= right;
}

// divides two polynomials and returns the quotient
Polynomial Polynomial::operator/(const Polynomial &right) {
    Polynomial result(*this);
    return result /= right;
}

// divides two polynomials and returns the remainder
Polynomial Polynomial::operator%(const Polynomial &right) {
    Polynomial result(*this);
    return result %= right;
}

// accesses and/or mutates coefficient of degree index.
// throws OutOfRange exception when an attempt is made to access a section of
// memory that is not part of the array or contains garbage
double& Polynomial::operator[](int index) {
    if ( index >= 0 && index <= degree ) {
        return coefficients[index];
    }
    else {
        throw OutOfRange();
    }
}

// determines whether two polynomials have the same degrees and coefficients.
// polynomials are equal iff their degrees are equal and each coefficient of
// the same degree is equal
bool Polynomial::operator==(const Polynomial &right) {
    if ( this->degree != right.degree ) {
        return false;
    }
    for ( int i = 0; i <= this->degree; i++ ) {
        if ( this->coefficients[i] != right.coefficients[i] ) {
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
        if ( this->coefficients[i] != right.coefficients[i] ) {
            return true;
        }
    }
    return false;
}

// outputs a polynomial as a line
std::ostream& operator<<(std::ostream &out, Polynomial &poly) {
    out << poly.degree << "\t";
    for ( int i = 0; i <= poly.degree; i++ ) {
        out << " " << poly[i];
    }
    out << std::endl;
    return out;
}

/************************
 * mutators and accessors
 ************************/

// changes the degree and adjusts the coefficients accordingly
// throws NoMemory exception
void Polynomial::setDegree(int deg) {
    if ( deg != degree ) {
        double* temp = coefficients;
        int old_scale = memory_scale;
        if ( degree == -1 ) {
            memory_scale = 1;
        }
        while ( memory_scale > 0 && memory_scale*BASE < deg+1 ) {
            memory_scale *= 2;
        }
        while ( memory_scale > 0 && (deg < 0 ||
                (deg > 6 && deg+1 < (memory_scale*BASE)/4)) ) {
            memory_scale /= 2;
        }
        if ( memory_scale != old_scale ) {
            if ( memory_scale > 0 ) {
                try {
                    coefficients = new double[memory_scale*BASE];
                }
                catch (std::bad_alloc) {
                    throw NoMemory();
                }
                if ( degree < deg ) {
                    for ( int i = 0; i <= degree; i++ ) {
                        coefficients[i] = temp[i];
                    }
                    for ( int i = degree+1; i < deg; i++ ) {
                        coefficients[i] = 0;
                    }
                    coefficients[deg] = 1;
                }
                else {
                    for ( int i = 0; i <= deg; i++ ) {
                        coefficients[i] = temp[i];
                    }
                }
            }
            else {
                coefficients = NULL;
            }
            if ( !temp ) {
                delete [] temp;
            }
        }
        degree = deg;
    }
}

// DEPRECATED: sets coefficient of the term with degree index to value
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

// DEPRECATED: fetches coefficent of the term with degree index
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

// evaluate polynomial at a point via Horner's method,
// i.e., ax^3 + bx^2 + cx + d = ((ax + b)x + c)x + d .
// this method reduces error in evaluating a polynomial at a real point
double Polynomial::evaluate(const double point) {
    double result = this->coefficients[this->degree];
    for ( int i = this->degree - 1; i >= 0; i-- ) {
        result *= point;
        result += this->coefficients[i];
    }
    return result;
}

// divides two polynomials to obtain a Euclid pair.
// this function is private because no check is made for the zero polynomial
EuclidPair Polynomial::EuclideanDivision(const Polynomial &left,
                                         const Polynomial &right) {
    // if the dividend has a lesser degree than the divisor then we know that
    // left = 0*right + left, with the remainder having a lesser degree than
    // the divisor
    EuclidPair result;
    if ( left.degree < right.degree ) {
        result.remainder = left;
        return result;
    }
    else {
        int index = left.degree - right.degree;
        result.quotient.setDegree(index);
        Polynomial dividend(left), divisor(right);
        // this loop performs long division on two polynomial
        while ( dividend.degree >= divisor.degree ) {
            result.quotient[index] = 
                dividend[dividend.degree]/divisor[divisor.degree];
            divisor = result.quotient.subterm(index) * divisor;
            dividend = dividend - divisor;
            divisor = right;
            // if ( dividend.degree - divisor.degree == index ) error;
            index = dividend.degree - divisor.degree;
        }
        result.remainder = dividend;
        return result;
    }
}

// creates a polynomial identical to a single term of the caller,
// i.e., (ax^2+bx+c).subterm(1) = bx
Polynomial Polynomial::subterm(int degree) {
    Polynomial result(degree);
    result.coefficients[degree] = this->coefficients[degree];
    return result;
}

// resets the degree if leading coefficients are 0
void Polynomial::simplify() {
    int index = this->degree;
    while ( index >= 0 && this->coefficients[index] == 0  ) {
        index--;
    }
    this->setDegree(index);
}
