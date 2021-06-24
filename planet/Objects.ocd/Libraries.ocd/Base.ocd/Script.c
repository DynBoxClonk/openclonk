/*--- The Base ---*/

// Author: Randrian

// Determines it the Building is acutally a base
local library_homebase;

public func Construction()
{
	if (!library_homebase)
	{
		library_homebase = 
		{
			is_base = false,
			healing_capacity = 0,
		};
	}
	return _inherited(...);
}

// ---------------- Settings for base funcionallity --------------------
// --- these functions can be overloaded for vendors or special bases ---

// Determines if the base can heal allied clonks
public func CanHeal() { return true; }
// The amount of energy the base can store, if none the base can't heal; precision = 1
public func GetHeal() { return 100000; }
// The amount of energy that is healed per frame; precision = 1 
public func GetHealRate() { return 200;}
// The money one filling of the bases energy storage costs
public func GetHealCost() { return 5;}

// Determines if the base can extinguish allied clonks
public func CanExtinguish() { return true; }

// Does the base block enemies?
public func CanBlockEnemies() { return true; }


// ------------- Base states -------------------------------

// This Building can be a base
public func IsBaseBuilding() { return true; }

// This Building is a base at the moment
public func IsBase() { return library_homebase.is_base; }

// Makes this building a base or removes the base functionallity
public func MakeBase(bool enable, int healing_capacity)
{
	library_homebase.is_base = enable ?? true;
	library_homebase.healing_capacity = Max(0, healing_capacity);
	if (library_homebase.is_base)
	{
		CreateEffect(FxHomeBase, 1, 10);
	}
	else
	{
		RemoveEffect("FxHomeBase", this);
	}
}

// ---------- Saving -----------

public func SaveScenarioObject(proplist props)
{
	if (!inherited(props, ...)) return false;
	props->AddCall("HomeBase", this, "MakeBase", library_homebase.is_base, library_homebase.healing_capacity);
	return true;
}

// ---------- Healing, Extinguishing and Autosell -----------

local FxHomeBase = new Effect
{
	Timer = func ()
	{
		// Can this base heal? Then look for clonks that need some
		var crew;
		if (Target->CanHeal() && Target->GetHeal())
			for (crew in FindObjects(Find_Container(Target), Find_OCF(OCF_CrewMember), Find_Allied(Target->GetOwner())))
			{
				if (crew->GetEnergy() < crew->GetMaxEnergy() && !GetEffect("FxHomeBaseHeal", crew))
					crew->CreateEffect(Target.FxHomeBaseHeal, 1, 1, Target);
			}
		// Can this base extinguish? Then look for something on fire
		var burning;
		if (Target->CanExtinguish())
			for (burning in FindObjects(Find_Container(Target), Find_OCF(OCF_OnFire), Find_Allied(Target->GetOwner())))
				burning->Extinguish();
	},
};

local FxHomeBaseHeal = new Effect
{
	Base = nil,

	Start = func(int temporary, object base)
	{
		if (!temporary)
		{
			Base = base;
		}
	},

	Timer = func()
	{
		// The clonk has left the base? Stop!
		if (Target->Contained() != Base) return FX_Execute_Kill;
		// Full energy? Stop too.
		if (Target->GetEnergy() >= Target->GetMaxEnergy()) return FX_Execute_Kill;
		var owner = Base->GetOwner();
		var rate = Base->GetHealRate();
	
		// No energy left? Buy some
		if (Base.library_homebase.healing_capacity < rate)
		{
			var cost = Base->GetHealCost();
			if (GetWealth(owner) >= cost)
			{
				DoWealth(owner, -cost);
				Base->Sound("UI::UnCash?", {player = owner});
				Base.library_homebase.healing_capacity += Base->GetHeal();
			}
		}
		// Some energy in the storage? heal clonk
		if (Base.library_homebase.healing_capacity >= rate)
		{
			Target->DoEnergy(rate, 1, FX_Call_EngBaseRefresh, owner + 1);
			Base.library_homebase.healing_capacity -= rate;
		}
	},
};
