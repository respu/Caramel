// Caramel C++ Library - Value Amenity - Shared Array Header

#ifndef __CARAMEL_VALUE_SHARED_ARRAY_H
#define __CARAMEL_VALUE_SHARED_ARRAY_H

#include <Caramel/Caramel.h>

#if defined( CARAMEL_COMPILER_HAS_PRAGMA_ONCE )
#pragma once
#endif

#include <boost/shared_array.hpp>
#include <Caramel/Error/Assert.h>
#include <Caramel/Trace/Trace.h>


namespace Caramel
{

///////////////////////////////////////////////////////////////////////////////
//
// Shared Array
// - A lightweight wrapper for std::shared_array< T >.
//   NOT thread-safe.
//

template< typename T >
class ConstSharedArray
{
public:

    ConstSharedArray();
    explicit ConstSharedArray( Uint size );

    
    /// Accessors ///

    Bool IsEmpty() const { return 0 == m_size; }
    Uint Size()    const { return m_size; }


    typedef T ValueType;

    const ValueType& operator[]( Uint i ) const;
    

    typedef const ValueType* ConstIterator;

    ConstIterator Begin() const { return &m_array[0]; }
    ConstIterator End()   const { return &m_array[ m_size ]; }

    
protected:

    typedef boost::shared_array< T > ArrayType;
    ArrayType m_array;

    Uint m_size;
};


template< typename T >
class SharedArray : public ConstSharedArray< T >
{
public:

    SharedArray();
    explicit SharedArray( Uint size );


    /// Accessors (constant) ///

    using ConstSharedArray< T >::operator[];
    using ConstSharedArray< T >::Begin;
    using ConstSharedArray< T >::End;


    /// Accessors (non-constant) ///

    typedef T ValueType;

    ValueType& operator[]( Uint i );


    typedef ValueType* Iterator;

    Iterator Begin() const { return &m_array[0]; }
    Iterator End()   const { return &m_array[ m_size ]; }


    /// Modifiers ///

    void Reset( Uint size );
};


///////////////////////////////////////////////////////////////////////////////
//
// Implementation
//

//
// Constructors
//

template< typename T >
ConstSharedArray< T >::ConstSharedArray()
    : m_array( new T[1] )  // dummy content
    , m_size( 0 )
{
}


template< typename T >
ConstSharedArray< T >::ConstSharedArray( Uint size )
    : m_size( size )
{
    if ( 0 == size )
    {
        CARAMEL_TRACE_FAIL_HERE( "Size can't be 0" );
        CARAMEL_INVALID_ARGUMENT();
    }

    m_array.reset( new T[size] );
}


template< typename T >
SharedArray< T >::SharedArray()
    : ConstSharedArray< T >()
{
}


template< typename T >
SharedArray< T >::SharedArray ( Uint size )
    : ConstSharedArray< T >( size )
{
}


///////////////////////////////////////////////////////////////////////////////

} // namespace Caramel

#endif // __CARAMEL_VALUE_SHARED_ARRAY_H
