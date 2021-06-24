/**
	Dynamite
	A volatile tool that can be pressed into wallsfor accurate 
	mining, burning a short fuse before exploding.
*/

#include Dynamite

// time in frames until explosion
local FuseTime = 100;

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;

local BlastIncinerate = 1;
local ContactIncinerate = 1;
local Components = {GunPowder = 1, Sulphur = 1};
