// Caramel C++ Library - Value Facility - Implementation

#include "CaramelPch.h"

#include "Value/NamedValuesImpl.h"
#include <Caramel/Lexical/Boolean.h>
#include <Caramel/Lexical/Floating.h>
#include <Caramel/Lexical/Integer.h>
#include <Caramel/String/ToString.h>


namespace Caramel
{

//
// Contents
//
//   NamedValues
//   ConstNamedValueRef
//   NamedValueRef
//   NamedValueEntry
//

///////////////////////////////////////////////////////////////////////////////
//
// Named Values
//

NamedValues::NamedValues()
    : m_impl( new NamedValuesImpl )
{
}


void NamedValues::Init()
{
    m_impl.reset( new NamedValuesImpl );
}


//
// Properties
//

Uint NamedValues::Size() const
{
    return static_cast< Uint >( m_impl->m_valueEntries.size() );
}


//
// Accessors
//

Detail::NamedValueRef NamedValues::operator[]( const std::string& name )
{
    Detail::NamedValueEntry* entry = nullptr;

    auto ientry = m_impl->m_valueEntries.find( name );
    if ( m_impl->m_valueEntries.end() != ientry )
    {
        entry = &( ientry->second );
    }

    return Detail::NamedValueRef( this, name, entry );
}


Detail::ConstNamedValueRef NamedValues::operator[]( const std::string& name ) const
{
    auto ientry = m_impl->m_valueEntries.find( name );
    if ( m_impl->m_valueEntries.end() == ientry )
    {
        CARAMEL_THROW( "Value not found, name: %s", name );
    }

    return Detail::ConstNamedValueRef( name, &( ientry->second ));
}


NamedValues::ValueMap NamedValues::GetValueMap() const
{
    ValueMap vmap;

    auto ientry = m_impl->m_valueEntries.begin();
    for ( ; m_impl->m_valueEntries.end() != ientry; ++ ientry )
    {
        const std::string name = ientry->first;
        
        Detail::ConstNamedValueRef valueRef( name, &( ientry->second ));

        vmap.insert( std::make_pair( name, valueRef.AsString() ));
    }

    return vmap;
}


//
// Implementation
//

Detail::NamedValueEntry* NamedValuesImpl::InsertValue( const std::string& name )
{
    return &m_valueEntries[ name ];
}


namespace Detail
{

///////////////////////////////////////////////////////////////////////////////
//
// Constant Named Value Reference
//

ConstNamedValueRef::ConstNamedValueRef( const std::string& name, NamedValueEntry* entry )
    : m_name( name )
    , m_entry( entry )
{
}


//
// Accessors 
//

Bool ConstNamedValueRef::AsBool() const
{
    switch ( m_entry->type )
    {
    case NAMED_VALUE_BOOL:
        return m_entry->GetBool();

    case NAMED_VALUE_INT:
    case NAMED_VALUE_UINT:
        return ( 0 != m_entry->GetInt() );

    case NAMED_VALUE_INT64:
        return ( 0 != m_entry->GetInt64() );

    case NAMED_VALUE_DOUBLE:
        return ( 0.0 != m_entry->GetDouble() );
    
    case NAMED_VALUE_STRING:
    {
        Lexical::Boolean xbool;
        if ( ! xbool.TryParse( m_entry->GetString() ))
        {
            CARAMEL_THROW( "Can't convert string value to bool, name: %s", m_name );
        }
        return xbool;
    }

    default:
        CARAMEL_NOT_REACHED();
    }
}


Int ConstNamedValueRef::AsInt() const
{
    switch ( m_entry->type )
    {
    case NAMED_VALUE_BOOL:
    case NAMED_VALUE_INT:
    case NAMED_VALUE_UINT:
        return m_entry->GetInt();

    case NAMED_VALUE_STRING:
    {
        Lexical::Integer< Int > xint;
        if ( ! xint.TryParse( m_entry->GetString() ))
        {
            CARAMEL_THROW( "Can't convert string value to int, name: %s", m_name );
        }
        return xint;
    }

    case NAMED_VALUE_INT64:
        CARAMEL_THROW( "Can't convert int64 value to int, name: %s", m_name );

    case NAMED_VALUE_DOUBLE:
        CARAMEL_THROW( "Can't convert double value to int, name: %s", m_name );
    
    default:
        CARAMEL_NOT_REACHED();
    }
}


Uint ConstNamedValueRef::AsUint() const
{
    switch ( m_entry->type )
    {
    case NAMED_VALUE_BOOL:
    case NAMED_VALUE_INT:
    case NAMED_VALUE_UINT:
        return m_entry->GetUint();

    case NAMED_VALUE_STRING:
    {
        Lexical::Integer< Uint > xuint;
        if ( ! xuint.TryParse( m_entry->GetString() ))
        {
            CARAMEL_THROW( "Can't convert string value to uint, name: %s", m_name );
        }
        return xuint;
    }

    case NAMED_VALUE_INT64:
        CARAMEL_THROW( "Can't convert int64 value to uint, name: %s", m_name );

    case NAMED_VALUE_DOUBLE:
        CARAMEL_THROW( "Can't convert double value to uint, name: %s", m_name );
    
    default:
        CARAMEL_NOT_REACHED();
    }
}


Int64 ConstNamedValueRef::AsInt64() const
{
    switch ( m_entry->type )
    {
    case NAMED_VALUE_BOOL:
    case NAMED_VALUE_INT:
    case NAMED_VALUE_UINT:
    case NAMED_VALUE_INT64:
        return m_entry->GetInt64();

    case NAMED_VALUE_STRING:
    {
        Lexical::Integer< Int64 > xuint;
        if ( ! xuint.TryParse( m_entry->GetString() ))
        {
            CARAMEL_THROW( "Can't convert string value to int64, name: %s", m_name );
        }
        return xuint;
    }

    case NAMED_VALUE_DOUBLE:
        CARAMEL_THROW( "Can't convert double value to int64, name: %s", m_name );

    default:
        CARAMEL_NOT_REACHED();
    }
}


Double ConstNamedValueRef::AsDouble() const
{
    switch ( m_entry->type )
    {
    case NAMED_VALUE_BOOL:
    case NAMED_VALUE_INT:
        return static_cast< Double >( m_entry->GetInt() );

    case NAMED_VALUE_UINT:
        return static_cast< Double >( m_entry->GetUint() );

    case NAMED_VALUE_INT64:
        return static_cast< Double >( m_entry->GetInt64() );

    case NAMED_VALUE_DOUBLE:
        return m_entry->GetDouble();

    case NAMED_VALUE_STRING:
    {
        Lexical::Floating< Double > xdouble;
        if ( ! xdouble.TryParse( m_entry->GetString() ))
        {
            CARAMEL_THROW( "Can't convert string value to double, name: %s", m_name );
        }
        return xdouble;
    }

    default:
        CARAMEL_NOT_REACHED();
    }
}


std::string ConstNamedValueRef::AsString() const
{
    switch ( m_entry->type )
    {
    case NAMED_VALUE_BOOL:   return ToString( m_entry->GetBool() );
    case NAMED_VALUE_INT:    return ToString( m_entry->GetInt() );
    case NAMED_VALUE_UINT:   return ToString( m_entry->GetUint() );
    case NAMED_VALUE_INT64:  return ToString( m_entry->GetInt64() );
    case NAMED_VALUE_DOUBLE: return ToString( m_entry->GetDouble() );
    case NAMED_VALUE_STRING: return m_entry->GetString();

    default:
        CARAMEL_NOT_REACHED();
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// Name Value Reference
//

NamedValueRef::NamedValueRef( NamedValues* host, const std::string& name, NamedValueEntry* entry )
    : ConstNamedValueRef( name, entry )
    , m_host( host )
{
}


NamedValueRef& NamedValueRef::operator=( Bool value )
{
    this->PrepareEntry();

    m_entry->value = static_cast< Int64 >( value ? 1 : 0 );
    m_entry->type  = NAMED_VALUE_BOOL;

    return *this;
}


NamedValueRef& NamedValueRef::operator=( Int value )
{
    this->PrepareEntry();

    m_entry->value = static_cast< Int64 >( value );
    m_entry->type  = NAMED_VALUE_INT;

    return *this;
}


NamedValueRef& NamedValueRef::operator=( Uint value )
{
    this->PrepareEntry();

    m_entry->value = static_cast< Int64 >( value );
    m_entry->type  = NAMED_VALUE_UINT;

    return *this;
}


NamedValueRef& NamedValueRef::operator=( Int64 value )
{
    this->PrepareEntry();

    m_entry->value = value;
    m_entry->type  = NAMED_VALUE_INT64;

    return *this;
}


NamedValueRef& NamedValueRef::operator=( Double value )
{
    this->PrepareEntry();

    m_entry->value = value;
    m_entry->type  = NAMED_VALUE_DOUBLE;

    return *this;
}


NamedValueRef& NamedValueRef::operator=( const std::string& value )
{
    this->PrepareEntry();

    m_entry->value = value;
    m_entry->type  = NAMED_VALUE_STRING;

    return *this;
}


NamedValueRef& NamedValueRef::operator=( const Char* value )
{
    if ( ! value )
    {
        CARAMEL_INVALID_ARGUMENT();
    }

    return this->operator=( std::string( value ));
}


void NamedValueRef::PrepareEntry()
{
    if ( ! m_entry )
    {
        m_entry = m_host->m_impl->InsertValue( m_name );
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// Named Value Entry
//

Bool NamedValueEntry::GetBool() const
{
    return 0 != boost::get< Int64 >( value );
}


Int NamedValueEntry::GetInt() const
{
    return static_cast< Int >( boost::get< Int64 >( value ));
}


Uint NamedValueEntry::GetUint() const
{
    return static_cast< Uint >( boost::get< Int64 >( value ));
}


Int64 NamedValueEntry::GetInt64() const
{
    return boost::get< Int64 >( value );
}


Double NamedValueEntry::GetDouble() const
{
    return boost::get< Double >( value );
}


std::string NamedValueEntry::GetString() const
{
    return boost::get< std::string >( value );
}


///////////////////////////////////////////////////////////////////////////////

} // namespace Detail

} // namespace Caramel
