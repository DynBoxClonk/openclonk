/**
	Knowdlege
	Provides global functions to give players knowledge for scenarios.
	
	@author Maikel
*/

 
// Gives the player specific knowledge as given in the knowledge array.
global func GivePlayerSpecificKnowledge(int player, array knowledge)
{
	for (var plan in knowledge)
		SetPlrKnowledge(player, plan);
}

// Removes the player specific knowledge as given in the knowledge array.
global func RemovePlayerSpecificKnowledge(int player, array knowledge)
{
	for (var plan in knowledge)
		SetPlrKnowledge(player, plan, true);
}

// Gives the player plans according to basic knowledge.
global func GivePlayerBasicKnowledge(int player)
{
	// Basic tools for mining and tree chopping
	var knowledge =
	[
		Shovel, Axe, Pickaxe, Barrel, Torch, Flint, Loam, ClassicFlag
	];
	GivePlayerSpecificKnowledge(player, knowledge);
}

global func GivePlayerCraftingKnowledge(int player)
{
	GivePlayerSpecificKnowledge(player, [Sawmill, ClassicFoundry, ClassicWorkshop, WoodenBridge]);
}

global func GivePlayerPowerKnowledge(int player)
{
	GivePlayerSpecificKnowledge(player, [ClassicWindmill, ClassicPowerPlant]);
}

global func GivePlayerChemicalKnowledge(int player)
{
	GivePlayerSpecificKnowledge(player, [ClassicChemicalFactory, TFlint, GunPowder, FireBomb]);
}

global func GivePlayerPumpingKnowledge(int player)
{
	GivePlayerSpecificKnowledge(player, [ClassicPump, MetalBarrel, Pipe]);
}

global func GivePlayerMiningKnowledge(int player)
{
	GivePlayerSpecificKnowledge(player, [ClassicElevator, ClassicLorry, ClassicDynamiteBox, WoodenBridge]);
}

global func GivePlayerTransportationKnowledge(int player)
{
	GivePlayerSpecificKnowledge(player, [ClassicLorry, Blimp, WoodenBridge]);
}

global func GivePlayerUndergroundKnowledge(int player)
{
	GivePlayerSpecificKnowledge(player, [Torch, WoodenBridge]);
}
