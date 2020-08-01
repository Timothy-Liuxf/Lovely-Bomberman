#ifndef TNT_H

#define TNT_H

#include "basic.h"

/////////////////////////////////////
//
//  关于炸弹
//

//普通炸弹类
class TNT : public obj_base
{
public: 
	TNT(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed, int ownerID, int distance, int timeLeft) 
		: obj_base(x, y, isMoving, moveSpeed), ownerID(ownerID), distance(distance), timeLeft(timeLeft), state(tntState::laid) {}

	int GetOwnerID() const { return ownerID; }
	int GetDistance() const { return distance; }

	virtual objType GetObjType() const override { return objType::tnt; }

	//获取剩余爆炸或存留时间
	int GetTimeLeft() const { return timeLeft; }

	//减少爆炸或存留时间
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//是否将要爆炸或消失
	bool AboutToDisappear() const { return timeLeft <= 0; }

	//爆炸
	void Bomb(int newTimeLeft) { timeLeft = newTimeLeft; state = tntState::bomb; }

	//是否处于放置状态
	bool IsLaid() const { return state == tntState::laid; }

	//是否处于爆炸状态
	bool IsBomb() const { return state == tntState::bomb; }

	virtual ~TNT() {}

private: 

	enum class tntState
	{
		laid,	//放置状态
		bomb	//爆炸状态
	};

	int ownerID;	//主人的ID
	int distance;	//爆炸范围
	int timeLeft;	//剩余爆炸时间
	tntState state; //所处状态
}; 


#endif  //#ifndef BOMB_H
