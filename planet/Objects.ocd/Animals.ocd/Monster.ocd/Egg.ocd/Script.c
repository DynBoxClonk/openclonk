#include Library_AnimalEgg
#include Library_CarryHeavy

/* -- Carry heavy stuff -- */

private func GetCarryTransform(clonk)
{
	var offset;
	if (GetCarrySpecial(clonk))
	{
		offset = Trans_Translate(3000, 0, 0);
	}
	else
	{
		offset = Trans_Identity();
	}
	return Trans_Mul(Trans_Rotate(90, 0, 0, 1), offset);
}

private func GetCarryBone() { return "Main"; }
private func GetCarryPhase() { return 500; }

/* -- Animal hunt goal -- */

private func IsAnimalType(type)
{
	return type == Monster
 	    || type == GOAL_AnimalType_Monster;
}

/* -- Birth -- */

private func CreateHatchling()
{
	var baby = CreateObjectAbove(Monster, 0, 0, GetOwner());
	baby->SetCon(20);
	return baby;
}

/* -- Properties -- */

private func GetEggColor() { return RGB(243, 244, 242); } // For particle effects

local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
