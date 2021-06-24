/**
 	Better base material

	Creating custom setups for base material is tedious, so here are some functions that should help speed up the process.

	@author Marky
 */

/* -- Additional functions -- */

/**
 Sets the minimum amount of base material that a player has.
 Does nothing if the player already has more than this amount.
 */
global func SetBaseMaterialAtLeast(int player, id material, int amount)
{
	return SetBaseMaterial(player, material, Max(amount, GetBaseMaterial(player, material)));
}


/**
 Sets the minimum amount of base production that a player has.
 Does nothing if the player already has more than this amount.
 */
global func SetBaseProductionAtLeast(int player, id material, int amount)
{
	return SetBaseProduction(player, material, Max(amount, GetBaseMaterial(player, material)));
}

/* -- Best effort default homebase -- */

/**
 Best effort function for giving players their homebase material.
 Should be called after scenario initialization (because it relies
 on objects having been placed), and after
 the players were given knowledge (because it relies on the knowledge).
 */
global func GivePlayerHomebaseMaterial(int player)
{
	// --- Initialize the lists
	var material_list = {};
	var tool_list =
	{
		Conkit = 3,
		Flint = 5,
		Loam = 5,
		Metal = 5,
		ClassicFlag = 3,
	};
	
	// --- Get at least the material to create these
	var essential_constructions = [Sawmill, ClassicFoundry, ClassicWindmill];
	
	// Fill the list
	for (var knowledge in essential_constructions)
	{
		var index = 0;
		for (var component = knowledge->GetComponent(nil, index); !!component; component = knowledge->GetComponent(nil, ++index))
		{
			material_list[Format("%i", component)] += knowledge->GetComponent(component);
		}
	}
	
	// --- Tools can be bought if you do not have them
	
	// Default tools
	var essential_tools = [Shovel, Axe, Conkit];
	
	// Fill the list
	for (var tool in essential_tools)
	{
		// Already has such a tool? If not, make one buyable
		var tool_name = Format("%i", tool);
		var has_tool = !!FindObject(Find_ID(tool), Find_Owner(player));
		if (!has_tool)
		{
			tool_list[tool_name] = Max(tool_list[tool_name], 1);
		}
		
		// Tools get auto-produced if you do not have the knowledge
		if (!GetPlrKnowledge(player, tool) || !GetPlrKnowledge(player, ClassicWorkshop))
		{
			material_list[tool_name] = Max(material_list[tool_name], 1);
		}
	}
	
	// --- Add materials with production
	var material_production = 1;
	var tool_production = 0;
	ConfigurePlayerHomebaseMaterial(player, material_list, material_production);
	ConfigurePlayerHomebaseMaterial(player, tool_list, tool_production);

	// --- Configure crew production
	ConfigurePlayerHomebaseMaterial(player, { ClassicClonk = 3 }, 1);
	
	// --- Everything else that can be produced is determined by the individual knowledge
	var index = 0;
	var categories = C4D_Structure | C4D_Vehicle | C4D_Object;
	for (var knowledge = GetPlrKnowledge(player, nil, index, categories); !!knowledge; knowledge = GetPlrKnowledge(player, nil, ++index, categories))
	{
		knowledge->~UpdatePlayerHomebaseMaterial(player);
	}
}


global func  ConfigurePlayerHomebaseMaterial(int player, proplist materials, int production)
{
	// Update the home base material
	for (var material_name in GetProperties(materials))
	{
		var material = GetDefinition(material_name);
		if (material)
		{
			// Set home base material: At least the minimum required amount,
			// but deduct the materials that the player already has
			var amount = Max(0, materials[material_name] - ObjectCount(Find_ID(material), Find_Owner(player)));
			SetBaseMaterialAtLeast(player, material, amount);
			SetBaseProductionAtLeast(player, material, production);
		}
		else
		{
			FatalError(Format("Definition not found: %s", material_name));
		}
	}
}
