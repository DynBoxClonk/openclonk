/*-- Coal --*/

protected func Construction()
{
	var graphic = Random(5);
	if (graphic)
		SetGraphics(Format("%d", graphic));
}

protected func Hit(x, y)
{
	StonyObjectHit(x, y);
	return true;
}

public func IsFuel() { return true; }
public func GetFuelAmount(bool get_partial) 
{ 
	if (get_partial)
		return GetCon();
	return 100;
}


local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local Rebuy = true;
local BlastIncinerate = 5;
local ContactIncinerate = 1;
local Plane = 460;
local Components = {Wood = 2};