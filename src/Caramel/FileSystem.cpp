// Caramel C++ Library - File System Facility - Implementation

#include <Caramel/CaramelPch.h>

#include <Caramel/FileSystem/FileInfoImpl.h>
#include <Caramel/FileSystem/PathImpl.h>


namespace Caramel
{

//
// Contents
//
//   FileInfo
//   Path
//

///////////////////////////////////////////////////////////////////////////////
//
// File Info
//

FileInfo::FileInfo( const Path& path )
    : m_impl( new FileInfoImpl( *( path.m_impl )))
{
}


FileInfo::~FileInfo()
{
}


Bool FileInfo::Exists() const
{
    return boost::filesystem::is_regular_file( m_impl->m_status )
        && boost::filesystem::exists( m_impl->m_status );
}


Path FileInfo::GetPath() const
{
    return Path( new PathImpl( boost::filesystem::path( m_impl->m_path )));
}


//
// Implementations
//

FileInfoImpl::FileInfoImpl( const boost::filesystem::path& path )
    : m_path( path )
    , m_status( boost::filesystem::status( path ))
{
}


///////////////////////////////////////////////////////////////////////////////
//
// Path
//

Path::Path()
    : m_impl( new PathImpl( boost::filesystem::path() ))
{
}


Path::Path( PathImpl* impl )
    : m_impl( impl )
{
}


Path::Path( const Utf8String& path )
    : m_impl( new PathImpl( path ))
{
}


//
// Construct from OS default encoding.
// - In Windows, it is ACP (acitve code page).
//   In other OS, it is UTF-8.
//

Path::Path( const std::string& path )
    : m_impl( new PathImpl( boost::filesystem::path( path )))
{
}


Path::Path( const Char* path )
    : m_impl( new PathImpl( boost::filesystem::path( path )))
{
}


#if defined( CARAMEL_SYSTEM_IS_WINDOWS )

//
// Constructors from wide string
// - Windows specific
//

Path::Path( const std::wstring& wpath )
    : m_impl( new PathImpl( boost::filesystem::path( wpath )))
{
}


Path::Path( const Wchar* wpath )
    : m_impl( new PathImpl( boost::filesystem::path( wpath )))
{
}


//
// Conversion to wide string
// - Windows specific
//

std::wstring Path::ToWstring() const
{
    return m_impl->wstring();
}


#endif // CARAMEL_SYSTEM_IS_WINDOWS


//
// Splits
//

Path Path::Directory() const { return Path( new PathImpl( m_impl->parent_path() )); }
Path Path::Stem()      const { return Path( new PathImpl( m_impl->stem() )); }
Path Path::Extension() const { return Path( new PathImpl( m_impl->extension() )); }


//
// Predicates
//

Bool Path::HasExtension() const { return m_impl->has_extension(); }


//
// Conversions
//

Path::operator Utf8String() const { return this->ToUtf8String(); }

Utf8String  Path::ToUtf8String() const { return Utf8String( m_impl->native() ); }
std::string Path::ToString()     const { return this->ToUtf8String().ToString(); }


//
// Compositions
//

//
// Combine
//
void Path::Combine( const Path& subpath )
{
    PathPtr temp = m_impl;
    m_impl.reset( new PathImpl( *temp / *subpath.m_impl ));
}


Path& Path::operator/=( const Path& subpath )
{
    this->Combine( subpath );
    return *this;
}


//
// Append Extension
// - Whether or not the extension has a prefix '.', the result is append with just one '.'
//
void Path::AppendExtension( const std::string& extension )
{
    if ( extension.empty() ) { return; }

    const Utf8String u8Path( *this );
    const Utf8String u8Dot( '.' );
    const Utf8String u8Ext( extension );

    const Utf8String result = ( '.' == extension[0] )
                            ? u8Path + u8Ext
                            : u8Path + u8Dot + u8Ext;

    m_impl.reset( new PathImpl( result ));
}


void Path::InsertStemSuffix( const std::string& suffix )
{
    if ( suffix.empty() ) { return; }

    const Path dir( this->Directory() );
    const Path ext( this->Extension() );

    const Utf8String newStemStr = this->Stem() + Utf8String( suffix );
    const Path newStem( newStemStr );

    Path result = dir / newStem;
    result.AppendExtension( ext.ToString() );

    *this = result;
}


//
// Implementation
//

PathImpl::PathImpl( boost::filesystem::path&& path )
    : boost::filesystem::path( path )
{
}


PathImpl::PathImpl( const Utf8String& path )
    #if defined( CARAMEL_SYSTEM_IS_WINDOWS )
    : boost::filesystem::path( path.ToWstring() )
    #else
    : boost::filesystem::path( path.ToString() )
    #endif
{
}



///////////////////////////////////////////////////////////////////////////////

} // namespace Caramel
