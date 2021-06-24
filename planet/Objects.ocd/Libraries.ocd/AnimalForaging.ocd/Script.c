/*
 * Animal foraging
 *
 * Handles how animals eat and feed.
 *
 * Author: Marky
 */

// activity calls
static const ANIMAL_ActivityCall_Forage = "Forage"; 	// this call is used for finding food and following baits

private func AnimalAddActivityCalls() // this should be overloaded by the other libraries for adding new stuff
{
	PushBack(this.animalActivityList, ANIMAL_ActivityCall_Forage);
	_inherited(...);
}

