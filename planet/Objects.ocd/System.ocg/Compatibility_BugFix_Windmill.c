#appendto ClassicWindmill

// Turns wind into power and adjusts the power production accordingly.
public func Wind2Turn()
{
	// Only produce power if fully constructed.
	if (GetCon() < 100) 
		return;
	// Determine the current power production.	
	var power = 0;
	if (!wheel->Stuck() && !wheel->HasStopped())
	{		
		// Produced power ranges from 0 to 80 in steps of 10.
		power = Abs(wheel->GetRDir(MinRevolutionTime() / 90));
		power = BoundBy((10 * power + 60) / 125 * 10, 0, 80);
	}
	// Register the new power production if it changed.
	if (last_power != power)
	{
		last_power = power;
		RegisterPowerProduction(last_power);
	}
	// Adjust the wheel speed.
	var current_wind = GetWeightedWind();
	wheel->SetRDir(current_wind * 90, MinRevolutionTime());
	// Make some sounds.
	if (Abs(current_wind) >= 10 && Random(15 - Abs(current_wind / 10)) < 5)
		DoCreak();
	return;
}


private func DoCreak()
{
	Sound(["Hits::Materials::Wood::WoodCreak?","Structures::HingeCreak?"][Random(2)], {custom_falloff_distance = 75, volume = 50});
}