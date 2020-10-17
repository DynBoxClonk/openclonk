/*-- 
	Repair statue
	Author: Sven2
	
	Player must repair the statue
--*/


#include Library_Goal

local statue;

public func SetStatue(object to_statue)
{
	statue = to_statue;
	return true;
}


/*-- Goal interface --*/

// The goal is fulfilled if a statue has been assigned and it's repaired.
public func IsFulfilled()
{
	return statue && !statue->IsBroken();
}

public func GetDescription(int plr)
{
	var message;
	if (IsFulfilled())
		message = "$MsgGoalFulfilled$";		
	else
		message = "$MsgGoalUnfulfilled$";
	return message;
}

// Shows or hides a message window with information.
public func Activate(proplist plr)
{
	// If goal message open -> hide it.
	if (GetEffect("GoalMessage", this))
	{
		CustomMessage("", nil, plr, nil, nil, nil, nil, nil, MSG_HCenter);
		RemoveEffect("GoalMessage", this);
		return;
	}
	// Otherwise open a new message.
	AddEffect("GoalMessage", this, 100, 0, this);
	var message;
	if (IsFulfilled())
	{
		message = "@$MsgGoalFulfilled$";		
	}
	else
	{
		message = "@$MsgGoalUnfulfilled$";
	}
	CustomMessage(message, nil, plr, 0, 16 + 64, 0xffffff, GUI_MenuDeco, this, MSG_HCenter);
	return;
}

protected func FxGoalMessageStart() {}

//public func GetShortDescription(int plr) { return ""; }

/*-- Proplist --*/

local Name = "$Name$";
