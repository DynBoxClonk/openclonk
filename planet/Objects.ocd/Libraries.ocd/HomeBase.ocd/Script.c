/*
 *	The Home Base
 *
 *	Author: Marky
 */

#include Library_Base
#include Library_Vendor

private func Collection2(object item)
{
	if (item->~IsBaseFlag())
	{
		if (!IsBase() && item->GetOwner() != NO_OWNER)
		{
			item->~SetBase(this);
			MakeBase(true);
		}
		ScheduleCall(this, this.UpdateInteractionMenus, 2);
	}

	_inherited(item);
}

private func OnFlagLost()
{
	UpdateInteractionMenus();
	MakeBase(false);
}

/* --- Buy materials at base --- */

// Provides an interaction menu for buying things.
public func HasInteractionMenu() { return true; }

public func AllowBuyMenuEntries() { return GetFlag() != nil;}
public func AllowSellMenuEntries() { return GetFlag() != nil;}

public func GetInteractionMenus(object clonk)
{
	var menus = _inherited(clonk) ?? [];

	// only open the menus if ready
	var base_menu =
	{
		title = "$MsgBaseControl$",
		entries_callback = this.GetBaseControlMenuEntries,
		callback = "OnBaseControlSelection",
		callback_hover = "OnBaseControlHover",
		callback_target = this,
		BackgroundColor = GetPlayerColor(GetOwner()),
		Priority = 60
	};
	PushBack(menus, base_menu);
	
	return menus;
}


public func GetBaseControlMenuEntries(object clonk)
{
	var menu_entries = [];
	
	if (GetFlag())
	{
		PushBack(menu_entries,
			{symbol = this, extra_data = clonk,
				custom =
				{
					Right = "100%", Bottom = "2em",
					BackgroundColor = {Std = 0, OnHover = 0x50ff0000},
					image = {Right = "2em", Symbol = GetFlag()},
					text = {Left = "2em", Text = "$MsgRemoveFlag$"},
					Bottom = "1.2em",
					Priority = -1,
					BackgroundColor = RGB(25, 100, 100),
				}});
	}
	
	return menu_entries;
}

public func OnBaseControlSelection(id symbol, object clonk)
{
	RemoveFlag(clonk);
	UpdateInteractionMenus(this.GetBaseControlMenuEntries);	
}

public func OnBaseControlHover(id symbol, object clonk, desc_menu_target, menu_id)
{
	var text = "$DescRemoveFlag$";
	GuiUpdateText(text, menu_id, 1, desc_menu_target);
}

public func RemoveFlag(object clonk)
{
	var flag = GetFlag();
	
	if (flag == nil) return false;
	
	flag->LostBase();
	
	var x = flag->GetX();
	var y = flag->GetY();
	
	if (!clonk->Collect(flag))
	{
		flag->SetPosition(x, y);
	}
	
	return true;
}

public func GetFlag()
{
	return FindObject(Find_Func("IsBaseFlag"), Find_Owner(GetOwner()), Find_ActionTarget(this));
}

protected func OnStructureDestroyed()
{
	var flag = GetFlag();
	if (flag != nil)
	{
		flag->~LostBase();
	}
	
	_inherited(...);
}

/* --- Placement --- */

public func PlaceHomebase(int player, proplist settings, proplist area)
{
	// Defaults, these are hard-coded here.
	settings = settings ?? {};
	settings.max_tries = 1000;
	settings.completion = 100;
	settings.owner = player;

	var bases = this->Place(1, area, settings);
	if (GetLength(bases) == 1)
	{
		// Create a flag
		var homebase = bases[0];
		homebase->SetOwner(player);
		homebase->CreateContents(ClassicFlag);
		
		// Place everything in the home base
		for (var item in FindObjects(Find_Owner(player), 
		                             Find_NoContainer(),
		                             Find_Or(Find_OCF(OCF_CrewMember), Find_OCF(OCF_Collectible), Find_OCF(OCF_Grab))))
		{
			item->Enter(homebase);
		}
		
		// Let the first crew exit
		GetHiRank(player)->SetCommand("Exit");
		return homebase;
	}
	else
	{
		// TODO: Give the conkit and construction material to the crew?
	}
}
