// Intro sequence for this tutorial.

#appendto Sequence

public func Intro_Init(proplist for_plr)
{
	return true;
}

public func Intro_JoinPlayer(proplist plr)
{
	return;
}

public func Intro_Start()
{
	return ScheduleNext(40);
}

public func Intro_1()
{
	return Stop();
}

public func Intro_Stop()
{
	GameCall("OnIntroSequenceFinished", this.plr);
	return true;
}