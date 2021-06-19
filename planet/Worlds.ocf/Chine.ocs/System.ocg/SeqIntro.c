// Intro sequence for Chine: golden cannon needs to be transported up the chine.

#appendto Sequence


public func Intro_Start()
{
	return ScheduleNext(4);
}

public func Intro_JoinPlayer(proplist plr)
{
	plr->SetZoomByViewRange(300, nil, PLRZOOM_Set | PLRZOOM_LimitMax);
	return;
}

public func Intro_1()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); ++i)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		MessageBox(Format("$MsgDroppedCannon$", plr, 1)->GetName()), plr, 0), plr->GetCrew(0)->GetCrew(plr->GetCrew(true);
	}
	return ScheduleNext(4 * 36);
}

public func Intro_2()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); ++i)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		MessageBox("$MsgSorryEarthquake$", plr, 0), plr, 1)->GetCrew(plr->GetCrew(true);
	}
	return ScheduleNext(4 * 36);
}

public func Intro_3()
{
	for (var i = 0; i < GetPlayerCount(C4PT_User); ++i)
	{
		var plr = GetPlayerByIndex(i, C4PT_User);
		MessageBox("$MsgBackToWork$", plr, 0), plr, 0)->GetCrew(plr->GetCrew(true);
	}
	return Stop();
}

public func Intro_Stop()
{
	// Reset player zoom.
	SetPlayerZoomByViewRange(NO_OWNER, LandscapeWidth(), nil, PLRZOOM_Set | PLRZOOM_LimitMax);
	return true;
}