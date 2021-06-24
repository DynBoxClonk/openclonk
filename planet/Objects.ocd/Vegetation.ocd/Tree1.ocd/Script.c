/*-- Baum --*/

#include Library_ChoppableTree

local FireTop = 10;
local Components = {Wood = 7};

local ActMap = {

Still = {
		Length=10,
		Delay=4,
		X=0, Y=0, Wdt=73, Hgt=73,
		NextAction="StillSlow",
		StartCall="Still",
},

StillSlow = {
		Prototype = Action,
		Name="StillSlow",
		Length=2,
		Delay=5,
		X=730, Y=0, Wdt=73, Hgt=73,
		FacetBase=0,
		NextAction="StillSlower",
},

StillSlower = {
		Prototype = Action,
		Name="StillSlower",
		Length=2,
		Delay=7,
		X=876, Y=0, Wdt=73, Hgt=73,
		FacetBase=0,
		NextAction="StillSlowBack",
},

StillSlowBack = {
		Prototype = Action,
		Name="StillSlowBack",
		Length=4,
		Delay=7,
		X=730, Y=0, Wdt=73, Hgt=73,
		Reverse=1,
		FacetBase=0,
		NextAction="StillBack",
},

StillBack = {
		Prototype = Action,
		Name="StillBack",
		Length=10,
		Delay=4,
		X=0, Y=0, Wdt=73, Hgt=73,
		Reverse=1,
		FacetBase=0,
		NextAction="Still",
},

Breeze = {
		Length=40,
		Delay=2,
		X=0, Y=0, Wdt=73, Hgt=73,
},
};

