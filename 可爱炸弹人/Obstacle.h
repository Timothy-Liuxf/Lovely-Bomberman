#ifndef OBSTACLE_H

#define OBSTACLE_H

#include "basic.h"

class SoftObstacle final : public obj_base
{
public:
	SoftObstacle(sigPosType x, sigPosType y) : obj_base(x, y, false, 0) {}
	virtual objType GetObjType() const override { return objType::softObstacle; }
	~SoftObstacle() {}
};

class HardObstacle final : public obj_base
{
public:
	HardObstacle(sigPosType x, sigPosType y) : obj_base(x, y, false, 0) {}
	virtual objType GetObjType() const override { return objType::hardObstacle; }
	~HardObstacle() {}
};

#endif // #ifndef OBSTACLE_H
