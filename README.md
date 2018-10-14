# Expression templates

This is a somewhat elaborate example of what expression templates can do in C++. The idea is to encode algebraic expressions in types and let the compiler do all of the manipulation. Then at run-time only the code to evaluate the simplified expression has to run. Values for the variables used in the expression are passed in a simple array of doubles.

### Templated structures as functions

Templated `struct`s in C++ can be used almost as a functional language. A simple example to determine whether two types are the same is shown here.

```c++
struct True {};  // type to encode true state of boolean
struct False {}; // same but for false

template <typename LHS, typename RHS>
struct IsSame {
  typedef False Answer;

  static bool answer(void) {
    return false;
  }
};

template <typename E>
struct IsSame<E,E>
{
  typedef True Answer;

  static bool answer(void) {
    return true;
  }
};

int main(void) {
  typedef IsSame<int, int> TestA;
  typedef IsSame<int, double> TestB;

  TestA::answer(); // returns true
  TestB::answer(); // returns false;
}
```


### Simplification rules

The algebraic expressions are simplified by recursively trying simplification rules. An example of such a rule is to replace (A - A) by 0. If expressions currently are not simplified as you want them, other or more rules should be appended. Generating a large and unambiguous set of simplification rules is the trick.


### Derivatives at compile-time

Besides simplification expressions can also be turned into their derivatives, which in turn can be simplified compile-time. This enables us to do all the calculus compile-time and get efficient code to evaluate the expressions at run-time.

### Avoiding unnecessary computations.

The final remaining problem is how to avoid doing unnecessary computations. If in the expression tree there are two subtrees that are exactly the same, the run-time will evaluate that part twice (as compilers are not smart enough yet). This happens especially often when taking derivatives (because of the chain rule). 
