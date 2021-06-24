/*-- Wind generator --*/

#include WindGenerator
#include Basement26

local DefaultFlagRadius = 1000;

func Definition(id def)
{
	def.MeshTransformation = Trans_Translate(-1200, -11000, 0);
	def.PictureTransformation = Trans_Mul(Trans_Translate(2000, 0, 7000),
	                                      Trans_Rotate(-20, 1, 0, 0),
	                                      Trans_Rotate( 30, 0, 1, 0));
}

func NoConstructionFlip(){ return true; } // not supported

protected func Construction()
{
	_inherited(...);
	this.MeshTransformation = Trans_Mul(Trans_Rotate(RandomX(-5, 5), 0, 1, 0), 
	                                    Trans_Translate(-1200, -11000, 0));
}

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 60;
local ContactIncinerate = 5;
local HitPoints = 50;
local Components = {Wood = 4, Metal = 1};
