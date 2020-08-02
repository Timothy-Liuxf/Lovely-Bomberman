#ifndef OBSTACLE_H

#define OBSTACLE_H

#include "obj_base.h"

//’œ∞≠ª˘¿‡
class Obstacle : public obj_base
{
public: 
	Obstacle(sigPosType x, sigPosType y) : obj_base(x, y, false, 0) {}
	virtual ~Obstacle() {}
};

class SoftObstacle final : public Obstacle
{
public:
	SoftObstacle(sigPosType x, sigPosType y) : Obstacle(x, y) {}
	virtual objType GetObjType() const override { return objType::softObstacle; }
	virtual ~SoftObstacle() {}
};

class HardObstacle final : public Obstacle
{
public:
	HardObstacle(sigPosType x, sigPosType y) : Obstacle(x, y) {}
	virtual objType GetObjType() const override { return objType::hardObstacle; }
	virtual ~HardObstacle() {}
};

#endif // #ifndef OBSTACLE_H
