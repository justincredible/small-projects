#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

/* TODO:
    ERROR CHECKING:
        overflow/underflow
    operators
    run code through lint
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
    Polynomial operator+=(const Polynomial &);
    Polynomial operator-=(const Polynomial &);
    Polynomial operator*=(const Polynomial &);
    Polynomial operator/=(const Polynomial &);
    Polynomial operator%=(const Polynomial &);
    friend std::ostream& operator<<(std::ostream &, Polynomial &);
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
};

struct EuclidPair {
    Polynomial quotient;
    Polynomial remainder;
};

#endif
