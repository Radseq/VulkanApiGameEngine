#ifndef VERTEX_LAYOUT_HPP
#define VERTEX_LAYOUT_HPP

#include <vector>
#include <vulkan/vulkan.hpp>

namespace GraphicCore {
    /** @brief Stores vertex layout components for model loading and Vulkan vertex input and atribute bindings  */
    class VertexLayout {
       public:
        /** @brief Vertex layout components */
        typedef enum class Component {
            VERTEX_COMPONENT_POSITION    = 0x0,
            VERTEX_COMPONENT_NORMAL      = 0x1,
            VERTEX_COMPONENT_COLOR       = 0x2,
            VERTEX_COMPONENT_UV          = 0x3,
            VERTEX_COMPONENT_TANGENT     = 0x4,
            VERTEX_COMPONENT_BITANGENT   = 0x5,
            VERTEX_COMPONENT_DUMMY_FLOAT = 0x6,
            VERTEX_COMPONENT_DUMMY_INT   = 0x7,
            VERTEX_COMPONENT_DUMMY_VEC4  = 0x8,
            VERTEX_COMPONENT_DUMMY_INT4  = 0x9,
            VERTEX_COMPONENT_DUMMY_UINT4 = 0xA,
            VERTEX_COMPONENT_TEXT_COORD  = 0xA1,
        } Component;

        /** @brief Components used to generate vertices from */
        std::vector<Component> components;

        VertexLayout (std::vector<Component>&& components, uint32_t binding = 0)
            : components (std::move (components)) { }

        [[nodiscard]] uint32_t componentIndex (Component component) const;
        static vk::Format      componentFormat (Component component);
        static uint32_t        componentSize (Component component);
        [[nodiscard]] uint32_t stride( ) const;
        [[nodiscard]] uint32_t offset (uint32_t index) const;
    };
}  // namespace GraphicCore

#endif VERTEX_LAYOUT_HPP
