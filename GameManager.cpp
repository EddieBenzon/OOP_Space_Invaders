#include "GameManager.hpp"

GameManager::GameManager() : 
	shipTexture(LoadTexture("Assets/spaceship.png")), spaceship(&shipTexture, Vector2{400,500}) {
}
GameManager::~GameManager() {
	UnloadTexture(shipTexture);
}

void GameManager::Draw() {

}

void GameManager::HandleInput() {

}