#appendto Library_Vendor

private func EjectAllContents()
{
	// Called when buying stuff, should do nothing, so that contents stay in the vendor
}


// ----- Selling

public func AllowSellMenuEntries(){ return true;}


private func GetSellableContents(int wealth_player, object container)
{
	// Defaults
	var inventory = [];
	container = container ?? this;
	if (!container)
	{
		return inventory;
	}
	// Get the contents
	var obj, i = 0;
	while (obj = container->Contents(i++))
	{
		if (obj->~QueryOnSell(wealth_player)) continue;
		
		var sellable = obj->~GetCategory() & C4D_Object
		            || obj->~GetCategory() & C4D_Vehicle
		            || obj->~GetCategory() & 65536/*C4D_TradeLiving*/;

		if (!sellable) continue;

		// Check if already exists (and then stack!)
		var found = false;

		// How many objects are this object?!
		var object_amount = obj->~GetStackCount() ?? 1;

		// Infinite stacks work differently - showing an arbitrary amount would not make sense.
		if (object_amount > 1 && obj->~IsInfiniteStackCount())
			object_amount = 1;

		// Empty containers can be stacked.
		for (var inv in inventory)
		{
			if (!inv.objects[0]->CanBeStackedWith(obj)) continue;
			if (!obj->CanBeStackedWith(inv.objects[0])) continue;
			if (!CanStackSellableContents(inv.objects[0], obj)) continue;
			inv.count += object_amount;
			PushBack(inv.objects, obj);
			
			// This object has a custom symbol (because it's a container)? Then the normal text would not be displayed.
			if (inv.custom != nil)
			{
				inv.custom.top.Text = inv.text;
				inv.custom.top.Style = inv.custom.top.Style | GUI_TextRight | GUI_TextBottom;
			}
			
			found = true;
			break;
		}

		// Add new!
		if (!found)
		{
			PushBack(inventory,
			{
					objects = [obj],
					count = object_amount,
			});
		}
	}

	return inventory;
}


private func CanStackSellableContents(object first, object second)
{
	return this->GetSellValue(first) == this->GetSellValue(second); // and the same value
}


private func GetInteractionMenus(object clonk)
{
	var menus = _inherited(clonk, ...) ?? [];

	if (this->AllowSellMenuEntries())
	{
		var sell_menu =
		{
			title = "$MsgSell$",
			entries_callback = this.GetSellMenuEntries,
			callback = "OnSellMenuSelection",
			callback_target = this,
			BackgroundColor = RGB(50, 50, 0),
			Priority = 21
		};
		
		var has_sell_menu = false;
		for (var menu in menus)
		{
			if (menu.title == "$MsgSell$")
			{
				has_sell_menu = true;
				break;
			}
		}
		
		if (!has_sell_menu)
		{
			PushBack(menus, sell_menu);
		}
	}
	
	return menus;
}


// ----- Selling

public func GetSellMenuEntries(object clonk)
{	
	// Distinguish owners here
	var wealth_player = clonk->GetController();

	var menu_entries = [];
	var index = 0;
	
	// Sellable items in the structure
	for (var item in this->GetSellableContents(wealth_player))
	{
		AddSellMenuEntry(menu_entries, item, index);
		++index;
	}
	
	// Sellable items in the clonk
	for (var item in this->GetSellableContents(wealth_player, clonk))
	{
		AddSellMenuEntry(menu_entries, item, index, clonk);
		++index;
	}
	
	// Sellable items in the a vehicle?
	if (clonk && clonk->GetProcedure() == "PUSH")
	{
		var vehicle = clonk->GetActionTarget();
		if (vehicle)
		{
			for (var item in this->GetSellableContents(wealth_player, vehicle))
			{
				AddSellMenuEntry(menu_entries, item, index, vehicle);
				++index;
			}
		}
	}

	return menu_entries;
}


private func AddSellMenuEntry(array menu_entries, proplist item, int index, symbol_overlay)
{
		var instance = item.objects[0];
		var amount = GetLength(item.objects);
		var value = this->GetSellValue(instance);
		var entry = GetBuyMenuEntry(index, instance, amount, value);
		
		if (symbol_overlay)
		{
			entry.image.overlay_top =
			{
				Left = ToPercentString(400),
				Bottom = ToPercentString(600),
				Symbol = symbol_overlay,
			};
		}
		
		PushBack(menu_entries, {symbol = instance, extra_data = nil, custom = entry});
}


private func OnSellMenuSelection(object item, extra_data, object clonk)
{
	// Distinguish owners here
	var wealth_player = clonk->GetController();
	// Sell
	DoSell(item, wealth_player);
	UpdateInteractionMenus([this.GetBuyMenuEntries, this.GetSellMenuEntries]);
}


private func Collection2(object item)
{
	UpdateInteractionMenus(this.GetSellMenuEntries);
	_inherited(item);
}


private func Ejection(object item)
{
	UpdateInteractionMenus(this.GetSellMenuEntries);
	_inherited(item);
}
