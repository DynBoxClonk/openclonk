protected func Initialize()
{
}

protected func InitializePlayer(int player)
{
	StartTest(player);
	return;
}

global func StartTest(int player)
{
	var passed = true;
	
	passed &= Test1(player);
	passed &= Test2(player);
	passed &= Test3(player);

	Log("---------------------");
	Log("Done!");
	if (passed)
	{
		Log("All tests passed!");
	}
	else
	{
		Log("Test failed");
	}
}



global func Test1(int player)
{
	var passed = true;
	var seller = CreateObjectAbove(ClassicHutWooden, GetCrew(player)->GetX(), GetCrew(player)->GetY() + 10, player);

	Log("---------------------");
	Log("Testing Sell System:");

	var test_data = [
		[Oil, 16],
		[Lava, 4],
		[Acid, 4],
		[Water, 0]];
		
	var barrels = [Barrel, MetalBarrel];
	
	for (var barrel_type in barrels)
	{
		for (var data in test_data)
		{
			var liquid = data[0];
			var profit = data[1];
			if (!barrel_type->IsLiquidContainerForMaterial(liquid->GetLiquidType())) continue;
			
			passed &= TestSellBarrel(player, seller, barrel_type, liquid, barrel_type->GetLiquidContainerMaxFillLevel(), profit);
			passed &= TestSellBarrel(player, seller, barrel_type, liquid, barrel_type->GetLiquidContainerMaxFillLevel() / 2, profit / 2);
			passed &= TestSellBarrel(player, seller, barrel_type, liquid, barrel_type->GetLiquidContainerMaxFillLevel() / 4, profit / 4);
		}
	}

	if (seller) seller->RemoveObject();

	return passed;
}


global func Test2(int player)
{
	var passed = true;
	var base = CreateObjectAbove(ClassicHutWooden, GetCrew(player)->GetX(), GetCrew(player)->GetY() + 10, player);

	DoBaseMaterial(player, ClassicClonk, 1);

	Log("---------------------");
	Log("Test: Buy a clonk");
	
	SetWealth(player, ClassicClonk->GetValue());

	var clonk = base->DoBuy(ClassicClonk, player, player, base);
	
	passed &= doTest("Clonk should be created. Got %v, expected %v.", !!clonk, true);
	
	if (clonk)
	{
		passed &= doTest("Clonk has the correct owner. Got %d, expected %d.", clonk->GetOwner(), player);
		passed &= doTest("Clonk is in the hut. Got %v, expected %v.", clonk->Contained(), base);
		passed &= doTest("Clonk should be a crew member. Got %v, expected %d", GetCrew(player, 1), clonk);
	}

	if (clonk) clonk->RemoveObject();
	if (base) base->RemoveObject();

	return passed;
}


global func Test3(int player)
{
	var passed = true;
	var base = CreateObjectAbove(ClassicHutWooden, GetCrew(player)->GetX(), GetCrew(player)->GetY() + 10, player);

	Log("---------------------");
	Log("Test: Sell a shovel");

	passed &= doTest("Player should have no shovel buyable before selling it. Got %d, expected %d.", GetBaseMaterial(player, Shovel), 0);
	
	var shovel = base->CreateContents(Shovel);
	
	base->DoSell(shovel, player);
	
	passed &= doTest("Player should have a shovel buyable after selling it. Got %d, expected %d.", GetBaseMaterial(player, Shovel), 1);
	
	if (base) base->RemoveObject();

	return passed;
}


global func TestSellBarrel(int player, object seller, id def, id liquid, int fill_level, int expected_profit)
{
	var passed = true;
	SetWealth(player, 0);

	var barrel = seller->CreateContents(def);
	liquid->CreateLiquid(fill_level, barrel); //barrel->SetLiquidContainer(liquid, fill_level);

	seller->DoSell(barrel, player);
	
	var profit = GetWealth(player);
	var test = profit == expected_profit; passed &= test;
	Log(" - Selling %v barrel, filled 100%: profit = %d, expected %d; passed: %v", liquid, profit, expected_profit, test);	

	SetWealth(player, 0);
	
	test = (barrel == nil); passed &= test;
	Log("  * Barrel got removed: %v", test);
	var remaining_barrel = FindObject(Find_ID(def));
	test = (remaining_barrel != nil); passed &= test;
	Log("  * New barrel spawned: %v", test);

	if (remaining_barrel)
	{
		test = (remaining_barrel->GetLiquidAmount() <= 0); passed &= test;
		Log("  * New barrel is empty: %v", test);
		
		remaining_barrel->RemoveObject();
	}
	
	if (barrel) barrel->RemoveObject();

	return test;
}



global func doTest(description, returned, expected)
{
	var test = (returned == expected);
	
	var predicate = "[Fail]";
	if (test) predicate = "[Pass]";
	
	Log(Format("%s %s", predicate, description), returned, expected);
	return test;
}
