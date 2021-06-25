
global func EliminateAllPlayers()
{
	for (var player in GetPlayers())
	{
		player->Eliminate();
	}
}
