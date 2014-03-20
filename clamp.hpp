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

#include "std14.hpp"

#include <iterator>
#include <cassert>

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
