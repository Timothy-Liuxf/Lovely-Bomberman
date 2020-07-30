#include "Prop.h"

void SpecialBomb::SetLaid(sigPosType x, sigPosType y, direction laidDirect, int newMoveSpeed)
{
	pos = posType(x, y); 
	state = propState::laid; 
	direct = laidDirect; 
	moveSpeed = newMoveSpeed; 
}

void Grenade::Move() 
{ 
	if (AboutToDisappear()) return; 
	obj_base::Move(direct); 
	leftDistance -= moveSpeed;
}

