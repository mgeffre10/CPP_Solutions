#pragma once
#ifndef TEXTADVENTURE_GAMEMANAGER
#define TEXTADVENTURE_GAMEMANAGER

#include "Player.h"
#include "Dungeon.h"
#include "Structs.h"
#include "Enums.h"
#include "Spider.h"

#include <ctime> // time
#include <map>
#include <random> // mt19937
#include <string>
#include <string_view>

class GameManager
{
private:
	Player m_Player{};
	Dungeon m_dungeonMap{};
	static std::map<std::string_view, std::string_view> m_commandList;

public:

	void constructDungeon();
	void intro();
	GameStatus gameLoop();
	void determineResponseToMovement(int roomId, RoomType roomType);
	CombatStatus combatLoop(Enemy &enemy);
	std::string parseInput(const std::string &input);
	void executeCommand(const std::string_view &command);
	void printCommandList();
	void outro(const GameStatus &gameStatus);

};

#endif