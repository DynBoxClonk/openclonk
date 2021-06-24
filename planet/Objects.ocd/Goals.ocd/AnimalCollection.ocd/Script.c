/**
	Animal rescue

	A certain amount of animals has to be collected.
	
	@author Marky
*/

#include Library_Goal


local list_animals;				// List of animal types to be collected.
local list_death_allowed;		// List of allowed deaths per animal.
local list_death_count;			// List of animals that are already dead.

private func Initialize()
{
	list_animals = [];
	list_death_allowed = [];
	list_death_count = [];
	CreateEffect(FxDeathMonitor, 1, 1);
	return inherited(...);
}

/*-- Interface --*/

/**
 Determines what animals have to be caught and sold.
 
 @par type This type of animal has to be sold.
 @par allowed_deaths [optional] This many animals are allowed to die.
 @par min_amount [optional] This many animals have to be rescued at least
 @par placed_by_scenario [optional] The value 'true' means, that the scenario
                         is responsible for placing the animals. Should it fail
                         to do so, you will automatically fulfill the goal,
                         because there are no animals left to rescue.
                         On the other hand, 'false' means that the goal will
                         place the minimum amount of animals that need to be rescued,
                         or 1-4 animals if no amount was specified. 
 */
public func SetAnimal(id type, int allowed_deaths, int min_amount, bool placed_by_scenario)
{
	// Set goal values
	SaveSetting(type, allowed_deaths);

	// Do some logic for placing the minimal amount
	var count = ObjectCount(Find_ID(type));
	var to_place = Max(min_amount, allowed_deaths) - count;
	if (!placed_by_scenario)
	{
		// Add animals in case there are none
		if (count == 0 && to_place <= 0)
		{
			to_place = RandomX(1, 4);
		}
		
		// Place them
		type->Place(to_place);
	}
	return this;
}

/*-- Scenario saving --*/

private func SaveSetting(id type, int allowed_deaths)
{
	var index = GetIndexOf(list_animals, type);
	if (-1 == index) index = GetLength(list_animals);
	
	list_animals[index] = type;
	list_death_allowed[index] = Max(0, allowed_deaths);
}

public func SaveScenarioObject(props)
{
	if (!inherited(props, ...)) return false;
	for (var i = 0; i < GetLength(list_animals); i++)
	{
		props->AddCall("Goal", this, "SaveSetting", list_animals[i], list_death_allowed[i]);
	}
	return true;
}


/*-- Goal interface --*/


// The goal is fulfilled if all specified animals have been rescued.
public func IsFulfilled()
{
	for (var i = 0; i < GetLength(list_animals); i++)
	{
		var animal = list_animals[i];
		var remaining = ObjectCount(Find_ID(animal), Find_OCF(OCF_Alive));

		// Allowed deaths are exceeded?
		if (list_death_count[i] > list_death_allowed[i])
		{
			EliminateAllPlayers();
			return false;
		}
		
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
		message = "$MsgGoalAnimalRescue$";
		for (var i = 0; i < GetLength(list_animals); i++)
		{
			// Current data
			var animal = list_animals[i];
			var remaining = ObjectCount(Find_ID(animal), Find_OCF(OCF_Alive));
			var icon = GetMessageIcon(animal);
			var forgiveness = Max(0, list_death_allowed[i] - list_death_count[i]);
	
			// Put the message together
			var add_msg;
			if (list_death_allowed[i] > 1)
			{
				add_msg = Format("$MsgGoalRescueWithDeathsLot$", icon, remaining, forgiveness);
			}
			else if (list_death_allowed[i] == 1)
			{
				add_msg = Format("$MsgGoalRescueWithDeathsOne$", icon, remaining, forgiveness);
			}
			else
			{
				add_msg = Format("$MsgGoalRescueNoDeaths$", icon, remaining); 
			}
			// Add only if you still have to catch them
			if (remaining > 0)
			{
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

private func GetMessageIcon(id animal)
{
	return animal; // Use the same icon for the animal as it is; can be overloaded, though
}

private func FxGoalMessageStart() {}

public func GetShortDescription(int player)
{
	// Show animal image. Displays the one with least deaths remaining or least to catch.
	var remaining = [];
	var forgiveness = [];
	
	var message_index = 0;
	
	for (var i = 0; i < GetLength(list_animals); i++)
	{
		// Current data
		var animal = list_animals[i];
		remaining[i] = ObjectCount(Find_ID(animal), Find_OCF(OCF_Alive));
		forgiveness[i] = Max(0, list_death_allowed[i] - list_death_count[i]);
		
		if (Inside(forgiveness[i], 1, forgiveness[message_index] - 1))
		{
			message_index = i;
		}
		else if (Inside(remaining[i], forgiveness[message_index] + 1, remaining[message_index] - 1))
		{
			message_index = i;
		}
	}
	

	// Put the message together
	var icon = GetMessageIcon( list_animals[message_index]);
	if (list_death_allowed[message_index] > 0)
	{
		return Format("{{%i}}: %d|{{%i}} %d", icon, remaining[message_index], Icon_Skull, forgiveness[message_index]);
	}
	else
	{
		return Format("{{%i}}: %d", icon, remaining[message_index]);
	}
}

/* -- Death monitors -- */

// Notifies the goal of the death of a target
local FxRegisterDeath = new Effect
{
	Start = func (temporary, object goal)
	{
		if (!temporary)
		{
			this.notify = goal;
		}
		return true;
	},
	
	Destruction = func (int reason)
	{
		if (FX_Call_RemoveDeath == reason)
		{
			if (this.notify)
			{
				this.notify->RegisterDeath(this.Target);
			}
		}
	}
};

// Adds death notification effects to animals
// Cannot register animals that are removed/killed during placement,
// but this is actually an advantage here because those should not count ;)
local FxDeathMonitor = new Effect
{
	Timer = func()
	{
		for (var animal in Target.list_animals)
		{
			if (!animal)
				continue;
				
			for (var living in FindObjects(Find_ID(animal), Find_OCF(OCF_Alive)))
			{
				if (!GetEffect("FxRegisterDeath", living))
				{
					living->CreateEffect(Goal_CollectAnimals.FxRegisterDeath, 1, nil, this.Target);
				}
			}
		}
	}
};

// Callback by death monitor
private func RegisterDeath(object animal)
{
	var type = animal->GetID();
	var index = GetIndexOf(list_animals, type);
	if (-1 != index)
	{
		list_death_count[index] += 1;
	}
}

/*-- Proplist --*/

local Name = "$Name$";
