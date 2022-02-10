#include "GuiManager.hpp"

#include <Misc/Logger.hpp>

#include <ResourceLoading/EntityFactory.hpp>



#include <Renderer/GuiEntityRenderer.hpp>

#include <Entity/GuiEntity.hpp>

GuiManager::GuiManager()
{
    const float size = Globals::GUI_SIZE;

    // Heads-up-display (no buttons)
    m_hud = std::make_shared<GuiWindow>("hud");
    m_hud->createElement("bar", -1.0f, 0.8f, 1.4f, 0.2f, "bar.png", true);
    m_hud->createElement("food", -1.0f, 0.85f, size, convSize(size), "icons/food.png", true);
    m_hud->createElement("wood", -0.8f, 0.85f, size, convSize(size), "icons/wood.png", true);
    m_hud->createElement("gold", -0.6f, 0.85f, size, convSize(size), "icons/gold.png", true);
    m_hud->createElement("stone", -0.4f, 0.85f, size, convSize(size), "icons/stone.png", true);
    m_hud->createElement("playerHealth", -0.2f, 0.85f, size, convSize(size), "icons/fCastle.png", true);
    m_hud->createElement("opponentHealth", 0.0f, 0.85f, size, convSize(size), "icons/eCastle.png", true);
    m_hud->createElement("workerCount", 0.2f, 0.85f, size, convSize(size), "icons/worker.png", true);
    m_hud->createText("foodCount", "0", -0.9f, 0.875f, 0.025f, 0.05f);
    m_hud->createText("woodCount", "0", -0.7f, 0.875f, 0.025f, 0.05f);
    m_hud->createText("goldCount", "0", -0.5f, 0.875f, 0.025f, 0.05f);
    m_hud->createText("stoneCount", "0", -0.3f, 0.875f, 0.025f, 0.05f);
    m_hud->createText("playerHealth", "100", -0.1f, 0.875f, 0.025f, 0.05f);
    m_hud->createText("opponentHealth", "100", 0.1f, 0.875f, 0.025f, 0.05f);
    m_hud->createText("workerCount", "0", 0.3f, 0.875f, 0.025f, 0.05f);

    // Menu window
    auto menuWindow = std::make_shared<GuiWindow>("menuWindow");
    menuWindow->createElement("titleIcon", -0.5f, 0.35f, 1.0f, 0.2f, "icons/title.png", true);
    menuWindow->createButton("playButton", -0.125f, -0.20f, 0.25f, 0.25f, "buttons/play.png", false);
    menuWindow->createButton("exitButton", -0.125f, -0.55f, 0.25f, 0.25f, "buttons/exit.png", false);
    m_windows.push_back(menuWindow);

    // Building window
    constexpr float hackyWidth     = 2.2f;
    auto            buildingWindow = std::make_shared<GuiWindow>("buildWindow");
    buildingWindow->createElement("buildingBar", -1.0f, -1.0f, 2.0f, 0.2f, "bar.png", true);
    buildingWindow->createButton(spawnButtonStr("barrack"), -0.9f, -1.0f + ((0.2f - convSize(size)) / 2.0f),
                                 size * hackyWidth, convSize(size), "buttons/build_barrack.png", true);
    buildingWindow->createButton(spawnButtonStr("butchery"), -0.7f, -1.0f + ((0.2f - convSize(size)) / 2.0f),
                                 size * hackyWidth, convSize(size), "buttons/build_farm.png", true);
    buildingWindow->createButton(spawnButtonStr("woodshop"), -0.5f, -1.0f + ((0.2f - convSize(size)) / 2.0f),
                                 size * hackyWidth, convSize(size), "buttons/build_lumbercamp.png", true);
    buildingWindow->createButton(spawnButtonStr("goldmine"), -0.3f, -1.0f + ((0.2f - convSize(size)) / 2.0f),
                                 size * hackyWidth, convSize(size), "buttons/build_goldmine.png", true);
    buildingWindow->createButton(spawnButtonStr("stonemine"), -0.1f, -1.0f + ((0.2f - convSize(size)) / 2.0f),
                                 size * hackyWidth, convSize(size), "buttons/build_stonemine.png", true);
    buildingWindow->createButton(spawnButtonStr("tower"), 0.1f, -1.0f + ((0.2f - convSize(size)) / 2.0f),
                                 size * hackyWidth, convSize(size), "buttons/build_tower.png", true);


    m_windows.push_back(buildingWindow);

    // Resource assignment window
    m_resourceWindow = std::make_shared<GuiWindow>("resourceWindow");
    m_resourceWindow->createButton("foodButton", 0.4f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size, convSize(size),
                                   "icons/food.png", true);
    m_resourceWindow->createText("foodText", "0", 0.4f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size, convSize(size));

    m_resourceWindow->createButton("woodButton", 0.55f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size, convSize(size),
                                   "icons/wood.png", true);
    m_resourceWindow->createText("woodText", "0", 0.55f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size,
                                 convSize(size));

    m_resourceWindow->createButton("goldButton", 0.70f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size, convSize(size),
                                   "icons/gold.png", true);
    m_resourceWindow->createText("goldText", "0", 0.70f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size,
                                 convSize(size));

    m_resourceWindow->createButton("stoneButton", 0.85f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size, convSize(size),
                                   "icons/stone.png", true);
    m_resourceWindow->createText("stoneText", "0", 0.85f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size,
                                 convSize(size));

    // win window
    auto winWindow = std::make_shared<GuiWindow>("winWindow");
    winWindow->createButton("winScreen", -1.0f, -1.0f, 2.0f, 2.0f, "icons/win.png", true);
    m_windows.push_back(winWindow);

    auto loseWindow = std::make_shared<GuiWindow>("loseWindow");
    loseWindow->createButton("loseScreen", -1.0f, -1.0f, 2.0f, 2.0f, "icons/lose.png", true);
    m_windows.push_back(loseWindow);

    // Default
    m_selectedWindow = "menuWindow";
}

void GuiManager::update(const float deltaTime, glm::ivec2 cursorPos)
{
    float x = m_convertToNdc(cursorPos).x;
    float y = m_convertToNdc(cursorPos).y;
    if (getSelectedWindow() != nullptr)
    {

        m_updateHoverState(getSelectedWindow(), x, y);
    }
    m_updateHoverState(m_resourceWindow, x, y);
}

void GuiManager::m_updateHoverState(std::shared_ptr<GuiWindow> window, float x, float y)
{
    for (auto& button : window->components) // For all buttons in current window
    {
        if (button->isClickable()) // Check if clickable{
            // If mouse inside button
            if ((x > button->getPosition().x && x < button->getPosition().x + button->getSize().x) &&
                (y > button->getPosition().y && y < button->getPosition().y + button->getSize().y))
            {
                if (!button->isHovered()) // Was not hovered yet
                {
                    button->setPosition(button->getPosition() - (button->getSize() * 0.125f)); // Position correction
                    button->setSize(button->getSize() * 1.25f);                                // Increase size
                    button->setHovered(true);                                                  // Set hovered
                }
            }
            else
            {
                if (button->isHovered()) // Was hovered
                {
                    button->setSize(button->getSize() / 1.25f);                                // Decrease size
                    button->setPosition(button->getPosition() + (button->getSize() * 0.125f)); // Position correction
                    button->setHovered(false);                                                 // Set not hovered
                }
            }
    }
}

/// Create a new static element and add it to the window.
void GuiWindow::createElement(STRING id, float x, float y, float w, float h, STRING texFile, bool texfiltering)
{
    this->components.push_back(EntityFactory::createGuiEntity(id, x, y, w, h, texFile, false, texfiltering));
}

/// Create a new clickable button and add it to the window.
void GuiWindow::createButton(STRING id, float x, float y, float w, float h, STRING texFile, bool texfiltering)
{
    this->components.push_back(EntityFactory::createGuiEntity(id, x, y, w, h, texFile, true, texfiltering));
}

/// Create a new text element and add it to the window.
void GuiWindow::createText(STRING id, const STRING chars, float x, float y, float w, float h)
{
    this->texts.push_back(std::make_shared<Text>(id, chars, x, y, w, h));
}

/// Get the selected windor, or nullptr when there is no window selected.
std::shared_ptr<GuiWindow> GuiManager::getSelectedWindow()
{
    if (m_selectedWindow.empty())
    {
        return nullptr;
    }

    for (auto& window : m_windows)
    {
        if (window->ID == m_selectedWindow)
        {
            return window;
        }
    }

    Logger::throwWarning("No GUI windows or no selected GUI window!");

    return nullptr;
}

glm::vec2 GuiManager::m_convertToNdc(glm::ivec2 cursorPos)
{
    float x = float(cursorPos.x) / float(Globals::WIN_WIDTH);
    float y = float(cursorPos.y) / float(Globals::WIN_HEIGHT);
    x       = (x * 2.0f) - 1.0f;
    y       = (y * 2.0f) - 1.0f;
    y *= -1.0f;
    return glm::vec2(x, y);
}

float convSize(float size)
{
    return (float(Globals::WIN_WIDTH) / float(Globals::WIN_HEIGHT)) * size;
}

/// Select a window based on its ID.
void GuiManager::selectWindow(const std::string& id)
{
    m_selectedWindow = id;
    if (getSelectedWindow() == nullptr)
    {
        Logger::throwError("Newly selected window " + id + " does not exist!");
    }
}

void GuiManager::updateHud(const ResourcesCollection& resources)
{
    static ResourcesCollection oldCollection;

    if (resources < oldCollection || oldCollection < resources)
    {
        m_hud->texts[0]->setChars(std::to_string(resources.food));
        m_hud->texts[1]->setChars(std::to_string(resources.wood));
        m_hud->texts[2]->setChars(std::to_string(resources.gold));
        m_hud->texts[3]->setChars(std::to_string(resources.stone));
        oldCollection = resources;
    }
}

void GuiManager::updatePlayerWorkerAssignmentWindow(const ResourcesCollection& resources)
{
    m_resourceWindow->texts[0]->setChars(std::to_string(resources.food));
    m_resourceWindow->texts[1]->setChars(std::to_string(resources.wood));
    m_resourceWindow->texts[2]->setChars(std::to_string(resources.gold));
    m_resourceWindow->texts[3]->setChars(std::to_string(resources.stone));
}

void GuiManager::setFriendlyCastleHealth(int health)
{
    static int oldHealth = health;

    if (health != oldHealth)
    {
        m_hud->texts[4]->setChars(std::to_string(health));
        oldHealth = health;
    }
}

void GuiManager::setEnemyCastleHealth(int health)
{
    static int oldHealth = health;

    if (health != oldHealth)
    {
        m_hud->texts[5]->setChars(std::to_string(health));
        oldHealth = health;
    }
}

void GuiManager::setIdleWorkerCount(int count)
{
    static int oldCount = count;

    if (count != oldCount)
    {
        m_hud->texts[6]->setChars(std::to_string(count));
        oldCount = count;
    }
}

/// Returns true when the cursor is inside the window, and false if not (or if there is no selected window).
bool GuiManager::isCursorInWindow(glm::ivec2 cursorPos)
{
    if (getSelectedWindow() == nullptr)
    {
        return false;
    }
    else
    {
        float x = m_convertToNdc(cursorPos).x;
        float y = m_convertToNdc(cursorPos).y;

        for (auto& button : getSelectedWindow()->components) // For all buttons in current window
        {
            // If mouse inside button
            if ((x > button->getPosition().x && x < button->getPosition().x + button->getSize().x) &&
                (y > button->getPosition().y && y < button->getPosition().y + button->getSize().y))
            {
                return true;
            }
        }

        return false;
    }
}

/// Return true if there is a selected window and the button with the given ID is hovered.
bool GuiManager::isButtonHovered(const std::string& buttonId)
{
    if (getSelectedWindow() != nullptr)
    {
        for (auto& button : getSelectedWindow()->components)
        {
            if (button->getId() == buttonId)
            {
                return button->isHovered();
            }
        }
    }
    for (auto& button : m_resourceWindow->components)
    {
        if (button->getId() == buttonId)
        {
            return button->isHovered();
        }
    }
    return false;
}

void GuiManager::renderAll(GuiEntityRenderer& renderer)
{
    // Selected window
    if (getSelectedWindow() != nullptr)
    {
        m_renderWindow(renderer, *getSelectedWindow());
    }

    if (m_hudEnabled)
    {
        m_renderWindow(renderer, *m_hud);
        m_renderWindow(renderer, *m_resourceWindow);
    }
}

void GuiManager::addWindow(std::shared_ptr<GuiWindow> window)
{
    m_windows.push_back(std::move(window));
}

std::string GuiManager::spawnButtonStr(const std::string& id)
{
    return std::string("spawn_") + id + "_button";
}

void GuiManager::m_renderWindow(GuiEntityRenderer& renderer, const GuiWindow& window)
{
    // HUD buttons
    for (const auto& component : window.components)
    {
        renderer.render(*component);
    }

    // HUD texts
    for (auto& text : window.texts)
    {
        for (auto& chr : text->chars)
        {
            renderer.render(*chr);
        }
    }
}

bool GuiManager::isHudEnabled() const
{
    return m_hudEnabled;
}

void GuiManager::setHudEnabled(bool hudEnabled)
{
    m_hudEnabled = hudEnabled;
}

void Text::setChars(const std::string& chrs)
{
    chars.clear();
    for (size_t i = 0; i < chrs.size(); i++)
    {
        if (isdigit(chrs[i]))
        {
            this->chars.push_back(EntityFactory::createGuiEntity(
                id, x + (i * w), y, w, h, "numbers/" + std::string{chrs[i]} + ".png", false, true));
        }
    }
}

CreationWindow::CreationWindow(const string& constructionId, float size)
    : GuiWindow(constructionId + "Window"), constructionId(constructionId), size(size)
{
    createElement(constructionId + "Bar", -1.0f, -1.0f, 2.0f, 0.2f, "bar.png", true);
    // createElement(constructionId + "Icon", -0.9f, -1.0f + ((0.2f - convSize(size)) / 2.0f), size, convSize(size),
    //               std::string{"icons/"} + constructionId + ".png", true);
}

void CreationWindow::addSpawnButton(const std::string& unitId)
{
    createButton(GuiManager::spawnButtonStr(unitId), currentX, -1.0f + ((0.2f - convSize(size)) / 2.0f), 0.2f,
                 convSize(size), std::string{"buttons/spawn_"} + unitId + ".png", true);
    currentX += 0.25f;
}
