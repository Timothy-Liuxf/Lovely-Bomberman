#ifndef ROLE_H

#define ROLE_H

////////////////////////////////////////////
//
//  角色类
//

#include "Prop.h"
#include <list>

//角色类
class Role : public obj_base
{
public: 

	Role(sigPosType x, sigPosType y, int moveSpeed, int id)
		: obj_base(x, y, false, moveSpeed),
		id(id), 
		distance(2), 
		tntNum(1), 
		nowTntNum(0), 
		score(0),
		canPushTnt(false), 
		haveDefence(false), 
		isMissing(false), 
		weapon(NULL) {}

	virtual objType GetObjType() const override { return objType::role; }

	//重设角色（是否重设分数）
	void Reset(bool resetScore); 
	
	//移动
	void MoveUp() { Move(direction::Up); }
	void MoveDown() { Move(direction::Down); }
	void MoveLeft() { Move(direction::Left); }
	void MoveRight() { Move(direction::Right); }

	//放置TNT
	bool LayTNT(); 
	//放置的TNT爆炸
	void TNTBomb(); 

	//增加最多放置的炸弹数
	void AddTntNum() { ++tntNum; }
	//获取最多放置的炸弹数
	int GetTntNum() const { return tntNum; }
	//获取剩余炸弹数
	int GetLeftTntNum() const { return tntNum - nowTntNum; }

	//获取炸弹爆炸范围
	int GetDistance() const { return distance; }
	//增加炸弹爆炸范围
	void AddDistance() { ++distance; }

	//获取分数
	int GetScore() const { return score; }
	//增加分数
	void AddScore(int add) { score += add; }

	//增加移动速度
	void AddMoveSpeed(); 
	//减慢移动速度
	void SubMoveSpeed(); 
	
	//能否推动炸弹
	bool CanPushTnt() const { return canPushTnt; }
	//获得手套——推动Tnt的能力
	void GetGlove() { canPushTnt = true; }

	//是否具有盾牌防御
	bool HaveDefence() const { return haveDefence; }
	//获得盾牌
	bool GetDefence() { haveDefence = true; }

	//是否处于被炸后的保护状态
	bool IsMissing() const { return isMissing; }
	//设置保护状态
	void SetMissing(bool missing) { isMissing = missing; }

	//获取武器
	SpecialBomb* GetWeapon() const { return weapon; }
	//重设武器
	void SetWeapon(SpecialBomb* newWeapon) { weapon = newWeapon; }
	
	~Role() {}

private: 
	
	int id;					//角色ID: 1~4
	int distance;			//炸弹的爆炸范围
	int tntNum;				//最多可以同时放置的炸弹数
	int nowTntNum;			//目前已经放置的炸弹数
	int score;				//分数
	bool canPushTnt;		//能否推动TNT
	bool haveDefence;		//是否拥有盾牌防御
	bool isMissing;			//是否正在处于刚被炸的保护状态
	SpecialBomb* weapon;	//手中持有的武器
};


#endif  // #ifndef ROLE_H
