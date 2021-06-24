#appendto BoilingLava_Bubble

private func Construction()
{
	var self = this;
	_inherited(...);
	if (self) // Object can be removed by constructor
	{
		SetGraphics(Format("%d", RandomX(1, 3)), BoilingLiquid_Bubble);
		SetObjectBlitMode(GFX_BLIT_Mod2);
		SetClrModulation(GetAverageTextureColor("lava_red"));

		// Add light to the chunk.
		SetLightRange(60, 40);
		SetLightColor(FIRE_LIGHT_COLOR);
	}
}
