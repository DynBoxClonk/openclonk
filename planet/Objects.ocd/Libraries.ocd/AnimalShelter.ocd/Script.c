/*
 * Animal shelter
 *
 * Some animals sleep in caves or build nests.
 *
 * Author: Marky
 */

// activity calls
static const ANIMAL_ActivityCall_Shelter = "Shelter"; 	// this call is used for shelter habits

private func AnimalAddActivityCalls() // this should be overloaded by the other libraries for adding new stuff
{
	PushBack(this.animalActivityList, ANIMAL_ActivityCall_Shelter);
	_inherited(...);
}

