
/* -- Engine callbacks -- */

public func Initialize()
{
	_inherited(...);
	Init_Rules();
	Init_Goals();
	Init_Environment();
	Init_Vegetation();
	Init_Animals();
	Init_Material();
}

public func InitializePlayer(int player)
{
	Player_InitialKnowledge(player);
	Player_StartingMaterial(player);
	Player_StartingEquipment(player);
	GivePlayerHomebaseMaterial(player);
}

/* -- Default functions -- */

private func Init_Rules()
{
	CreateObject(Rule_TeamAccount);
	
	if (SCENPAR_PowerNeed == 2) CreateObject(Rule_NoPowerNeed);
}

private func Init_Goals(){ }
private func Init_Environment(){ }
private func Init_Vegetation(){ }
private func Init_Animals(){ }
private func Init_Material(){ }

/* -- Player specific stuff -- */

private func Player_InitialKnowledge(int player){ }
private func Player_StartingMaterial(int player){ }

private func Player_StartingEquipment(int player)
{
	var crew_members = FindObjects(Find_Owner(player), Find_OCF(OCF_CrewMember));
	for (var i = 0; i < GetLength(crew_members); ++i)
	{
		var crew = crew_members[i];
		Player_StartingEquipmentForCrewMember(player, crew, i);
	}
}

private func Player_StartingEquipmentForCrewMember(int player, object crew, int crew_index)
{
	// Fill with your own equipment
}

/* -- Goals -- */

private func AddGoal_AnimalRescue(id type, int allowed_deaths, int min_amount, bool placed_by_scenario)
{
	min_amount = min_amount ?? (4 + SCENPAR_Difficulty);
	allowed_deaths = allowed_deaths ?? Max(0, (25 - 10 * SCENPAR_Difficulty) * min_amount / 100);
	CreateObject(Goal_CollectAnimals)->SetAnimal(type, allowed_deaths, min_amount, placed_by_scenario);
}

private func AddGoal_Wealth(int to_collect)
{
	GUI_Controller->ShowWealth();
	var default = 100 + 100 * SCENPAR_Difficulty;
	CreateObject(Goal_Wealth)->SetWealthGoal(to_collect ?? default);
}

private func AddGoal_Resource(id type, int percentage)
{
	var default = 70 + 10 * SCENPAR_Difficulty;
	CreateObject(Goal_ResourceExtraction)->SetResource(Format("%i", type), Min(100, percentage ?? default));
}

/* -- Ambience -- */

private func AddAmbience_Time(int hours, int minutes, int seconds)
{
	var time = CreateObject(Time);
	time->SetCycleSpeed(20);
	
	if (hours || minutes || seconds)
	{
		SetTime(ToSeconds(hours, minutes, seconds));
	}
	return time;
}

/* -- Disasters -- */

private func Disaster(id type, int chance, int variance, int min_difficulty)
{
	if (SCENPAR_Difficulty >= min_difficulty)
	{
		type->SetChance(chance + SCENPAR_Difficulty * variance);
	}
}

/* -- Materials -- */
