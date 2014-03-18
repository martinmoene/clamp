// Copyright (C) 2014 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea and interface is based on the Boost.Algorithm library
// authored by Marshall Clow.

#ifndef CLAMP_H_INCLUDED
#define CLAMP_H_INCLUDED

#include <algorithm>
#include <functional>
#include <iterator>

#include <cassert>

//
// emulate C++14 std::less<> if necessary (constexpr):
//
// see 20.9.5 Comparisons [comparisons] of
// http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3797.pdf
//
#if __cplusplus == 201103L
namespace std14 {

template<class T = void>
struct less
{
    constexpr bool operator()( const T& x, const T& y ) const
    {
        return x < y;
    }

    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
};

template <>
struct less<void>
{
    template <class T, class U>
    constexpr auto operator()(T&& t, U&& u) const
    -> decltype( std::forward<T>(t) < std::forward<U>(u) )
    {
        return t < u;
    }

    // typedef unspecified is_transparent;
};

}

#else // __cplusplus

#include <functional>
namespace std14 {
    using std::less;
}

#endif // __cplusplus

// ---------------------------------------------------------------------------
// Interface

// clamp value per predicate, default std::less<>:

template<class T, class Compare = std14::less<>>
constexpr const T& clamp( const T& val, const T& lo, const T& hi, Compare comp = Compare() );

// Boost also contains clamp_range():

// clamp range of values per predicate, default std::less<>:

template<class InputIterator, class OutputIterator, class Compare = std14::less<>>
OutputIterator clamp_range( InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type const& lo,
    typename std::iterator_traits<InputIterator>::value_type const& hi, Compare comp = Compare() );

// ---------------------------------------------------------------------------
// Possible implementation:

template<class T, class Compare>
constexpr const T& clamp( const T& val, const T& lo, const T& hi, Compare comp )
{
    return assert( !comp(hi, lo) ),
        comp(val, lo) ? lo : comp(hi, val) ? hi : val;
}

// clamp range of values per predicate:

template<class InputIterator, class OutputIterator, class Compare>
OutputIterator clamp_range(
    InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type const& lo,
    typename std::iterator_traits<InputIterator>::value_type const& hi, Compare comp )
{
    using arg_type = decltype(lo);

    return std::transform(
        first, last, out, [&](arg_type val) -> arg_type { return clamp(val, lo, hi, comp); } );
}

#endif // CLAMP_H_INCLUDED

// end of file
