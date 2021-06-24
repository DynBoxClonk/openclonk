/**
	Basement Library
	Basic library for basements, handles:
	* Damage
	* Creation
	
	Author: Marky, original found in Clonk Rage
*/

/* Engine Overrides */

protected func Construction()
{
	_inherited(...);
	ScheduleCall(this, this.CreateBasement, 1);
}
  
protected func Destruction()
{
	RemoveBasement();
	_inherited(...);
}

/* Fundament */

local basement;
static const FX_BasementHandler = "IntBasementHandler";

private func BasementID(){ return nil; }
private func BasementWidth(){ return 0; }


private func CreateBasement()
{
	if (!GetEffect(FX_BasementHandler, this)) CreateEffect(IntBasementHandler, 1, 0, BasementID());
}


private func RemoveBasement()
{
	RemoveEffect(nil, this, GetEffect(FX_BasementHandler, this));
}


/* Effects */

local IntBasementHandler = new Effect
{
	Start = func (int temporary, id basementID)
	{
		if (temporary) return;
	
		if (this.Target->GetID() == basementID) return;
	
		if (!this.basement)
		{
			this.basement = this.Target->CreateObjectAbove(basementID, 0, -1, this.Target->GetOwner());
			
			var x = this.basement->GetX();
			var defBottom = this.Target->GetDefBottom();
			var offsetY = this.basement->GetDefOffset(1);
			var y = defBottom - offsetY;
			//Log("Creating basement at %d %d (%d - %d", x, y, defBottom, offsetY);
			
			this.basement -> SetPosition(x, y);
			this.basement -> MoveOutClonks();
			this.basement -> SetCategory(C4D_StaticBack);
			this.Target->~SetBasement(this.basement);
		}
	},
	
	Stop = func (int reason, bool temporary)
	{
		if (temporary) return;
	
		if (this.basement)
		{
			this.basement->RemoveBasement();
		}
	},
};


/* Just copied, needs better code! */

private func MoveOutClonks()
{
	var clonk;
	if (GetID() != BasementID()) return;

	var tol = 10;

	for (clonk in FindObjects(Find_InRect(-tol-BasementWidth()/2,-15, +2*tol + BasementWidth(), +25),
                                Find_OCF(OCF_Collectible | OCF_Grab | OCF_Alive),
                                Find_NoContainer(),
                                Find_Not(Find_Func("IsBridge")),
                                Find_Not(Find_Category(C4D_Structure))))
		MoveOutClonk(clonk);
}
  
private func MoveOutClonk(object clonk)
{
	if (GetID() != BasementID()) return;

	while (clonk->Stuck() && Inside(clonk->GetY() - this->GetY(), -15, +5))
		clonk->SetPosition(clonk->GetX(), clonk->GetY() - 1);  
}

local Name="$Name$";
local Description="$Description$";

