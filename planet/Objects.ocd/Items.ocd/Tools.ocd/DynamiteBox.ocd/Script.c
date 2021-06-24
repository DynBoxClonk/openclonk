/**
	Dynamite box
	Contains five dynamite sticks which can be placed and detonated from a distance. 
*/

#include DynamiteBox

func Construction()
{
	_inherited(...);
	SetGraphics(nil, DynamiteBox);
}

local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 1;
local ContactIncinerate = 2;
local Components = {Wood = 2, GunPowder = 2, Sulphur = 3};
