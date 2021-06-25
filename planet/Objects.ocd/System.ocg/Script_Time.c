/*
 *  Some global functions for manipulating the time object
 *
 *  Author: Marky
 */

global func ToSeconds(int hours, int minutes, int seconds, fFrames)
{
	hours	= hours%24;
	minutes	= minutes%60;
	seconds = seconds%60;

	if (fFrames)
	{
		var time = FindObject(Find_ID(Time));

		if (time)
		{
			return 10 * (hours * 3600 + minutes * 60 + seconds)/(time->GetCycleSpeed());
		}
	}

	return hours * 3600 + minutes * 60 + seconds;
}

global func GetTime()
{
	var time = FindObject(Find_ID(Time));

	if (time)
	{
		return time->GetTime();
	}

	return ToSeconds(12, 0, 0); // it's always noon :D
}

global func SetTime(int to_time)
{
	var time = FindObject(Find_ID(Time));

	if (time)
	{
		return time->SetTime(to_time/60); //possible only by the minute
	}
}

global func PauseTime()
{
	var time = FindObject(Find_ID(Time));
	if (time) time->PauseTime();
}

global func ResumeTime()
{
	var time = FindObject(Find_ID(Time));
	if (time) time->ResumeTime();
}

global func IsDay()
{
	var time = FindObject(Find_ID(Time));
	if (time) return time->IsDayLocal();
	else return true;
}

global func IsNight()
{
	var time = FindObject(Find_ID(Time));
	if (time) return time->IsNightLocal();
	else return false;
}

global func IsDusk()
{
	var time = FindObject(Find_ID(Time));
	if (time) return time->IsDuskLocal();
	else return false;
}

global func IsDawn()
{
	var time = FindObject(Find_ID(Time));
	if (time) return time->IsDawnLocal();
	else return false;
}
