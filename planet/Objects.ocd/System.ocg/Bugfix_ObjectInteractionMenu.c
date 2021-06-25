/**
	Transfer vehicles from structures 
 */
 
#appendto GUI_ObjectInteractionMenu


private func TransferObjectsFromToSimple(array to_transfer, object source, object destination)
{
	var vehicles_transferred = TransferVehiclesAndCrewFromStructures(to_transfer, source, destination);
	return vehicles_transferred + _inherited(to_transfer, source, destination, ...);
}


private func TransferObjectsFromTo(array to_transfer, object source, object destination)
{
	var vehicles_transferred = TransferVehiclesAndCrewFromStructures(to_transfer, source, destination);
	return vehicles_transferred + _inherited(to_transfer, source, destination, ...);
}

private func TransferVehiclesAndCrewFromStructures(array to_transfer, object source, object destination)
{
	if (source->~IsStructure())
	{
		// Let all vehicles exit
		var transferred = [];
		for (var item in to_transfer)
		{
			if (item->~IsVehicle() || item->~IsClonk())
			{
				item->SetCommand("Exit");
				PushBack(transferred, item);
			}
		}
		// Remove from original array
		for (var vehicle in transferred)
		{
			RemoveArrayValue(to_transfer, vehicle);
		}
		return GetLength(transferred);
	}
	else
	{
		return 0;
	}
}
