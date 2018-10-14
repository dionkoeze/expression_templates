// This file contains some examples of how expression templates can be used.

#include "expression.h"
#include "simplify.h"
#include "derivative.h"

#include <iostream>


// A little helper class to not forget to free the memory where
// variables are stored
struct DeferDelete {
  double *mem;

  DeferDelete(double *mem) : mem(mem) {}

  ~DeferDelete(void) {
    delete[] mem;
  }
};


int main(void) {

  // Reserve some memory where the put the variable values
  double *args = new double[VARS_count];
  DeferDelete defer(args);

  // Let's give the variables we have some values
  args[VARS_x] = 1.;
  args[VARS_y] = 2.;
  args[VARS_z] = 3.;

  // Write them to std out to remember them
  std::cout << "x = " << args[VARS_x] << ", y = " << args[VARS_y] << ", z = " << args[VARS_z] << std::endl;
  std::cout << "---" << std::endl;


  // Let's first see how we define an expression, let's take 2x + 4y
  typedef Add<
            Mul<Const<2>, Var<VARS_x>>
          ,
            Mul<Const<4>, Var<VARS_y>>
          > Expr1;

  // Simplify the expression
  typedef typename Simplify<Expr1>::Result Expr1Simp;

  // Check the outcome
  std::cout << "Input:      " << Expr1::toString() << std::endl;
  std::cout << "Simplified: " << Expr1Simp::toString() << std::endl;
  std::cout << "Evaluated:  " << Expr1Simp::eval(args) << std::endl;
  std::cout << "---" << std::endl;


  // Now take a simple expression that can be simplified: 2x * 2x + x
  typedef Add<
            Mul<
              Mul<
                Const<2>,
                Var<VARS_x>
              >,
              Mul<
                Const<2>,
                Var<VARS_x>
              >
            >,
            Var<VARS_x>
          > Expr2;

  // Simplify the expression
  typedef typename Simplify<Expr2>::Result Expr2Simp;

  // Check the outcome
  std::cout << "Input:      " << Expr2::toString() << std::endl;
  std::cout << "Simplified: " << Expr2Simp::toString() << std::endl;
  std::cout << "Evaluated:  " << Expr2Simp::eval(args) << std::endl;
  std::cout << "---" << std::endl;


  // Finally let's take a simple polynomial derivative:
  // d/dx (5x^4 + 2x^3 + 6x^2 + x - 5)
  typedef Add<
            Mul<
              Const<5>,
              Exp<
                Var<VARS_x>,
                Const<4>
              >
            >,
            Add<
              Mul<
                Const<2>,
                Exp<
                  Var<VARS_x>,
                  Const<3>
                >
              >,
              Add<
                Mul<
                  Const<6>,
                  Exp<
                    Var<VARS_x>,
                    Const<2>
                  >
                >,
                Add<
                  Var<VARS_x>,
                  Neg<Const<5>>
                >
              >
            >
          > Expr3;

  // Simplify the expression
  typedef typename Simplify<Expr3>::Result Expr3Simp;

  // Take the derivative wrt x
  typedef typename Derivative<Expr3Simp, Var<VARS_x>>::Result Expr3Der;

  // Check the outcome
  std::cout << "Input:      " << Expr3::toString() << std::endl;
  std::cout << "Simplified: " << Expr3Simp::toString() << std::endl;
  std::cout << "Evaluated:  " << Expr3Simp::eval(args) << std::endl;
  std::cout << "Derivative: " << Expr3Der::toString() << std::endl;
  std::cout << "Evaluated:  " << Expr3Der::eval(args) << std::endl;
  std::cout << "---" << std::endl;

}
