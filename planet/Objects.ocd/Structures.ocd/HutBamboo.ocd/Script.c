/*
 * Bamboo hut
 *
 * Author: Marky
 */

#include Library_Structure
#include Library_Ownable
#include Library_HomeBase
#include Library_DoorControl


func Definition(id def)
{
	def.MeshTransformation = Trans_Mul(Trans_Rotate(5, 0, 1, 0));
	def.PictureTransformation = Trans_Mul(Trans_Rotate(-10, 0, 1, 0), 
	                                      Trans_Rotate( -5, 0, 0, 1),
	                                      Trans_Rotate(-10, 1, 0, 0));
}

func NoConstructionFlip(){ return true; } // not supported

func IsContainer(){ return true; }

protected func SoundOpenDoor() { return nil;}
protected func SoundCloseDoor() { return nil;}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 10;
local ContactIncinerate = 3;
local Components = {Wood = 3};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
OpenDoor = {
	Prototype = Action,
	Name = "OpenDoor",
	Length = 10,
	Delay = 1,
	NextAction = "DoorOpen",
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
	Length = 10,
	Delay = 1,
	NextAction = "Idle",
	Animation = "CloseDoor",
},

};
