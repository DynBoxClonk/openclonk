#include Library_Scenario


private func Init_Goals()
{
	AddGoal_Wealth();
	AddGoal_Resource(Gold, 30 + 20 * SCENPAR_Difficulty);
}

private func Init_Environment()
{
	SetSkyParallax(0, 33, 57, 0, 0, 0, 0); 
	AddAmbience_Time();

	// Some dark clouds which rain few ashes.
	Cloud->Place(15);
	Cloud->SetPrecipitation("Ashes", 17 + 10 * SCENPAR_Difficulty);
	Cloud->SetCloudRGB(60, 35, 25);
	Cloud->SetLightning(0 + 1 * SCENPAR_Difficulty);
	Cloud->SetInsertPxs(true);

	// Some natural disasters.
	Disaster(Meteor, 8, 4, 2);
	Disaster(Volcano, 15, 6);
	Disaster(Earthquake, 8, 4);
	Volcano->SetMaterial("DuroLava");
	BoilingLava->Place(2);
}

private func Init_Vegetation()
{
	var burned_trees_1 = Tree1->Place(AdjustToMapSize(6));

	for (var tree in burned_trees_1)
		tree->~SetBurned();

	Tree1->Place(AdjustToMapSize(15));
	Tree2->Place(AdjustToMapSize(9));
	Tree3->Place(AdjustToMapSize(12));

	PlaceGrass(15);

	for (var grass in FindObjects(Find_ID(Grass)))
	{
		grass->SetClrModulation(RGB(225+Random(30), Random(30), Random(30)));
	}

	Stalactite->Place(AdjustToMapSize(20));
}

private func Init_Animals()
{
	Bird->Place(AdjustToMapSize(3));
	Bat->Place(AdjustToMapSize(5));

	var relative = 7;
	var level = 28;
	MonsterEgg->PlaceInEarth(2, relative, level, 10);
	FireMonsterEgg->PlaceInEarth(4, relative, level, 10);
	ZapNest->PlaceInEarth(1, relative, level, 10);
}

private func Init_Material()
{
	// Some objects in the earth.
	var relative = 7;
	var level = 28;
	Bone->PlaceInEarth(2, relative, level);
	Flint->PlaceInEarth(7, relative, level);
	Gold->PlaceInEarth(4, relative, level);
	Loam->PlaceInEarth(5, relative, level);
	
	// Some volcano remains
	Skull->PlaceOnSurface(AdjustToMapSize(10));
	Rock->PlaceOnSurface(AdjustToMapSize(10));
	Coal->PlaceOnSurface(AdjustToMapSize(10));
}


private func Player_StartingMaterial(int player)
{
	SetWealth(player, 80);
	
	var trees = FindObjects(Find_Func("IsTree"));
	var start = trees[Random(GetLength(trees))];

	for (var crew in FindObjects(Find_Category(OCF_CrewMember), Find_Owner(player)))
	{
		crew->SetPosition(start->GetX(), start->GetY());
	}
}

private func Player_StartingEquipmentForCrewMember(int player, object crew, int crew_index)
{
	_inherited(player, crew, crew_index);

	if (0 == crew_index)
	{
		crew->CreateContents(ClassicFlag);
		crew->CreateContents(Conkit);
	}
}

private func Player_InitialKnowledge(int player)
{
	var needs_power = !FindObject(Find_ID(Rule_NoPowerNeed));

	
	GivePlayerBasicKnowledge(player);
	GivePlayerSpecificKnowledge(player, [ClassicHutWooden, ClassicHutStone]);
	if (needs_power) GivePlayerPowerKnowledge(player);
	GivePlayerCraftingKnowledge(player);
	GivePlayerMiningKnowledge(player);
	GivePlayerPumpingKnowledge(player);	
	GivePlayerChemicalKnowledge(player);
	GivePlayerUndergroundKnowledge(player);

	return true;
}
