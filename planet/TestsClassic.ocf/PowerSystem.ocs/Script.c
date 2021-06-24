/**
	Power System
	Unit tests for the power system. 
*/


static script_player;

protected func Initialize()
{
	// Create a script player for some tests.
	script_player = nil;
	CreateScriptPlayer("PowerBuddy", RGB(0, 0, 255), nil, CSPF_NoEliminationCheck);
	return;
}

protected func InitializePlayer(int player)
{
	// Set zoom to full map size.
	SetPlayerZoomByViewRange(player, LandscapeWidth(), nil, PLRZOOM_Direct);
	
	// No FoW to see everything happening.
	SetFoW(false, player);
	
	// All players belong to the first team.
	// The second team only exists for testing.
	SetPlayerTeam(player, 1);
		
	// Initialize script player.
	if (GetPlayerType(player) == C4PT_Script)
	{
		// Store the player number.
		if (script_player == nil)
			script_player = player;
		// No crew needed.
		GetCrew(player)->RemoveObject();
		return;
	}	
	
	// Move player to the start of the scenario.
	GetCrew(player)->SetPosition(120, 150);
	
	// Some knowledge to construct a flagpole.
	GetCrew(player)->CreateContents(Hammer);
	SetPlrKnowledge(player, Flagpole);
	
	// Add test control effect.
	StartTests(player);
	return;
}

protected func RemovePlayer(int player)
{
	// Remove script player.
	if (GetPlayerType(player) == C4PT_Script)
	{
		if (player == script_player)
			script_player = nil;
		return;	
	}
	return;
}

/*-- Power Tests --*/

// Test for power plant fueled by oil barrels.
global func Test1_OnStart(int player)
{
	// Power source: one power plant.
	var engine = CreateObjectAbove(ClassicPowerPlant, 100, 160, player);

	for (var i = 0; i < 3; ++i)
	{
		var barrel = CreateObject(Barrel); //engine->CreateContents(Barrel, 1);
		Oil->CreateLiquid(10, barrel); //->SetLiquidContainer("Oil", 10);
		engine->Collect(barrel, true);
	}
	
	// Power consumer: chemical plant.
	var chemical = CreateObjectAbove(ClassicChemicalFactory, 280, 160, player);
	chemical->CreateContents(Coal, 5);
	chemical->AddToQueue(GunPowder, 5);

	// Log what the test is about.
	Log("A power plant fueled by oil barrels.");
	return true;
}

global func Test1_Completed()
{
	return (ObjectCount(Find_ID(GunPowder)) >= 5);
}

global func Test1_OnFinished()
{
	RemoveAll(Find_Or(Find_ID(ClassicPowerPlant), Find_ID(ClassicChemicalFactory), Find_ID(Barrel)));
	return;
}

// Test for power plant fueled by oil field and pump.
global func Test2_OnStart(int player)
{
	// Oil field
	DrawMaterialQuad("Oil", 144, 168, 208 + 1, 168, 208 + 1, 304, 144, 304, true);

	// Power source: one power plant.
	var engine = CreateObjectAbove(ClassicPowerPlant, 70, 160, player);
	engine.fuel_amount = 100;
	
	// Power consumer: one pump.
	var pump = CreateObjectAbove(ClassicPump, 124, 160, player);
	var source = CreateObjectAbove(Pipe, 176, 292, player);
	source->ConnectPipeTo(pump, PIPE_STATE_Source);
	var drain = CreateObjectAbove(Pipe, 100, 160, player);
	drain->ConnectPipeTo(pump, PIPE_STATE_Drain);
	drain->ConnectPipeTo(engine);
	
	
	// Power consumer: chemical plant.
	var chemical = CreateObjectAbove(ClassicChemicalFactory, 280, 160, player);
	chemical->CreateContents(Coal, 10);
	chemical->AddToQueue(GunPowder, 10);

	// Log what the test is about.
	Log("A power plant fueled by an oil field via pump.");
	return true;
}

global func Test2_Completed()
{
	return (ObjectCount(Find_ID(GunPowder)) >= 10);
}

global func Test2_OnFinished()
{
	RestoreWaterLevels();
	RemoveAll(Find_Or(Find_ID(ClassicPowerPlant), Find_ID(ClassicChemicalFactory), Find_ID(Pipe), Find_ID(ClassicPump)));
	return;
}


/*-- Helper Functions --*/

global func RestoreWaterLevels()
{
	// Restore water levels.
	DrawMaterialQuad("Water", 144, 168, 208 + 1, 168, 208 + 1, 304, 144, 304, true);
	for (var x = 216; x <= 280; x++)
		for (var y = 24; y <= 120; y++)
			if (GetMaterial(x, y) == Material("Water"))
				ClearFreeRect(x, y, 1, 1);
	return;
}

global func RemoveWater()
{
	for (var x = 144; x <= 208 + 1; x++)
		for (var y = 168; y <= 304; y++)
			if (GetMaterial(x, y) == Material("Water"))
				ClearFreeRect(x, y, 1, 1);
	return;
}
