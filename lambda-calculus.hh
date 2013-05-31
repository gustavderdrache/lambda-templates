#ifndef LAMBDA_CALCULUS
#define LAMBDA_CALCULUS

#include <string>

// Syntax

template <typename Variable, typename Body>
struct abstraction;

template <typename Function, typename Argument>
struct application;

// String Representation

template <typename>
struct repr;

template <typename Variable, typename Body>
struct repr<abstraction<Variable, Body>> {
    std::string operator()()
    {
        return std::string("(\\") + repr<Variable>()() + "." + repr<Body>()() + ")";
    }
};

template <typename Function, typename Argument>
struct repr<application<Function, Argument>> {
    std::string operator()()
    {
        return std::string("(") + repr<Function>()() + " " + repr<Argument>()() + ")";
    }
};

// Construction Utilities

template<typename X, typename Y, typename ...Tail>
struct application_list {
    typedef application<X, Y> Head;

    typedef typename application_list<Head, Tail...>::result result;
};

template<typename X, typename Y>
struct application_list<X, Y> {
    typedef application<X, Y> result;
};

// right fold
// abstraction_list [x] = x
// abstraction_list (x : xs) = abstraction(x, abstraction_list(xs))
template<typename Variable, typename ...Rest>
struct abstraction_list {
    typedef typename abstraction_list<Rest...>::result _result;
    typedef abstraction<Variable, _result> result;
};

template <typename Term>
struct abstraction_list<Term> {
    typedef Term result;
};

// Capture-avoiding substitution

template <typename Term, typename, typename>
struct substitute {
    typedef Term result;
};

template <typename Term, typename Value>
struct substitute<Term, Term, Value> {
    typedef Value result;
};

template <typename Function, typename Argument, typename Term, typename Value>
struct substitute<application<Function, Argument>, Term, Value> {
    typedef typename substitute<Function, Term, Value>::result head;
    typedef typename substitute<Argument, Term, Value>::result tail;

    typedef application<head, tail> result;
};

template <typename Term, typename Body, typename Value>
struct substitute<abstraction<Term, Body>, Term, Value> {
    typedef abstraction<Term, Body> result;
};

template <typename Argument, typename Body, typename Term, typename Value>
struct substitute<abstraction<Argument, Body>, Term, Value> {
    typedef typename substitute<Body, Term, Value>::result _result;
    typedef abstraction<Argument, _result> result;
};

// Beta reduction (single-step)

template <typename Term>
struct reduce {
    typedef Term result;
};

template <typename Parameter, typename Body, typename Argument>
struct reduce<application<abstraction<Parameter, Body>, Argument>> : public substitute<Body, Parameter, Argument>
{
};

template <typename Argument, typename Body>
struct reduce<abstraction<Argument, Body>> {
    typedef typename reduce<Body>::result _result;
    typedef abstraction<Argument, _result> result;
};

template <typename Function, typename Argument>
struct reduce<application<Function, Argument>> {
    typedef typename reduce<Function>::result head;
    typedef typename reduce<Argument>::result tail;

    typedef application<head, tail> result;
};

// Conditionals

template <bool, typename, typename Alternative>
struct when {
    typedef typename Alternative::result result;
};

template <typename Consequent, typename Alternative>
struct when<true, Consequent, Alternative> {
    typedef typename Consequent::result result;
};

// Equality (liberty, brotherhood sold separately)

template <typename, typename>
struct equalp {
    const static bool result = false;
};

template <typename A>
struct equalp<A, A> {
    const static bool result = true;
};

// Reducibility

template <typename Term>
struct reducible {
    const static bool result = false;
};

template <typename Parameter, typename Body, typename Argument>
struct reducible<application<abstraction<Parameter, Body>, Argument>> {
    const static bool result = true;
};

template <typename Function, typename Argument>
struct reducible<application<Function, Argument>> {
    const static bool result = reducible<Function>::result || reducible<Argument>::result;
};

template <typename Argument, typename Body>
struct reducible<abstraction<Argument, Body>> : public reducible<Body> {};

// Unbounded (?) reduction

template <typename Term>
struct evaluate : public reduce<Term> {};

template <typename Argument, typename Body>
struct evaluate<abstraction<Argument, Body>> {
    typedef abstraction<Argument, Body> Term;
    typedef reduce<Term> Reduction;

    typedef typename when<reducible<typename Reduction::result>::result, evaluate<typename Reduction::result>, Reduction>::result result;
};

template <typename Function, typename Argument>
struct evaluate<application<Function, Argument>> {
    typedef application<Function, Argument> Term;
    typedef reduce<Term> Reduction;

    typedef typename when<reducible<typename Reduction::result>::result, evaluate<typename Reduction::result>, Reduction>::result result;
};

#endif
