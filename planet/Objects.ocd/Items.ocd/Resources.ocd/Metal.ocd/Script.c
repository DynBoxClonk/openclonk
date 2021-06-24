/*--- Metal ---*/

#include WallKit

private func MaxStackCount() { return 1; }

private func Construction()
{
	_inherited(...);
	if (GBackSemiSolid())
		SetGraphics("Old");
}

private func Hit()
{
	Sound("Hits::Materials::Metal::LightMetalHit?");
}

public func IsFoundryProduct() { return true; }
public func GetFuelNeed() { return 100; }
public func IsTool() { return false; }
public func IsToolProduct() { return false; }

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;
local Rebuy = true;
local Plane = 470;
local Components = {Ore = 1, /* Fix contents of wallkit */ Metal = 0};
