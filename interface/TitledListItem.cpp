#include <Message.h>
#include <View.h>
#include <String.h>
#include "TitledListItem.h"

TitledListItem::TitledListItem(const char *text, uint32 outlineLevel = 0, 
	bool expanded = true): BStringItem(text, outlineLevel, expanded),
	position(B_ORIGIN), editable(false)
{
}

TitledListItem::TitledListItem(BMessage *data):
	BStringItem(data), position(B_ORIGIN), editable(false)
{
	BPoint point = B_ORIGIN;
	
	if (data && data->FindPoint("title_pos", &point) == B_OK)
		position = point;
}

TitledListItem::~TitledListItem()
{
}

status_t TitledListItem::Archive(BMessage *data, bool deep = true) const
{
	BStringItem::Archive(data, deep);
	
	if (data)
		data->AddPoint("title_pos", position);
	
	return 0;
}

BArchivable *TitledListItem::Instantiate(BMessage *data)
{
	if (!validate_instantiation(data, "TitledListItem"))
		return NULL;
	return new TitledListItem(data);
}
	
void TitledListItem::DrawItem(BView *view, BRect itemRect, bool drawEverything)
{
	if (!view || !itemRect.IsValid())
		return ;
	
	rgb_color itemColor = IsSelected()?
		ui_color(B_MENU_SELECTION_BACKGROUND_COLOR):
		ui_color(B_MENU_BACKGROUND_COLOR);
	BRect titleRect = BRect();
	BString title;
	BPoint whereTitle = TitlePosition();
	
	title.SetTo(Text());
	
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
		
		titleRect.Set(0, 0, view->StringWidth(title.String()), 
			height.ascent + height.descent + height.leading + 1);
		titleRect.OffsetTo(itemRect.left + whereTitle.x, 
			itemRect.top + whereTitle.y);
		
		view->MovePenTo(titleRect.left, titleRect.bottom - height.descent);
		
		if (!itemRect.Contains(titleRect))
		{
			BString tmpTitle(title);
			titleRect.right = itemRect.right - 10;
			view->TruncateString(&tmpTitle, B_TRUNCATE_END, 
				titleRect.Width() - 5);
			view->DrawString(tmpTitle.String());
		}
		else
			view->DrawString(title.String());
		
	view->PopState();
}

void TitledListItem::Update(BView *owner, const BFont *font)
{
	SetWidth(owner->Bounds().Width());
//	BStringItem::Update(owner, font);
}
	
void TitledListItem::SetEditable(bool Editable = true)
{
	editable = Editable;
}
	
void TitledListItem::SetTitlePosition(BPoint point)
{
	position = point;
}
