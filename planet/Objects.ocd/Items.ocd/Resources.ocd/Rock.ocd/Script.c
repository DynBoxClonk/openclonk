/*--- Rock ---*/

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

local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local Plane = 450;
