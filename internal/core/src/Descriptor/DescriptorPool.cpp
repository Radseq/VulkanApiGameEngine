#include "DescriptorPool.hpp"

#ifdef DEBUG
void GameCore::DescriptorPool::addTypesToPair (const std::pair<vk::DescriptorType, uint32_t>& pair) {
    if (typesCanBeUsed.empty()) {
        typesCanBeUsed.push_back (pair);
    } else {
        for (auto& typesToUsed : typesCanBeUsed) {
            if (pair.first == typesToUsed.first) {
                typesToUsed.second += pair.second;
                break;
            } else {
                typesCanBeUsed.push_back (pair);
            }
        }
    }
}

void GameCore::DescriptorPool::addDescriptorPoolSize (const vk::DescriptorType& descType,
                                                      const uint32_t&           descriptorCount) {
    descPoolSize.push_back ({descType, descriptorCount});
    ++count;

    addTypesToPair ({descType, descriptorCount});
}
#endif

void GameCore::DescriptorPool::create (const vk::Device& device, const uint32_t& descMaxSets) {
    // assert (descMaxSets == 0);
    // assert (count == 0);

    vk::DescriptorPoolCreateInfo descCreateInfo {{ }, descMaxSets, count, descPoolSize.data( )};

    descriptorPool = device.createDescriptorPool (descCreateInfo);
}

const vk::DescriptorPool& GameCore::DescriptorPool::getDescriptorPool( ) { return descriptorPool; }

void GameCore::DescriptorPool::destroy (const vk::Device& device) { device.destroyDescriptorPool (descriptorPool); }

#ifdef DEBUG
std::vector<std::pair<vk::DescriptorType, uint32_t>>& GameCore::DescriptorPool::usedDescTypes( ) {
    return typesCanBeUsed;
}
#endif