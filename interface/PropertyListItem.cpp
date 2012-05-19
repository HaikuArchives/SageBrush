/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include "PropertyListItem.h"

PropertyListItem::PropertyListItem(const char *text, uint32 outlineLevel = 0, 
	bool expanded = true):
	EditableListItem(text, outlineLevel, expanded)
{
	SetEditable(true);
	SetName("No name");
}

PropertyListItem::~PropertyListItem()
{
}

void PropertyListItem::DrawItem(BView *owner, BRect itemRect, 
		bool drawEverything = false)
{
	float d = itemRect.Width()*0.35;
	
	SetTextPosition(BPoint(d + 3, 0));

	EditableListItem::DrawItem(owner, itemRect, drawEverything);
	
	rgb_color itemColor = IsSelected()?
		ui_color(B_MENU_SELECTION_BACKGROUND_COLOR):
		ui_color(B_MENU_BACKGROUND_COLOR);
	
	BRect nameRect = BRect(0, 0, d - 3, itemRect.Height());
	nameRect.OffsetTo(itemRect.left, itemRect.top);
	
	BString name = Name();
	
	font_height height;
	owner->GetFontHeight(&height);
	
	owner->PushState();		
		owner->SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), 
			B_DARKEN_2_TINT));
		
		owner->SetLowColor(itemColor);
				
		owner->StrokeLine(itemRect.LeftBottom(), itemRect.RightBottom(),
			B_MIXED_COLORS);
		
		owner->SetHighColor(tint_color(itemColor, B_LIGHTEN_1_TINT));
		owner->StrokeLine(itemRect.LeftTop() + BPoint(d, 0), 
			itemRect.LeftBottom() + BPoint(d, - 1));
	
		owner->SetHighColor(ui_color(B_MENU_ITEM_TEXT_COLOR));
		owner->MovePenTo(nameRect.left, nameRect.bottom - height.descent);
		
		if (owner->StringWidth(name.String()) > nameRect.Width())
		{
			BString tmpName(name);
			owner->TruncateString(&tmpName, B_TRUNCATE_END, 
				nameRect.Width() - 5);
			owner->DrawString(tmpName.String());
		}
		else
			owner->DrawString(name.String());
	
	owner->PopState();
}

void PropertyListItem::Update(BView *owner, const BFont *font)
{
}

void PropertyListItem::SetName(const char *name)
{
	PropertyListItem::name.SetTo(name);
}
	
const char *PropertyListItem::Name(void) const
{
	return name.String();
}
