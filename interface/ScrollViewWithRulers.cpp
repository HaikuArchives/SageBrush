#include <AppDefs.h>
#include <Message.h>
#include <View.h>
#include <Menu.h>
#include <MenuItem.h>
#include <ClassInfo.h>
#include <Debug.h>
#include "MiniMenuField.h"
#include "ScrollViewWithRulers.h"

static filter_result scroll_ruler(BMessage *msg, BHandler **h, 
	BMessageFilter *f)
{
	int32 value = 0;

	if ( !(msg && msg->FindInt32("value", &value) == B_OK) )
	{
		PRINT(("Scroll View with rulers filter function bad message\n"));
		return B_DISPATCH_MESSAGE;
	}

	BScrollBar *scrollBar = cast_as(*h, BScrollBar);
	
	if (!scrollBar)
	{
		PRINT(("Scroll View with rulers filter function bad handler\n"));
		return B_DISPATCH_MESSAGE;
	}
	
	ScrollViewWithRulers *scroll = cast_as(scrollBar->Parent(), ScrollViewWithRulers);
	
	if (!scroll)
	{
		PRINT(("Scroll View with rulers filter function bad parent\n"));
		return B_DISPATCH_MESSAGE;
	}
	
	Ruler *ruler = scroll->GetRuler(scrollBar->Orientation());
	
	if (ruler)
	{
		if (ruler->Orientation() == B_HORIZONTAL)
			ruler->ScrollBy(value*ruler->Ratio() - ruler->Bounds().left, 0);
		else
			ruler->ScrollBy(0, value*ruler->Ratio() - ruler->Bounds().top);
	}
	
	return B_DISPATCH_MESSAGE;
}

static filter_result move_mark(BMessage *msg, BHandler **h, 
	BMessageFilter *f)
{
	BPoint where = B_ORIGIN;
	
	if (msg->FindPoint("where", &where) != B_OK)
		return B_DISPATCH_MESSAGE;
	
	int32 transit = 0;
	
	if (msg->FindInt32("be:transit", &transit) != B_OK)
		return B_DISPATCH_MESSAGE;
	
	BView *view = cast_as(*h, BView);

	ScrollViewWithRulers *scroll = cast_as(view->Parent(), ScrollViewWithRulers);
	
	if (!scroll)
		return B_DISPATCH_MESSAGE;
	
	scroll->ConvertFromParent(&where);
	
	Ruler *ruler_h = scroll->GetRuler(B_HORIZONTAL);
	Ruler *ruler_v = scroll->GetRuler(B_VERTICAL);
	float a = 0, b = 0, c = 0, d = 0;
	
	if (ruler_h)
	{
		where.x += ruler_h->Bounds().left - ruler_h->Frame().left;
		ruler_h->GetRangeInPixels(a, b);
		a *= ruler_h->Ratio();
		b *= ruler_h->Ratio();
	}
	
	if (ruler_v)
	{
		where.y += ruler_v->Bounds().top - ruler_v->Frame().top;
		ruler_v->GetRangeInPixels(c, d);
		c *= ruler_v->Ratio();
		d *= ruler_v->Ratio();
	}
	
	BRect frame = BRect(a, c, b, d);
	
	if (ruler_h && ruler_v && a != b && c != d && !frame.Contains(where))
	{
		ruler_h->HideMark();
		ruler_v->HideMark();
		return B_DISPATCH_MESSAGE;
	}
	
	if (ruler_h)
	{
		if (transit == B_INSIDE_VIEW && 
		  ((a != b)? (where.x > a && where.x < b): 1))
			ruler_h->SetMark(where.x);
		else
			ruler_h->HideMark();
	}
	
	if (ruler_v)
	{
		if (transit == B_INSIDE_VIEW && 
		  ((c != d)? (where.y > c && where.y < d): 1))
			ruler_v->SetMark(where.y);
		else
			ruler_v->HideMark();
	}
	
	return B_DISPATCH_MESSAGE;
}

ScrollViewWithRulers::ScrollViewWithRulers(const char *name, BView *target, 
	uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = 0,
	bool horizontal = false, bool vertical = false,
	border_style border = B_FANCY_BORDER):
	BScrollView(name, target, resizingMode, flags, horizontal, vertical, border),
	rulerSettings(0), ruler_v(0), ruler_h(0), filter_h(0), filter_v(0)
{
	if (target)
		target->AddFilter(new BMessageFilter(B_MOUSE_MOVED, move_mark));
}
	
ScrollViewWithRulers::~ScrollViewWithRulers()
{
}

static void fill_picture(BPictureButton *button)
{
	if (!button)
		return ;
		
	rgb_color color = ui_color(B_PANEL_BACKGROUND_COLOR);
		
	BPoint  a = BPoint(2.0*button->Bounds().Width()/3.0, 1.0),
		b = BPoint(a.x, button->Bounds().Height() - 1.0),
		c = BPoint(1.0, 2.0*button->Bounds().Height()/3.0 - 1.0),
		d = BPoint(button->Bounds().Width() - 1.0, c.y);
		
	button->BeginPicture(new BPicture);
		button->PushState();
		button->SetHighColor(tint_color(color, B_DARKEN_2_TINT));
		button->FillRect(button->Bounds());
		button->SetHighColor(tint_color(color, B_DARKEN_3_TINT));
		button->SetLowColor(tint_color(color, B_DARKEN_2_TINT));
//		button->StrokeRect(button->Bounds());
		button->StrokeLine(a, b, B_MIXED_COLORS);
		button->StrokeLine(c, d, B_MIXED_COLORS);
		button->PopState();
	BPicture *on = button->EndPicture();
		
	button->BeginPicture(new BPicture);
		button->PushState();
		button->SetHighColor(color);
		button->FillRect(button->Bounds());
		button->SetHighColor(tint_color(color, B_DARKEN_3_TINT));
		button->SetLowColor(color);
//		button->StrokeRect(button->Bounds());
		button->StrokeLine(a, b, B_MIXED_COLORS);
		button->StrokeLine(c, d, B_MIXED_COLORS);
		button->PopState();
	BPicture *off = button->EndPicture();
		
	button->SetEnabledOn(on);
	button->SetEnabledOff(off);
		
	if (on)
		delete on;
	
	if (off)
		delete off;
}
	
void ScrollViewWithRulers::ShowRulers(bool show = true)
{
	if (show? HasRulers(): !HasRulers())
		return ;
	
	BView *target = Target();
	BScrollBar *scrollBar = 0;
	
	if (!target)
	{
		PRINT(("ScrollView with Rulers target is NULL"));
		return;
	}
	
	if (show)
	{
		long border_size = (Border() == B_PLAIN_BORDER)? 1:
			((Border() == B_FANCY_BORDER)? 2: 0);
		Ruler *ruler = 0;
	
		BMenu *menu = new BMenu("Metrics.Menu");
		
		if (menu)
		{
			BMenuItem *menuItem = 0;
			BString s = "";
			
			Units::GetUnitsName(Units::INCHES, s);
			menu->AddItem(new BMenuItem(s.String(), new BMessage(SET_INCHES)));
			
			// Set CENTIMETERS marked
			Units::GetUnitsName(Units::CENTIMETERS, s);
			menu->AddItem( (menuItem = 	
				new BMenuItem(s.String(), new BMessage(SET_CENTIMETERS))) );
			
			if (menuItem)
				menuItem->SetMarked(true);
			
			Units::GetUnitsName(Units::PICAS, s);
			menu->AddItem(new BMenuItem(s.String(), new BMessage(SET_PICAS)));
			
			Units::GetUnitsName(Units::PIXELS, s);
			menu->AddItem(new BMenuItem(s.String(), new BMessage(SET_PIXELS)));
			
			menu->AddSeparatorItem();
			
			menu->SetTargetForItems(this);
			
			menu->AddItem( (menuItem = 
				new BMenuItem("Hide", new BMessage(HIDE_RULERS))) );
			
			if (menuItem)
				menuItem->SetTarget(this);
		}
		
		MiniMenuField *button = new MiniMenuField(
			BRect(0.0, 0.0, Ruler::v_width(), Ruler::h_height()), 
			"ruler:SettingsButton", "", menu);

		if ( (rulerSettings = cast_as(button, BPictureButton)) )
		{	
			button->SetLabelFromMarked(false);
			AddChild(rulerSettings);
			rulerSettings->MoveTo(border_size, border_size);
			fill_picture(button);
		}
	
		target->ResizeBy(-(Ruler::v_width() + 1.0), -(Ruler::h_height() + 1.0));
		target->MoveBy(Ruler::v_width() + 1.0, Ruler::h_height() + 1.0);

		ruler = new Ruler(B_HORIZONTAL, B_ALIGN_TOP, Ruler::v_width() + 1, B_V_SCROLL_BAR_WIDTH + border_size*2);
		
		if ( (ruler_h = ruler) )
		{
			AddChild(ruler);
			ruler->MoveBy(border_size, border_size);
		}
		
		ruler = new Ruler(B_VERTICAL, B_ALIGN_LEFT, Ruler::h_height() + 1, B_H_SCROLL_BAR_HEIGHT + border_size*2);
		
		if ( (ruler_v = ruler) )
		{
			AddChild(ruler);
			ruler->MoveBy(border_size, border_size);
		}
				
		if ((scrollBar = ScrollBar(B_HORIZONTAL)) && filter_h == 0)
		{
			PRINT(("ScrollView with rulers create horizontal filter\n"));
			filter_h = new BMessageFilter(B_VALUE_CHANGED, scroll_ruler);
			scrollBar->AddFilter(filter_h);
		}

		if ((scrollBar = ScrollBar(B_VERTICAL)) && filter_v == 0)
		{
			PRINT(("ScrollView with rulers create vertical filter\n"));
			filter_v = new BMessageFilter(B_VALUE_CHANGED, scroll_ruler);
			scrollBar->AddFilter(filter_v);
		}
	}
	else
	{
		/* Удаляем линейки */
		
		// Удаляем фильтры на скролирование
		if ((scrollBar = ScrollBar(B_HORIZONTAL)) && filter_h)
			if (scrollBar->RemoveFilter(filter_h))
			{
				PRINT(("ScrollView with rulers delete horizontal filter\n"));
				delete filter_h;
				filter_h = 0;
			}
		
		if ((scrollBar = ScrollBar(B_VERTICAL)) && filter_v)
			if (scrollBar->RemoveFilter(filter_v))
			{
				PRINT(("ScrollView with rulers delete vertical filter\n"));
				delete filter_v;
				filter_v = 0;
			}

		// Возращаем target на место, Удаляем линейки
		target->ResizeBy(Ruler::v_width() + 1.0, Ruler::h_height() + 1.0);
		target->MoveBy(-(Ruler::v_width() + 1.0), -(Ruler::h_height() + 1.0));
		
		RemoveChild(rulerSettings);
		delete rulerSettings;
		rulerSettings = 0;
		
		if (ruler_h)
		{
			ruler_h->RemoveSelf();
			delete ruler_h;
			ruler_h = 0;
		}
		
		if (ruler_v)
		{
			ruler_v->RemoveSelf();
			delete ruler_v;
			ruler_v = 0;
		}		
	}
}
	
void ScrollViewWithRulers::MessageReceived(BMessage *message)
{
	if (!message)
		return ;
	
	switch (message->what)
	{
		case SET_INCHES:
			if (HasRulers())
			{
				ruler_h->SetUnits(Units::INCHES);
				ruler_v->SetUnits(Units::INCHES);
			}
			break;
			
		case SET_CENTIMETERS:
			if (HasRulers())
			{
				ruler_h->SetUnits(Units::CENTIMETERS);
				ruler_v->SetUnits(Units::CENTIMETERS);
			}
			break;
			
		case SET_PICAS:
			if (HasRulers())
			{
				ruler_h->SetUnits(Units::PICAS);
				ruler_v->SetUnits(Units::PICAS);
			}
			break;
			
		case SET_PIXELS:
			if (HasRulers())
			{
				ruler_h->SetUnits(Units::PIXELS);
				ruler_v->SetUnits(Units::PIXELS);
			}
			break;
	
		case HIDE_RULERS:
			if (HasRulers())
				ShowRulers(false);
			break;
		
		default:
			BScrollView::MessageReceived(message);
			break;
	}
}
