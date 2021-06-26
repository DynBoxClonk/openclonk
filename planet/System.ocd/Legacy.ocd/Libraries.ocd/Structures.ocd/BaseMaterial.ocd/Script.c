/**
	Base Material & Production
	Library to control the players base material and production. The initial values are read
	from the Scenario.txt entries and per script one can modify these by:
     * GetBaseMaterial(id material, int index, int category)
     * SetBaseMaterial(id material, int amount)
     * DoBaseMaterial(id material, int change)
     * GetBaseProduction(id material, int index, int category)
     * SetBaseProduction(id material, int amount)
     * DoBaseProduction(id material, int change)
    Performs also two callbacks to a base of the player:
     * OnBaseMaterialChange(id material, int change);
     * OnBaseProductionChange(id material, int change);
	
	@author Randrian, Maikel
*/

// Maximum number of material. 
static const BASEMATERIAL_MaxBaseMaterial = 25;
// Maximum number of production. 
static const BASEMATERIAL_MaxBaseProduction = 10;
// Produce every X frames (currently set to a minute).
static const BASEMATERIAL_ProductionRate = 2160;

/*-- Add to player interface --*/


protected func Definition(def type)
{
	AddProperties(Player, {
		GetBaseMaterial = type.GetBaseMaterial,
		SetBaseMaterial = type.SetBaseMaterial,
		DoBaseMaterial = type.DoBaseMaterial,
		GetBaseProduction = type.GetBaseProduction,
		SetBaseProduction = type.SetBaseProduction,
		DoBaseProduction = type.DoBaseProduction,
		GiveBaseMaterial = type.GiveBaseMaterial,
		BroadcastBaseMaterialChange = type.BroadcastBaseMaterialChange,
		BroadcastBaseProductionChange = type.BroadcastBaseProductionChange,
		GetBaseMaterialData = type.GetBaseMaterialData,
		GetBaseProductionData = type.GetBaseProductionData,
		SetBaseProductionAtLeast = type.SetBaseProductionAtLeast,
		SetBaseMaterialAtLeast = type.SetBaseMaterialAtLeast,
		});
}


/*-- Object interface --*/

func GetBaseMaterialData()
{
	// Local variables to store the player's material.
	// Is an array filled with [id, count] arrays.
	if (this.Data.BaseMaterial == nil)
	{
		this.Data.BaseMaterial = [];
	}
	return this.Data.BaseMaterial;
}

func GetBaseProductionData()
{
	// Local variables to store the player's production.
	// Is an array filled with [id, count] arrays.
	if (this.Data.BaseProduction == nil)
	{
		this.Data.BaseProduction = [];
	}
	if (this.Data.BaseProductionManager == nil)
	{
		this.Data.BaseProductionManager = CreateObject(Library_BaseMaterial, 0, 0, this);
	}
	return this.Data.BaseProduction;
}

func GetBaseMaterial(id material, int index, int category)
{
	var base_material = GetBaseMaterialData();
	// Get the count if the id is given.
	if (material)
	{
		for (var combo in base_material)
			if (combo[0] == material)
				return combo[1];
		return nil;
	}
	// If an index is given look for the id.
	category = category ?? 0xffffff;
	index = Max(0, index);
	var count = 0;
	for (var combo in base_material)
	{
		if (combo[0]->GetCategory() & category)
		{
			if (count == index)
			{
				return combo[0];
			}
			count++;
		}
	}
	return;
}

func SetBaseMaterial(id material, int amount)
{
	if (amount == nil)
		return;
	amount = Max(0, amount);
	var change = 0;
	// Scan through current list of id's and set material if available.
	var found = false;
	var base_material = GetBaseMaterialData();
	for (var index = 0; index < GetLength(base_material); ++index)
	{
		if (base_material[index][0] == material)
		{
			change = amount - base_material[index][1];
			if (amount > 0)
			{
				base_material[index][1] = amount;
			}
			else
			{
				RemoveArrayIndex(base_material, index);
			}
			found = true;
			break;
		}
	}
	// If material is not available add it to the existing list.
	if (!found && amount > 0)
	{
		change = amount;
		PushBack(base_material, [material, amount]);
	}
	// Callback to the bases of the player.
	BroadcastBaseMaterialChange(material, change);
	return;
}


/**
 Sets the minimum amount of base material that a player has.
 Does nothing if the player already has more than this amount.
 */
func SetBaseMaterialAtLeast(id material, int amount)
{
	return SetBaseMaterial(material, Max(amount, GetBaseMaterial(material)));
}

func DoBaseMaterial(id material, int change)
{
	if (change == 0)
	{
		return;
	}
	// Scan through current list of id's and increase material if available. 
	var found = false;
	var base_material = GetBaseMaterialData();
	for (var index = 0; index < GetLength(base_material); ++index)
	{
		if (base_material[index][0] == material)
		{
			// Change must at least be minus the original value.
			change = Max(change, -base_material[index][1]);
			base_material[index][1] += change;
			if (base_material[index][1] == 0)
			{
				RemoveArrayIndex(base_material, index);
			}
			found = true;
			break;
		}
	}
	// If material is not available add it to the existing list.
	if (!found)
	{
		// Change must at least be zero.
		change = Max(change, 0);
		if (change > 0)
		{
			PushBack(base_material, [material, change]);
		}
	}
	// Callback to the bases of the player.
	BroadcastBaseMaterialChange(material, change);
	return;
}

func GetBaseProduction(id material, int index, int category)
{
	var base_production = GetBaseProductionData();
	// Get the count if the id is given.
	if (material)
	{
		for (var combo in base_production)
			if (combo[0] == material)
				return combo[1];
		return;
	}
	// If an index is given look for the id.
	category = category ?? 0xffffff;
	index = Max(0, index);
	var count = 0;
	for (var combo in base_production)
	{
		if (combo[0]->GetCategory() & category)
		{
			if (count == index) 
			{
				return combo[0];
			}
			count++;
		}
	}
	return;
}

func SetBaseProduction(id material, int amount)
{
	if (amount == nil)
	{
		return;
	}
	var base_production = GetBaseProductionData();
	amount = Max(0, amount);
	var change = 0;
	// Scan through current list of id's and set production if available.
	var found = false;
	for (var index = 0; index < GetLength(base_production); ++index)
	{
		if (base_production[index][0] == material)
		{
			change = amount - base_production[index][1];
			if (amount > 0)
			{
				base_production[index][1] = amount;
			}
			else
			{
				RemoveArrayIndex(base_production, index);
			}
			found = true;
			break;
		}
	}
	// If material is not available add it to the existing list.
	if (!found && amount > 0)
	{
		change = amount;
		PushBack(base_production, [material, amount]);
	}
	// Callback to the bases of the player.
	BroadcastBaseProductionChange(material, change);
	return;

}

/**
 Sets the minimum amount of base production that a player has.
 Does nothing if the player already has more than this amount.
 */
func SetBaseProductionAtLeast(id material, int amount)
{
	return SetBaseProduction(material, Max(amount, GetBaseMaterial(material)));
}

func DoBaseProduction(id material, int change)
{
	if (change == 0)
	{
		return;
	}
	var base_production = GetBaseProductionData();
	// Scan through current list of id's and increase production if available. 
	var found = false;
	for (var index = 0; index < GetLength(base_production); ++index)
	{
		if (base_production[index][0] == material)
		{
			// Change must at least be minus the original value.
			change = Max(change, -base_production[index][1]);
			base_production[index][1] += change;
			if (base_production[index][1] == 0)
			{
				RemoveArrayIndex(base_production, index);
			}
			found = true;
			break;
		}
	}
	// If production is not available add it to the existing list.
	if (!found)
	{
		// Change must at least be zero.
		change = Max(change, 0);
		if (change > 0)
		{
			PushBack(base_production, [material, change]);
		}
	}
	// Callback to the bases of the player.
	BroadcastBaseProductionChange(material, change);
	return;

}

// Gives the player specific base materials as given in the materials array.
func GiveBaseMaterial(array base_mats)
{
	if (base_mats)
	{
		for (var mat in base_mats)
		{
			DoBaseMaterial(mat[0], mat[1]);
			DoBaseProduction(mat[0], mat[2]);
		}
	}
}

/*-- Internal Interface --*/

local production_unit = 0;

func Initialize()
{
	// Add a timer for executing base production.
	AddTimer("ExecBaseProduction", BASEMATERIAL_ProductionRate);
	return;
}

// Called every minute and updates the materials according to production.
public func ExecBaseProduction()
{
	production_unit++;
	// Look at all production.
	for (var combo in GetOwner()->GetBaseProductionData())
	{
		// Check if this id is produced and check if it isn't already full.
		if (combo[1] > 0 && GetOwner()->GetBaseMaterial(combo[0]) < BASEMATERIAL_MaxBaseMaterial)
		{
			// Produce the material every production value / BASEMATERIAL_MaxBaseProduction times.
			if (production_unit % BoundBy(BASEMATERIAL_MaxBaseProduction + 1 - combo[1], 1, BASEMATERIAL_MaxBaseProduction) == 0)
				GetOwner()->DoBaseMaterial(combo[0], 1);
		}
	}
	return;
}


/*-- Miscellaneous --*/

func BroadcastBaseProductionChange(id material, int change)
{
	var i = 0, base;
	while (base = FindBase(this, i++))
	{
		base->~OnBaseProductionChange(material, change);
	}
}

func BroadcastBaseMaterialChange(id material, int change)
{
	var i = 0, base;
	while (base = FindBase(this, i++))
	{
		base->~OnBaseMaterialChange(material, change);
	}
}
