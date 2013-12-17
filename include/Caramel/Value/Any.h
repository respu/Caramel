// Caramel C++ Library - Value Facility - Any Header

#ifndef __CARAMEL_VALUE_ANY_H
#define __CARAMEL_VALUE_ANY_H
#pragma once

#include <Caramel/Caramel.h>
#include <Caramel/Value/Detail/AnyHolders.h>


namespace Caramel
{

///////////////////////////////////////////////////////////////////////////////
//
// Any
//

class Any
{
public:

    Any() {}

    template< typename T >
    Any( const T& value );

    
    /// Retrieve Value ///

    template< typename T >
    T As() const;


    /// Properties ///

    Bool IsEmpty() const { return ! m_holder; }


private:
    std::shared_ptr< Detail::AnyHolder > m_holder;
        
};


///////////////////////////////////////////////////////////////////////////////
//
// Implementation
//

template< typename T >
inline Any::Any( const T& value )
    : m_holder( new Detail::AnyHolderSelect< T >::Type( value ))
{
}


template< typename T >
inline T Any::As() const
{
    typedef Detail::AnyHolderSelect< T >::Type HolderType;
    return HolderType::CastTo< T >( m_holder.get() );
}


///////////////////////////////////////////////////////////////////////////////

} // namespace Caramel

#endif // __CARAMEL_VALUE_ANY_H
