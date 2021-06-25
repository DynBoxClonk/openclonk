/*-- Baum --*/

#include Library_ChoppableTree

local FireTop = 5;
local Components = {Wood = 3};

private func RootDepth() { return 0;} // This is how far it is rooted in the earth
public func NoForegroundVegetation(object plant){ return plant->GetID() == Grass; } // Grass in front of it looks stupid :/

local ActMap = {
Initialize = {
		Length=20,
},
Still = {
		Length=1,
		Delay=50,
		X=0, Y=0, Wdt=40, Hgt=56,
		NextAction="Still",
},
Breeze = {
		Length=20,
		Delay=5,
		X=0, Y=0, Wdt=40, Hgt=56,
},
};
