#ifndef CORE_UTIL_HPP
#define CORE_UTIL_HPP

#include <climits>
#include <stdexcept>
#include <vector>

namespace GraphicCore::Util
{
    inline void constCharToString (std::string& outMsg, char const* charMsg)
    {
        const std::string stringMsg (charMsg, strnlen (charMsg, sizeof (charMsg)));
        outMsg = stringMsg;
    }

    template <typename T>
    static void updatePointerFromVector (const std::vector<T>& srcVector, uint32_t& destCount, const T*& destData)
    {
        if (srcVector.empty( ))
        {
            destCount = 0;
            destData  = nullptr;
        }
        else
        {
            destCount = static_cast<uint32_t> (srcVector.size( ));
            destData  = srcVector.data( );
        }
    }

    template <typename T> static void updatePointerFromData (const T& srcData, uint32_t& destCount, const T*& destData)
    {
        destCount = 1;
        destData  = &srcData;
    }

    template <typename T> static void updatePointerFromVector (const std::vector<T>& srcVector, const T*& destData)
    {
        if (srcVector.empty( )) { destData = nullptr; }
        else
        {
            destData = srcVector.data( );
        }
    }

    template <typename T, typename... Args> void PassToVec (std::vector<T>& vector, Args&&... args)
    {
        vector.emplace_back (std::forward<Args> (args)...);
    }
}  // namespace GraphicCore::Util
#endif  // CORE_UTIL_HPP