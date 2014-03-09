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
};

template <>
struct less<void>
{
    template <class T, class U>
    auto operator()(T&& t, U&& u) const
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

// clamp value:

template<class T>
constexpr const T& clamp( const T& val, const T& lo, const T& hi );

// clamp value per predicate:

template<class T, class Compare>
constexpr const T& clamp( const T& val, const T& lo, const T& hi, Compare comp );

// Boost also contains clamp_range():

// clamp range of values:

template<class InputIterator, class OutputIterator>
OutputIterator clamp_range( InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type const& lo,
    typename std::iterator_traits<InputIterator>::value_type const& hi );

// clamp range of values per predicate:

template<class InputIterator, class OutputIterator, class Compare>
OutputIterator clamp_range( InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type const& lo,
    typename std::iterator_traits<InputIterator>::value_type const& hi, Compare comp );

// ---------------------------------------------------------------------------
// Possible implementation:

template<class T>
constexpr const T& clamp( const T& val, const T& lo, const T& hi )
{
    return clamp( val, lo, hi, std14::less<T>() );
}

template<class T, class Compare>
constexpr const T& clamp( const T& val, const T& lo, const T& hi, Compare comp )
{
    return comp(val, lo) ? lo : comp(hi, val) ? hi : val;
}

template<class InputIterator, class OutputIterator>
OutputIterator clamp_range(
    InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type const& lo,
    typename std::iterator_traits<InputIterator>::value_type const& hi )
{
    return clamp_range(first, last, out, lo, hi, std14::less<>());
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
        first, last, out, [&](arg_type val){ return clamp(val, lo, hi, comp); } );
}

#endif // CLAMP_H_INCLUDED

// end of file
