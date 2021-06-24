#appendto Fish
#appendto Wipf

private func Death()
{
	SetName(Format("$DeadDescMasculinum$ %s", GetName()));
	return _inherited(...);
}
