#pragma once
#include <string>

class Player {
	std::string name;
	int score;
public:
	std::string getName() const;
	int getScore() const;
	void setName(const std::string& input);
	void addToScore(int input);
};