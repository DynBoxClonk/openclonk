#appendto Pump

private func UpdatePlayerHomebaseMaterial(int player)
{
	// Create at least two pipes if the player knows how to build pumps
	ConfigurePlayerHomebaseMaterial(player, {Pipe = 2}, 1);
	
	// Create some barrels
	var barrel = GetPumpDefaultBarrel();
	var amount = 5, production = 1, props = {};
	if (GetPlrKnowledge(player, barrel))
	{
		amount = 3;
		production = 0;
	}
	props[Format("%i", barrel)] = amount;
	ConfigurePlayerHomebaseMaterial(player, props, production);
}


private func GetPumpDefaultBarrel()
{
	return MetalBarrel;
}
