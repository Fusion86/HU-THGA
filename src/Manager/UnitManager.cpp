#include "UnitManager.hpp"
#include "Opponent/LaneInsight.hpp"

#include <Unit/Unit.hpp>

#include <Manager/ConstructionManager.hpp>
#include <Manager/GameplayManager.hpp>

#include <ResourceLoading/EntityFactory.hpp>
#include <Unit/Worker.hpp>
#include <Unit/Defender.hpp>

#include <Renderer/BaseRenderer.hpp>
#include <ResourceLoading/TextureLoader.hpp>


#include <Construction/Butchery.hpp>
#include <Construction/Woodshop.hpp>
#include <Construction/Goldmine.hpp>
#include <Construction/Stonemine.hpp>
#include <Construction/Tower.hpp>

UnitManager::UnitManager(ConstructionManager& mConstructionManager, SoundManager& mSoundManager)
    : m_constructionManager(mConstructionManager), m_soundManager(mSoundManager)
{
    // Friendly units
    m_unitStats[UnitType::ARCHER]            = {100, 5, 0, 1, {}, {}, 3, 2.0f};
    m_unitStats[UnitType::ARCHER].animations = {{"walking", 11}, {"attacking", 6}};
    m_unitStats[UnitType::ARCHER].price      = ResourcesCollection{0, 120, 20, 0}; // Don't move this to the line above!

    m_unitStats[UnitType::SWORDSMAN] = {100, 40, 0, 1, {}, {}, 1, 2.0f};
    //m_unitStats[UnitType::SWORDSMAN].animations = { {"walking", 11}, {"attacking", 6} };
    m_unitStats[UnitType::SWORDSMAN].price = ResourcesCollection{70, 25, 0, 0};

    m_unitStats[UnitType::KNIGHT] = {100, 75, 0, 1, {}, {}, 1, 2.0f};
    //m_unitStats[UnitType::KNIGHT].animations = { {"walking", 11}, {"attacking", 6} };
    m_unitStats[UnitType::KNIGHT].price = ResourcesCollection{150, 0, 50, 0};

    m_unitStats[UnitType::WORKER]       = {100, 0, 0, 1, {}, {}};
    m_unitStats[UnitType::WORKER].price = {100, 0, 0, 0};

    m_unitStats[UnitType::RAM]            = {200, 2, 20, 1, {}, {}, 1, 2.0f};
    m_unitStats[UnitType::RAM].animations = {{"moving", 6}, {"attacking", 6}};
    m_unitStats[UnitType::RAM].price      = ResourcesCollection{0, 0, 50, 150};

    m_unitStats[UnitType::CROSSBOWMAN] = {100, 10, 0, 1, {}, {}, 3, 2.0f};
    //m_unitStats[UnitType::CROSSBOWMAN].animations = {{"walking", 11}, {"attacking", 6}};
    m_unitStats[UnitType::CROSSBOWMAN].price = ResourcesCollection{150, 0, 50, 0};

    // Enemy units
    m_unitStats[UnitType::RAIDER]            = {100, 2, 0, 1, {}, {}, 1, 2.0f};
    m_unitStats[UnitType::RAIDER].animations = {{"walking", 11}, {"attacking", 6}};
    m_unitStats[UnitType::RAIDER].price      = ResourcesCollection{150, 0, 50, 0};
}

void UnitManager::update(const float deltaTime, GameplayManager& gameplayManager)
{
    for (auto& unit : m_units)
    {
        switch (unit->getUnitType())
        {
            case UnitType::SWORDSMAN:
            case UnitType::ARCHER:
            case UnitType::KNIGHT:
            case UnitType::RAIDER:
                m_updateSimpleAttacker(*unit, deltaTime);
                break;
            case UnitType::RAM:
                m_updateSimpleDestructor(*unit, deltaTime);
                break;
            case UnitType::CROSSBOWMAN:
                m_updateDefender(unit, deltaTime);
                break;
            case UnitType::WORKER:
                m_updateCollector(unit, deltaTime, gameplayManager);
                break;
        }
    }

    for (size_t i = 0; i < m_units.size(); i++)
    {
        if (m_units[i]->isDestroyed())
        { // Check if the unit was destroyed
            for (auto* listener : m_unitListeners)
            {
                listener->onUnitSpawned(m_units[i]);
            }
            m_units.erase(m_units.begin() + i);
            i--;
            continue;
        }
    }
}

void UnitManager::spawnUnit(UnitType type, vec3 position, Globals::lane lane, Globals::side owner)
{
    std::string  name       = Unit::typeString(type);
    vec3         size       = vec3(0.4f);
    vec3         rotation   = vec3(0.0f);
    unsigned int frameCount = 1;

    if (owner == Globals::side::RIGHT)
    {
        rotation = vec3(0.0f, 180.0f, 0.0f);
    }

    switch (type)
    {
        case UnitType::KNIGHT:
            size = vec3(0.5f);
            break;
        case UnitType::ARCHER:
            frameCount = 11;
            name += "-walking";
            break;
        case UnitType::RAIDER:
            frameCount = 11;
            name += "-walking";
            break;
        case UnitType::RAM:
            frameCount = 6;
            name += "-moving";
            break;
    }

    auto gameEntity =
        EntityFactory::createGameEntity(name, position, rotation, size, "units/" + name + ".png", "plane.obj");
    gameEntity->totalFrames = frameCount;
    std::shared_ptr<Unit> unit;
    switch (type)
    {
        case UnitType::WORKER:
            unit = std::make_shared<Worker>(gameEntity, type, lane, owner);
            unit->setNonInteractable(true); // Unit can
            break;
        case UnitType::CROSSBOWMAN:
            unit = std::make_shared<Defender>(gameEntity, type, lane, owner);
            break;
        default:
            unit = std::make_shared<Unit>(gameEntity, type, lane, owner);
    }
    unit->currentHealth = m_unitStats[type].healthPoints;
    m_units.push_back(unit);
    for (auto* listener : m_unitListeners)
    {
        listener->onUnitSpawned(unit);
    }
    const char* hack = owner == Globals::side::LEFT ? "left" : "right";
    Logger::throwInfo("Spawned unit '", type, "' for owner '", hack, "'");
}

bool UnitManager::tryBuyUnit(ResourcesCollection& resourceInventory, UnitType type, vec3 position, Globals::lane lane,
                             Globals::side owner)
{
    // TODO: Check if user can buy unit

    const auto& unitPrice = m_unitStats[type].price;
    if (resourceInventory < unitPrice)
    {
        // Only show warnings for the player, because the bot calls this method a lot.
        if (owner == Globals::side::LEFT)
        {
            Logger::throwInfo("You don't have enough resources to buy this unit!\nPrice:\n", unitPrice,
                              "\nAvailable:\n", resourceInventory);
        }

        return false;
    }
    else
    {
        resourceInventory -= unitPrice;
        spawnUnit(type, position, lane, owner);
        return true;
    }
}

std::vector<std::shared_ptr<Unit>>& UnitManager::getUnits()
{
    return m_units;
}

void UnitManager::m_updateSimpleAttacker(Unit& attacker, float deltaTime)
{
    auto closestUnit         = LaneInsight::getClosestUnit(attacker, m_units);
    auto closestConstruction = LaneInsight::getClosestConstruction(attacker, m_constructionManager.getConstructions());

    if (closestConstruction.distance < closestUnit.distance && closestConstruction.spawnable->isPathBlocking() &&
        closestConstruction.distance < Globals::CONSTRUCTION_BLOCKING_DISTANCE)
    {
        attacker.isAttacking = true;
        m_changeAnimation(attacker, Animation::IDLE);
        return;
    }

    if (!m_tryAttack(attacker, *closestUnit.spawnable, closestUnit.distance,
                     m_unitStats[attacker.unitType].attackPoints, deltaTime))
    {
        attacker.isAttacking = false;
        m_tryMove(attacker, deltaTime);
        m_changeAnimation(attacker, Animation::WALKING);
    }
    else
    {
        attacker.isAttacking = true;
        m_changeAnimation(attacker, Animation::ATTACKING);
    }
}

void UnitManager::m_updateSimpleDestructor(Unit& destructor, float deltaTime)
{
    auto closestConstruction =
        LaneInsight::getClosestConstruction(destructor, m_constructionManager.getConstructions());

    if (!m_tryAttack(destructor, *closestConstruction.spawnable, closestConstruction.distance,
                     m_unitStats[destructor.unitType].damageAgainstBuildings, deltaTime))
    {
        m_tryMove(destructor, deltaTime);
        destructor.isAttacking = false;
        m_changeAnimation(destructor, Animation::WALKING);
    }
    else
    {
        destructor.isAttacking = true;
        m_changeAnimation(destructor, Animation::ATTACKING);
    }
}

void UnitManager::m_updateCollector(const std::shared_ptr<Unit>& pUnit, float d, GameplayManager& gameplayManager)
{
    auto worker = std::dynamic_pointer_cast<Worker>(pUnit);

    const vec3& workerPosition = worker->getGameEntity()->getPosition();
    switch (worker->getCurrentState())
    {
        case Worker::State::HOME:
        case Worker::State::RETURNING:
        {
            Globals::lane lookingLane = Globals::lane::NONE;
            if (worker->getCurrentState() == Worker::State::RETURNING)
            {
                if ((pUnit->getOwner() == Globals::side::LEFT && workerPosition.x < -25.0f) ||
                    (pUnit->getOwner() == Globals::side::RIGHT && workerPosition.x > 25.0f))
                {
                    worker->setCurrentState(Worker::State::HOME);
                    worker->setNonInteractable(true);
                    return;
                }
                lookingLane = worker->getCurrentLane();
            }
            for (auto& resourceType : ResourcesCollection::allTypes)
            {
                if (gameplayManager.getWorkerAssignment(worker->getOwner())[resourceType] <
                    gameplayManager.getWorkerGoal(worker->getOwner())[resourceType])
                {
                    std::shared_ptr<Building> collectionBuilding{};
                    switch (resourceType)
                    {
                        case ResourceType::FOOD:
                            collectionBuilding = LaneInsight::findFirstConstructionOfType<Butchery>(
                                worker->getOwner(), m_constructionManager.getConstructions(), lookingLane);
                            break;
                        case ResourceType::WOOD:
                            collectionBuilding = LaneInsight::findFirstConstructionOfType<Woodshop>(
                                worker->getOwner(), m_constructionManager.getConstructions(), lookingLane);
                            break;
                        case ResourceType::GOLD:
                            collectionBuilding = LaneInsight::findFirstConstructionOfType<Goldmine>(
                                worker->getOwner(), m_constructionManager.getConstructions(), lookingLane);
                            break;
                        case ResourceType::STONE:
                            collectionBuilding = LaneInsight::findFirstConstructionOfType<Stonemine>(
                                worker->getOwner(), m_constructionManager.getConstructions(), lookingLane);
                            break;
                    }

                    if (collectionBuilding)
                    {
                        worker->setCurrentTargetBuilding(collectionBuilding);
                        worker->setCurrentLane(collectionBuilding->getCurrentLane());
                        worker->setCurrentResource(resourceType);
                        gameplayManager.getWorkerAssignment(worker->getOwner())[resourceType]++;
                        worker->setCurrentState(Worker::State::COMMUTING);
                        worker->setNonInteractable(false);
                        return;
                    }
                }
            }
            if (worker->getCurrentState() == Worker::State::RETURNING)
            {
                worker->currentMovementDirection = worker->getOwner();
                m_tryMove(*worker, d);
            }
            break;
        }
        case Worker::State::COMMUTING:
        {
            float distance = LaneInsight::getHorizontalDistance(*worker, *worker->getCurrentTargetBuilding());
            if (distance > Globals::BUILDING_ENTER_DISTANCE)
            {
                m_tryMove(*worker, d, worker->getCurrentTargetBuilding());
            }
            else
            {
                worker->setCurrentState(Worker::State::WORKING);
                worker->setNonInteractable(true);
            }
            break;
        }
        case Worker::State::WORKING:
            // TODO building destroyed exception, but would kinda need shared pointers
            auto& goal       = gameplayManager.getWorkerGoal(*worker);
            auto& assignment = gameplayManager.getWorkerAssignment(*worker);
            if (goal < assignment)
            {
                worker->setCurrentState(Worker::State::RETURNING);
                worker->setNonInteractable(false);
                assignment--;
                worker->setCurrentTargetBuilding(nullptr);
                break;
            }
            if (worker->currentCollectCooldown < d)
            {
                worker->resetCurrentCollectCooldown();
                gameplayManager.getResource(*worker) +=
                    Globals::RESOURCE_PER_COLLECTION.at(worker->getCurrentResource());
            }
            else
            {
                worker->currentCollectCooldown -= d;
            }
            break;
    }
}

void UnitManager::m_updateDefender(const std::shared_ptr<Unit>& unit, float deltaTime)
{
    auto& defender = dynamic_cast<Defender&>(*unit);

    switch (defender.getCurrentState())
    {
        case Defender::State::IDLE:
        {
            auto construction = LaneInsight::getClosestConstruction<Tower>(
                defender, m_constructionManager.getConstructions(), true, true);
            if (construction.spawnable)
            {
                defender.setCurrentTargetBuilding(construction.spawnable);
                defender.setCurrentState(Defender::State::COMMUTING);
            }
            break;
        }
        case Defender::State::COMMUTING:
        {
            float distance = LaneInsight::getHorizontalDistance(defender, *defender.getCurrentTargetBuilding());
            if (distance > Globals::BUILDING_ENTER_DISTANCE)
            {
                m_tryMove(defender, deltaTime, defender.getCurrentTargetBuilding());
                m_changeAnimation(defender, Animation::WALKING);
            }
            else
            {
                defender.setCurrentState(Defender::State::DEFENDING);
                defender.setNonInteractable(true);
            }
            break;
        }
        case Defender::State::DEFENDING:
            if (defender.getCurrentTargetBuilding()->isDestroyed())
            {
                defender.getCurrentTargetBuilding().reset();
                defender.setNonInteractable(false);
                defender.setCurrentState(Defender::State::IDLE);
            }
            else
            {
                auto distance =
                    LaneInsight::getClosestUnit(defender, m_units); // TODO make defenders look backward aswell
                m_tryAttack(defender, *distance.spawnable, distance.distance,
                            m_unitStats[defender.getUnitType()].attackPoints, deltaTime);
            }
            break;
    }
}

bool UnitManager::m_tryAttack(Unit& attacker, Spawnable& defender, float distance, int attackPoints, float deltaTime)
{
    auto& myStats = m_unitStats[attacker.getUnitType()];
    if (distance < myStats.attackRange)
    {
        if (attacker.attackCooldown <= 0)
        {
            std::cout << "hit" << std::endl;
            defender.onHit(attacker, attackPoints, m_soundManager);
            attacker.attackCooldown = 1000 / myStats.attackSpeed;
        }
        else
        {
            attacker.attackCooldown -= deltaTime;
        }
        return true;
    }
    return false;
}

bool UnitManager::m_tryMove(Unit& pUnit, float d, std::shared_ptr<Spawnable> target)
{
    if (target)
    {
        pUnit.currentMovementDirection = LaneInsight::getDirection(pUnit, *target);
    }

    auto closest = LaneInsight::getClosestUnit(pUnit, m_units, true, true);

    if (closest.distance > Globals::MIN_UNIT_DISTANCE || closest.distance == 0.0f || closest.spawnable->isAttacking ||
        closest.spawnable->currentMovementDirection != pUnit.currentMovementDirection)
    {
        float val = pUnit.currentMovementDirection == Globals::side::RIGHT ? d / 1000 : -d / 1000;
        val *= m_unitStats[pUnit.getUnitType()].movementStats;
        pUnit.getGameEntity()->setPosition(pUnit.getGameEntity()->getPosition() + vec3{val, 0, 0});
        return true;
    }
    return false;
}

void UnitManager::renderAll(BaseRenderer& renderer)
{
    for (auto& unit : m_units)
    {
        if (!unit->isNonInteractable())
        {
            renderer.render(*unit->getGameEntity());
        }
    }
}

void UnitManager::m_changeAnimation(Unit& pUnit, const std::string& id)
{
    for (size_t i = 0; i < m_unitStats[pUnit.unitType].animations.size(); i++)
    {
        if (m_unitStats[pUnit.unitType].animations[i].id == id)
        {
            m_changeAnimation(pUnit, i);
        }
    }
}

void UnitManager::m_changeAnimation(Unit& pUnit, const int& index)
{
    auto& animation                    = m_unitStats[pUnit.unitType].animations[index];
    pUnit.getGameEntity()->totalFrames = animation.frameCount;
    pUnit.getGameEntity()->changeTexture(TextureLoader::loadTexture(
        "units/" + string(Unit::typeString(pUnit.unitType)) + "-" + animation.id + ".png", false, false));
}

const std::map<UnitType, UnitStats>& UnitManager::getUnitStats() const
{
    return m_unitStats;
}

void UnitManager::addUnitListener(UnitListener* listener)
{
    m_unitListeners.emplace_back(listener);
}
