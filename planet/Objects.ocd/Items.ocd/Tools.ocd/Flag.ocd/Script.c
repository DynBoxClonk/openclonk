/*
	Flag
	Author: Marky

	High resolution version of the original flag :)
*/

public func IsBaseFlag(){ return true;} // Can mark a home base

private func Flying()
{
	SetDir(BoundBy(GetWind()/10+4, 0, 8));

	if (!GetActTime() && GetActionTarget())
	{
		SetCategory(31 & GetActionTarget()->GetCategory());
	}
}

func ResetCategory()
{
	SetCategory(C4D_Object);
}

func OnOwnerChanged(int new_owner, int old_owner)
{
	// Flag lost its owner? Fall down!
	if (new_owner == NO_OWNER) SetAction("Idle");
}


public func SetBase(object homebase, bool actual_call)
{
	// the GUI stuff causes a problem in the flag. If it is collected, 
	// then it is ejected an idle right away, so it won't stay a flag
	// the trick is to delay the actual call by 1 frame
	if (!actual_call)
	{
		ScheduleCall(this, this.SetBase, 1, 0, homebase, true);
	}
	else
	{
		if (Contained()) Exit();
		homebase->SetOwner(GetOwner());
		SetAction("FlyBase", homebase);
		AddEffect("IntBaseCheck", homebase, 1, 1, this);
		DoTrumpet();
	
		this.Collectible = 0;
	}
}

func DoTrumpet()
{
	Sound("Interface::Trumpet");
}

public func LostBase()
{
	this.Collectible = 1;
	SetAction("Idle");
}

private func FxIntBaseCheckTimer(object target, proplist effect, int time)
{
	if (!target)
	{
		SetAction("Idle");
		LostBase();
		return FX_Execute_Kill;
	}

	if (ActIdle())
	{
		target->~OnFlagLost();
		LostBase();
		return FX_Execute_Kill;
	}
}

/*-- Saving --*/

public func SaveScenarioObject(proplist props)
{
	if (!inherited(props, ...)) return false;
	if (GetActionTarget() && GetAction() == "FlyBase") props->AddCall("Home base", this, "SetBase", GetActionTarget());
	return true;
}


local ActMap = {
FlyBase = {
	Prototype = Action,
	Name = "FlyBase",
	Procedure = DFA_ATTACH,
	Directions = 9,
	FlipDir = 5,
	Length = 6,
	Delay = 3,
	X = 0,
	Y = 12,
	Wdt = 24,
	Hgt = 12,
	NextAction = "FlyBase",
	StartCall = "Flying",
	AbortCall = "ResetCategory",
},
};


local Collectible = true;
local Name = "$Name$";
local Description = "$Description$";
local UsageHelp = "$UsageHelp$";
local Rebuy = true;
local BorderBound = 1;
