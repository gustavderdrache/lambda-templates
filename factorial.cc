#include <iostream>

#include "lambda-calculus.hh"

#define VAR(name) \
    struct name; \
    template<> \
    struct repr<name> { \
        std::string operator()() \
        { \
            return #name; \
        } \
    }

VAR(a);
VAR(b);
VAR(f);
VAR(g);
VAR(k);
VAR(m);
VAR(n);
VAR(p);
VAR(q);
VAR(v);
VAR(x);
VAR(y);
VAR(z);

// Church Numerals
template <int N>
struct church_guts {
    typedef typename church_guts<N - 1>::result _result;
    typedef application<f, _result> result;
};

template <>
struct church_guts<0> {
    typedef x result;
};

template <int N>
struct church_numeral : public abstraction_list<f, x, typename church_guts<N>::result> {};

typedef church_numeral<0>::result ZERO;
typedef church_numeral<1>::result ONE;

typedef abstraction_list<x, y, x>::result TRUE;
typedef abstraction_list<x, y, y>::result FALSE;
typedef abstraction_list<p, a, b, application_list<p, a, b>::result>::result IF;

typedef abstraction<n, application_list<n, abstraction<x, FALSE>, TRUE>::result> ISZERO;

typedef abstraction_list<n, f, x, application<f, application_list<n, f, x>::result>>::result SUCC;
typedef abstraction_list<m, n, application_list<m, SUCC, n>::result>::result PLUS;
typedef abstraction_list<m, n, f, application<m, application<n, f>>>::result MULT;

typedef abstraction<n,
          application_list<n,
            abstraction_list<g, k,
              application_list<
                ISZERO,
                application<g, ONE>,
                k,
                application_list<PLUS, application<g, k>, ONE>::result
              >::result>::result,
              abstraction<v, ZERO>, ZERO>::result> PRED;

typedef abstraction_list<m, n, application_list<n, PRED, m>::result>::result SUB;

typedef abstraction_list<f, n,
        application_list<IF,
            application<ISZERO, n>,
            ONE,
            application_list<MULT, n, application<f, application<PRED, n>>>::result
        >::result
    >::result FACT;

typedef abstraction<f, application<
    abstraction<x, application<f, application<x, x>>>,
    abstraction<x, application<f, application<x, x>>>>> FIX;

typedef application<FIX, FACT> FACTORIAL;

int main()
{
    // clang++ and g++ both give up after church_numeral<5>, so use something safe(ish)
    typedef application<FACTORIAL, church_numeral<3>::result> EXPR;
    typedef evaluate<EXPR>::result expr;

    std::cout << repr<EXPR>()() << std::endl;
    std::cout << repr<expr>()() << std::endl;
}
