#ifndef RESOURCE_PATCH_HPP
#define RESOURCE_PATCH_HPP

#include "utils.hpp"
#include "pch.h"

class ResourcePatch
{
    inline static ResourcePatch* m_Instance = nullptr;

    std::unordered_map<std::string_view, std::string> m_Paches;

    const std::string emptyString {""};

   public:
    static ResourcePatch* GetInstance( )
    {
        if (!m_Instance) m_Instance = new ResourcePatch;
        return m_Instance;
    }

    const std::string&       GetPatch (const std::string_view& patchName) const;
    void                     SetPatch (const std::string_view& patch, const std::string_view& patchName);
    std::vector<std::string> listOfAvailablePatch( );
};

#endif  // RESOURCE_PATCH_HPP
