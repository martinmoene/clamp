// Copyright (C) 2014 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// The idea and interface is based on the Boost.Algorithm library
// authored by Marshall Clow.

#include <algorithm>
#include <functional>
#include <iterator>

//
// emulate C++14 std::less<> if necessary (constexpr):
//
#if __cplusplus == 201103L
namespace std14 {

template<class T>
struct less
{
    constexpr bool operator()( const T& lhs, const T& rhs ) const
    {
        return lhs < rhs;
    }
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

// Boost also contains clamp_range(), here called clamp():

// clamp range of values:

template<class InputIterator, class OutputIterator>
OutputIterator clamp( InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type lo,
    typename std::iterator_traits<InputIterator>::value_type hi );

// clamp range of values per predicate:

template<class InputIterator, class OutputIterator, class Compare>
OutputIterator clamp( InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type lo,
    typename std::iterator_traits<InputIterator>::value_type hi, Compare comp );

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
OutputIterator clamp(
    InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type lo,
    typename std::iterator_traits<InputIterator>::value_type hi )
{
    using value_type = decltype(lo);

    return std::transform(
        first, last, out, [&](value_type& val){ return clamp(val, lo, hi); } );
}

// clamp range of values per predicate:

template<class InputIterator, class OutputIterator, class Compare>
OutputIterator clamp(
    InputIterator first, InputIterator last, OutputIterator out,
    typename std::iterator_traits<InputIterator>::value_type lo,
    typename std::iterator_traits<InputIterator>::value_type hi, Compare comp )
{
    using value_type = decltype(lo);

    return std::transform(
        first, last, out, [&](value_type& val){ return clamp(val, lo, hi, comp); } );
}

// end of file
