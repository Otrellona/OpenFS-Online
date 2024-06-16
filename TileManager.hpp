#include <SFML/Graphics.hpp>
#pragma once

class TileManager {
public:
	sf::Sprite tile;
	std::string color;

	int lives;
	int growtime;
	int tomato_grow_time = 10;

	bool isWater;

	int digtime;
	int planttime;
	int gathertime;
};
