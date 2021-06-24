#appendto Torch

private func UpdatePlayerHomebaseMaterial(proplist player)
{
	// The players may buy and produce torches, if they have the knowledge
	ConfigurePlayerHomebaseMaterial(player, {Torch = 5}, 1);
}
