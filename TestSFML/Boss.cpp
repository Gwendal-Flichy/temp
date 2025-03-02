#include "Boss.h"

#include "BossBehavior.h"
#include "Fireball.h"
#include "Game.h"



int getPistolBulletSpeed() { return 150; }
float getBossFluidFrictionCoef() { return 5.0f; }

Vec2 getBossSize()
{
    return { 64.f, 64.f };
}

Vec2 getVelo() { return { 32.f, 32.f }; }

Boss::Boss(IGameObjectContainer& game, const Vec2& position)
    : IGameObject(game)
    , m_position(position)
    , m_velocity(0,0)   
	,m_rootNode(this)
	,m_speed(0)
{
    m_sprite.setTexture(getOwner().getGame().getTextureCache().getTexture("Enemy.bmp"));


        auto* behavior = new BT::Retry(&m_rootNode);
        //new BT::FindEnemy(behavior);
        auto* ifDistance = new BT::PlayerDictance(behavior,100, BT::inf);
        auto* testSeq = new BT::Sequence(ifDistance);
        new BT::Walk(testSeq);
        new BT::FireShotgun(testSeq);
        new BT::FirePistol(testSeq);
        auto* defaultseq = new BT::Sequence(behavior);
        new BT::Idle(defaultseq);

        new BT::IsPlayerDead(behavior);


}

Vec2 Boss::getDistanceToPlayer() const
{
	return m_currentTarget->getPositon() - m_position;
}

float Boss::calculateAngleToTarget() const
{
    if (!m_currentTarget)
        return m_angle;

    Vec2 directionToPlayer = getDistanceToPlayer();

    return std::atan2(directionToPlayer.y, directionToPlayer.x);
}

bool Boss::findValidTarget()
{
    

    for (auto& target : m_allGameObjects)
    {
        if (target->gameObjectType()==PLAYERSHIP_TYPE)
        {
            m_currentTarget = static_cast<PlayerShip*>(target);
            if (isCurrentTargetValid())
				return true;

        }

    }
    return false;

}

bool Boss::isCurrentTargetValid() const
{
    if (m_currentTarget->getPV() <= 0)
        return false;

    return true;
}
void Boss::setSpeed(const int& Speed)
{
    m_speed = Speed;
}

void Boss::fireWithPistol()
{
    
    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle) ,  getPistolBulletSpeed() * std::sin(m_angle) });

}

void Boss::fireWithShotgun()
{


    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle) ,  getPistolBulletSpeed() * std::sin(m_angle) });
    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle+0.2f) ,  getPistolBulletSpeed() * std::sin(m_angle+ 0.2f) });
    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle- 0.2f) ,  getPistolBulletSpeed() * std::sin(m_angle- 0.2f) });
    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle+ 0.5f) ,  getPistolBulletSpeed() * std::sin(m_angle + 0.5f) });
    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle - 0.5f) ,  getPistolBulletSpeed() * std::sin(m_angle- 0.5f) });
    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle + 0.7f) ,  getPistolBulletSpeed() * std::sin(m_angle + 0.7f) });
    new Fireball(m_owner, m_position, Vec2{ getPistolBulletSpeed() * std::cos(m_angle - 0.7f) ,  getPistolBulletSpeed() * std::sin(m_angle - 0.7f) });
}


Vec2 Boss::getPosition() const
{
    return m_position;
}

void Boss::handleInputs(const sf::Event& event)
{

}

void Boss::update(float deltaTime)
{
    m_allGameObjects = getOwner().getAllGameOjects();
    findValidTarget();
    int toto = 5;
    m_rootNode.tick();
    m_angle = calculateAngleToTarget();
    Vec2 acceleration{ 0.f, 0.f };

    if (m_speed==0)
        acceleration = -getBossFluidFrictionCoef() * m_velocity;

    if (m_speed>0)
		acceleration += m_speed * Vec2 { std::cos(m_angle), std::sin(m_angle) };


    m_position += m_velocity * deltaTime;
    m_velocity += acceleration * deltaTime;

    if (m_velocity.getLength() > 60)
        m_velocity = m_velocity * (60 / m_velocity.getLength());
}

void Boss::render(sf::RenderWindow& window)
{
    m_sprite.setRotation(m_angle / 3.14159265f * 180.f);
    m_sprite.setOrigin(getBossSize().x / 2.f, getBossSize().y / 2.f);
    m_sprite.setPosition(m_position.x, m_position.y);
    window.draw(m_sprite);
}

AABB Boss::getBoundingBox() const
{
    return { m_position - getBossSize() / 2.f, m_position + getBossSize() / 2.f };
}

GameObjectType Boss::gameObjectType()
{
    return ENEMY_TYPE;
}
