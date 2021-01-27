#pragma once
#ifndef __OBSTACLE__
#define __OBSTACLE__
#include <glm/vec2.hpp>

#include "DisplayObject.h"

class Obstacle final: public DisplayObject
{
public:
	Obstacle();

	~Obstacle();
	
	void draw() override;
	void update() override;
	void clean() override;

private:
};


#endif /* defined (__OBSTACLE__) */
