// Caramel C++ Library - File System Amenity - Path Header

#ifndef __CARAMEL_FILE_SYSTEM_PATH_H
#define __CARAMEL_FILE_SYSTEM_PATH_H

#include <Caramel/Caramel.h>

#if defined( CARAMEL_COMPILER_HAS_PRAGMA_ONCE )
#pragma once
#endif

#include <Caramel/String/Utf8String.h>
#include <boost/filesystem.hpp>


namespace Caramel
{

///////////////////////////////////////////////////////////////////////////////
//
// Path
// - Immutable
//   Used as function parameters in other FileSystem classes,
//   Therefore it provides both std::string and const Char* constructors.
//
//   Different |  directory       |  value_type  |  string_type
//             |  separators      |              |
//  ------------------------------------------------------------
//   Windows   |  back-slash '\'  |  wchar_t     |  wstring
//   Other OS  |  slash '/'       |  char        |  string
//

class Path : public boost::filesystem::path
{
public:

    Path( const Utf8String& u8path );

    //
    // Construct from native encoding
    //

    Path( const std::string& path );
    Path( const Char* path );
    

    //
    // Properties
    //

    Utf8String Stem()      const { return Utf8String( this->stem().native() ); }
    Utf8String Extension() const { return Utf8String( this->extension().native() ); }


    //
    // Conversion
    // - Path always can be converted to UTF-8 string.
    //

    operator Utf8String() const { return Utf8String( this->native() ); }


#if defined( CARAMEL_SYSTEM_IS_WINDOWS )

    //
    // Construct from wide string
    //

    Path( const std::wstring& wpath );
    Path( const Wchar* wpath );

#endif // CARAMEL_SYSTEM_IS_WINDOWS

};


///////////////////////////////////////////////////////////////////////////////
//
// Implementations
//

#if defined( CARAMEL_SYSTEM_IS_WINDOWS )

//
// For Windows, path's value_type is wchar_t.
// If you don't give codecvt, it would think the input string as ACP (active code page).
//
inline Path::Path( const Utf8String& path )
    : boost::filesystem::path( path.ToWstring() )
{
}


inline Path::Path( const std::wstring& wpath )
    : boost::filesystem::path( wpath )
{
}


inline Path::Path( const Wchar* wpath )
    : boost::filesystem::path( wpath )
{
}

#else

inline Path::Path( const Utf8String& path )
    : boost::filesystem::path( path )
{
}

#endif  // CARAMEL_SYSTEM_IS_WINDOWS


//
// Construct from native encoding.
// - In Windows, it is ACP (acitve code page).
//   In other OS, it is UTF-8.
//

inline Path::Path( const std::string& path )
    : boost::filesystem::path( path )
{
}


inline Path::Path( const Char* path )
    : boost::filesystem::path( path )
{
}


///////////////////////////////////////////////////////////////////////////////

} // namespace Caramel

#endif // __CARAMEL_FILE_SYSTEM_PATH_H
