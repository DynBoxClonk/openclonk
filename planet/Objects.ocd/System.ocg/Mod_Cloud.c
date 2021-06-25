#appendto Cloud

local rain_inserts_pxs = false;


// Changes the behavior of this cloud:
// if the parameter is true, the cloud will create material pxs
// Also an id call: Changes all clouds to this settings.
public func SetInsertPxs(bool should_insert)
{
	// Called to proplist: change all clouds.
	if (this == Cloud)
	{
		for (var cloud in FindObjects(Find_ID(Cloud)))
			cloud->SetInsertPxs(should_insert);
	}
	else // Otherwise change the clouds precipitation.
	{
		rain_inserts_pxs = should_insert;
	}
	return;
}

// Raindrop somewhere from the cloud.
private func RainDrop()
{
	if (rain_inserts_pxs)
	{
		var con = GetCon();
		var wdt = GetDefWidth() * con / 500;
		var hgt = GetDefHeight() * con / 700;
	
		var count = Max(rain_visual_strength, 1);
		for (var i = 0; i < count; ++i)
		{
			var x = RandomX(-wdt, wdt);
			var y = RandomX(-hgt, hgt);
			var xdir = RandomX(GetWind(0, 0, true) - 5, GetWind(0, 0, true) + 5) / 5;
			var ydir = 30;
			if (GBackSky(x, y))
			{
				InsertMaterial(Material(rain_mat), x, y, xdir, ydir);
			}
		}
	}
	else
	{
		return _inherited(...);
	}
}