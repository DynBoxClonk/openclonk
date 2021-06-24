/**
	Animal hunt

	A certain amount of animals has to be killed.
	
	The goal uses the following callbacks in objects to
	to determine information:
	* IsAnimalType(id|string type) [optional] used to find out whether this
	                          object is a certain animal type.
	                          Has to return 'false' if the animal is dead.
	
	@author Marky
*/

#include Library_Goal


local list_animals;	// List of animal types to be hunted

private func Initialize()
{
	list_animals = [];
	return inherited(...);
}

private func FindAnimals(animal)
{
	var find = FindObjects(Find_Func("IsAnimalType", animal));
	if (GetType(animal) == C4V_Def)
	{
		var all = Concatenate(find, FindObjects(Find_ID(animal), Find_OCF(OCF_Alive)));
		RemoveDuplicates(all);
		return all;
	}
	return find;
}

/*-- Interface --*/

/**
 Determines what animals have to be killed.
 
 @par id|string type This type of animal has to killed. All animals
                     with this ID count as remaining animals, or
                     if an object the function IsAnimalType(id|string type)
                     returns true in an object.

 @par min_amount [optional] This many animals have to be killed at least
 @par placed_by_scenario [optional] The value 'true' means, that the scenario
                         is responsible for placing the animals. Should it fail
                         to do so, you will automatically fulfill the goal,
                         because there are no animals left to rescue.
                         On the other hand, 'false' means that the goal will
                         place the minimum amount of animals that need to be rescued,
                         or 1-4 animals if no amount was specified. 
 */
public func SetAnimal(type, int min_amount, bool placed_by_scenario)
{
	// Set goal values
	SaveSetting(type);

	// Do some logic for placing the minimal amount
	var count = GetLength(FindAnimals(type));
	var to_place = min_amount- count;
	if (!placed_by_scenario)
	{
		// Add animals in case there are none
		if (count == 0 && to_place <= 0)
		{
			to_place = RandomX(1, 4);
		}

		// Place them
		var animal = GetAnimalID(type);
		if (animal) animal->Place(to_place);
	}
	return this;
}

/*-- Scenario saving --*/

private func SaveSetting(type, int allowed_deaths)
{
	var index = GetIndexOf(list_animals, type);
	if (-1 == index) index = GetLength(list_animals);

	list_animals[index] = type;
}

public func SaveScenarioObject(props)
{
	if (!inherited(props, ...)) return false;
	for (var i = 0; i < GetLength(list_animals); i++)
	{
		props->AddCall("Goal", this, "SaveSetting", list_animals[i]);
	}
	return true;
}


/*-- Goal interface --*/


// The goal is fulfilled if all specified animals have been killed.
public func IsFulfilled()
{
	for (var i = 0; i < GetLength(list_animals); i++)
	{
		var animal = list_animals[i];
		var remaining = GetLength(FindAnimals(animal));
		if (remaining) return false;
	}
	// Goal fulfilled.
	return true;
}


// Shows or hides a message window with information.
public func GetDescription(int player)
{
	var message;
	if (IsFulfilled())
	{
		message = "$MsgGoalFulfilled$";		
	}
	else
	{
		message = "$MsgGoalAnimalHunt$";
		for (var i = 0; i < GetLength(list_animals); i++)
		{
			// Current data
			var animal = list_animals[i];
			var remaining = GetLength(FindAnimals(animal));
			var icon = GetAnimalID(animal);
	
			// Put the message together, do not add this to the list if there are none left
			var add_msg;
			if (remaining > 0)
			{
				add_msg = Format("$MsgGoalRemaining$", icon, remaining);
				message = Format("%s%s", message, add_msg);
			}
		}
	}
	return message;
}

// Shows or hides a message window with information.
public func Activate(int player)
{
	// If goal message open -> hide it.
	if (GetEffect("GoalMessage", this))
	{
		CustomMessage("", nil, player, nil, nil, nil, nil, nil, MSG_HCenter);
		RemoveEffect("GoalMessage", this);
		return;
	}
	// Otherwise open a new message.
	AddEffect("GoalMessage", this, 100, 0, this);

	CustomMessage(GetDescription(player), nil, player, 0, 16 + 64, 0xffffff, GUI_MenuDeco, this, MSG_HCenter);
	return;
}


public func GetAnimalID(animal)
{
	// Already known?
	var type = _inherited(animal, ...);
	if (type)
	{
		return type;
	}
	// This one is easy :)
	if (GetType(animal) == C4V_Def)
	{
		return animal;
	}
	else // This one is not - the goal has to implement some value locally, if the type is "monster" or whatever
	{
		return nil;
	}
}

private func FxGoalMessageStart() {}

public func GetShortDescription(int player)
{
	// Show animal image. Displays the one with least remaining.
	var remaining = [];
	
	var index = 0;
	for (var i = 0; i < GetLength(list_animals); i++)
	{
		// Current data
		remaining[i] = GetLength(FindAnimals(list_animals[i]));
		
		if (Inside(remaining[i], 1, remaining[index] - 1))
		{
			index = i;
		}
	}
	
	// Put the message together
	var icon = GetAnimalID(list_animals[index]);
	return Format("{{%i}}: %d", icon, remaining[index]);
}


/*-- Proplist --*/

local Name = "$Name$";
