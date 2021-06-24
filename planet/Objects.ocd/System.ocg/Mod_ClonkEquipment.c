/**
	Starting equipment for the clonk
 */
 
#appendto Clonk

local starting_equipment = [];

private func Recruitment(int player)
{
	// Starting equipment
	EnsureHasContents(Shovel);
	EnsureHasContents(Axe);
	
	// Lock equipment
	AddToStartingEquipment(FindContents(Shovel));
	AddToStartingEquipment(FindContents(Axe));

	return _inherited(player, ...);
}

private func AddToStartingEquipment(object item)
{
	if (item)
	{
		// Make the item worthless
		item.CalcValue = this.CalcValueStartingEquipment;
		// Add it to the list
		PushBack(starting_equipment, item);
		RemoveDuplicates(starting_equipment);
	}
}

// custom throw
public func ControlThrow(object target, int x, int y)
{
	if (IsValueInArray(starting_equipment, target))
	{
		// Prevent item from being thrown, but dropping is OK
		return true;
	}

	return _inherited(target, x, y, ...);
}

private func CalcValueStartingEquipment() { return 0; }
