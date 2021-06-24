#include Library_Scenario

static const SCENARIO_LandscapeWidth_OnePlayer = 640;


private func Init_Goals()
{
	AddGoal_Resource(Gold);
}

private func Init_Environment()
{
	AddAmbience_Time();
}


private func Init_Vegetation()
{
	PlaceGrass(85);

	Tree1->Place(AdjustToMapSize(4));
	Tree2->Place(AdjustToMapSize(12));
	Tree3->Place(AdjustToMapSize(7));
	Tree4->Place(AdjustToMapSize(12));
	
	Flower->Place(AdjustToMapSize(2));
	Fern->Place(AdjustToMapSize(2));
}

private func Init_Animals()
{
	// players are not initialized yet, but the map can expand. Cheat a little here :)
	Bird->Place(AdjustToMapSize(3));
}

private func Init_Material(int map_size)
{
	var relative = 4;
	var percentage = 65;
	Rock->PlaceInEarth(1, relative, percentage);
	Gold->PlaceInEarth(1, relative, percentage);
	Flint->PlaceInEarth(1, relative, percentage);
	Loam->PlaceInEarth(1, relative, percentage);
}

private func Player_StartingMaterial(int player)
{
	SetWealth(player, 50 - (SCENPAR_Difficulty - 1) * 25); // 50; 25; 0
	ClassicHutWooden->PlaceHomebase(player);
}

private func Player_InitialKnowledge(int player)
{
	var needs_power = !FindObject(Find_ID(Rule_NoPowerNeed));
	
	GivePlayerBasicKnowledge(player);
	GivePlayerSpecificKnowledge(player, [TFlint]);
	GivePlayerSpecificKnowledge(player, [ClassicHutWooden, Sawmill, ClassicWorkshop, ClassicChemicalFactory]);
	if (needs_power) GivePlayerSpecificKnowledge(player, [ClassicWindmill]);
	GivePlayerMiningKnowledge(player);
	return true;
}
