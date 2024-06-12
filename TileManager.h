#include <SFML/Graphics.hpp>
#pragma once

class TileManager {
public:
	sf::Sprite tile;
	int growtime;
	bool isWater;

	int tomato_grow_time = 10;
};
