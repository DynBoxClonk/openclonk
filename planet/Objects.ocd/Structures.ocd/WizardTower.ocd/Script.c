/*
 *	Wizard Tower
 *
 *	Author: Marky, Mesh: constantin, Texture: constantin
 */

#include Library_ClassicStructure
#include Library_Ownable

#include Library_DoorControl

#include Basement42


func NoConstructionFlip(){ return true; } // not supported

local BlastIncinerate = 100;
local HitPoints = 100;

local Name="$Name$";
local Description = "$Description$";
local ContainBlast = true;
local Components = {Rock = 4, Wood = 2, Metal = 2};