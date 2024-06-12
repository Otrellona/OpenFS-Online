#pragma once

class PlayerManager {
public:
	int money_p;
	int tomato_p;
	int tomato_seed_p;

	void NewGameSettings() {
		money_p = 100;
		tomato_p = 0;
	}
};
