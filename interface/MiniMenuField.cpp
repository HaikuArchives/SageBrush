/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include <Picture.h>
#include <MenuItem.h>
#include <Window.h>
#include <String.h>
#include <Messenger.h>
#include "MiniMenuField.h"

static BPicture on = BPicture(), off = BPicture();

MiniMenuField::MiniMenuField(BRect frame, const char *name, const char *label, 
	BMenu *menu, uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, 
	uint32 flags = B_WILL_DRAW | B_NAVIGABLE):
	BPictureButton(frame, name, &off, &on, 0, B_ONE_STATE_BUTTON, resizingMode, flags),
	BPopUpMenu(BString(name).Append(":PopUpMenu").String())
{
	BControl::SetLabel(label);
	
	BMenuItem *item = 0;
	
	if (menu)
		while (menu->CountItems() > 0 && (item = menu->RemoveItem((int32) 0)))
			AddItem(item);
}

MiniMenuField::~MiniMenuField()
{
}
	
void MiniMenuField::MouseDown(BPoint point)
{
	if (BPopUpMenu::IsFocus())
		BPopUpMenu::MouseDown(point);
	else
	{
		if (!BPictureButton::IsEnabled())
			return ;
		
		BPictureButton::MouseDown(point);
		BPictureButton::ConvertToScreen(&point);
		
		point.Set(point.x - 5.0, point.y - 5.0);
		
		BMenuItem *menuItem = BPopUpMenu::Go(point);
		
		BPictureButton::MouseUp(B_ORIGIN);
		
		if (!menuItem)
			return ;
		
		if (BPopUpMenu::IsRadioMode())
			menuItem->SetMarked(true);
		
		if (BPopUpMenu::IsLabelFromMarked())
			SetLabel(menuItem->Label());
		
		if (menuItem->Message())
			menuItem->Messenger().SendMessage(menuItem->Message());
	}
}

void MiniMenuField::SetLabel(const char *string)
{
	BPictureButton::SetLabel(string);
	
	if (!BPictureButton::Window())
		return ;
	
	BPicture *picture = 0;
	
	BPictureButton::SetFontSize(BPictureButton::Bounds().Height()/1.25);
	BString s = BString(string);	
	BPictureButton::TruncateString(&s, B_TRUNCATE_END, BPictureButton::Bounds().Width() - BPictureButton::Bounds().Height()*1.5);
	font_height height;
	BPictureButton::GetFontHeight(&height);
	BPoint point = BPoint(
		(BPictureButton::Bounds().Width() - BPictureButton::StringWidth(s.String()))/2.0, 
		BPictureButton::Bounds().Height() - height.descent - 1.0);
	
	BPictureButton::BeginPicture(new BPicture);
		BPictureButton::PushState();
		BPictureButton::SetHighColor(ui_color(B_MENU_BACKGROUND_COLOR));
		BPictureButton::FillRect(BPictureButton::Bounds());		
		BPictureButton::SetHighColor(ui_color(B_MENU_ITEM_TEXT_COLOR));
		BPictureButton::SetLowColor(ui_color(B_MENU_BACKGROUND_COLOR));
		BPictureButton::DrawString(s.String(), point);
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_DARKEN_4_TINT));
		BPictureButton::FillTriangle(
			BPictureButton::Bounds().RightBottom() - BPoint(8, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(2, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(5, 3));
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_DARKEN_2_TINT));
		BPictureButton::PopState();	
	picture = BPictureButton::EndPicture();
	
	if (picture)
	{
		BPictureButton::SetEnabledOff(picture);
		delete picture;
	}
	
	BPictureButton::BeginPicture(new BPicture);
		BPictureButton::PushState();
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_HIGHLIGHT_BACKGROUND_TINT));
		BPictureButton::FillRect(BPictureButton::Bounds());		
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_LIGHTEN_1_TINT));
		BPictureButton::SetLowColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_HIGHLIGHT_BACKGROUND_TINT));
		BPictureButton::DrawString(s.String(), point);
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_LIGHTEN_1_TINT));
		BPictureButton::FillTriangle(
			BPictureButton::Bounds().RightBottom() - BPoint(8, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(2, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(5, 3));
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_DARKEN_2_TINT));
		BPictureButton::PopState();		
	picture = BPictureButton::EndPicture();
	
	if (picture)
	{
		BPictureButton::SetEnabledOn(picture);
		delete picture;
	}
	
	BPictureButton::BeginPicture(new BPicture);
		BPictureButton::PushState();
		BPictureButton::SetHighColor(ui_color(B_MENU_BACKGROUND_COLOR));
		BPictureButton::FillRect(BPictureButton::Bounds());		
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_DISABLED_LABEL_TINT));
		BPictureButton::SetLowColor(ui_color(B_MENU_BACKGROUND_COLOR));
		BPictureButton::DrawString(s.String(), point);
		BPictureButton::FillTriangle(
			BPictureButton::Bounds().RightBottom() - BPoint(8, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(2, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(5, 3));
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_DARKEN_2_TINT));
		BPictureButton::PopState();	
	picture = BPictureButton::EndPicture();
	
	if (picture)
	{
		BPictureButton::SetDisabledOff(picture);
		delete picture;
	}
	
	BPictureButton::BeginPicture(new BPicture);
		BPictureButton::PushState();
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_HIGHLIGHT_BACKGROUND_TINT));
		BPictureButton::FillRect(BPictureButton::Bounds());		
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_DISABLED_LABEL_TINT));
		BPictureButton::SetLowColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_HIGHLIGHT_BACKGROUND_TINT));
		BPictureButton::DrawString(s.String(), point);
		BPictureButton::FillTriangle(
			BPictureButton::Bounds().RightBottom() - BPoint(8, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(2, 6), 
			BPictureButton::Bounds().RightBottom() - BPoint(5, 3));
		BPictureButton::SetHighColor(tint_color(ui_color(B_MENU_BACKGROUND_COLOR), B_DARKEN_2_TINT));
		BPictureButton::PopState();		
	picture = BPictureButton::EndPicture();
	
	if (picture)
	{
		BPictureButton::SetDisabledOn(picture);
		delete picture;
	}
}

void MiniMenuField::AttachedToWindow(void)
{
	if (!BPopUpMenu::Window())
		SetLabel(Label());
	else
		BPopUpMenu::AttachedToWindow();
}
