

/**
 Places an object on surfaces.
 
 @par settings the possible settings:
               - no_stuck => true does place objects if they are stuck
               - liquid => true finds in liquid, false finds everything but liquid
               - location => set to additional Loc_*() functions if desired.
 */
global func PlaceOnSurface(int amount, proplist rectangle, proplist settings)
{
	AssertDefinitionContext();

	var max_tries = 4 * amount;

	if (!settings) settings = {};
	settings.no_stuck = settings.no_stuck ?? false;
	settings.liquid = settings.liquid ?? false;
	
	
	var loc_area = nil;
	if (rectangle) 
		loc_area = Loc_InArea(rectangle);

	var objects = [];
	var loc_bkg;
	if (settings.liquid)
	{
		loc_bkg = Loc_Liquid();
	}
	else
	{
		loc_bkg = Loc_And(Loc_Or(Loc_Tunnel(), Loc_Sky()), Loc_Not(Loc_Liquid()));
	}

	while ((amount > 0) && (--max_tries > 0))
	{
		var spot = FindLocation(loc_bkg, loc_area, Loc_Wall(CNAT_Bottom), settings.location);
		if (!spot)
			continue;
		
		var thing = CreateObject(this, spot.x, spot.y, NO_OWNER);
		if (!thing) 
			continue;
		
		if (settings.no_stuck && thing->Stuck())
		{
			thing->RemoveObject();
			continue;
		}
		PushBack(objects, thing);
		--amount;
	}
	
	return objects;
}


global func PlaceByCriteria(int amount)
{
	AssertDefinitionContext();
	var max_tries = 4 * amount;
	var placed = [];
	while ((amount > 0) && (--max_tries > 0))
	{
		var spot = FindLocation(...);
		if (!spot) continue;

		var obj = CreateObject(this, spot.x, spot.y, NO_OWNER);
		PushBack(placed, obj);
		--amount;
	}
	return placed;
}

/* -- Special placement -- */


global func PlaceInEarth(int amount, int in_relation_to, int level, int divisor)
{
	AssertDefinitionContext();
	PlaceInMaterialByLevel("Earth", amount, in_relation_to, level, divisor);
}


global func PlaceInMaterialByLevel(string material, int amount, int in_relation_to, int level, int divisor)
{
	level = level ?? 100;
	PlaceInMaterial(material, amount, in_relation_to, ConvertInMatAmount(material, level), divisor);
}


global func PlaceInMaterial(string material, int amount, int in_relation_to, int factor, int divisor)
{
	AssertDefinitionContext();
	in_relation_to = Max(1, in_relation_to);
	factor = factor ?? 1;
	divisor = Max(1, divisor);
	var total = (factor * amount / in_relation_to) / divisor;
	//Log("Will place %d; from: %d/%d, to_place = %d, divisor = %d", total, amount, in_relation_to, factor, divisor);
	PlaceObjects(this, total, material);
}


/**
 Adjusts an amount to the map size, if 
 the scenario has a function 'AdjustAmountToMapSize'.
 Otherwise leaves the amount as it is.
 */
global func AdjustToMapSize(int amount)
{
	if (Scenario.AdjustAmountToMapSize)
	{
		return Scenario->AdjustAmountToMapSize(amount);
	}
	else
	{
		return amount;
	}
}

/* -- Location conditions -- */

 // The original is broken, because liquid can count as sky...
global func Loc_Sky() {	return Loc_Material("Sky"); }


global func LocFunc_HasSunLight(int x, int y)
{
	var limit = y - 250;
	for (var ynew = y; ynew >= limit; ynew -= 5)
	{
		if (GBackSky(x, ynew))
		{
			return true;
		}
	}
	return false;
}

