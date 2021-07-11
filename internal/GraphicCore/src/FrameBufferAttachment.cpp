#include "pch.h"
#include "FrameBufferAttachment.hpp"

namespace GraphicCore
{
    const FrameBufferAttachment::CreateFunc FrameBufferAttachment::DEFAULT_CREATE_FUNC =
        [] (std::shared_ptr<CoreImage> &&swapchainImage) -> FrameBufferAttachment {
        auto depthImage = std::make_shared<CoreImage> (
            swapchainImage->GetLocalDevice( ), std::move (swapchainImage->GetExtend( )), vk::Format::eD32Sfloat,
            vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eTransientAttachment);

        depthImage->Crete (vk::MemoryPropertyFlagBits::eDeviceLocal);

        std::vector<std::shared_ptr<CoreImage>> images;

        GraphicCore::Util::PassToVec (images, swapchainImage);
        GraphicCore::Util::PassToVec (images, depthImage);

        return FrameBufferAttachment {std::move (images)};
    };

    /*
    vk::ResultValueType<void>::type FrameBufferAttachment::Init( )
    {
        assert (!this->m_Images.empty( ) && "Should specify at least 1 image");

        std::set<vk::Extent2D, vkHelper::CompareExtent2D> uniqueExtent;

        auto getImageExtend = [] (std::shared_ptr<CoreImage> image) {
            return vk::Extent2D {image->GetExtend( ).width, image->GetExtend( ).height};
        };

        // Constructs a set of unique image extens given a vector of images
        std::transform (this->m_Images.begin( ), this->m_Images.end( ),
                        std::inserter (uniqueExtent, uniqueExtent.end( )), getImageExtend);

        // Allow only one extent size for a render target
        if (uniqueExtent.size( ) != 1)
        {
            return vk::createResultValue (vk::Result::eErrorInitializationFailed, "Extent size is not unique");
        }

        extent = *uniqueExtent.begin( );

        for (auto &image : this->m_Images)
        {
            if (image->GetType( ) != vk::ImageType::e2D)
            {
                return vk::createResultValue (vk::Result::eErrorInitializationFailed, "CoreImage type is not 2D");
            }

            auto imageView = std::make_shared<CoreImageView> (m_LogicalDevice.getVkDevice( ));

            imageView->CreateImageView (image, vk::ImageViewType::e2D);

            m_Views.emplace_back (imageView);

            m_Attachments.emplace_back (vkBasicModels::AttachmentModel {image->GetFormat( ), image->GetSamplesFlag( ),
                                                                        image->GetUsageFlags( )});
        }

        return vk::createResultValue (vk::Result::eSuccess, "Success");
    }*/

    FrameBufferAttachment::FrameBufferAttachment (std::vector<std::shared_ptr<CoreImage>> &&images) noexcept
        : m_LogicalDevice {images.back( )->GetLocalDevice( )}
        , m_Images {std::move (images)}
    {
        assert (!m_Images.empty( ) && "Should specify at least 1 image");

        std::set<vk::Extent2D, vkHelper::CompareExtent2D> uniqueExtent;

        auto getImageExtend = [] (std::shared_ptr<CoreImage> image) {
            return vk::Extent2D {image->GetExtend( ).width, image->GetExtend( ).height};
        };

        // Constructs a set of unique image extens given a vector of images
        std::transform (m_Images.begin( ), m_Images.end( ), std::inserter (uniqueExtent, uniqueExtent.end( )),
                        getImageExtend);

        // Allow only one extent size for a render target
        if (uniqueExtent.size( ) != 1)
        {  // return vk::createResultValue (vk::Result::eErrorInitializationFailed, "Extent size is not unique"); }
            assert ("Extent size is not unique");
        }
        extent = *uniqueExtent.begin( );

        for (auto &image : m_Images)
        {
            if (image->GetType( ) != vk::ImageType::e2D)
            {  // return vk::createResultValue (vk::Result::eErrorInitializationFailed, "CoreImage type is not 2D"); }
                assert ("CoreImage type is not 2D");
            }
            auto imageView = std::make_shared<CoreImageView> (m_LogicalDevice.getVkDevice( ));

            imageView->CreateImageView (image, vk::ImageViewType::e2D);

            GraphicCore::Util::PassToVec (m_Views, imageView);

            // m_Attachments.emplace_back (vkBasicModels::AttachmentModel {image->GetFormat( ), image->GetSamplesFlag(
            // ),
            //                                                            image->GetUsageFlags( )});

            GraphicCore::Util::PassToVec (m_Attachments, image->GetFormat( ), image->GetSamplesFlag( ),
                                          image->GetUsageFlags( ));
        }
    }

    const vk::Extent2D &                              FrameBufferAttachment::GetExtent( ) const { return extent; }
    const std::vector<std::shared_ptr<CoreImageView>> FrameBufferAttachment::GetViews( ) const { return m_Views; }

    const std::vector<uint32_t> &FrameBufferAttachment::GetInputAttachments( ) const { return m_InputAttachments; }
    const std::vector<uint32_t> &FrameBufferAttachment::GetOutputAttachments( ) const { return m_OutputAttachments; }
    const std::vector<vkBasicModels::AttachmentModel> &FrameBufferAttachment::GetAttachments( ) const
    {
        return m_Attachments;
    }

    void FrameBufferAttachment::SetInputAttachments (std::vector<uint32_t> &input) { m_InputAttachments = input; }
    void FrameBufferAttachment::SetOutputAttachments (std::vector<uint32_t> &output) { m_OutputAttachments = output; }

}  // namespace GraphicCore
