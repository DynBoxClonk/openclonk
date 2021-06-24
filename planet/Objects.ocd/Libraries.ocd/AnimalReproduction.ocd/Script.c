/*
 * Animal reproduction
 *
 *
 * Author: Marky
 */


static const ANIMAL_ActivityCall_Reproduction = "Reproduction";

// Population control
private func AnimalReproductionAreaSize() { return 800; }   // Size of the area where animals can reproduce
private func AnimalReproductionRate()     { return 4000; }  // Chance for reproduction in a given time interval
private func AnimalReproductionMaxCount()       { return 10; }    // Maximum amount of animals in the area
private func AnimalReproductionBirthSize() { return 40; } // Starting con


private func AnimalAddActivityCalls() // this should be overloaded by the other libraries for adding new stuff
{
	PushBack(this.animalActivityList, ANIMAL_ActivityCall_Reproduction);
	_inherited(...);
}

// Special behaviour, e.g. monster eggs
private func AnimalReproductionCustom()
{
}

// Special conditions, for example fish with swimming animation
private func AnimalReproductionCondition()
{
	return true;
}

private func AnimalReproductionTooMany()
{
//	var reproductionControl = FindObject(Find_ID(REPR));
//	if (!reproductionControl)
//	{
//		if (AnimalReproductionCount() >= AnimalReproductionMaxCount()) return true;
//	}
//	else
//	{
//		if (ObjectCount(GetID())+1 >= GetComponent(GetID(), 0, reproductionControl)) return true;
//	}

	return false;
}

private func AnimalIsAdult()
{
	return GetCon() >= 100;
}

// Count animals
private func AnimalReproductionCount()
{
	return ObjectCount(Find_ID(GetID()), Find_Distance(AnimalReproductionAreaSize()), Find_OCF(OCF_Alive));
}

/* Reproduction */

private func FxIntAnimalActivityReproduction( object target, proplist effect)
{
	if (target != this) return;

	// Check conditions...
	if (!GetAlive()) return;
	if (!AnimalIsAdult()) return;
	if (!AnimalReproductionCondition()) return;
	if (AnimalReproductionTooMany()) return;

	// Reproduce!
	if (!AnimalReproductionCustom())
	{
		// standard reproduction
		var pChild = CreateConstruction(GetID(), 0, 0, -1, AnimalReproductionBirthSize());
		pChild->~AnimalBirth();
	}
	//return true;
}

/* Birth */

public func AnimalBirth()
{
	if (!GetEffect("IntAnimalGrowth", this))
		 AddEffect("IntAnimalGrowth", this, 1, AnimalGrowthRate(), this);
}

public func AnimalGrowthRate(){ return 45; }

protected func FxIntAnimalGrowthTimer(object target, proplist effect)
{
	if (target->GetCon() >= 100) return -1;
	target->DoCon(1);
}
