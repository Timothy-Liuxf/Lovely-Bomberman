#include "basic.h"

obj_base::posType obj_base::minPos(0, 0); 
obj_base::posType obj_base::maxPos(0, 0); 
const obj_base::sigPosType obj_base::EPS = 1; 
int obj_base::posUnitPerCell = 0; 

void obj_base::SetMapBound(int rows, int cols, int posUnitPerCell)
{
	minPos.x = minPos.y = 0; 
	maxPos.x = rows * posUnitPerCell; 
	maxPos.y = cols * posUnitPerCell; 
	obj_base::posUnitPerCell = posUnitPerCell; 
}

void obj_base::CorrectPos()
{
	if (pos.x < minPos.x) pos.x = minPos.x;
	else if (pos.x >= maxPos.x) pos.x = maxPos.x - EPS; 
	if (pos.y < minPos.y) pos.y = minPos.y;
	else if (pos.y >= maxPos.y) pos.y = maxPos.y - EPS;
}

void obj_base::Move(direction direc)
{
	switch (direc)
	{
	case direction::Null: 
		break; 
	case direction::Up: 
		pos.x -= moveSpeed; 
		break; 
	case direction::Down: 
		pos.x += moveSpeed; 
		break; 
	case direction::Left: 
		pos.y -= moveSpeed; 
		break; 
	case direction::Right: 
		pos.y += moveSpeed; 
		break; 
	}
	CorrectPos(); 
}
