// Copyright (C) 2014 Martin Moene.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// std14.hpp - emulate C++14 std::equal<>, std::std::less<>

#ifndef STD14_H_INCLUDED
#define STD14_H_INCLUDED

#include <algorithm>

//
// emulate C++14 std::equal<> if necessary (2 x range):
//
#if __cplusplus == 201103L

namespace std14 {

template< class InputIt1, class InputIt2 >
bool equal( InputIt1 first1, InputIt1 last1,
            InputIt2 first2, InputIt2 last2 )
{
    if ( std::distance( first1, last1 ) != std::distance( first2, last2 ) )
        return false;

    for ( ; first1 != last1; ++first1, ++first2 )
    {
        if ( ! (*first1 == *first2) )
        {
            return false;
        }
    }
    return true;
}
} // namespace std14

#else // __cplusplus

#include <functional>
namespace std14 { 
    using std::equal; 
}

#endif // __cplusplus

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

template<class T = void>
struct greater
{
    constexpr bool operator()( const T& x, const T& y ) const
    {
        return x > y;
    }

    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
};

template <>
struct greater<void>
{
    template <class T, class U>
    constexpr auto operator()(T&& t, U&& u) const
    -> decltype( std::forward<T>(t) < std::forward<U>(u) )
    {
        return t > u;
    }

    // typedef unspecified is_transparent;
};

}

#else // __cplusplus

#include <functional>
namespace std14 {
    using std::less;
    using std::greater;
}

#endif // __cplusplus

#endif // STD14_H_INCLUDED
