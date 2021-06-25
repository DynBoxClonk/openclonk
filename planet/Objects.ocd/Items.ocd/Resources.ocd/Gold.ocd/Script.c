/*--- Nugget ---*/

protected func Construction()
{
	var graphic = Random(3);
	if (graphic)
		SetGraphics(Format("%d", graphic));
}

protected func Hit(x, y)
{
	StonyObjectHit(x, y);
	return true;
}

public func IsFoundryIngredient() { return true; }
public func IsValuable(){ return true; }


local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local Plane = 470;