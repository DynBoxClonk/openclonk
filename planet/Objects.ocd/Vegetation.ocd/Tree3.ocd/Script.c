/*-- Baum --*/

#include Library_ChoppableTree

local FireTop = 30;
local Components = {Wood = 5};

local ActMap = {
Still = {
		Length=40,
		Delay=10,
		X=0, Y=0, Wdt=52, Hgt=75,
		NextAction="Still",
},
Breeze = {
		Length=40,
		Delay=2,
		X=0, Y=0, Wdt=52, Hgt=75,
},
};
