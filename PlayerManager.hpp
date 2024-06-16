#pragma once

class PlayerManager {
public:
	int money_p;
	int max_worker;
	int cur_worker;

	void NewGameSettings() {
		money_p = 100;
		max_worker = 1;
		cur_worker = max_worker;
	}
};
