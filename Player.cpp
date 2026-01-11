#include "Player.hpp"

std::string Player::getName() const { return name; }
int Player::getScore() const { return score; }
void Player::setName(const std::string& input) { name = input; }
void Player::addToScore(int input) { score += input; }
