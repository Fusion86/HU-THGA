#pragma once

#include <Renderer/BaseRenderer.hpp>
#include <Entity/BaseEntity.hpp>

/// \brief
/// This class serves as a renderer for GUI entities
/// \details
/// The class overrides the 3 abstract base functions and fills them with details
class GuiEntityRenderer final : public BaseRenderer
{
  public:
    using BaseRenderer::BaseRenderer;

    void bind() override;

    void render(const BaseEntity& baseEntity) override;

    void unbind() override;
};