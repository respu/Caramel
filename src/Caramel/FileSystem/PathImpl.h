// Caramel C++ Library - File System Facility - Path Private Header

#ifndef __CARAMEL_FILE_SYSTEM_PATH_IMPL_H
#define __CARAMEL_FILE_SYSTEM_PATH_IMPL_H

#include <Caramel/Caramel.h>

#if defined( CARAMEL_COMPILER_HAS_PRAGMA_ONCE )
#pragma once
#endif

#include <Caramel/FileSystem/Path.h>
#include <boost/filesystem.hpp>


namespace Caramel
{

///////////////////////////////////////////////////////////////////////////////
//
// Path
//

class PathImpl : public boost::filesystem::path
{
public:

    explicit PathImpl( boost::filesystem::path&& path );
    
};


///////////////////////////////////////////////////////////////////////////////

} // namespace Caramel

#endif // __CARAMEL_FILE_SYSTEM_PATH_IMPL_H
