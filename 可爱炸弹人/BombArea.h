#ifndef BOMBAREA_H

#define BOMBAREA_H

#include "TNT.h"
#include "Prop.h"

//爆炸区域
class BombArea : public obj_base
{
public: 
	BombArea(sigPosType x, sigPosType y, int ownerID, Prop::propType bomb) 
		: obj_base(x, y, false, 0), ownerID(ownerID), bomb(bomb) {}

	virtual objType GetObjType() const override { return objType::bombArea; }

	virtual ~BombArea() {}
private: 
	
	int ownerID; 
	Prop::propType bomb;	//产生这个爆炸区域的武器类型
};

#endif	// #ifndef BOMBAREA_H
