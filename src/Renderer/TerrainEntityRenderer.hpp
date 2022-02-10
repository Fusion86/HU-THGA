#pragma once

#include <glm/glm.hpp>

using glm::mat4;

#include <Renderer/BaseRenderer.hpp>
#include <Entity/BaseEntity.hpp>

/// \brief
/// This class serves as a renderer for a terrain entity
/// \details
/// The class overrides the 3 abstract base functions and fills them with details
class TerrainEntityRenderer final : public BaseRenderer
{
  public:
    using BaseRenderer::BaseRenderer;

    void setMatrices(mat4 viewMatrix, mat4 projectionMatrix);

    void bind() override;

    void render(const BaseEntity& baseEntity) override;

    void unbind() override;

  private:
    mat4 m_viewMatrix;
    mat4 m_projectionMatrix;
};
