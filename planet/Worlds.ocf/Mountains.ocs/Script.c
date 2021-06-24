#include Library_Scenario


private func Init_Goals()
{
	AddGoal_Resource(Ore);
}

func Init_Environment()
{
	SetSkyParallax(0, 14, 14, 0, 0, nil, nil);

	AddAmbience_Time();
	
	Volcano->SetMinStrength(20);

	// Disasters
	Disaster(Meteor, 0, 2);
	Disaster(Volcano, 0, 4);
	Disaster(Earthquake, 0, 2);
	
	// Clouds
	Cloud->Place(5);
	Cloud->SetPrecipitation("Water", 5 * SCENPAR_Difficulty);
}

private func Init_Vegetation()
{
	Tree1->Place(AdjustToMapSize(1));
	Tree2->Place(AdjustToMapSize(5));
	Tree3->Place(AdjustToMapSize(3));
	Tree4->Place(AdjustToMapSize(30));
	Flower->Place(AdjustToMapSize(5));
	Bone->PlaceOnSurface(AdjustToMapSize(RandomX(2, 7)));
	Skull->PlaceOnSurface(AdjustToMapSize(RandomX(2, 4)));
	PlaceGrass(30);
}

private func Init_Material()
{
	Rock->PlaceOnSurface(AdjustToMapSize(5));

	var relative = 18;
	var level = 20;
	Rock->PlaceInEarth(5, relative, level);
	Flint->PlaceInEarth(10, relative, level);
	Loam->PlaceInEarth(2, relative, level);
	Gold->PlaceInEarth(1, relative, level);
}


private func Player_StartingMaterial(int player)
{
	SetWealth(player, 50);
	ClassicHutStone->PlaceHomebase(player);
}

private func Player_InitialKnowledge(int player)
{
	var needs_power = !FindObject(Find_ID(Rule_NoPowerNeed));

	var vehicleKnowledge =
	[
		ClassicCatapult,
		Balloon
	];

	GivePlayerBasicKnowledge(player);	
	GivePlayerSpecificKnowledge(player, [ClassicHutWooden, ClassicHutStone]);
	GivePlayerCraftingKnowledge(player);
	if (needs_power) GivePlayerPowerKnowledge(player);
	GivePlayerMiningKnowledge(player);
	GivePlayerChemicalKnowledge(player);
	GivePlayerSpecificKnowledge(player, [WoodenBridge]);
	GivePlayerSpecificKnowledge(player, vehicleKnowledge);

	return true;
}
