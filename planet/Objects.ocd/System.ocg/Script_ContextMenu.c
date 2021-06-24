/*
 * Generic implementation of the CR context menu.
 * 
 * Author: Marky
 */

global func OpenContextMenu(object cursor)
{
	if (!cursor) return false;
	
	//CreateMenu(Icon_Context, cursor, C4MN_Extra_None, "$TxtContextMenu$", nil, C4MN_Style_Context, false, Icon_Context);
	CreateMenu(Icon_Context, cursor, C4MN_Extra_None, "$TxtContextMenu$", nil, C4MN_Style_Context);
	
	if (!(cursor->~ContextMenuAddEntries()))
		cursor->CloseMenu();
}
