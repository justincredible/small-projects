#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

class Polynomial {
private:
	int degree;
	double* coefficients;
public:
	// {Con,De}structor(s)
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
	// mutators and accessor functions
	void setDegree(int);
	void setCoefficient(int, double);
	int getDegree();
	double getCoefficient(int);
};

#endif
