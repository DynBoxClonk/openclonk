/*--- Earth ---*/

protected func Construction()
{
	var graphic = Random(5);
	if (graphic)
		SetGraphics(Format("%d", graphic));
}

protected func Hit()
{
	
	CastPXS("Sand", 200, 18);
	Sound("Hits::SoftHit?");
	RemoveObject();
	return 1;
}

local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local Plane = 450;