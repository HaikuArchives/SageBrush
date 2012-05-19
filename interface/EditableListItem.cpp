/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#include <Message.h>
#include <View.h>
#include <String.h>
#include <Font.h>
#include "EditableListItem.h"

EditableListItem::EditableListItem(const char *text, uint32 outlineLevel = 0, 
	bool expanded = true): BStringItem(text, outlineLevel, expanded),
	position(B_ORIGIN), editable(false)
{
	font_height height;
	be_plain_font->GetHeight(&height);
	SetHeight(height.ascent + height.descent + height.leading + 4);
}

EditableListItem::EditableListItem(BMessage *data):
	BStringItem(data), position(B_ORIGIN), editable(false)
{
	BPoint point = B_ORIGIN;
	
	if (data && data->FindPoint("text_pos", &point) == B_OK)
		position = point;
}

EditableListItem::~EditableListItem()
{
}

status_t EditableListItem::Archive(BMessage *data, bool deep = true) const
{
	BStringItem::Archive(data, deep);
	
	if (data)
		data->AddPoint("text_pos", position);
	
	return 0;
}

BArchivable *EditableListItem::Instantiate(BMessage *data)
{
	if (!validate_instantiation(data, "EditableListItem"))
		return NULL;
	return new EditableListItem(data);
}
	
void EditableListItem::DrawItem(BView *view, BRect itemRect, bool drawEverything)
{
	if (!view || !itemRect.IsValid())
		return ;
	
	rgb_color itemColor = IsSelected()?
		ui_color(B_MENU_SELECTION_BACKGROUND_COLOR):
		ui_color(B_MENU_BACKGROUND_COLOR);
		
	BRect textRect = BRect();
	BString text;
	BPoint whereText = TextPosition();
	
	text.SetTo(Text());
	
	view->PushState();
		view->SetHighColor(itemColor);
		view->SetLowColor(itemColor);
		view->FillRect(itemRect);
		view->SetHighColor(ui_color(B_MENU_ITEM_TEXT_COLOR));
		
		if (!IsEditable())
		{
			BFont font;
			view->GetFont(&font);
			font.SetFace(B_ITALIC_FACE);
			view->SetFont(&font);
			view->SetHighColor(tint_color(view->HighColor(), B_LIGHTEN_1_TINT));
		}
		
		font_height height;
		view->GetFontHeight(&height);
		
		textRect.Set(0, 0, view->StringWidth(text.String()), 
			height.ascent + height.descent + height.leading + 1);
			
		textRect.OffsetTo(itemRect.left + whereText.x, 
			itemRect.top + whereText.y);
		
		view->MovePenTo(textRect.left, textRect.bottom - height.descent + 2);
		
		if (!itemRect.Contains(textRect))
		{
			BString tmpText(text);
			textRect.right = itemRect.right - 10;
			view->TruncateString(&tmpText, B_TRUNCATE_END, 
				textRect.Width() - 5);
			view->DrawString(tmpText.String());
		}
		else
			view->DrawString(text.String());
		
	view->PopState();
}

void EditableListItem::Update(BView *owner, const BFont *font)
{
	SetWidth(owner->Bounds().Width());
//	BStringItem::Update(owner, font);
}
	
void EditableListItem::SetEditable(bool Editable = true)
{
	editable = Editable;
}
	
void EditableListItem::SetTextPosition(BPoint point)
{
	position = point;
}
