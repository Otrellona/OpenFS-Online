#pragma once

class PlayerManager {
public:
	string name;

	sf::Text name_balance;
	sf::Font font;

	int money_p;
	int max_worker;
	int cur_worker;

	void NewGameSettings() {
		money_p = 100;
		max_worker = 1;
		cur_worker = max_worker;
	}

	void draw(RenderWindow& window, int x)
	{
		font.loadFromFile("textures/font/arial.ttf");

		name_balance.setFont(font);
		name_balance.setString(name + std::to_string(money_p));
		name_balance.setCharacterSize(24);
		name_balance.setFillColor(sf::Color::Black);

		name_balance.setPosition(10, 15 * x);
		window.draw(name_balance);
	};
};
