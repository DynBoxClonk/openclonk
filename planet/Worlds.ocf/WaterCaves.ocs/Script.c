#include Library_Scenario

private func Init_Goals()
{
	if (SCENPAR_Goal == 1)
	{
		AddGoal_Resource(Crystal);
	}
	else if (SCENPAR_Goal == 2)
	{
		AddGoal_Resource(Gold);
	}
	else
	{
		AddGoal_Wealth(800 + 200 * SCENPAR_Difficulty); // TODO: Replace with value gain goal
	}
}

func Init_Environment()
{
	//SetGamma(RGB(0,0,0),RGB(68,68,83),RGB(147,147,173));
	AddAmbience_Time();

	// Disasters
	Disaster(Earthquake, 12, 4);
	
	// Clouds
	Cloud->Place(5);
	Cloud->SetPrecipitation("Water", 5 * SCENPAR_Difficulty);

	// Other
	Stalactite->Place(AdjustToMapSize(25));
	
	// Sounds
	for (var y = 0; y < LandscapeHeight(); y += 510)
	for (var x = 0; x < LandscapeWidth(); x += 530)
	{
		var drip = CreateObject(Ambience_Sounds, x, y, NO_OWNER);
		drip->SetSound("Ambience::CaveDrip?")
            ->SetInterval(800)
            ->SetChance(100)
            ->Wait(RandomX(1, 4) * 50);
	}
}

private func Init_Vegetation()
{
	Seaweed->Place(AdjustToMapSize(70));
	Coral->Place(AdjustToMapSize(10));

	LargeCaveMushroom->Place(AdjustToMapSize(10), nil, {terraform = false});
	Mushroom->Place(AdjustToMapSize(25));

	Branch->Place(AdjustToMapSize(50));
	
	CallForEach(Grass->PlaceOnSurface(AdjustToMapSize(70)), Grass.Randomize, [1, 1, 1, 1, 100, 100, 500, 1000], 50, 200);
}

private func Init_Material()
{
	var level = 30;
	var relative = 24;
	Bone->PlaceInEarth(1, relative, level);
	Rock->PlaceInEarth(4, relative, level);
	Flint->PlaceInEarth(8, relative, level);
	Gold->PlaceInEarth(1, relative, level);
	Loam->PlaceInEarth(10, relative, level);
}

private func Init_Animals()
{
	// Animals
	Wipf->Place(AdjustToMapSize(3));
	ClassicFish->Place(AdjustToMapSize(30));
	Shark->Place(AdjustToMapSize(2));
	Bat->Place(AdjustToMapSize(5));
	
	// Nests
	var level = 30;
	var relative = 11;
	FireMonsterEgg->PlaceInEarth(4, relative, level, 10);
	MonsterEgg->PlaceInEarth(5, relative, level, 10);
	ZapNest->PlaceInEarth(2, relative, level, 10);
}

private func Player_StartingMaterial(int player)
{
	SetWealth(player, 60);
	ClassicHutStone->PlaceHomebase(player);
}

private func Player_InitialKnowledge(int player) // TODO
{
	var needs_power = !FindObject(Find_ID(Rule_NoPowerNeed));

	GivePlayerBasicKnowledge(player);	
	GivePlayerSpecificKnowledge(player, [ClassicHutWooden, ClassicHutStone]);
	GivePlayerCraftingKnowledge(player);
	if (needs_power) GivePlayerPowerKnowledge(player);
	GivePlayerMiningKnowledge(player);
	GivePlayerChemicalKnowledge(player);
	GivePlayerUndergroundKnowledge(player);

	return true;
}
