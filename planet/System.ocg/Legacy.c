/**
	Legacy.c
	Contains legacy code that will be removed after some time.
	
	@author Marky
*/

static const VERSION_10_0_OC = "10.0";

/* -- Scenario stuff -- */

global func GainMissionAccess(string password)
{
	LogLegacyWarning("GainMissionAccess", "GainScenarioAccess", VERSION_10_0_OC);
	return GainScenarioAccess(password);
}

global func GetMissionAccess(string password)
{
	LogLegacyWarning("GetMissionAccess", "GetScenarioAccess", VERSION_10_0_OC);
	return GetScenarioAccess(password);
}

global func GetPlayerID(int plr_nr)
{
	LogLegacyWarning("GetPlayerID", "The player ID and player number are the same since 9.0 OC", VERSION_10_0_OC);
	return plr_nr;
}

global func GetPlayerByID(int plr_id)
{
	LogLegacyWarning("GetPlayerByID", "The player ID and player number are the same since 9.0 OC", VERSION_10_0_OC);
	return plr_id;
}

global func GetPlrClonkSkin(int plr_id)
{
	LogLegacyWarning("GetPlrClonkSkin", "GetPlayerClonkSkin", VERSION_10_0_OC);
	return GetPlayerClonkSkin(plr_id, ...);
}

global func GetPlrExtraData(int plr_id, string name)
{
	LogLegacyWarning("GetPlrExtraData", "GetPlayerExtraData", VERSION_10_0_OC);
	return GetPlayerExtraData(plr_id, name);
}

global func GetPlrView(int plr_id)
{
	LogLegacyWarning("GetPlrView", "GetPlayerView", VERSION_10_0_OC);
	return GetPlayerView(plr_id);
}

global func GetPlrViewMode(int plr_id)
{
	LogLegacyWarning("GetPlrViewMode", "GetPlayerViewMode", VERSION_10_0_OC);
	return GetPlayerViewMode(plr_id);
}

global func PlaceAnimal(id animal_id)
{
	LogLegacyWarning("PlaceAnimal", "id->Place()", VERSION_10_0_OC);
	if (animal_id)
	{
		if (animal_id.Place)
		{
			var animals = animal_id->Place(1);
			return animals[0];
		}
		else
		{
			Log("WARNING: Place(int amount, proplist area, proplist settings) not supported by ID: %i", animal_id);
			return nil;
		}
	}
	return nil;
}

global func SetNextMission(string filename, string title, string description)
{
	LogLegacyWarning("SetNextMission", "SetNextScenario", VERSION_10_0_OC);
	return SetNextScenario(filename, title, description);
}

global func SetPlrView(int plr_id, object target, bool immediate)
{
	LogLegacyWarning("SetPlrView", "SetPlayerView", VERSION_10_0_OC);
	return SetPlayerView(plr_id, target, immediate);
}

global func SetPlrExtraData(int plr_id, string name, any data)
{
	LogLegacyWarning("SetPlrExtraData", "SetPlayerExtraData", VERSION_10_0_OC);
	return SetPlayerExtraData(plr_id, name, data);
}

global func SetBridgeActionData()
{
	LogLegacyWarningRemoved("SetBridgeActionData");
}

/* -- Internal helpers -- */

global func LogLegacyWarning(string function_name, string replacement_name, string version)
{
	Log("WARNING: Do not use the legacy function \"%s\" anymore; use \"%s\" instead (complete removal is planned for \"%s\").", function_name, replacement_name, version);
}

global func LogLegacyWarningRemoved(string function_name)
{
	Log("WARNING: Do not use the legacy function \"%s\" anymore; it was removed and has no effect.", function_name);
}
