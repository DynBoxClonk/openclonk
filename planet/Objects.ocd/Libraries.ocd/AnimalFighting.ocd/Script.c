/*
 * Animal fighting
 *
 * Handles animals fighting or escaping enemies.
 *
 * Author: Marky
 */

// activity calls
static const ANIMAL_ActivityCall_Fight = "Fight"; 	// this call is used for shelter habits

private func AnimalAddActivityCalls() // this should be overloaded by the other libraries for adding new stuff
{
	PushBack(this.animalActivityList, ANIMAL_ActivityCall_Fight);
	_inherited(...);
}


