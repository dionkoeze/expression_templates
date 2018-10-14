/* Expression templates

Algebraic expressions are encoded in the type system. The templates of a type
are the subexpressions of that expression.

Algebraic operations are done by template specialization.

*/

#pragma once

#include <string>
#include <cmath>


// all variables that could be used need to be declared here
enum {
  VARS_x,
  VARS_y,
  VARS_z,
  VARS_count
};

// translation of enum to string for pretty printing
std::string varname(unsigned int id) {
  switch(id){
    case VARS_x:
      return "x";
    case VARS_y:
      return "y";
    case VARS_z:
      return "z";
    default:
      return "unknown";
  }
}


// all classes used as expressions are forward declared
template <int> struct Const;
template <unsigned int> struct Var;
struct NumE;

template <typename> struct Neg;
template <typename> struct Sqrt;
template <typename> struct Log;

template <typename, typename> struct Add;
template <typename, typename> struct Sub;
template <typename, typename> struct Mul;
template <typename, typename> struct Div;
template <typename, typename> struct Exp;


// constant
template <int N>
struct Const {
  static double eval(const double *args) {
    return N;
  }

  static std::string toString(void) {
    return std::to_string(N);
  }
};

// variable
template <unsigned int id>
struct Var {
  static double eval(const double *args) {
    return args[id];
  }

  static std::string toString(void) {
    return varname(id);
  }
};

// number e
struct NumE {};

// negation
template <typename E>
struct Neg {
  static double eval(const double *args) {
    return - E::eval(args);
  }

  static std::string toString(void) {
    return "( - " + E::toString() + " )";
  }
};

// square root
template <typename E>
struct Sqrt {
  static double eval(const double *args) {
    return std::sqrt(E::eval(args));
  }

  static std::string toString(void) {
    return "sqrt( " + E::toString() + " )";
  }
};

// natural logarithm
template <typename E>
struct Log {
  static double eval(const double *args) {
    return std::log(E::eval(args));
  }

  static std::string toString(void) {
    return "log( " + E::toString() + " )";
  }
};

// addition
template <typename LHS, typename RHS>
struct Add {
  static double eval(const double *args) {
    return LHS::eval(args) + RHS::eval(args);
  }

  static std::string toString(void) {
    return "( " + LHS::toString() + " + " + RHS::toString() + " )";
  }
};

// subtraction
template <typename LHS, typename RHS>
struct Sub {
  static double eval(const double *args) {
    return LHS::eval(args) - RHS::eval(args);
  }

  static std::string toString(void) {
    return "( " + LHS::toString() + " - " + RHS::toString() + " )";
  }
};

// multiplication
template <typename LHS, typename RHS>
struct Mul {
  static double eval(const double *args) {
    return LHS::eval(args) * RHS::eval(args);
  }

  static std::string toString(void) {
    return "( " + LHS::toString() + " * " + RHS::toString() + " )";
  }
};

// division
template <typename LHS, typename RHS>
struct Div {
  static double eval(const double *args) {
    return LHS::eval(args) / RHS::eval(args);
  }

  static std::string toString(void) {
    return "( " + LHS::toString() + " / " + RHS::toString() + " )";
  }
};

// exponent
template <typename LHS, typename RHS>
struct Exp {
  static double eval(const double *args) {
    return std::pow(LHS::eval(args), RHS::eval(args));
  }

  static std::string toString(void) {
    return "( " + LHS::toString() + " ^ " + RHS::toString() + " )";
  }
};
