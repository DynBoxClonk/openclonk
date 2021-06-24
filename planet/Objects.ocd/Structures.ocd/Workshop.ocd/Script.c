/*
 *  Workshop
 *
 *  Author: Marky
 */


#include Library_Structure
#include Library_Ownable
#include Basement72
#include Library_DoorControl
#include Library_Producer

static const WORKSHOP_AnimSlot_Working = 1;

local hold_production;

func Definition(id def)
{
	def.MeshTransformation = Trans_Mul(Trans_Rotate(35, 0, 1, 0),
	                                   Trans_Translate(-3000));
	def.PictureTransformation = Trans_Mul(Trans_Rotate( 35, 0, 1, 0), 
	                                      Trans_Rotate( -5, 0, 0, 1),
	                                      Trans_Rotate(-10, 1, 0, 0),
	                                      Trans_Translate(1000, -5000, -2000));
}

func NoConstructionFlip(){ return true; } // not supported

public func Initialize()
{
	hold_production = false;
	return _inherited(...);
}


/*-- Production --*/

public func IsProduct(id product_id)
{
	return product_id->~IsToolProduct()
	    || product_id->~IsInventorProduct()
	    || product_id->~IsShipyardProduct()
	    || product_id->~IsArmoryProduct();
}

private func ProductionTime(id product_id)
{
	if (product_id->~IsToolProduct()) return 150;
	if (product_id->~IsInventorProduct()) return 100;
	if (product_id->~IsShipyardProduct()) return 400;
	if (product_id->~IsArmoryProduct()) return 100;

	return 150;
}

public func PowerNeed() { return 50; }


public func DoBuildAnimation(bool build)
{
	if (build)
	{
		Sound("Structures::Gear", nil, nil, nil, 1);
		SetAnimAction("Build", WORKSHOP_AnimSlot_Working);
	}
	else
	{
		Sound("Structures::Gear", nil, nil, nil, -1);
		SetAnimAction("Idle", WORKSHOP_AnimSlot_Working);
	}
}


public func OnProductionStart(id product)
{
	DoBuildAnimation(true);
	hold_production = false;
	return;
}


public func OnProductionHold(id product)
{
	DoBuildAnimation(false);
	hold_production = true;
	return;
}


public func OnProductionContinued(id product)
{
	DoBuildAnimation(true);
	hold_production = false;
	return;
}


public func OnProductionFinish(id product)
{
	DoBuildAnimation(false);
	return;
}


protected func Working()
{
	if (!hold_production) Smoking();
}


private func Smoking()
{  
	if (Random(6)) Smoke(-17 * GetCalcDir(), -24, 8);
	if (Random(8)) Smoke(-9 * GetCalcDir(), -24, 5+Random(3));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

public func IsHammerBuildable() { return true; }

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 100;
local ContainBlast = 1;
local HitPoints = 100;
local Components = {Wood = 6, Metal = 3};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
OpenDoor = {
	Prototype = Action,
	Name = "OpenDoor",
	Length = 20,
	Delay = 1,
	NextAction = "DoorOpen",
	StartCall = "SoundOpenDoor",
	Animation = "OpenDoor",
},

DoorOpen = {
	Prototype = Action,
	Name = "DoorOpen",
	Delay = 20,
	NextAction = "CloseDoor",
	StartCall = "OpenEntrance",
	EndCall = "CloseEntrance",
	Animation = "DoorOpen",
},

CloseDoor = {
	Prototype = Action,
	Name = "CloseDoor",
	Length = 15,
	Delay = 1,
	NextAction = "Idle",
	StartCall = "SoundCloseDoor",
	EndCall = "DoorClosed",
	Animation = "CloseDoor",
},

Build = {
	Prototype = Action,
	Name = "Build",
	Procedure = DFA_NONE,
	Length = 20,
	Delay = 1,
	NextAction = "Build",
	PhaseCall = "Working",
	Animation = "Build",
},

};
