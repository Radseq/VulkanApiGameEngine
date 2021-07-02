#ifndef CORE_MATCH_HPP
#define CORE_MATCH_HPP

#include <limits>
#include <stdexcept>

namespace GraphicCore::Util
{
    /*
    * not working
    * first arg is value (to check that value is no larger than second arg type)
    * second arg determine outout type
    * how should work:
    * size_t val{10000000000000};
    * bool a = castValue(val, bool)
    *

    template <typename T, typename N>
    inline N castValue (const T& value, N) {
            constexpr auto a = std::numeric_limits<N>::max;

            if (value > a)
            {
                    throw std::overflow_error (std::string ("arg is too large!"));
            }

            return static_cast<N>(value);
    }*/


    template <typename T> inline uint32_t toUint32t (const T& arg)
    {
        if (arg > UINT_MAX) { throw std::overflow_error (std::string ("arg is too large!")); }

        return static_cast<uint32_t> (arg);
    }

    // this is stupid
    inline uint32_t checkMaxUint32Size (const uint32_t& size)
    {
        if (size > USHRT_MAX) { throw std::overflow_error (std::string ("uint32_t is too large!")); }
        return static_cast<uint32_t> (size);
    }

    template <typename T> inline int32_t toInt32 (const T& arg)
    {
        if (arg > INT_MAX) { throw std::overflow_error (std::string ("arg is too large!")); }
        return static_cast<int32_t> (arg);
    }

    // this is stupid
    inline uint64_t checkMaxUint64Size (const uint64_t& size)
    {
        if (size > UINT_MAX) { throw std::overflow_error (std::string ("uint64_t is too large!")); }
        return static_cast<uint64_t> (size);
    }

    // this is stupid
    inline float checkMaxFloatSize (const float& size)
    {
        if (size > FLT_MAX) { throw std::overflow_error (std::string ("float is too large!")); }
        return static_cast<float> (size);
    }

    inline int maxFromTwo (const uint32_t& a, const uint32_t& b) { return (((a) > (b)) ? (a) : (b)); }
}  // namespace GraphicCore::Util
#endif  // CORE_MATCH_HPP