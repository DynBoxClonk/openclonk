/*
 * Structures are always containers if the caller is inside of them!
 * 
 * Author: Marky
 */

#appendto Library_Structure

public func IsContainer(){ return true;}

public func Place(int amount, proplist rectangle, proplist settings)
{
	// No calls to objects, only definitions
	AssertDefinitionContext();

	// Default parameters	
	settings = settings ?? { };
	settings.owner = settings.owner ?? NO_OWNER;
	settings.completion = settings.completion ?? 100;
	settings.adjust_terrain = settings.adjust_terrain ?? true;
	settings.check_site = settings.check_site ?? false;
	settings.location = settings.location ?? Loc_Or(Loc_Sky(), Loc_Tunnel());
	settings.max_tries = settings.max_tries ?? 4;

	var max_tries = settings.max_tries * amount;
	
	rectangle = Shape->LandscapeRectangle();
	Shape->RectangleFit(rectangle, this);
	var loc_area = Loc_InArea(rectangle);
	
	var loc_space = Loc_And(
		Loc_Space(this->GetDefHeight(), CNAT_Top),
		Loc_Space(Abs(this->GetDefOffset()), CNAT_Left),
		Loc_Space(Abs(this->GetDefWidth() + this->GetDefOffset()), CNAT_Right)
	);
	
	// Start placing stuff
	var structures = [];

	while ((amount > 0) && (--max_tries > 0))
	{
		var spot = FindLocation(Loc_Wall(CNAT_Bottom), loc_space, loc_area, settings.location, Loc_Func(this.Loc_IsNotBlocked));
		if (!spot)
			continue;

		var struct = CreateConstruction(this, spot.x, spot.y, settings.owner, settings.completion, settings.adjust_terrain, settings.check_site);
		if (!struct) 
			continue;

		PushBack(structures, struct);
		--amount;
	}
	
	return structures;
}

private func Loc_IsNotBlocked(int x, int y)
{
	return !FindObject(Find_AtPoint(x, y), Find_OCF(OCF_Exclusive));
}
