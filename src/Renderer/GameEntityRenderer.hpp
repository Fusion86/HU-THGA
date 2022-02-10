#pragma once

#include <Renderer/BaseRenderer.hpp>
#include <Entity/BaseEntity.hpp>
#include <Entity/GameEntity.hpp>

/// \brief
/// This class serves as a renderer for GAME entities
/// \details
/// The class overrides the 3 abstract base functions and fills them with details
class GameEntityRenderer final : public BaseRenderer
{
  public:
    using BaseRenderer::BaseRenderer;

    void setMatrices(mat4 viewMatrix, mat4 projectionMatrix);

    // todo this update probably shouldn't be in that class
    static void update(GameEntity& entity, float delta);

    void bind() override;

    void render(const BaseEntity& baseEntity) override;

    void unbind() override;

  private:
    mat4 m_viewMatrix;
    mat4 m_projectionMatrix;
};
