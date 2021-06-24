#include Library_Scenario

private func Init_Goals()
{
	AddGoal_Wealth();
}

private func Init_Environment()
{
	//SetGamma(RGB(10, 9, 10), RGB(161, 157, 159), RGB(255, 254, 255), 2);
	AddAmbience_Time();
	
	// Bird song
	var birds = Ambience_Sounds->PlaceOnSurface(AdjustToMapSize(10), nil, {location = Loc_Space(30, CNAT_Top)});
	for (var bird in birds)
	{
		bird->SetSound("Ambience::BirdSong?")
		    ->SetInterval(25)
		    ->SetChance(13)
		    ->SetCondition(Global.IsDay)
		    ->Wait(RandomX(1, 10) * 25);
		bird->MovePosition(0, -30);
	}
	
	// Clouds
	Cloud->Place(5);
	Cloud->SetLightning(10 * SCENPAR_Difficulty);
}

private func Init_Vegetation()
{
	Tree1->Place(AdjustToMapSize(3));
	Tree2->Place(AdjustToMapSize(6));
	Tree3->Place(AdjustToMapSize(3));
	Tree4->Place(AdjustToMapSize(3));
	Vine->Place(AdjustToMapSize(5));
	Branch->Place(AdjustToMapSize(5));
	Flower->Place(AdjustToMapSize(5));
	PlaceGrass(90);
}

private func Init_Material()
{
	var relative = 4;
	var level = 65;
	Flint->PlaceInEarth(1, relative, level);
	Gold->PlaceInEarth(1, relative, level);
	Loam->PlaceInEarth(1, relative, level);
	Rock->PlaceInEarth(1, relative, level);
}

private func Init_Animals()
{
	Bird->Place(AdjustToMapSize(3));
	var level = 65;
	MonsterEgg->PlaceInEarth(7, 11, level, 10);
	ZapNest->PlaceInEarth(4, 11, level, 10);
}

private func Player_StartingMaterial(int player)
{
	SetWealth(player, 70);
	ClassicHutWooden->PlaceHomebase(player);
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
	GivePlayerTransportationKnowledge(player);

	return true;
}
