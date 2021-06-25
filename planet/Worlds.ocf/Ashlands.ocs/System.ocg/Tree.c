#appendto Library_ChoppableTree

local dead = false;

// Prevent tree movement
func StartAnimation()
{
	if (GetAction() == "Chopped") return;
	_inherited(...);
}

func SetBurned()
{
	dead = true;
	SetGraphics("Burned");
	SetAction("Chopped");
}

func Construction()
{
	_inherited(...);
	
	// new normal trees should be extremely rare in Ashlands
	// other burned trees may grow though, even if this makes 
	// no sense => you need some wood sometimes
	if (Random(50))
	{
		SetBurned();
	}
}


func IsDeadTree(){ return dead; }