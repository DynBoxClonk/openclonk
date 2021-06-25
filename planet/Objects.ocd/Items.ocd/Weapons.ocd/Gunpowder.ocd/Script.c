/*--- Gun powder ---*/


protected func Hit()
{
	Sound("Hits::Materials::Wood::WoodHit?");
}  
  
protected func Damage()
{
	// Does not explode while carried by a clonk
	if (Contained() && Contained()->~IsClonk())
    {
		Extinguish();
	}
	else
	{
		// Explosion - delayed to avoid endless recursion
		ScheduleCall(this, this.DoExplode, 1);
	}
}

public func OnShockwaveHit(level, x, y)
{
	Incinerate();
}

public func DoExplode()
{
	Explode(30);
}

public func IsChemicalProduct() { return true;}
public func IsArmoryProduct() { return true;}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;
local BlastIncinerate = 1;
local ContactIncinerate = 1;
local Components = {Coal = 1};
