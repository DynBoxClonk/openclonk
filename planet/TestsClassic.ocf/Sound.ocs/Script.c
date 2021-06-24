/**
	Power System
	Unit tests for the power system. 
*/


static script_player;

protected func Initialize()
{
}

protected func InitializePlayer(int player)
{
	StartTest();
	return;
}

global func StartTest()
{
	Log("---------------------");
	Log("Testing sounds:");

	var sounds = [
		[BirdEgg, [BirdEgg.DoBreak]],
		[Bird, [Bird.DoTwitter]],
		[ClassicPiranha, [ClassicPiranha.BiteEffect]],
		[Zap, [Zap.DoBuzz, Zap.DoSting]],
		[Crystal, [Crystal.Hit]],
		[Ice, [Ice.Hit]],
		[Loam, [Loam.Hit]],
		[Metal, [Metal.Hit]],
		[Sand, [Sand.Hit]],
		[Sulphur, [Sulphur.Hit]],
		[Wood, [Wood.Hit]],
		[ClassicFlag, [ClassicFlag.DoTrumpet]],
		[FireBomb, [FireBomb.Hit]],
		[GunPowder, [GunPowder.Hit]],
		//[TFlint, [.]],
		[ClassicCastle, [ClassicCastle.SoundOpenDoor, ClassicCastle.SoundCloseDoor, ClassicCastle.SoundOpenGate, ClassicCastle.SoundCloseGate, ClassicCastle.FireArrow]],
		//[ClassicChemicalFactory, [.]],
		//[ClassicElevator, [.]],
		[ClassicFoundry, [ClassicFoundry.Collection]],
		[ClassicPowerPlant, [ClassicPowerPlant.SoundOpenDoor, ClassicPowerPlant.SoundCloseDoor, ClassicPowerPlant.Collection]],
		//[ClassicWorkshop, [.]],
		[ClassicLorry, [ClassicLorry.Hit3, ClassicLorry.DoClonk, ClassicLorry.DoSoftHit]],
		[ConstructionSite, [ConstructionSite.Interact]]
	];
	
	for (var test in sounds)
	{
		var def = test[0];
		for (var function in test[1])
		{
			Log(" - Function %v", function);
			var obj = CreateObject(def, 10, 10, -1);
			obj->Call(function);
			if (obj) obj->RemoveObject();
		}
	}

	Log("---------------------");
	Log("Done!");

}
