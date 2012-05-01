#include <InterfaceDefs.h>
#include <ScrollBar.h>
#include <Region.h>
#include <Window.h>
#include <Message.h>
#include <Application.h>
#include <Debug.h>
#include "Viewport.h"

Viewport::Viewport(BRect frame, const char *name, 
	uint32 resizingMode = B_FOLLOW_ALL_SIDES, 
	uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS):
	BView(frame, name, resizingMode, flags),
	fRatio(1.0), minRatio(0.25), maxRatio(2.5),
	action(false), apply_cursor(false), cursor(0)
{
	SetViewColor(tint_color(ui_color(B_PANEL_BACKGROUND_COLOR), 
		B_LIGHTEN_2_TINT));
}
	
Viewport::~Viewport()
{
}
	
void Viewport::AttachedToWindow(void)
{
	BView::AttachedToWindow();
	update_scrollbars();
}

void Viewport::Draw(BRect updateRect)
{
	BRect frame = DestinationRect() & updateRect;
	
	if (frame.IsValid())
		DrawTarget(frame);
}

void Viewport::FrameResized(float width, float height)
{
	BView::FrameResized(width, height);	
	update_scrollbars();
	Invalidate();
}

void Viewport::ScrollTo(BPoint where)
{
	BView::ScrollTo(where);
	Draw(DestinationRect());
}

void Viewport::MessageReceived(BMessage *message)
{
	BView::MessageReceived(message);
}

void Viewport::MouseMoved(BPoint where, uint32 transit, const BMessage *msg)
{
	BView::MouseMoved(where, transit, msg);
	
	if (Window() && !Window()->IsActive())
		return ;
		
	if (transit == B_INSIDE_VIEW && DestinationRect().Contains(where))
	{
		if (cursor && !apply_cursor)
		{
			apply_cursor = true;
			SetViewCursor(cursor);
		}
		
		if (action)
			AppendToAction(ConvertToTarget(where));
	}
	else
	{
		if (cursor && apply_cursor)
		{
			apply_cursor = false;
			SetViewCursor(B_CURSOR_SYSTEM_DEFAULT);
		}
	}
	
/*
	if (action)
	{
		if (transit == B_INSIDE_VIEW && DestinationRect().Contains(where))
			AppendToAction(ConvertToTarget(where));
		else
		{
			action = false;
			FinishAction();
		}
	}
*/
}

void Viewport::MouseDown(BPoint where)
{
	BView::MouseDown(where);
	
	if (DestinationRect().Contains(where))
		action = CreateAction(ConvertToTarget(where));
}

void Viewport::MouseUp(BPoint where)
{
	BView::MouseUp(where);
	
	if (action)
		FinishAction();
	
	action = false;
}

BRect Viewport::SourceRect(void) const
{
	BScrollBar *scrollBar = 0;
	
	BPoint origin = BPoint((scrollBar = ScrollBar(B_HORIZONTAL))? scrollBar->Value(): 0.0,
		(scrollBar = ScrollBar(B_VERTICAL))? scrollBar->Value(): 0.0);
	BRect source = BRect(origin, 
		origin + BPoint((Bounds().Width())/Ratio(), 
		(Bounds().Height())/Ratio()));
		
	if ( !BRect(0, 0, Width(), Height()).Contains(source) )
		source = BRect(0, 0, Width(), Height()) & source;
	
//	PRINT_OBJECT(source);

	return source;
}

BRect Viewport::DestinationRect(void) const
{
	BRect bounds = Bounds();
		
	BRect source = SourceRect();
	
	BPoint center = BPoint(bounds.left + bounds.Width()/2.0, 
		bounds.top + bounds.Height()/2.0);

	BRect destination = BRect(0.0, 0.0, 
		source.Width()*Ratio(), source.Height()*Ratio());
		
	destination.OffsetBy(center.x - (destination.Width())/2.0, 
		center.y - (destination.Height())/2.0 );

//	PRINT_OBJECT(destination);

	return destination;
}

BPoint Viewport::ConvertToTarget(BPoint point) const
{	
	BPoint convert_to_target = point - DestinationRect().LeftTop();
	convert_to_target.Set(convert_to_target.x/Ratio(), convert_to_target.y/Ratio());
//	convert_to_target = SourceRect().LeftTop() + convert_to_target;
	return SourceRect().LeftTop() + convert_to_target;
	
//	PRINT_OBJECT(convert_to_target);

//	return convert_to_target;
}

BPoint Viewport::ConvertFromTarget(BPoint point) const
{
	BPoint convert_from_target = point - SourceRect().LeftTop();
	convert_from_target.Set(convert_from_target.x*Ratio(), convert_from_target.y*Ratio());	
//	convert_from_target = DestinationRect().LeftTop() + convert_from_target;
	return DestinationRect().LeftTop() + convert_from_target;
//	return convert_from_target;
}

void Viewport::update_scrollbars(void)
{
	BScrollBar *scrollBar = 0;
	float k = 0.0;
	
	if ((scrollBar = ScrollBar(B_HORIZONTAL)))
	{
		k = Width() - Bounds().Width()/Ratio();
//		PRINT(("horizontal K = %f\n", k));
		scrollBar->SetRange(0.0, (k < 1.0)? 0.0: k);
		scrollBar->SetProportion(1.0 - k/Bounds().Width());
	}

	if ((scrollBar = ScrollBar(B_VERTICAL)))
	{
		k = Height() - Bounds().Height()/Ratio();
//		PRINT(("vertical K = %f\n", k));
		scrollBar->SetRange(0.0, (k < 1.0)? 0.0: k);
		scrollBar->SetProportion(1.0 - k/Bounds().Height());
	}
}

void Viewport::update_cursor(void)
{
	if (!cursor || (Window() && !Window()->IsActive()))
		return ;
	
	BPoint where = B_ORIGIN;
	uint32 buttons = 0;
	GetMouse(&where, &buttons);

	if (DestinationRect().Contains(where))
	{
		if (!apply_cursor)
		{
			apply_cursor = true;
			SetViewCursor(cursor);
			be_app->HideCursor();
			be_app->ShowCursor();
		}
	}
	else
	
	if (apply_cursor)
	{
		apply_cursor = false;
		SetViewCursor(B_CURSOR_SYSTEM_DEFAULT);
		be_app->HideCursor();
		be_app->ShowCursor();
	}
}

void Viewport::SetRatio(float ratio)
{
	if (ratio < minRatio || ratio > maxRatio)
		return ;

	fRatio = ratio;

	update_scrollbars();
	update_cursor();
	Invalidate();
	
	if (Window())
		Window()->PostMessage(RATIO_CHANGED, this);
}

void Viewport::SetRatioLimits(float min, float max)
{
	minRatio = min;
	maxRatio = max;
}

void Viewport::GetRatioLimits(float &min, float &max) const
{
	min = minRatio;
	max = maxRatio;
}

void Viewport::WindowActivated(bool state)
{
	BView::WindowActivated(state);
	
	// установить простой курсор (если менялся)
	if (apply_cursor && !state)
	{
		apply_cursor = false;
		SetViewCursor(B_CURSOR_SYSTEM_DEFAULT);
	}

	if (state)
		update_cursor();
	
	// ??? закончить "действие"
}

void Viewport::SetViewportCursor(const BCursor *new_cursor)
{
	cursor = new_cursor? new_cursor: 0;
	apply_cursor = false;
	update_cursor();
}
