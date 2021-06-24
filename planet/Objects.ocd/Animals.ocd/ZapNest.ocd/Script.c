/*-- Zap nest --*/

protected func Hit(){	Break(28);}
//public func Exhaustion() { Break(14);} // what was that for??

public func Break(int num)
{
	var x, y;
	if (!num) num=28;
	for (var i=0;i<num;++i)
	{
		x=Random(11)-5;
		y=Random(11)-5;
		if (!GBackSolid(x, y))
			CreateObject(Zap, x, y, NO_OWNER);
	}
	RemoveObject();
}

local Name="$Name$";
local Description="$Description$";
local Collectible = true;

