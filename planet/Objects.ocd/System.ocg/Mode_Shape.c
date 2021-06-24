#appendto Shape

public func RectangleFit(proplist rectangle, id definition)
{
	rectangle.x -= definition->GetDefOffset(0);
	rectangle.y -= definition->GetDefOffset(1);
	rectangle.wdt -= definition->GetDefWidth();
	rectangle.hgt -= definition->GetDefHeight();
	return rectangle;
}
