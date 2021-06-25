/*--- Flint ---*/

protected func Construction()
{
	var graphic = Random(3);
	if (graphic) SetGraphics(Format("%d", graphic));
}

protected func Hit()
{
	Explode(this->ExplodeSize());
}

public func ExplodeSize() 
{
	 return 18;
}

local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local Plane = 530; // cause it's explosive, players should see it in a pile of stuff

/* Can be produced in the chemical plant */
public func IsChemicalProduct() { return true; }

func IsAlchemContainer() { return true ; }
func AlchemProcessTime() { return 100; }
local Components = {Sulphur = 1};
