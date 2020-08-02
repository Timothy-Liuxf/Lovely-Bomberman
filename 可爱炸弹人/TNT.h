#ifndef TNT_H

#define TNT_H

#include "obj_base.h"

/////////////////////////////////////
//
//  关于炸弹
//

//普通炸弹类
class TNT : public obj_base
{
public: 
	TNT(sigPosType x, sigPosType y, bool isMoving, sigPosType moveSpeed, int ownerID, int distance, int timeLeft) 
		: obj_base(x, y, isMoving, moveSpeed), ownerID(ownerID), distance(distance), timeLeft(timeLeft) {}

	int GetOwnerID() const { return ownerID; }
	int GetDistance() const { return distance; }

	virtual objType GetObjType() const override { return objType::tnt; }

	//获取剩余爆炸时间
	int GetTimeLeft() const { return timeLeft; }

	//减少爆炸或存留时间
	void SubTimeLeft(int subTime) { timeLeft -= subTime; }

	//是否将要爆炸
	bool AboutToDisappear() const { return timeLeft <= 0; }

	virtual ~TNT() {}

private: 

	int ownerID;	//主人的ID
	int distance;	//爆炸范围
	int timeLeft;	//剩余爆炸时间
}; 


#endif  //#ifndef BOMB_H
