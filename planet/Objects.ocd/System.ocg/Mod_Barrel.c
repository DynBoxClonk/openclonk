#appendto Barrel


public func CalcValue(object in_base, proplist for_player)
{
	return GetDefValue();
}

public func QueryOnSell(proplist for_player, object in_base)
{
	if (Contents() && in_base)
	{
		// Sell contents first
		for (var contents in FindObjects(Find_Container(this)))
		{
			in_base->~DoSell(contents, for_player);
		}
		return true;
	}
	return false;
}
