#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

/* TODO:
	ERROR CHECKING:
		overflow/underflow
	euclidean division
	dynamic memory optimization
*/

class Polynomial {
private:
	int degree;
	double* coefficients;
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
	// overloaded operators
	Polynomial operator=(const Polynomial &);
	Polynomial operator+(const Polynomial &);
	Polynomial operator*(const Polynomial &);
	double& operator[](int);
	// mutators and accessors
	void setDegree(int);
	void setCoefficient(int, double);
	int getDegree();
	double getCoefficient(int);
	// miscellaneous functions
	double evaluate(const double);
};

#endif
