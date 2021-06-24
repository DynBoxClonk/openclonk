#appendto ClassicCastle
#appendto ClassicChemicalFactory
#appendto ClassicElevator
#appendto ClassicFoundry
#appendto ClassicHutBamboo
#appendto ClassicHutWooden
#appendto ClassicPowerPlant
#appendto ClassicPump
#appendto ClassicWindmill
#appendto ClassicWizardTower
#appendto ClassicWorkshop


func DoConstructionEffects(object site)
{
	// Full con
	SetCon(100);
	// Fade
	FadeIn(50);
	// Overlay the effect: Fade in, then update to 100% con
	site->Sound("Structures::FinishBuilding");
	site->FadeOut(50, true);
	//site->RemoveObject();
	return true;
}
