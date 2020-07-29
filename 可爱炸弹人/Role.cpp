#include "Role.h"

void Role::Reset(bool resetScore)
{
	distance = 2; 
	tntNum = 1; 
	nowTntNum = 0; 
	canPushTnt = false; 
	haveDefence = false; 
	isMissing = false; 
	weapon = NULL; 
	if (resetScore) score = 0; 
}

bool Role::LayTNT()
{
	if (nowTntNum == tntNum) return false; 
	++nowTntNum; 
	return true; 
}

void Role::TNTBomb()
{
	if (nowTntNum > 0) --nowTntNum; 
	throw 1; 
}

void Role::AddMoveSpeed()
{
	moveSpeed *= 2; 
	if (moveSpeed > posUnitPerCell) moveSpeed = posUnitPerCell; 
}

void Role::SubMoveSpeed()
{
	moveSpeed /= 2; 
	if (moveSpeed == 0) moveSpeed = 1; 
}
