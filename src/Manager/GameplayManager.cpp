#include "GameplayManager.hpp"

#include <Manager/GuiManager.hpp>
#include <Manager/ConstructionManager.hpp>
#include <Manager/UnitManager.hpp>
#include <Manager/CameraManager.hpp>
#include <Manager/UnitManager.hpp>


#include <Opponent/LaneInsight.hpp>

#include <Construction/Building.hpp>
#include <Construction/GridSquare.hpp>

#include <Unit/Unit.hpp>
#include <Unit/Worker.hpp>


GameplayManager::GameplayManager(UnitManager& unit, ConstructionManager& construction, GuiManager& gui,
                                 CameraManager& camera)
    : m_unitManager(unit), m_constructionManager(construction), m_guiManager(gui), m_cameraManager(camera)
{
}

void GameplayManager::update(const float deltaTime, glm::ivec2 cursorPos, sf::Keyboard::Key key,
                             sf::Mouse::Button mouseButton, bool isFocused, TerrainEntity& friendlyCastle,
                             float scrollWheelDelta)
{

    if (!m_gameFinished)
    {
        m_updateCastleHealth();
        m_guiManager.updateHud(m_resources[Globals::side::LEFT]);

        if (isFocused)
        {
            m_updateCamera(deltaTime, scrollWheelDelta);

            if (m_guiManager.isCursorInWindow(cursorPos)) // If cursor is hovering a 2D GUI Part
            {
                m_update2DSelection(mouseButton);
            }
            else
            {
                m_update3DSelection(cursorPos, mouseButton);
            }
        }
    }
}

void GameplayManager::m_update2DSelection(const sf::Mouse::Button& mouseButton)
{
    if (mouseButton == sf::Mouse::Left)
    {
        bool updateHud = false;
        if (m_guiManager.isButtonHovered("foodButton") && m_changeWorkerGoal(ResourceType::FOOD, true))
        {
            updateHud = true;
        }
        if (m_guiManager.isButtonHovered("woodButton") && m_changeWorkerGoal(ResourceType::WOOD, true))
        {
            updateHud = true;
        }
        if (m_guiManager.isButtonHovered("goldButton") && m_changeWorkerGoal(ResourceType::GOLD, true))
        {
            updateHud = true;
        }
        if (m_guiManager.isButtonHovered("stoneButton") && m_changeWorkerGoal(ResourceType::STONE, true))
        {
            updateHud = true;
        }
        if (updateHud)
        {
            m_guiManager.updatePlayerWorkerAssignmentWindow(getWorkerGoal(Globals::side::LEFT));
        }

        for (auto& buildingStats : m_constructionManager.getBuildingStats())
        {
            if (m_guiManager.isButtonHovered(GuiManager::spawnButtonStr(Building::typeString(buildingStats.first))))
            {
                // TODO nice building spawning
                m_constructionManager.tryBuyConstruction(m_resources[Globals::side::LEFT], buildingStats.first,
                                                         m_selectedGrid->getGameEntity()->getPosition(),
                                                         *m_selectedGrid, Globals::side::LEFT);
            }

            for (auto& unit : buildingStats.second.spawnableUnits)
            {

                if (m_guiManager.isButtonHovered(m_guiManager.spawnButtonStr(Unit::typeString(unit))))
                {

                    m_unitManager.tryBuyUnit(m_resources[Globals::side::LEFT], unit,
                                             m_selectedGrid->getGameEntity()->getPosition(),
                                             m_selectedGrid->getCurrentLane(), m_selectedGrid->getOwner());
                }
            }
        }
    }
    if (mouseButton == sf::Mouse::Right)
    {
        bool updateHud = false;
        if (m_guiManager.isButtonHovered("foodButton") && m_changeWorkerGoal(ResourceType::FOOD, false))
        {
            updateHud = true;
        }
        if (m_guiManager.isButtonHovered("woodButton") && m_changeWorkerGoal(ResourceType::WOOD, false))
        {
            updateHud = true;
        }
        if (m_guiManager.isButtonHovered("goldButton") && m_changeWorkerGoal(ResourceType::GOLD, false))
        {
            updateHud = true;
        }
        if (m_guiManager.isButtonHovered("stoneButton") && m_changeWorkerGoal(ResourceType::STONE, false))
        {
            updateHud = true;
        }
        if (updateHud)
        {
            m_guiManager.updatePlayerWorkerAssignmentWindow(getWorkerGoal(Globals::side::LEFT));
        }
    }
}

bool GameplayManager::m_changeWorkerGoal(ResourceType type, bool increase)
{
    auto& playerWorkerGoal = m_workerGoal[Globals::side::LEFT];
    if (increase)
    {
        int assignedWorkers =
            playerWorkerGoal.food + playerWorkerGoal.wood + playerWorkerGoal.gold + playerWorkerGoal.stone;
        if (assignedWorkers < LaneInsight::countUnits(UnitType::WORKER, m_unitManager.getUnits()))
        {
            playerWorkerGoal[type]++;
            return true;
        }
    }
    else
    {
        if (playerWorkerGoal[type] > 0)
        {
            playerWorkerGoal[type]--;
            return true;
        }
    }
    return false;
}

void GameplayManager::m_updateCamera(const float deltaTime, float scrollWheelDelta)
{
    float cameraSpeedX = 0.0f;
    float cameraSpeedZ = 0.0f;

    // Moving left
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        cameraSpeedX = -m_cameraSpeedFactor;
    }

    // Moving right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        cameraSpeedX = m_cameraSpeedFactor;
    }

    // Moving forward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        cameraSpeedZ = -m_cameraSpeedFactor / 2.0f;
    }

    // Moving backward
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        cameraSpeedZ = m_cameraSpeedFactor / 2.0f;
    }

    // Scrolling left
    if (scrollWheelDelta == 1)
    {
        cameraSpeedX = m_cameraSpeedFactor * 5.0f;
    }

    // Scrolling right
    if (scrollWheelDelta == -1)
    {
        cameraSpeedX = -m_cameraSpeedFactor * 5.0f;
    }

    // Move the camera
    moveCamera(cameraSpeedX, cameraSpeedZ, deltaTime);
}

void GameplayManager::m_update3DSelection(const glm::ivec2& cursorPos, const sf::Mouse::Button& mouseButton)
{
    std::shared_ptr<GridSquare> currentSelectedGrid = m_selectedGrid;
    std::shared_ptr<GridSquare> currentHoveredGrid;

    bool  selectedTerrain = true;
    float winW            = float(Globals::WIN_WIDTH);

    // 3D point selection
    Globals::lane selectedLane = getSelectedLane(cursorPos);
    float         selectedX    = 0.0;
    switch (selectedLane)
    {
        case Globals::lane::TOP:
            selectedX = (m_cameraManager.getPosition().x - 10.5f) + ((float(cursorPos.x) / winW) * 21.0f);
            break;
        case Globals::lane::MID:
            selectedX = (m_cameraManager.getPosition().x - 8.5f) + ((float(cursorPos.x) / winW) * 17.0f);
            break;
        case Globals::lane::BOT:
            selectedX = (m_cameraManager.getPosition().x - 6.1f) + ((float(cursorPos.x) / winW) * 12.2f);
            break;
        default:
            selectedTerrain = false;
    }

    if (selectedTerrain)
    {
        for (auto& construction : m_constructionManager.getConstructions())
        {
            float x    = construction->getGameEntity()->getPosition().x;
            auto  grid = std::dynamic_pointer_cast<GridSquare>(construction);

            if (grid == nullptr) // We only want to select gridsquares
                continue;

            if (grid->getCurrentLane() != selectedLane || selectedX <= -25.0f ||
                selectedX >= 5.0f) // This position is invalid
                continue;

            if (currentHoveredGrid != nullptr)
            {
                if (fabsf(x - selectedX) > fabsf(currentHoveredGrid->getGameEntity()->getPosition().x - selectedX))
                {
                    continue;
                }
            }

            currentHoveredGrid = grid;
        }
    }

    // If player aimed at construction
    if (currentHoveredGrid != nullptr)
    {
        // Check if clicked
        if (mouseButton == sf::Mouse::Left)
        {
            currentSelectedGrid = currentHoveredGrid;
            m_guiManager.selectWindow(currentSelectedGrid->getBuildingMenuId());
        }
    }

    if (currentHoveredGrid != m_hoveredGrid)
    {
        if (currentHoveredGrid != nullptr && currentHoveredGrid != currentSelectedGrid)
        {
            currentHoveredGrid->setColor(Globals::HOVER_COLOR);
        }

        if (m_hoveredGrid != nullptr && m_hoveredGrid != m_selectedGrid)
        {
            m_hoveredGrid->setColor(vec3(1.0));
        }
    }
    if (m_selectedGrid != currentSelectedGrid)
    {
        if (currentSelectedGrid != nullptr)
        {
            currentSelectedGrid->setColor(Globals::SELECTED_COLOR);
        }

        if (m_selectedGrid != nullptr)
        {
            m_selectedGrid->setColor(vec3(1.0));
        }
    }

    m_hoveredGrid  = currentHoveredGrid;
    m_selectedGrid = currentSelectedGrid;

    // Testing
    // if (mouseButton == sf::Mouse::Right)
    // {
    //     m_unitManager.spawnUnit(UnitType::SWORDSMAN,
    //                             vec3(selectedX, Globals::LANE_Y.at(selectedLane), Globals::LANE_Z.at(selectedLane)),
    //                             selectedLane, Globals::side::RIGHT);
    // }
}

Globals::lane GameplayManager::getSelectedLane(glm::ivec2 cursorPos)
{
    constexpr float winH = float(Globals::WIN_HEIGHT);

    if (cursorPos.y >= winH * 0.45f && cursorPos.y < winH * 0.575f)
    {
        return Globals::lane::TOP;
    }
    else if (cursorPos.y >= winH * 0.575f && cursorPos.y < winH * 0.725f)
    {
        return Globals::lane::MID;
    }
    else if (cursorPos.y >= winH * 0.725f && cursorPos.y < winH * 1.0f)
    {
        return Globals::lane::BOT;
    }

    return Globals::lane::NONE;
}

void GameplayManager::moveCamera(float speedX, float speedZ, float deltaTime)
{
    // X movement
    if ((m_cameraManager.getPosition().x >= -17.5f && speedX < 0) ||
        (m_cameraManager.getPosition().x <= 17.5f && speedX > 0))
    {
        m_cameraManager.move(vec3(speedX * deltaTime, 0.0f, 0.0f));
    }

    // Z movement
    if ((m_cameraManager.getPosition().z >= -3.0f && speedZ < 0) ||
        (m_cameraManager.getPosition().z <= 0.5f && speedZ > 0))
    {
        m_cameraManager.move(vec3(0.0f, 0.0f, speedZ * deltaTime));
    }
}


void GameplayManager::m_updateCastleHealth()
{
    static float fHealth = 100.0f;
    static float eHealth = 100.0f;
    m_guiManager.setFriendlyCastleHealth(fHealth);
    m_guiManager.setEnemyCastleHealth(eHealth);

    for (auto& unit : m_unitManager.getUnits())
    {
        if (unit->getGameEntity()->getPosition().x < -24.0f && unit->getGameEntity()->getPosition().x > -24.5f)
        {
            if (unit->getOwner() == Globals::side::RIGHT)
            {
                unit->setDestroyed(true);
                fHealth -= 5.f;
            }
        }
        else if (unit->getGameEntity()->getPosition().x > 24.0f && unit->getGameEntity()->getPosition().x < 24.5f)
        {
            if (unit->getOwner() == Globals::side::LEFT)
            {
                unit->setDestroyed(true);
                eHealth -= 5.f;
            }
        }
    }

    if (fHealth <= 0) // Player dead
    {
        m_guiManager.setHudEnabled(false);
        m_guiManager.selectWindow("loseWindow");
        m_gameFinished = true;
    }
    else if (eHealth <= 0) // Enemy dead
    {
        m_guiManager.setHudEnabled(false);
        m_guiManager.selectWindow("winWindow");
        m_gameFinished = true;
    }


    auto& goal = m_workerGoal[Globals::side::LEFT];
    size_t workers = LaneInsight::countUnits(UnitType::WORKER, m_unitManager.getUnits(), Globals::side::LEFT);
    m_guiManager.setIdleWorkerCount(workers -(goal.food + goal.wood + goal.stone + goal.gold));
}

ResourcesCollection& GameplayManager::getResources(const Globals::side& player)
{
    return m_resources[player];
}

ResourcesCollection& GameplayManager::getWorkerAssignment(const Globals::side& player)
{
    return m_workerAssignment[player];
}

ResourcesCollection& GameplayManager::getWorkerGoal(const Globals::side& player)
{
    return m_workerGoal[player];
}

int& GameplayManager::getWorkerAssignment(const Worker& worker)
{
    return m_workerAssignment[worker.getOwner()][worker.getCurrentResource()];
}

int& GameplayManager::getWorkerGoal(const Worker& worker)
{
    return m_workerGoal[worker.getOwner()][worker.getCurrentResource()];
}

int& GameplayManager::getResource(const Worker& worker)
{
    return m_resources[worker.getOwner()][worker.getCurrentResource()];
}
