
global func EliminateAllPlayers()
{
	for (var i = 0; i < GetPlayerCount(); ++i)
	{
		EliminatePlayer(GetPlayerByIndex(i));
	}
}
