#include "VertexLayout.hpp"

namespace GraphicCore {
    uint32_t VertexLayout::componentIndex (Component component) const {
        for (size_t i = 0; i < components.size( ); ++i) {
            if (components [i] == component) { return uint32_t (i); }
        }
        return static_cast<uint32_t> (
            -1);  //????? uint has only positive values, so why - 1?? if should be -1 so why cast to uint32?
    }

    vk::Format VertexLayout::componentFormat (Component component) {
        switch (component) {
            case Component::VERTEX_COMPONENT_UV:
                return vk::Format::eR32G32Sfloat;
            case Component::VERTEX_COMPONENT_DUMMY_FLOAT:
                return vk::Format::eR32Sfloat;
            case Component::VERTEX_COMPONENT_DUMMY_INT:
                return vk::Format::eR32Sint;
            case Component::VERTEX_COMPONENT_DUMMY_VEC4:
                return vk::Format::eR32G32B32A32Sfloat;
            case Component::VERTEX_COMPONENT_DUMMY_INT4:
                return vk::Format::eR32G32B32A32Sint;
            case Component::VERTEX_COMPONENT_DUMMY_UINT4:
                return vk::Format::eR32G32B32A32Uint;
            case Component::VERTEX_COMPONENT_TEXT_COORD:
                return vk::Format::eR32G32Sfloat;
            default:
                return vk::Format::eR32G32B32Sfloat;
        }
    }

    uint32_t VertexLayout::componentSize (Component component) {
        switch (component) {
            case Component::VERTEX_COMPONENT_UV:
                return 2 * sizeof (float);
            case Component::VERTEX_COMPONENT_DUMMY_FLOAT:
                return sizeof (float);
            case Component::VERTEX_COMPONENT_DUMMY_INT:
                return sizeof (int);
            case Component::VERTEX_COMPONENT_DUMMY_VEC4:
                return 4 * sizeof (float);
            case Component::VERTEX_COMPONENT_DUMMY_INT4:
                return 4 * sizeof (int32_t);
            case Component::VERTEX_COMPONENT_DUMMY_UINT4:
                return 4 * sizeof (uint32_t);
            case Component::VERTEX_COMPONENT_TEXT_COORD:
                return 2 * sizeof (float);
            default:
                // All components except the ones listed above are made up of 3 floats
                return 3 * sizeof (float);
        }
    }

    uint32_t VertexLayout::stride( ) const {
        uint32_t res = 0;
        for (const auto& component : components) { res += componentSize (component); }
        return res;
    }

    uint32_t VertexLayout::offset (uint32_t index) const {
        uint32_t res = 0;
        assert (index < components.size( ));
        for (uint32_t i = 0; i < index; ++i) { res += componentSize (components [i]); }
        return res;
    }
}  // namespace GraphicCore
