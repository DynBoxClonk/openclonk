/*
 * Animal control
 *
 * Controls the basic behaviour of animals, specific behaviours can be plugged in.
 *
 * Author: Marky
 */

static const ANIMAL_Effect_Activity = "IntAnimalActivity";
static const ANIMAL_Effect_Turn = "IntAnimalTurn";

// activity calls
static const ANIMAL_ActivityCall_Status = "Status";			// this call is used for handling status changes
static const ANIMAL_ActivityCall_Movement = "Movement"; 	// this call is used for handling the animal movements

//// constants for activities, sorted by priority. The higher the number, the more important the activity
//static const ANIMAL_ActivityType_Idle 	= 0;
//static const ANIMAL_ActivityType_Sleep 	= 10;
//static const ANIMAL_ActivityType_Forage = 20;
//static const ANIMAL_ActivityType_Fight 	= 30;
//
// effect proplist documentation:
/**
 *
 * The effect passed to the FxIntAnimalActivityTimer has the following properties:
 *
 * - ignoreCalls	used	once this is set to true, all the other calls are ignored until the end of the activity cycle.
 * - remove			used	once this is set to true the effect will be removed completely from the animal on the next cycle.
 * - activity		unused	the current activity, see ANIMAL_ActivityType_* constants.
 * - shelter		used	the object where the animal shelters
 */


// Status
public func IsAnimal(){	return true;}
public func AnimalInitialAction(){	return "Walk";}

protected func AnimalIsCollectible(object pContainer){ return false; }
protected func AnimalActivityInterval(){ return 35;} // animal activity is executed in this interval in frames
protected func AnimalActivityCalls(){ return this.animalActivityList; } // Effect call list for the effect ANIMAL_Effect_Activity

protected func Construction()
{
	AnimalInit();
	_inherited(...);
}

private func AnimalInit()
{
	SetAction(this->~AnimalInitialAction());

	AnimalInitActivityCalls();

	if (Random(2))
		AnimalTurnLeft();
	else
		AnimalTurnRight();

	if ( AnimalGetActivityEffect() == nil)
		AddEffect(ANIMAL_Effect_Activity, this, 50, AnimalActivityInterval(), this);

	if ( GetEffect(ANIMAL_Effect_Turn, this) == nil)
		AddEffect(ANIMAL_Effect_Turn, this, 1, 1, this);
}

public func AnimalGetActivityEffect()
{
	return GetEffect(ANIMAL_Effect_Activity, this);
}

private func AnimalInitActivityCalls()
{
	if (GetType(this.animalActivityList) != C4V_Array)
		this.animalActivityList = CreateArray();

	AnimalAddActivityCalls();

	RemoveDuplicates(this.animalActivityList);
}

private func AnimalAddActivityCalls() // this should be overloaded by the other libraries for adding new stuff
{
	PushBack(this.animalActivityList, ANIMAL_ActivityCall_Movement);
	_inherited(...);
}

/* Collecting animals */

local fForceEnter;

// Force collection
public func ForceEnter(object pContainer)
{
	fForceEnter = true;
	Enter(pContainer);
	fForceEnter = false;
	return true;
}

protected func RejectEntrance(object pContainer)
{
	// Taking from a container is always ok
	if (Contained())
		return;
	// Collecting dead animals is ok, too
	if (!GetAlive())
		return;
	// forced collection is ok ;)
	if (fForceEnter)
		return;
	// In all other cases do an override
	return !AnimalIsCollectible(pContainer);
}

protected func FxIntAnimalActivityStart(object target, proplist effect, int temporary)
{
	if (temporary == 0)
	{
		effect.remove = false;
		effect.ignoreCalls = false;
	}

	return 1;
}

protected func FxIntAnimalActivityTimer(object target, proplist effect, int time)
{
	if (effect.remove) return -1;

	EffectCall(target, effect, ANIMAL_ActivityCall_Status); // this is always called

	if (effect.ignoreCalls) return;

	var call;
	for (call in target->~AnimalActivityCalls())
	{
		if (call == nil) break;

		EffectCall(target, effect, call);

		if (effect.ignoreCalls) break;
	}
}

/* Movement */

public func AnimalTurnRight()
{
	if (AnimalCanTurn())
	{
		if (GetXDir() < 0) SetXDir(0);
		SetDir(DIR_Right);
		SetComDir(COMD_Right);
		AnimalOnTurnRight();
	}
}

public func AnimalTurnLeft()
{
	if (AnimalCanTurn())
	{
		if (GetXDir() > 0) SetXDir(0);
		SetDir(DIR_Left);
		SetComDir(COMD_Left);
		AnimalOnTurnLeft();
	}
}

private func AnimalCanTurn(){ return !Stuck(); }
private func AnimalOnTurnLeft(){}
private func AnimalOnTurnRight(){}

