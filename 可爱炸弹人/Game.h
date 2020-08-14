#ifndef GAME_H

#define GAME_H

#include "Role.h"
#include "Obstacle.h"
#include "TNT.h"
#include "BombArea.h"
#include <ctime>
#include <random>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <thread>
#include <functional>

#ifdef _DEBUG
#include <iostream>
#endif

class Game
{
private: 

	static const int playerInitialLife;						//玩家初始生命
	static const int playerMissingTime;						//玩家受到攻击后的受保护时间
	static const obj_base::sigPosType defPosUnitPerCell;	//玩每格的长度
	static const obj_base::sigPosType roleInitialMoveSpeed;	//玩家初始移动速度
	static const obj_base::sigPosType tntInitialMoveSpeed;	//炸弹初始移动速度
	static const int tntInitialDistance;					//炸弹初始爆炸距离
	static const int tntInitialTime;						//炸弹初始爆炸时间（毫秒）
	static const int tntBombAreaTime;						//炸弹的爆炸区域存留时间
	static const int mineInitialTime;						//炸弹和催泪瓦斯初始存留时间
	static const int grenadeMaxDistance;					//手榴弹移动距离
	static const int grenadeInitialTime;					//手榴弹爆炸持续时间
	static const int fireMaxDistance;						//火焰枪攻击距离
	static const int fireInitialTime;						//火焰枪火焰持续时间
	static const int scoreOfDestroyObstacle;				//摧毁障碍物得分
	static const int scoreOfHitOthers;						//击中其它角色得分
	static const int scoreOfPickProp;						//捡道具得分

	//各特殊炸弹移动速度
	static const std::map<Prop::propType, obj_base::sigPosType> propMoveSpeed;
	//产生道具的代号
	static const std::vector<Prop::propType> propNums; 
	//随机数产生
	std::default_random_engine randNum;	

public: 
	Game(int numOfPlayer, int id1, int id2); 

	int PosToCell(obj_base::sigPosType x) const { return x / defPosUnitPerCell; }
	obj_base::sigPosType CellToPos(int x) const { return x * defPosUnitPerCell + defPosUnitPerCell / 2; }

	const std::vector<std::vector<int>>& GetGameMap(unsigned int num) const; 
	std::list<obj_base*> GetMapObj(int x, int y) const; 
	int GetID1() const { return id1; }
	int GetID2() const { return id2; }
	int GetNowLevel() const { return nowLevel; }
	static int GetPosUnitPerCell() { return defPosUnitPerCell; }
	const Role* GetRole(int roleID) { return roles[roleID]; }
	//////const std::vector<Role*>& GetRoles() const { return roles; }
	//////const std::list<Obstacle*>& GetObstacles() const { return obstacles; }
	//////const std::list<obj_base*>& GetOtherGameObjs() const { return otherGameObjs; }

	int GetNumOfPlayer() const { return numOfPlayer; }
	int GetNumOfLevel() const { return (int)gameMap.size(); }

	//开始新的一关
	void InitNewLevel(int newLevel, bool mergeScore); 

	//以下是游戏行为
	
	//行走
	void WalkUpOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Up, stepInterval); }			//向上走一格
	void WalkDownOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Down, stepInterval); }		//向下走一格
	void WalkLeftOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Left, stepInterval); }		//向左走一格
	void WalkRightOneCell(int roleID, int stepInterval) { WalkOneCell(roleID, direction::Right, stepInterval); }	//向右走一格

	//放置炸弹
	void LayTnt(int roleID); 

	//以下是数据检查

	//检查人物
	void CheckRole(); 
	//检查炸弹以及爆炸区域，多重锁
	void CheckBomb(int dataScanInterval);
	//检查游戏是否结束
	bool CheckGameEnd() const; 
	
	//获取随机数
	auto GetRandom() { return randNum(); }

	~Game(); 

private: 

	using direction = obj_base::direction; 

	enum class mapItem
	{
		blank = 0,			//空格
		birthPoint = 1,		//出生点
		softObstacle = 2,	//软障碍
		hardObstacle = 3	//硬障碍
	}; 

	//角色，roles[0]是未使用的
	std::vector<Role*> roles; 
	//障碍
	std::list<Obstacle*> obstacles; 
	mutable std::mutex obstaclesMutex; 
	//其他
	std::list<obj_base*> otherGameObjs; 
	mutable std::mutex otherGameObjsMutex; 
	//回收站，防止两个线程删除同一个对象时产生的异常行为
	std::list<obj_base*> deletedObjs; 
	mutable std::mutex deletedObjsMutex; 

	int numOfPlayer;						//游戏人数
	int id1;								//1P的ID
	int id2;								//2P的ID（如果有的话）
	int nowLevel;							//当前关

	
	
	//游戏地图
	static const std::vector<std::vector<std::vector<int>>> gameMap; 

	//游戏行为
	void WalkOneCell(int roleID, direction direct, int stepInterval);		//行走一格
	bool MoveTnt(TNT* pTnt, direction direct);								//推动炸弹
	void BombTnt(TNT* pTnt);												//使该炸弹爆炸
	void BombFire(Fire* pFire);												//使该火焰枪爆炸
	void BombGrenade(Grenade* pGrenade);									//使该手榴弹爆炸
	void BombMissile(Missile* pMissile);									//导弹爆炸
	void BombMapCell(BombArea* pBombArea);									//爆破地图的一块
	void RoleMiss(Role *pRole); 											//管理角色受伤后的保护状态
	void CreateProp(int xc, int yc);										//在该点产生道具
};

#endif	//#ifndef GAME_H
