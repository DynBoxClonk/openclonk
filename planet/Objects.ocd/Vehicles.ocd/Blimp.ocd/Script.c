#include Airship

local Components = {Metal = 6, Wood = 8};

func Construction()
{
	_inherited(...);
	SetGraphics(nil, Airship);
}

private func AirshipPilot()
{
	// Looks for a clonk within the gondola.
	return FindObject(Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_InRect(gondola[0], gondola[1], gondola[2], gondola[3]));
}


func Definition(def)
{
	def.PictureTransformation = Trans_Mul(Trans_Rotate(-25, 1, 0, 0), Trans_Rotate(40, 0, 1, 0));
}

local Name = "$Name$";
local Description = "$Description$";
