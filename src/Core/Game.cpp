#include "Game.hpp"

#include <Misc/Globals.hpp>
#include <ResourceLoading/EntityFactory.hpp>

Game::Game()
    : m_windowManager(Globals::WIN_FULLSCREEN, Globals::WIN_TITLE, 8, 4, 5, Globals::WIN_WIDTH, Globals::WIN_HEIGHT),
      m_cameraManager(Globals::CAM_POS, Globals::CAM_FOV, Globals::WIN_WIDTH, Globals::WIN_HEIGHT, Globals::CAM_NEAR,
                      Globals::CAM_FAR),
      m_skyEntityRenderer("SkyEntityShader.vert", "SkyEntityShader.frag"),
      m_terrainEntityRenderer("TerrainEntityShader.vert", "TerrainEntityShader.frag"),
      m_gameEntityRenderer("GameEntityShader.vert", "GameEntityShader.frag"),
      m_guiEntityRenderer("GuiEntityShader.vert", "GuiEntityShader.frag"), m_guiManager(), m_terrainManager(),
      m_soundManager(), m_constructionManager(m_guiManager, m_terrainManager),
      m_unitManager(m_constructionManager, m_soundManager),
      m_gameplayManager(m_unitManager, m_constructionManager, m_guiManager, m_cameraManager),
      m_opponentManager(m_unitManager, m_constructionManager, m_gameplayManager, m_terrainManager)
{
    m_initialize();
}

void Game::start()
{
    m_windowManager.closeSplashScreen();
    sf::Clock deltaClock;
    sf::Time  deltaTime;
    while (m_isRunning)
    {
        m_update(float(deltaTime.asMicroseconds()) / 1000.0f);
        m_render();
        deltaTime = deltaClock.restart();
    }
}

void Game::stop()
{
    m_isRunning = false;
}

void Game::m_initialize()
{
    m_soundManager.play("main_score", true);

    // Sky
    m_sky = EntityFactory::createSkyEntity("skybox", "terrain/right.png", "terrain/left.png", "terrain/top.png",
                                           "terrain/bottom.png", "terrain/back.png", "terrain/front.png");
}

void Game::m_update(const float deltaTime)
{
    m_updateEvents();

    // Update all managers
    m_guiManager.update(deltaTime, m_windowManager.getCursorPosition());
    m_cameraManager.update();
    m_soundManager.update();

    if (m_isInMenu)
    {
        m_updateMainMenu(deltaTime);
    }
    else
    {
        m_unitManager.update(deltaTime, m_gameplayManager);
        m_constructionManager.update(deltaTime);
        m_gameplayManager.update(deltaTime, m_windowManager.getCursorPosition(), m_lastPressedKey,
                                 m_lastPressedMouseButton, m_isFocused,
                                 *m_terrainManager.getCastle(Globals::side::LEFT), m_lastScrollWheelDelta);
        m_opponentManager.update(deltaTime);
    }

    // Render debugging
    if (m_lastPressedKey == sf::Keyboard::Q)
    {
        m_wireframed = !m_wireframed;
    }

    // Sky rotation
    m_sky->setRotation(m_sky->getRotation() + (0.001f * deltaTime));

    // Update sprite animations
    for (auto& unit : m_unitManager.getUnits())
    {
        GameEntityRenderer::update(*unit->getGameEntity(), deltaTime);
    }

    // Reset key registration
    m_lastPressedKey         = sf::Keyboard::Key::KeyCount;
    m_lastPressedMouseButton = sf::Mouse::Button::ButtonCount;
    m_lastScrollWheelDelta   = 0.0f;
}

void Game::m_updateMainMenu(const float deltaTime)
{
    if (m_guiManager.isButtonHovered("playButton") && m_lastPressedMouseButton == sf::Mouse::Left)
    {
        m_guiManager.setHudEnabled(true);
        m_constructionManager.setGridsEnabled(true);
        m_isInMenu                         = false;
        m_cameraManager.setPosition(vec3(-16.0f, 4.5f, 0.5f));
        m_guiManager.selectWindow("castleWindow");
    }
    else if (m_guiManager.isButtonHovered("exitButton") && m_lastPressedMouseButton == sf::Mouse::Left)
    {
        m_isRunning = false;
    }
    else
    {
        static bool zoomingIn = true;
        if (m_cameraManager.getPosition().z <= -3.0f || m_cameraManager.getPosition().z >= 0.05f)
        {
            zoomingIn = !zoomingIn;
        }

        m_cameraManager.move(vec3(0.0f, 0.0f, 0.0005f * deltaTime * (zoomingIn ? -1.0f : 1.0f)));
    }
}

void Game::m_updateEvents()
{
    // Update user input
    m_windowManager.update();

    // Handle events
    auto events = m_windowManager.getEvents();
    for (auto& event : events)
    {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            stop();
        }
        else if (event.type == sf::Event::MouseButtonPressed)
        {
            m_lastPressedMouseButton = event.mouseButton.button;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            m_lastPressedKey = event.key.code;
        }
        else if (event.type == sf::Event::GainedFocus)
        {
            m_isFocused = true;
        }
        else if (event.type == sf::Event::LostFocus)
        {
            m_isFocused = false;
        }
        else if (event.type == sf::Event::MouseWheelScrolled)
        {
            m_lastScrollWheelDelta = event.mouseWheelScroll.delta;
        }
    }
}

void Game::m_render()
{
    // Clear from backbuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframed ? GL_LINE : GL_FILL);

    // Render SKY entity
    m_skyEntityRenderer.setMatrices(m_cameraManager.getViewMatrix(), m_cameraManager.getProjectionMatrix());
    m_skyEntityRenderer.bind();
    m_skyEntityRenderer.render(*m_sky);
    m_skyEntityRenderer.unbind();

    // Render TERRAIN entities
    m_terrainEntityRenderer.setMatrices(m_cameraManager.getViewMatrix(), m_cameraManager.getProjectionMatrix());
    m_terrainEntityRenderer.bind();
    m_terrainManager.renderAll(m_terrainEntityRenderer);
    m_terrainEntityRenderer.unbind();

    // Render GAME entities
    m_gameEntityRenderer.setMatrices(m_cameraManager.getViewMatrix(), m_cameraManager.getProjectionMatrix());
    m_gameEntityRenderer.bind();
    m_unitManager.renderAll(m_gameEntityRenderer);
    m_constructionManager.renderAll(m_gameEntityRenderer);
    m_gameEntityRenderer.unbind();

    // Render GUI entities
    m_guiEntityRenderer.bind();
    m_guiManager.renderAll(m_guiEntityRenderer);
    m_guiEntityRenderer.unbind();

    // Flush to backbuffer
    m_windowManager.flush();
}
