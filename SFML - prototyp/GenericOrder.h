#pragma once
#include <SFML/Graphics.hpp>
#include "GenericOrder.h"
#include "GameObject.h"

class GenericOrder {

private:
	int x;

public:

	GenericOrder();

	/*
	virtual void cancel(GameObject& s) = 0;
	
	virtual void give(GameObject& s) = 0;

	virtual void apply(GameObject& s) = 0;

	virtual void remove(GameObject& s) = 0;
	*/
};