#appendto Stalactite

// Adjust stalactite color to the current background
private func Construction()
{
	ScheduleCall(this, this.UpdateColor, 1);
	return _inherited(...);
}

private func UpdateColor()
{
	var texture_color = GetAverageTextureColor(GetTexture());
	SetClrModulation(ColorSetValue(texture_color, Min(4 * ColorGetLightness(texture_color), 255)));
}
