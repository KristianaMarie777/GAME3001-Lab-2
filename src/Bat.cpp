#include "Bat.h"
#include "Game.h"
#include "Util.h"

//here

Bat::Bat() :m_maxspeed(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/bat.png", "bat");

	auto size = TextureManager::Instance()->getTextureSize("bat");

	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;

	setType(BAT);

	setOrientation(glm::vec2(0.0f, -1.0f));
	setRotation(0.0f);
	setAccelerationRate(10.0f);
	setTurnRate(10.0f);
}

Bat::~Bat()
= default;

void Bat::draw()
{

	
	
	
	TextureManager::Instance()->draw("bat", getTransform()->position.x, getTransform()->position.y, m_rotationAngle, 255, true);

	Util::DrawLine(getTransform()->position, (getTransform()->position + getOrientation() * 60.0f));

}

void Bat::update()
{


	if (seeking)
		m_SMove();
	if (fleeing)
		m_FMove();
}

void Bat::clean()
{
}

void Bat::setAvoidDestination(const glm::vec2 destination)
{
	m_avoidDestination = destination;
}

void Bat::setDestination(const glm::vec2 destination)
{
	m_destination = destination; 
}

void Bat::setMaxSpeed(const float speed)
{
	m_maxspeed = speed;
}

glm::vec2 Bat::getOrientation() const
{
	return m_orientation;
}


float Bat::getTurnRate() const
{
	return m_turnRate;
}

void Bat::setTurnRate(const float rate)
{
	m_turnRate = rate;
}

float Bat::getAccelerationRate() const
{
	return m_accelerationRate;
}

void Bat::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void Bat::Fleeing(bool a)
{
	fleeing = a;
}

void Bat::Seeking(bool a)
{
	seeking = a;
}


void Bat::setOrientation(const glm::vec2 orientation)
{

	m_orientation = orientation;
	
}

void Bat::setRotation(const float angle)
{
	m_rotationAngle = angle;

	
	
	const auto offset = -90.0f;
	const auto angle_in_radians = (angle + offset) * Util::Deg2Rad;

	const auto x = cos(angle_in_radians);
	const auto y = sin(angle_in_radians);
	
	// convert angle to normalized vector and store it in Orientation

	setOrientation(glm::vec2(x, y));
}

float Bat::getRotation() const
{
	return m_rotationAngle;
}

void Bat::m_SMove()
{

	auto deltaTime = TheGame::Instance()->getDeltaTime();
	
	//magnitude
	m_targetDirection = m_destination - getTransform()->position;

	//normalize direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if(abs(target_rotation) > turn_sensitivity)
	{
		//check this V
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}
		
	}


	
	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();
	
	//using formula pf = pi + vi*t+0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime) + 
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxspeed);
	
	getTransform()->position += getRigidBody()->velocity;
}

void Bat::m_FMove()
{

	auto deltaTime = TheGame::Instance()->getDeltaTime();

	//magnitude
	m_targetDirection = (m_avoidDestination - getTransform()->position)*-1.0f;

	//normalize direction
	m_targetDirection = Util::normalize(m_targetDirection);

	auto target_rotation = Util::signedAngle(getOrientation(), m_targetDirection);

	auto turn_sensitivity = 5.0f;

	if (abs(target_rotation) > turn_sensitivity)
	{
		//check this V
		if (target_rotation > 0.0f)
		{
			setRotation(getRotation() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setRotation(getRotation() - getTurnRate());
		}

	}



	getRigidBody()->acceleration = getOrientation() * getAccelerationRate();

	//using formula pf = pi + vi*t+0.5ai*t^2
	getRigidBody()->velocity += getOrientation() * (deltaTime)+
		0.5f * getRigidBody()->acceleration * (deltaTime);

	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, m_maxspeed);

	getTransform()->position += getRigidBody()->velocity;
}