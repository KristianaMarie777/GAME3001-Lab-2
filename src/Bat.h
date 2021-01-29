#pragma once
#ifndef _BAT_H_
#define _BAT_H_
#include "DisplayObject.h"
#include "TextureManager.h"

class Bat final : public DisplayObject
{
public:
	// constructor(s)
	Bat();

	// destructor
	~Bat();

	// life-cycle methods inherited from DisplayObject
	void draw() override;
	void update() override;
	void clean() override;

	// getters and setters
	
	void setDestination(const glm::vec2 destination);
	void setMaxSpeed(float speed);
	glm::vec2 getOrientation() const;
	void setOrientation(glm::vec2 orientation);
	float getRotation()const;
	void setRotation(float angle);
	float getTurnRate() const;
	void setTurnRate(float rate);
	float getAccelerationRate() const;
	void setAccelerationRate(float rate);

	void Seeking(bool a);

private:

	glm::vec2 m_destination;
	glm::vec2 m_targetDirection;
	glm::vec2 m_orientation;

	float m_maxspeed;
	float m_rotationAngle;
	float m_turnRate;
	float m_accelerationRate;

	bool seeking = false;

	//private function
	void m_Move();
	
};

#endif 