/*
 * Bird egg
 *
 * Author: Marky, original found in Clonk Rage
 */
 
#include Library_AnimalEgg

private func Initialize()
{
	this.MeshTransformation = Trans_Mul(Trans_Scale(800, 800, 800), Trans_Rotate(RandomX(25, 35), 0, 1, 0));
	_inherited(...);
}

private func Destruction()
{
	ClearScheduleCall(this, this.FinishBreeding);
	_inherited(...);
}

private func Fertilize(id type, int breedTime)
{
	//Log("Fertilize %v %d", type, breedTime);
	var child = CreateConstruction(type, 0, 0, -1, Max(10, type->~AnimalReproductionBirthSize()));
	if (child)
	{
		child->ForceEnter(this);
		ScheduleCall(this, this.FinishBreeding, breedTime);
	}
	RemoveTimer(this.Breed);

	return child;
}

private func FinishBreeding()
{
	Hatch();
}

private func Entrance(object container)
{
	if (!(container->~IsNest()))
	{
		ClearScheduleCall(this, this.FinishBreeding);
		for (var obj in FindObjects(Find_Container(this)))
		{
			if (obj) obj->RemoveObject();
		}
	}
}

private func GetEggColor() { return RGB(239, 177, 54); } // For particle effects

local Name = "$Name$";
local Description = "$Description";
local Collectible = true;
