#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

/* TODO:
	ERROR CHECKING:
		overflow/underflow
	dynamic memory optimization
*/

struct EuclidPair;

class Polynomial {
private:
	int degree;
	double* coefficients;
	// private functions
	EuclidPair EuclideanDivision(const Polynomial &, const Polynomial &);
	Polynomial subterm(int);
	void clean();
public:
	// {con,de}structor(s)
	Polynomial();
	Polynomial(const int);
	Polynomial(const int, double*);
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
	Polynomial operator=(const Polynomial &);
	Polynomial operator+(const Polynomial &);
	Polynomial operator-(const Polynomial &);
	Polynomial operator*(const Polynomial &);
	Polynomial operator/(const Polynomial &);
	Polynomial operator%(const Polynomial &);
	double& operator[](int);
	bool operator==(const Polynomial &);
	bool operator!=(const Polynomial &);
	// mutators and accessors
	void setDegree(int);
	void setCoefficient(int, double);
	int getDegree();
	double getCoefficient(int);
	// miscellaneous functions
	double evaluate(const double);
	// friends
	friend std::ostream& operator<<(std::ostream &, Polynomial &);
};

struct EuclidPair {
	Polynomial quotient;
	Polynomial remainder;
};

#endif
