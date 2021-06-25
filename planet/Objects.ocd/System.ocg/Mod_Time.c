/*
 *	Time Controller Addon
 *
 *	Adds some practical functions to the time object
 *
 *	Author: Marky
 */

#appendto Time

/* Local functions */

public func PauseTime()
{
	if (!GetCycleSpeed()) return false; // is already paused or not paused at all

	var effect = GetEffect("IntTimePaused", this);

	if (!effect)
	{
		effect = AddEffect("IntTimePaused", this, 1);
	}
	if (!effect) return false;

	effect.backup_advance_seconds_per_tick = advance_seconds_per_tick;
	advance_seconds_per_tick = 0;
	return true;
}

public func ResumeTime()
{
	var effect = GetEffect("IntTimePaused", this);
	if (!effect) return false;

	SetCycleSpeed(effect.backup_advance_seconds_per_tick);
	RemoveEffect(nil, nil, effect);
	return true;
}

public func IsDawnLocal()
{
	var dawn_start = (time_set["SunriseStart"] + time_set["SunriseEnd"]) / 2;
	var dawn_end = time_set["SunriseEnd"];

	return Inside(time, dawn_start, dawn_end);
}

public func IsDuskLocal()
{
	var dusk_start = (time_set["SunsetStart"] + time_set["SunsetEnd"]) / 2;
	var dusk_end = time_set["SunsetEnd"];

	return Inside(time, dusk_start, dusk_end);
}

public func IsDayLocal()
{
	var day_start = (time_set["SunriseStart"] + time_set["SunriseEnd"]) / 2;
	var day_end = (time_set["SunsetStart"] + time_set["SunsetEnd"]) / 2;

	return Inside(time, day_start, day_end);
}

public func IsNightLocal()
{
	var night_start = (time_set["SunsetStart"] + time_set["SunsetEnd"]) / 2;
	var night_end = (time_set["SunriseStart"] + time_set["SunriseEnd"]) / 2;

	return Inside((time + ToSeconds(12)) % (ToSeconds(12)*2), Min(night_start, night_end), Max(night_start, night_end));
}




// Adjusts the sky, celestial and others to the current time. Use SetTime() at runtime, not this.
private func AdjustToTime()
{
	if (Abs(time - time_set["SunriseEnd"]) <= advance_seconds_per_tick)
		OnSunriseEnd();
	else if (Abs(time - time_set["SunsetStart"]) <= advance_seconds_per_tick)
		OnSunsetStart();

	DoSkyShade();
}
