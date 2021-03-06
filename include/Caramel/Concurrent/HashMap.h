// Caramel C++ Library - Concurrent Amenity - Hash Map Header

#ifndef __CARAMEL_CONCURRENT_HASH_MAP_H
#define __CARAMEL_CONCURRENT_HASH_MAP_H
#pragma once

#include <Caramel/Caramel.h>
#include <Caramel/Concurrent/Detail/BasicMap.h>
#include <Caramel/Concurrent/ReplicatePolicies.h>
#include <unordered_map>


namespace Caramel
{

namespace Concurrent
{

///////////////////////////////////////////////////////////////////////////////
//
// Concurrent Hash Map
//
// NOTE: The ( Key, Value ) naming convention is belong to .NET Framework,
//       not STL/Boost style.
//

template< typename Key, typename Value, typename ReplicatePolicy = ReplicateNothing >
class HashMap : public Detail::BasicMap< std::unordered_map< Key, Value >, ReplicatePolicy >
{
};


///////////////////////////////////////////////////////////////////////////////

} // namespace Concurrent

} // namespace Caramel

#endif // __CARAMEL_CONCURRENT_HASH_MAP_H
