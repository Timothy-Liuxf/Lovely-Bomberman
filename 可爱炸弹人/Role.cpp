#include "Role.h"

void Role::Reset(bool resetTotalScore, bool resetLife)
{
	distance = 2; 
	tntNum = 1; 
	nowTntNum = 0; 
	canPushTnt = false; 
	haveShield = false; 
	isMissing = false; 
	weapon = NULL; 
	nowScore = 0; 
	if (resetTotalScore) totalScore = 0; 
	if (resetLife) life = initialLife; 
	if (life > 0) isLiving = true;
	else isLiving = false; 
}

bool Role::BeAttacked()
{
	if (!isLiving || isMissing) return false; 
	if (haveShield)
	{
		haveShield = false; 
		return true; 
	}
	//±»»÷ÖÐ
	isLiving = false; 
	--life; 
	return true; 
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

void Role::MergeScore()
{
	totalScore += nowScore; 
	nowScore = 0; 
}

Role::~Role()
{
	delete weapon; 
	weapon = nullptr; 
}

