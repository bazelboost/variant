//-----------------------------------------------------------------------------
// boost variant/get.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman, Itay Maman
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_VARIANT_GET_HPP
#define BOOST_VARIANT_GET_HPP

#include "boost/preprocessor/enum_params.hpp"
#include "boost/utility/addressof.hpp"
#include "boost/variant/variant_fwd.hpp"

namespace boost {

//////////////////////////////////////////////////////////////////////////
// class bad_get
//
// The exception thrown in the event of a failed get of a value.
//
class bad_get
    : public std::exception
{
public: // std::exception implementation

    virtual const char * what() const throw()
    {
        return "boost::bad_get: "
               "failed value get using boost::get";
    }

};

//////////////////////////////////////////////////////////////////////////
// function template get<T>
//
// Retrieves content of given variant object if content is of type T.
// Otherwise: pointer ver. returns 0; reference ver. throws bad_get.
//

namespace detail { namespace variant {

// (detail) class template get_visitor
//
// Generic static visitor that: if the value is of the specified type,
// returns a pointer to the value it visits; else a null pointer.
//
template <typename T>
struct get_visitor
{
public: // typedefs

    typedef T* result_type;

public: // visitor interfaces

    template <typename U>
    T* operator()(U&) const
    {
        return static_cast<T*>(0);
    }

    T* operator()(T& operand) const
    {
        return boost::addressof(operand);
    }

};

}} // namespace detail::variant

template <typename U, BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, typename T) >
inline U* get(
      boost::variant< BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, T) >* operand
    )
{
    if (!operand) return static_cast<U*>(0);

    detail::variant::get_visitor<U> v;
    return operand->apply_visitor(v);
}

template <typename U, BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, typename T) >
inline U* get(
      const boost::variant< BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, T) >* operand
    )
{
    if (!operand) return static_cast<U*>(0);

    detail::variant::get_visitor<U> v;
    return operand->apply_visitor(v);
}

template <typename U, BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, typename T) >
inline U& get(
      boost::variant< BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, T) >& operand
    )
{
    U* result = get<U>(&operand);
    if (!result)
        throw bad_get();
    return *result;
}

template <typename U, BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, typename T) >
inline U& get(
      const boost::variant< BOOST_PP_ENUM_PARAMS(BOOST_VARIANT_LIMIT_TYPES, T) >& operand
    )
{
    U* result = get<U>(&operand);
    if (!result)
        throw bad_get();
    return *result;
}

} // namespace boost

#endif // BOOST_VARIANT_GET_HPP
