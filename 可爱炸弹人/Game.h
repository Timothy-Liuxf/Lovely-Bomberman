#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include <list>
#include <vector>

class Game
{
public: 
	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 

private: 
	
	enum class mapItem
	{
		blank = 0,			//空格
		birthPoint = 1,		//出生点
		softObstacle = 2,	//软障碍
		hardObstacle = 3	//硬障碍
	};
	//游戏地图
	static const std::vector<std::vector<std::vector<int>>> gameMap; 
	static const unsigned int numOfMap; 
};

#endif	//#ifndef GAME_H
