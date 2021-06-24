/**
	Knowdlege
	Provides global functions to give players knowledge for scenarios.
	
	@author Maikel
*/

 
// Gives the player specific knowledge as given in the knowledge array.
global func GivePlayerSpecificKnowledge(proplist player, array knowledge)
{
	for (var plan in knowledge)
		player->GiveKnowledge(plan);
}

// Removes the player specific knowledge as given in the knowledge array.
global func RemovePlayerSpecificKnowledge(proplist player, array knowledge)
{
	for (var plan in knowledge)
		player->RemoveKnowledge(plan);
}

// Gives the player plans according to basic knowledge.
global func GivePlayerBasicKnowledge(proplist player)
{
	// Basic tools for mining and tree chopping
	var knowledge =
	[
		Shovel, Axe, Pickaxe, Barrel, Torch, Flint, Loam, ClassicFlag
	];
	GivePlayerSpecificKnowledge(player, knowledge);
}

global func GivePlayerCraftingKnowledge(proplist player)
{
	GivePlayerSpecificKnowledge(player, [Sawmill, ClassicFoundry, ClassicWorkshop, WoodenBridge]);
}

global func GivePlayerPowerKnowledge(proplist player)
{
	GivePlayerSpecificKnowledge(player, [ClassicWindmill, ClassicPowerPlant]);
}

global func GivePlayerChemicalKnowledge(proplist player)
{
	GivePlayerSpecificKnowledge(player, [ClassicChemicalFactory, TFlint, GunPowder, FireBomb]);
}

global func GivePlayerPumpingKnowledge(proplist player)
{
	GivePlayerSpecificKnowledge(player, [ClassicPump, MetalBarrel, Pipe]);
}

global func GivePlayerMiningKnowledge(proplist player)
{
	GivePlayerSpecificKnowledge(player, [ClassicElevator, ClassicLorry, ClassicDynamiteBox, WoodenBridge]);
}

global func GivePlayerTransportationKnowledge(proplist player)
{
	GivePlayerSpecificKnowledge(player, [ClassicLorry, Blimp, WoodenBridge]);
}

global func GivePlayerUndergroundKnowledge(proplist player)
{
	GivePlayerSpecificKnowledge(player, [Torch, WoodenBridge]);
}
