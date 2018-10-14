/* Derivatives are taken recursively in the expression tree. Simplification
should take care of cleaning up the general expressions.
*/

#pragma once

#include "expression.h"
#include "simplify.h"


// an error to denote an expression does not have a derivative in this list
struct DerivativeNotFound {};

// the general derivative operator (should not be encountered!)
// it takes the derivative of expression E with respect to variable D
template <typename E, typename D>
struct Derivative {
  typedef DerivativeNotFound Result;
};

// constant derivative
template <int N, typename D>
struct Derivative<Const<N>, D> {
  typedef Const<0> Result;
};

// variable derivative
template <unsigned int V, typename D>
struct Derivative<Var<V>, D> {
  typedef Const<0> Result;
};

template <unsigned int V>
struct Derivative<Var<V>, Var<V>> {
  typedef Const<1> Result;
};

// number e derivative
template <typename D>
struct Derivative<NumE, D> {
  typedef Const<0> Result;
};

// negation derivative
// - E -> - E'
template <typename E, typename D>
struct Derivative<Neg<E>, D> {
  typedef typename Simplify<
            Neg<
              typename Derivative<E, D>::Result
            >
          >::Result Result;
};

// square root derivative
// sqrt( A ) -> A' / (2 * sqrt( A ))
template <typename E, typename D>
struct Derivative<Sqrt<E>, D> {
  typedef typename Simplify<
            Div<
              typename Derivative<E, D>::Result,
              Mul<
                Const<2>,
                Sqrt<E>
              >
            >
          >::Result Result;
};

// logarithm derivative
// log( A ) -> A' / A
template <typename E, typename D>
struct Derivative<Log<E>, D> {
  typedef typename Simplify<
            Div<
              typename Derivative<E, D>::Result,
              E
            >
          >::Result Result;
};

// addition derivative
// A + B -> A' + B'
template <typename LHS, typename RHS, typename D>
struct Derivative<Add<LHS,RHS>, D> {
  typedef typename Simplify<
            Add<
              typename Derivative<LHS, D>::Result,
              typename Derivative<RHS, D>::Result
            >
          >::Result Result;
};

// subtraction derivative
// A - B -> A' - B'
template <typename LHS, typename RHS, typename D>
struct Derivative<Sub<LHS,RHS>, D> {
  typedef typename Simplify<
            Sub<
              typename Derivative<LHS, D>::Result,
              typename Derivative<RHS, D>::Result
            >
          >::Result Result;
};

// multiplication derivative
// A * B -> A' * B + A * B'
template <typename LHS, typename RHS, typename D>
struct Derivative<Mul<LHS, RHS>, D> {
  typedef typename Simplify<
            Add<
              Mul<
                typename Derivative<LHS, D>::Result,
                RHS
              >,
              Mul<
                LHS,
                typename Derivative<RHS, D>::Result
              >
            >
          >::Result Result;
};

// division derivative
// A / B -> A' / B - A * B' / B ^ 2
template <typename LHS, typename RHS, typename D>
struct Derivative<Div<LHS, RHS>, D> {
  typedef typename Simplify<
            Sub<
              Div<
                typename Derivative<LHS, D>::Result,
                RHS
              >,
              Div<
                Mul<
                  LHS,
                  typename Derivative<RHS, D>::Result
                >,
                Exp<
                  RHS,
                  Const<2>
                >
              >
            >
          >::Result Result;
};

// exponent derivative
// A ^ B -> B * A ^ (B - 1) * A' + A ^ B * log(A) * B'
template <typename LHS, typename RHS, typename D>
struct Derivative<Exp<LHS, RHS>, D> {
  typedef typename Simplify<
            Add<
              Mul<
                Mul<
                  RHS,
                  Exp<
                    LHS,
                    Sub<
                      RHS,
                      Const<1>
                    >
                  >
                >,
                typename Derivative<LHS, D>::Result
              >,
              Mul<
                Mul<
                  Exp<
                    LHS,
                    RHS
                  >,
                  Log<
                    LHS
                  >
                >,
                typename Derivative<RHS, D>::Result
              >
            >
          >::Result Result;
};
