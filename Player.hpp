#pragma once
#include <string>

class Player {
	std::string name;
	int score = 0;
	int lives = 3;
public:
	void setName(const std::string& input);
	std::string getName() const;
	void addToScore(int input);
	int getScore() const;
	void loseLife();
	int getLives() const;
	void reset();
};