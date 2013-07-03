// Caramel C++ Library - I/O Amenity - Detail - MBCS Stream Reader Header

#ifndef __CARAMEL_IO_DETAIL_MBCS_STREAM_READER_H
#define __CARAMEL_IO_DETAIL_MBCS_STREAM_READER_H

#include <Caramel/Caramel.h>

#if defined( CARAMEL_COMPILER_HAS_PRAGMA_ONCE )
#pragma once
#endif

#include <Caramel/Io/InputStream.h>
#include <Caramel/Io/TextReader.h>


namespace Caramel
{

namespace Detail
{

///////////////////////////////////////////////////////////////////////////////
//
// MBCS (Multi-Byte Character Set) Stream Reader
// - Only in Windows System
//

class MbcsStreamReader : public TextReader
{
public:

    MbcsStreamReader( InputStream& stream, TextEncoding encoding );


    //
    // Implements TextReader
    //

    Bool ReadLine( Utf8String& line );

};


///////////////////////////////////////////////////////////////////////////////
//
// Implementation
//

inline MbcsStreamReader::MbcsStreamReader( InputStream& stream, TextEncoding encoding )
{
    CARAMEL_NOT_IMPLEMENTED();
}


inline Bool MbcsStreamReader::ReadLine( Utf8String& line )
{
    CARAMEL_NOT_IMPLEMENTED();
}


///////////////////////////////////////////////////////////////////////////////

} // namespace Detail

} // namespace Caramel

#endif // __CARAMEL_IO_DETAIL_MBCS_STREAM_READER_H
