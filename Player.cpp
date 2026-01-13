#include "Player.hpp"

void Player::setName(const std::string& input) { name = input; }
std::string Player::getName() const { return name; }
int Player::getScore() const { return score; }
void Player::addToScore(int input) { score += input; }
int Player::getLives() const { return lives; }
void Player::loseLife() {
	if (lives > 0) --lives;
}
void Player::reset() {
	score = 0;
	lives = 2;
}
