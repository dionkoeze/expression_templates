/* Simplifications of expressions are done recursively. The typedef in the
Simplify struct is the output of the current iteration.

The first declaration of the struct Simplify states the general case. By
template specialization we can then add rules for specific expressions
*/

#pragma once

#include "expression.h"


// helper structure to determine whether two expressions are the same
struct True {};
struct False {};

template <typename E1, typename E2>
struct IsSame {
  typedef False Answer;
};

template <typename E>
struct IsSame<E,E> {
  typedef True Answer;
};


// by default we can just copy the expression
template <typename E>
struct Simplify {
  typedef E Result;
};


// expressions containing a subexpression need to pass the recursion on

// recursion on negation
// when subexpressions change upon simplification the negation also needs to be
// simplified, otherwise not: to avoid infinite recursion
template <typename E, typename Same>
struct NegSimplify {
  typedef typename Simplify<
            Neg<
              typename Simplify<E>::Result
            >
          >::Result Result;
};

template <typename E>
struct NegSimplify<E, True> {
  typedef Neg<E> Result;
};

template <typename E>
struct Simplify<Neg<E>> {
  typedef typename NegSimplify<
            E,
            typename IsSame<E, typename Simplify<E>::Result>::Answer
          >::Result Result;
};

// recursion on square root
// when subexpressions change upon simplification the square root also needs to be
// simplified, otherwise not: to avoid infinite recursion
template <typename E, typename Same>
struct SqrtSimplify {
  typedef typename Simplify<
            Sqrt<
              typename Simplify<E>::Result
            >
          >::Result Result;
};

template <typename E>
struct SqrtSimplify<E, True> {
  typedef Sqrt<E> Result;
};

template <typename E>
struct Simplify<Sqrt<E>> {
  typedef typename SqrtSimplify<
            E,
            typename IsSame<E, typename Simplify<E>::Result>::Answer
          >::Result Result;
};

// recursion on addition
// when subexpressions change upon simplification the addition also needs to be
// simplified, otherwise not: to avoid infinite recursion
template <typename LHS, typename RHS, typename LSame, typename RSame>
struct AddSimplify {
  typedef typename Simplify<
            Add<
              typename Simplify<LHS>::Result,
              typename Simplify<RHS>::Result
            >
          >::Result Result;
};

template <typename LHS, typename RHS>
struct AddSimplify<LHS, RHS, True, True> {
  typedef Add<LHS, RHS> Result;
};

template <typename LHS, typename RHS>
struct Simplify<Add<LHS, RHS>> {
  typedef typename AddSimplify<
            LHS,
            RHS,
            typename IsSame<LHS, typename Simplify<LHS>::Result>::Answer,
            typename IsSame<RHS, typename Simplify<RHS>::Result>::Answer
          >::Result Result;
};

// recursion on subtraction
// when subexpressions change upon simplification the subtraction also needs to be
// simplified, otherwise not: to avoid infinite recursion
template <typename LHS, typename RHS, typename LSame, typename RSame>
struct SubSimplify {
  typedef typename Simplify<
            Sub<
              typename Simplify<LHS>::Result,
              typename Simplify<RHS>::Result
            >
          >::Result Result;
};

template <typename LHS, typename RHS>
struct SubSimplify<LHS, RHS, True, True> {
  typedef Sub<LHS, RHS> Result;
};

template <typename LHS, typename RHS>
struct Simplify<Sub<LHS, RHS>> {
  typedef typename SubSimplify<
            LHS,
            RHS,
            typename IsSame<LHS, typename Simplify<LHS>::Result>::Answer,
            typename IsSame<RHS, typename Simplify<RHS>::Result>::Answer
          >::Result Result;
};

// recursion on multiplication
// when subexpressions change upon simplification the multiplication also needs to be
// simplified, otherwise not: to avoid infinite recursion
template <typename LHS, typename RHS, typename LSame, typename RSame>
struct MulSimplify {
  typedef typename Simplify<
            Mul<
              typename Simplify<LHS>::Result,
              typename Simplify<RHS>::Result
            >
          >::Result Result;
};

template <typename LHS, typename RHS>
struct MulSimplify<LHS, RHS, True, True> {
  typedef Mul<LHS, RHS> Result;
};

template <typename LHS, typename RHS>
struct Simplify<Mul<LHS, RHS>> {
  typedef typename MulSimplify<
            LHS,
            RHS,
            typename IsSame<LHS, typename Simplify<LHS>::Result>::Answer,
            typename IsSame<RHS, typename Simplify<RHS>::Result>::Answer
          >::Result Result;
};

// recursion on division
// when subexpressions change upon simplification the division also needs to be
// simplified, otherwise not: to avoid infinite recursion
template <typename LHS, typename RHS, typename LSame, typename RSame>
struct DivSimplify {
  typedef typename Simplify<
            Div<
              typename Simplify<LHS>::Result,
              typename Simplify<RHS>::Result
            >
          >::Result Result;
};

template <typename LHS, typename RHS>
struct DivSimplify<LHS, RHS, True, True> {
  typedef Div<LHS, RHS> Result;
};

template <typename LHS, typename RHS>
struct Simplify<Div<LHS, RHS>> {
  typedef typename DivSimplify<
            LHS,
            RHS,
            typename IsSame<LHS, typename Simplify<LHS>::Result>::Answer,
            typename IsSame<RHS, typename Simplify<RHS>::Result>::Answer
          >::Result Result;
};

// recursion on exponent
// when subexpressions change upon simplification the exponent also needs to be
// simplified, otherwise not: to avoid infinite recursion
template <typename LHS, typename RHS, typename LSame, typename RSame>
struct ExpSimplify {
  typedef typename Simplify<
            Exp<
              typename Simplify<LHS>::Result,
              typename Simplify<RHS>::Result
            >
          >::Result Result;
};

template <typename LHS, typename RHS>
struct ExpSimplify<LHS, RHS, True, True> {
  typedef Exp<LHS, RHS> Result;
};

template <typename LHS, typename RHS>
struct Simplify<Exp<LHS, RHS>> {
  typedef typename ExpSimplify<
            LHS,
            RHS,
            typename IsSame<LHS, typename Simplify<LHS>::Result>::Answer,
            typename IsSame<RHS, typename Simplify<RHS>::Result>::Answer
          >::Result Result;
};


// here the simplification rules start

// - N -> (-N)
template <int N>
struct Simplify<Neg<Const<N>>> {
  typedef Const<-N> Result;
};

// - (-E) -> E
template <typename E>
struct Simplify<Neg<Neg<E>>> {
  typedef typename Simplify<E>::Result Result;
};

// E + E -> 2 * E
template <typename E>
struct Simplify<Add<E,E>> {
  typedef typename Simplify<
            Mul<
              Const<2>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// E + 0 -> E
template <typename E>
struct Simplify<Add<E,Const<0>>> {
  typedef typename Simplify<E>::Result Result;
};

// 0 + E -> E
template <typename E>
struct Simplify<Add<Const<0>, E>> {
  typedef typename Simplify<E>::Result Result;
};

// E + - E -> 0
template <typename E>
struct Simplify<Add<E, Neg<E>>> {
  typedef Const<0> Result;
};

// A + (- B) -> A - B
template <typename LHS, typename RHS>
struct Simplify<Add<LHS, Neg<RHS>>> {
  typedef typename Simplify<
            Sub<
              typename Simplify<LHS>::Result,
              typename Simplify<RHS>::Result
            >
          >::Result Result;
};

// (A - B) + (B - A) -> 0
template <typename LHS, typename RHS>
struct Simplify<Add<Sub<LHS, RHS>, Sub<RHS, LHS>>> {
  typedef Const<0> Result;
};

// E + N -> N + E
template <int N, typename E>
struct Simplify<Add<E, Const<N>>> {
  typedef typename Simplify<
            Add<
              Const<N>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// N + (M + E) -> (N+M) + E
template <int N, int M, typename E>
struct Simplify<Add<Const<N>, Add<Const<M>, E>>> {
  typedef typename Simplify<
            Add<
              Const<N+M>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// (N * E) + (M * E) -> (N+M) * E
template <int N, int M, typename E>
struct Simplify<Add<Mul<Const<N>, E>, Mul<Const<M>, E>>> {
  typedef typename Simplify<
            Mul<
              Const<N+M>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// (N * E) + (N * E) -> (N+N) * E
// this specialization is to avoid ambiguity
template <int N, typename E>
struct Simplify<Add<Mul<Const<N>, E>, Mul<Const<N>, E>>> {
  typedef typename Simplify<
            Mul<
              Const<N+N>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// N + M -> (N+M)
template <int N, int M>
struct Simplify<Add<Const<N>, Const<M>>> {
  typedef Const<N+M> Result;
};

// 0 + 0 -> 0
// this specialization is to avoid ambiguity
template <>
struct Simplify<Add<Const<0>, Const<0>>> {
  typedef Const<0> Result;
};

// 0 + N -> N
// this specialization is to avoid ambiguity
template <int N>
struct Simplify<Add<Const<0>, Const<N>>> {
  typedef Const<N> Result;
};

// N + 0 -> N
// this specialization is to avoid ambiguity
template <int N>
struct Simplify<Add<Const<N>, Const<0>>> {
  typedef Const<N> Result;
};

// E - E -> 0
template <typename E>
struct Simplify<Sub<E, E>> {
  typedef Const<0> Result;
};

// (-E) - E -> - (2 * E)
template <typename E>
struct Simplify<Sub<Neg<E>, E>> {
  typedef typename Simplify<
            Neg<
              typename Simplify<
                Mul<
                  Const<2>,
                  typename Simplify<E>::Result
                >
              >::Result
            >
          >::Result Result;
};

// E - 0 -> E
template <typename E>
struct Simplify<Sub<E, Const<0>>> {
  typedef typename Simplify<E>::Result Result;
};

// 0 - E -> -E
template <typename E>
struct Simplify<Sub<Const<0>, E>> {
  typedef Neg<typename Simplify<E>::Result> Result;
};

// N - M -> (N-M)
template <int N, int M>
struct Simplify<Sub<Const<N>, Const<M>>> {
  typedef Const<N-M> Result;
};

// 0 - N -> (-N)
// this specialization is to avoid ambiguity
template <int N>
struct Simplify<Sub<Const<0>, Const<N>>> {
  typedef Const<-N> Result;
};

// N - 0 -> N
// this specialization is to avoid ambiguity
template <int N>
struct Simplify<Sub<Const<N>, Const<0>>> {
  typedef Const<N> Result;
};

// 0 - 0 -> 0
// this specialization is to avoid ambiguity
template <>
struct Simplify<Sub<Const<0>, Const<0>>> {
  typedef Const<0> Result;
};

// E * E -> E ^ 2
template <typename E>
struct Simplify<Mul<E, E>> {
  typedef typename Exp<
            typename Simplify<E>::Result,
            Const<2>
          >::Result Result;
};

// E * (E ^ N) -> E ^ (N+1)
template <int N, typename E>
struct Simplify<Mul<E, Exp<E, Const<N>>>> {
  typedef typename Exp<
            typename Simplify<E>::Result,
            Const<N+1>
          >::Result Result;
};

// (E ^ N) * E -> E ^ (N+1)
template <int N, typename E>
struct Simplify<Mul<Exp<E, Const<N>>, E>> {
  typedef typename Simplify<
            Exp<
              typename Simplify<E>::Result,
              Const<N+1>
            >
          >::Result Result;
};

// E * N -> N * E
template <int N, typename E>
struct Simplify<Mul<E, Const<N>>> {
  typedef typename Simplify<
            Mul<
              Const<N>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// - (N * E) -> (-N) * E
template <int N, typename E>
struct Simplify<Neg<Mul<Const<N>, E>>> {
  typedef typename Simplify<
            Mul<
              Const<-N>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// N * (M * E) -> (N*M) * E
template <int N, int M, typename E>
struct Simplify<Mul<Const<N>, Mul<Const<M>,E>>> {
  typedef typename Simplify<
            Mul<
              Const<N*M>,
              typename Simplify<E>::Result
            >
          >::Result Result;
};

// N * (M + E) -> (N*M) + N * E
template <int N, int M, typename E>
struct Simplify<Mul<Const<N>, Add<Const<M>, E>>> {
  typedef typename Simplify<
            Add<
              Const<N*M>,
              Mul<
                Const<N>,
                typename Simplify<E>::Result
              >
            >
          >::Result Result;
};

// (N * A) * (M * B) -> (N*M) * (A * B)
template <int N, int M, typename E1, typename E2>
struct Simplify<Mul<Mul<Const<N>,E1>,Mul<Const<M>,E2>>> {
  typedef typename Simplify<
            Mul<
              Const<N*M>,
              typename Simplify<
                Mul<E1, E2>
              >::Result
            >
          >::Result Result;
};

// (N * A) * (N * B) -> (N*N) * (A * B)
// this template specialization is to avoid ambiguity
template <int N, typename E1, typename E2>
struct Simplify<Mul<Mul<Const<N>,E1>,Mul<Const<N>,E2>>> {
  typedef typename Simplify<
            Mul<
              Const<N*N>,
              typename Simplify<
                Mul<E1, E2>
              >::Result
            >
          >::Result Result;
};

// (N * E) * (N * E) -> (N*N) * (E ^ 2)
// this template specialization is to avoid ambiguity
template <int N, typename E>
struct Simplify<Mul<Mul<Const<N>,E>,Mul<Const<N>,E>>> {
  typedef typename Simplify<
            Mul<
              Const<N*N>,
              typename Simplify<
                Exp<E, Const<2>>
              >::Result
            >
          >::Result Result;
};

// E * 1 -> E
template <typename E>
struct Simplify<Mul<E, Const<1>>> {
  typedef typename Simplify<E>::Result Result;
};

// 1 * E -> E
template <typename E>
struct Simplify<Mul<Const<1>, E>> {
  typedef typename Simplify<E>::Result Result;
};

// E * 0 -> 0
template <typename E>
struct Simplify<Mul<E, Const<0>>> {
  typedef Const<0> Result;
};

// 0 * E -> 0
template <typename E>
struct Simplify<Mul<Const<0>, E>> {
  typedef Const<0> Result;
};


// N * M -> (N*M)
template <int N, int M>
struct Simplify<Mul<Const<N>, Const<M>>> {
  typedef Const<N*M> Result;
};

// N * 1 -> N
template <int N>
struct Simplify<Mul<Const<N>, Const<1>>> {
  typedef Const<N> Result;
};

// 1 * N -> N
template <int N>
struct Simplify<Mul<Const<1>, Const<N>>> {
  typedef Const<N> Result;
};

// N * 0 -> 0
template <int N>
struct Simplify<Mul<Const<N>, Const<0>>> {
  typedef Const<0> Result;
};

// 0 * N -> 0
template <int N>
struct Simplify<Mul<Const<0>, Const<N>>> {
  typedef Const<0> Result;
};

// 0 * 0 -> 0
template <>
struct Simplify<Mul<Const<0>, Const<0>>> {
  typedef Const<0> Result;
};

// 1 * 1 -> 1
template <>
struct Simplify<Mul<Const<1>, Const<1>>> {
  typedef Const<1> Result;
};

// 0 * 1 -> 0
template <>
struct Simplify<Mul<Const<0>, Const<1>>> {
  typedef Const<0> Result;
};

// 1 * 0 -> 0
template <>
struct Simplify<Mul<Const<1>, Const<0>>> {
  typedef Const<0> Result;
};

// E / 1 -> E
template <typename E>
struct Simplify<Div<E, Const<1>>> {
  typedef typename Simplify<E>::Result Result;
};

// 0 / E -> 0
template <typename E>
struct Simplify<Div<Const<0>, E>> {
  typedef Const<0> Result;
};

// E ^ 0 -> 1
template <typename E>
struct Simplify<Exp<E, Const<0>>> {
  typedef Const<1> Result;
};

// E ^ 1 -> E
template <typename E>
struct Simplify<Exp<E, Const<1>>> {
  typedef typename Simplify<E>::Result Result;
};

// function to compute power of integers
constexpr unsigned int cpow(unsigned int b, unsigned int e)
{
  return e == 0 ? 1 : b * pow(b, e-1);
};

// N ^ M -> (N^M)
template <int N, int M>
struct Simplify<Exp<Const<N>, Const<M>>> {
  typedef Const<cpow(N,M)> Result;
};

// N ^ 0 -> 1
template <int N>
struct Simplify<Exp<Const<N>, Const<0>>> {
  typedef Const<1> Result;
};

// N ^ 1 -> N
template <int N>
struct Simplify<Exp<Const<N>, Const<1>>> {
  typedef Const<N> Result;
};

// sqrt( 0 ) -> 0
template <>
struct Simplify<Sqrt<Const<0>>> {
  typedef Const<0> Result;
};

// sqrt( 1 ) -> 1
template <>
struct Simplify<Sqrt<Const<1>>> {
  typedef Const<1> Result;
};

// sqrt( 4 ) -> 2
template <>
struct Simplify<Sqrt<Const<4>>> {
  typedef Const<2> Result;
};

// sqrt( 9 ) -> 3
template <>
struct Simplify<Sqrt<Const<9>>> {
  typedef Const<3> Result;
};

// sqrt( 16 ) -> 4
template <>
struct Simplify<Sqrt<Const<16>>> {
  typedef Const<4> Result;
};

// sqrt( 25 ) -> 5
template <>
struct Simplify<Sqrt<Const<25>>> {
  typedef Const<5> Result;
};

// sqrt( E ^ 2 ) -> E (note that we pick the positive branch only)
template <typename E>
struct Simplify<Sqrt<Exp<E, Const<2>>>> {
  typedef typename Simplify<E>::Result Result;
};

// log 1 -> 0
template <>
struct Simplify<Log<Const<0>>> {
  typedef Const<0> Result;
};

// log e -> 1
template <>
struct Simplify<Log<NumE>> {
  typedef Const<1> Result;
};

// log( E ^ N) -> N * log( E )
template <int N, typename E>
struct Simplify<Log<Exp<E, Const<N>>>> {
  typedef typename Simplify<
            Mul<
              Const<N>,
              Log<
                typename Simplify<E>::Result
              >
            >
          >::Result Result;
};
