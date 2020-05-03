/**
	Player Knowledge

	Library to control the players knowledge/plans:
     * GetPlayerKnowledge(int player, int index, int category)
     * HasPlayerKnowledge(int player, id plan)
     * SetPlayerKnowledge(int player, id plan, bool remove)
     * GivePlayerKnowledge(int player, id plan)
     * RemovePlayerKnowledge(int player, id plan)

    Should be incorporated into a player management system later

	@author Marky
*/

/* --- Properties --- */

// Local variables to store the player's knowledge.
local knowledge = [];



/* --- Global interface --- */

global func GetPlrKnowledge(int player, id plan, int index, int category)
{
	var manager = Library_PlayerKnowledge->GetKnowledgeManager(player);
	if (manager)
	{
		if (plan)
		{
			LogLegacyWarning("GetPlrKnowledge() using 'id' parameter", "HasPlayerKnowledge()", VERSION_10_0_OC);
			return manager->HasKnowledge(plan);
		}
		else
		{
			LogLegacyWarning("GetPlrKnowledge() using 'index/category' parameter", "GetPlayerKnowledge()", VERSION_10_0_OC);
			return manager->GetKnowledge(index, category);
		}
	}
}

global func GetPlayerKnowledge(int player, int index, int category)
{
	var manager = Library_PlayerKnowledge->GetKnowledgeManager(player);
	if (manager)
	{
		return manager->GetKnowledge(index, category);
	}
}

global func HasPlayerKnowledge(int player, id plan)
{
	var manager = Library_PlayerKnowledge->GetKnowledgeManager(player);
	if (manager)
	{
		return manager->HasKnowledge(plan);
	}
}

global func SetPlrKnowledge(int player, any plan, bool remove)
{
	if (remove)
	{
		LogLegacyWarning("SetPlrKnowledge() using 'bool remove' parameter", "RemovePlayesrKnowledge()", VERSION_10_0_OC);
		return RemovePlayerKnowledge(player, plan);
	}
	else
	{
		LogLegacyWarning("SetPlrKnowledge()", "GivePlayerKnowledge()", VERSION_10_0_OC);
		return GivePlayerKnowledge(player, plan);
	}
}


global func GivePlayerKnowledge(int player, any plan)
{
	// Do it for all players?
	if (player == nil)
	{
		var success = true;
		for (var i = 0; i < GetPlayerCount(); ++i)
		{
			success &= GivePlayerKnowledge(GetPlayerByIndex(i), plan);
		}
		return success;
	}

	// Do it for several plans?
	if (GetType(plan) == C4V_Array)
	{
		var success = true;
		for (var type in plan)
		{
			success &= GivePlayerKnowledge(player, type);
		}
		return success;
	}
	else
	{
		AssertTypeOneOf(C4V_Def, plan, "plan");
	}

	// Default handling for one player
	var manager = Library_PlayerKnowledge->GetKnowledgeManager(player);
	if (manager)
	{
		return manager->GiveKnowledge(plan);
	}
	return false;
}

global func RemovePlayerKnowledge(int player, id plan)
{
	// Do it for all players?
	if (player == nil)
	{
		var success = true;
		for (var i = 0; i < GetPlayerCount(); ++i)
		{
			success &= RemovePlayerKnowledge(GetPlayerByIndex(i), plan);
		}
		return success;
	}
	
	// Do it for several plans?
	if (GetType(plan) == C4V_Array)
	{
		var success = true;
		for (var type in plan)
		{
			success &= RemovePlayerKnowledge(player, type);
		}
		return success;
	}
	else
	{
		AssertTypeOneOf(C4V_Def, plan, "plan");
	}

	// Default handling for one player
	var manager = Library_PlayerKnowledge->GetKnowledgeManager(player);
	if (manager)
	{
		return manager->RemoveKnowledge(plan);
	}
	return false;
}


/* --- Definition Interface --- */

func GetKnowledgeManager(int player)
{
	var manager = FindObject(Find_ID(Library_PlayerKnowledge), Find_AnyLayer(),  Find_Owner(player));
	if (!manager)
	{
		manager = CreateObject(Library_PlayerKnowledge, 0, 0, player);
	}
	return manager;
}

/* --- Object Interface --- */


public func GetKnowledge(int index, int category)
{
	if (category)
	{
		var nr = -1;
		for (var plan in knowledge)
		{
			if (plan->GetCategory() & category)
			{
				nr += 1;
			}
			if (nr == index)
			{
				return plan;
			}
		}
		return nil;
	}
	else
	{
		return knowledge[index];
	}
}


public func HasKnowledge(id plan)
{
	return IsValueInArray(knowledge, plan);
}

public func GiveKnowledge(any plan)
{
	if (plan == nil)
	{
		return false;
	}
	if (!HasKnowledge(plan))
	{
		PushBack(knowledge, plan);
	}
	return true;
}

public func RemoveKnowledge(id plan)
{
	if (plan == nil)
	{
		return false;
	}
	return RemoveArrayValue(knowledge, plan);
}

// Do not save object, but save the callbacks
// Not sure if it works, but whatever
func SaveScenarioObject(proplist props)
{
	var player = GetOwner();
	for (var plan in knowledge)
	{
		props->Add("Knowledge", "GivePlayerKnowledge(%d, %i)", player, plan);
	}
	return false;
}
