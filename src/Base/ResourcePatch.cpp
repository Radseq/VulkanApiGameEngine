#include "ResourcePatch.hpp"

const std::string& ResourcePatch::GetPatch (const std::string_view& patchName) const
{
    for (const auto& patch : m_Paches)
    {
        if (patch.first == patchName) { return patch.second; }
    }
    LogMsg ("No Found patch: " + std::string (patchName), LogLevel::LOG_ERROR);
    return emptyString;
}

void ResourcePatch::SetPatch (const std::string_view& patch, const std::string_view& patchName)
{
    m_Paches [patchName] = patch;
}

std::vector<std::string> ResourcePatch::listOfAvailablePatch( )
{
    std::vector<std::string> result;
    for (const auto& patch : m_Paches) { result.push_back (std::string (patch.first)); }
    return result;
}
