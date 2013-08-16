// Caramel C++ Library - String Facility - Char Traits Header

#ifndef __CARAMEL_STRING_CHAR_TRAITS_H
#define __CARAMEL_STRING_CHAR_TRAITS_H

#include <Caramel/Caramel.h>

#if defined( CARAMEL_COMPILER_HAS_PRAGMA_ONCE )
#pragma once
#endif


namespace Caramel
{

///////////////////////////////////////////////////////////////////////////////
//
// Char Traits
//

/// Primary Template ///

template< typename CharT >
struct CharTraits;


/// Char Specialization ///

template<>
struct CharTraits< Char > : public std::char_traits< Char >
{
    typedef Char CharType;

    static const Char DOT_CHAR = '.';
};


/// Wide Char Specialization ///

template<>
struct CharTraits< Wchar > : public std::char_traits< Wchar >
{
    typedef Wchar CharType;

    static const Wchar DOT_CHAR = '.';
};


///////////////////////////////////////////////////////////////////////////////

} // namespace Caramel

#endif // __CARAMEL_STRING_CHAR_TRAITS_H
