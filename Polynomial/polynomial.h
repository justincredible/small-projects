#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

/* TODO:
    ERROR CHECKING:
        overflow/underflow
        constructor with array aregument should ensure intended memory access is valid
    exceptions should provide information
    reduce code for +=
    simplify logic of setDegree()
    run code through lint
*/

/* Polynomial
 ******************************************************************************
 *
 * a class for dealing with finite polynomials over the real numbers. the
 * member variables of this class are two ints and a double pointer. the double
 * pointer is used to store the coefficients of the polynomial in a dynamically
 * allocated array. one int stores the degree of the polynomial, while the
 * other is used internally to manage the memory allocated to the double*
 *
 * Operations:
 *
 * -    instantiation: 
 *          Polynomial a; Polynomial b(i); Polynomial c(i, array, i+1); 
 *          Polynomial d(a);
 *
 *          the default constructor creates a unique polynomial that has not
 *          yet allocated memory for the array coefficients. the second
 *          constructor creates a monic polynomial of degree max(i,-1) with no
 *          other term coefficients. the third constructor is like the second,
 *          but initiliazes the coefficients of the polynomial to match an
 *          array of i+1 elements. the copy constructor is the last of the
 *          constructors
 *
 * -    assignment:
 *          poly0 = poly1;
 *
 *          assignment of one polynomial to another polynomial results in the
 *          caller owning as much memory as the argument, also the degrees and
 *          and valid coefficients are equal
 *
 * -    addition:
 *          poly0 += poly1; poly2 + poly3;
 *
 *          addition with assignment is the logic for both operators, the only
 *          difference is the return type. the degree of the result is less
 *          than or equal to the maximum degree of the operands
 *
 * -    multiplication:
 *          poly0 *= poly1; poly2 * poly3;
 *
 *          multiplication with assignment is the logic for both operators,
 *          each having a different return type. the degree of the result is
 *          equal to the sum of the operands' degrees unless one of the
 *          operands is the zero polynomial
 *
 * -    subtraction:
 *          poly0 -= poly1; poly2 - poly 3;
 *
 *          subtraction is performed via addition with the additive inverse.
 *          thus *= and += are both called
 *
 * -    division:
 *          poly0 /= poly1; poly2 / poly3; poly4 %= poly5; poly6 % poly7;
 *
 *          the division theorem applies to polynomials over the real numbers.
 *          Euclidean division provides a quotient and remainder, which are
 *          also polynomials over the reals with the degree of the remainder
 *          strictly less than the degree of the divisor. only one of these
 *          polynomials can be examined per function call
 *
 * -    member variable access:
 *          polynomial.getDegree(); polynomial.setDegree(i); polynomial[i];
 *
 *          the value of the int variable degree can be fetched or overwritten.
 *          if the value is to be overwritten memory may also be (de)allocated.
 *          the address to the term with coefficient of degree i can be
 *          accesses through the [] operator
 *
 * -    equality testing:
 *          poly0 == poly1; poly2 != poly3
 *
 *          determines whether or not two polynomials have the same degree and
 *          match on each valid coefficient. the <=, >=, <, > operators only
 *          test the degree of the polynomials
 *
 */


const int BASE = 25;

struct EuclidPair;

class Polynomial {
private:
    int degree;
    int memory_scale;
    double* coefficients;
    // private functions
    EuclidPair EuclideanDivision(const Polynomial &, const Polynomial &);
    Polynomial subterm(int);
    void simplify();
public:
    // {con,de}structor(s)
    Polynomial();
    Polynomial(const int);
    Polynomial(const int, double*, const int);
    Polynomial(const Polynomial &);
    ~Polynomial();
    // exception classes
    class OutOfRange {
    };
    class NoMemory {
    };
    class DivideByZero {
    };
    // overloaded operators
    Polynomial& operator=(const Polynomial &);
    Polynomial& operator+=(const Polynomial &);
    Polynomial& operator-=(const Polynomial &);
    Polynomial& operator*=(const Polynomial &);
    Polynomial& operator/=(const Polynomial &);
    Polynomial& operator%=(const Polynomial &);
    Polynomial operator+(const Polynomial &);
    Polynomial operator*(const Polynomial &);
    Polynomial operator-(const Polynomial &);
    Polynomial operator/(const Polynomial &);
    Polynomial operator%(const Polynomial &);
    friend std::ostream& operator<<(std::ostream &, Polynomial &);
    double& operator[](int);
    bool operator==(const Polynomial &);
    bool operator!=(const Polynomial &);
    bool operator<=(const Polynomial &right) { 
        return this->degree <= right.degree; }
    bool operator>=(const Polynomial &right) {
        return this->degree >= right.degree; }
    bool operator<(const Polynomial &right) {
        return this->degree < right.degree; }
    bool operator>(const Polynomial &right) {
        return this->degree > right.degree; }
    // mutators and accessors
    void setDegree(int);
    void setCoefficient(int, double);
    int getDegree();
    double getCoefficient(int);
    // miscellaneous functions
    double evaluate(const double);
};

struct EuclidPair {
    Polynomial quotient;
    Polynomial remainder;
};

#endif
