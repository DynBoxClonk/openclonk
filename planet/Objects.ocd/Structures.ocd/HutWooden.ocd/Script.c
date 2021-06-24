/*
 *	Wooden cabin
 *
 *	Author: Marky
 */

#include Library_Structure
#include Library_Ownable
#include Library_HomeBase
#include Basement60
#include Library_DoorControl


local BlastIncinerate = 45;
local HitPoints = 45;
local Components = {Wood = 4, Rock = 1};

func IsContainer(){ return true; }

public func IsHammerBuildable() { return true; }

func NoConstructionFlip(){ return true; } // not supported

local ActMap = {
OpenDoor = {
	Prototype = Action,
	Name = "OpenDoor",
	Procedure = DFA_NONE,
	Length = 20,
	Delay = 1,
	NextAction = "DoorOpen",
	StartCall = "SoundOpenDoor",
	Animation = "OpenDoor",
},
DoorOpen = {
	Prototype = Action,
	Name = "DoorOpen",
	Procedure = DFA_NONE,
	Length = 10,
	Delay = 2,
	NextAction = "CloseDoor",
	StartCall = "OpenEntrance",
	EndCall = "CloseEntrance",
	Animation = "DoorOpen",
},
CloseDoor = {
	Prototype = Action,
	Name = "CloseDoor",
	Procedure = DFA_NONE,
	Length = 40,
	Delay = 1,
	NextAction = "Idle",
	StartCall = "SoundCloseDoor",
	Animation = "CloseDoor",
},
};

local Name = "$Name$";
local Description = "$Description$";
local ContainBlast = true;
