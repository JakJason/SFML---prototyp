#pragma once
#include "Chunk.h"
#include <SFML/Graphics.hpp>


class Moveable {
private:
	float speed_per_second;

public:

	virtual void move() = 0;
};