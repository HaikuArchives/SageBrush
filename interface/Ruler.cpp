/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include <Font.h>
#include <ScrollBar.h>
#include <Window.h>
#include <Debug.h>
#include <String.h>
#include <AppDefs.h>
#include <math.h>
#include <limits.h>
#include "Ruler.h"

static float V_RULER_WIDTH = B_V_SCROLL_BAR_WIDTH;
static float H_RULER_HEIGHT = B_H_SCROLL_BAR_HEIGHT;
static float HIDE_MARK_VALUE = LONG_MIN;

Ruler::Ruler(orientation direction, int32 align, 
	float border_start = 0.0, float border_end = 0.0):
	BView(BRect(), "ruler:", B_FOLLOW_NONE, B_WILL_DRAW),
	fOrientation(direction), ratio(1.0),
	markcolor(keyboard_navigation_color()), 
	units(Units::CENTIMETERS),
	rangeMin(0.0), rangeMax(0.0), mark(HIDE_MARK_VALUE)
{
	uint32 resizeMask = B_FOLLOW_NONE;
	BRect frame = BRect();

	if (direction == B_HORIZONTAL)
	{
		resizeMask = B_FOLLOW_LEFT_RIGHT;
		
		if (align == B_ALIGN_BOTTOM || align == B_ALIGN_TOP)
			resizeMask |= (align == B_ALIGN_TOP)? 
				(B_FOLLOW_TOP): (B_FOLLOW_BOTTOM);
		else
		{
			PRINT(("Ruler horizontal align not correct 0x%x with direction 0x%x", align, direction));
			resizeMask |= B_FOLLOW_TOP;
		}
		
		frame.Set(border_start, 0, border_start + border_end + 1.0, h_height());
		
		SetName("ruler:horizontal");
	}
	else
	{
		resizeMask = B_FOLLOW_TOP_BOTTOM;
		
		if (align == B_ALIGN_LEFT || align == B_ALIGN_RIGHT)
			resizeMask |= (align == B_ALIGN_LEFT)? 
				(B_FOLLOW_LEFT): (B_FOLLOW_RIGHT);
		else
		{
			PRINT(("Ruler vertical align not correct 0x%x with direction 0x%x", align, direction));
			resizeMask |= B_FOLLOW_LEFT;
		}
		
		frame.Set(0, border_start, v_width(), border_start + border_end + 1.0);
		
		SetName("ruler:vertical");
	}
	
	SetResizingMode(resizeMask);
	ResizeTo(frame.Width(), frame.Height());
	MoveTo(frame.LeftTop());
	
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	if (direction == B_VERTICAL)
	{
		// Повернем шрифт на 90°
		BFont font;
		GetFont(&font);
		font.SetRotation(90.0);
		SetFont(&font);
	}
}

Ruler::~Ruler()
{
}

void Ruler::AttachedToWindow(void)
{
	BRect frame = Parent()? Parent()->Bounds(): Window()->Bounds();
	
	if (Orientation() == B_HORIZONTAL)
	{
		ResizeTo(frame.Width() - Frame().right + 1.0, Frame().Height());
		
		if (Align() == B_ALIGN_BOTTOM)
			MoveBy(0, frame.Height() - Bounds().Height());
		
		SetFontSize(Bounds().Height()/1.5);
	}
	else
	{
		ResizeTo(Frame().Width(), frame.Height() - Frame().bottom + 1.0);
		
		if (Align() == B_ALIGN_RIGHT)
			MoveBy(frame.Width() - Bounds().Width(), 0);
		
		SetFontSize(Bounds().Width()/1.5);
	}
}

void Ruler::Draw(BRect updateRect)
{
	// rangeMin == rangeMax тогда линейка от -∞..∞
	
	float p = Units::GetConversion(Units::PIXELS)*Ratio()/
		((Units::GetConversion(units))*Units::GetDivisions(units));
	
	float _a = 0, _b = 0;
	int32 a = 0, b = 0;	
	BPoint c = B_ORIGIN, d = B_ORIGIN, e = B_ORIGIN, m = B_ORIGIN;
	const orientation direction = Orientation();
	
	font_height f_height; 
	GetFontHeight(&f_height);
	
	if (direction == B_HORIZONTAL)
	{
		if (rangeMin != rangeMax)
		{
			_a = max_c(updateRect.left, rangeMin*Ratio()); 
			_b = min_c(updateRect.right, rangeMax*Ratio());
		}
		else
			_a = updateRect.left, _b = updateRect.right;
	
		a = (int32) floor(_a/p); //updateRect.left/p);
		b = (int32) ceil(_b/p); //updateRect.right/p);
		
		c.Set(a*p, (Bounds().Height()/2.0) + 
			((f_height.ascent - f_height.descent)/2.0));

		d.Set(p, 0);
		
		if (Align() == B_ALIGN_TOP)
			m.Set(c.x, Bounds().bottom), e.Set(0, -2);
		else
			m.Set(c.x, Bounds().top), e.Set(0, 2);
	}
	else
	{
		if (rangeMin != rangeMax)
		{
			_a = max_c(updateRect.top, rangeMin*Ratio()); 
			_b = min_c(updateRect.bottom, rangeMax*Ratio());
		}
		else
			_a = updateRect.top, _b = updateRect.bottom;
		
		a = (int32) floor(_a/p); //updateRect.top/p);
		b = (int32) ceil(_b/p); //updateRect.bottom/p);
		c.Set((Bounds().Width()/2.0) + 
			((f_height.ascent - f_height.descent)/2.0), a*p);
		d.Set(0, p);
		
		if (Align() == B_ALIGN_LEFT)
			m.Set(Bounds().right, c.y),	e.Set(-2, 0);
		else
			m.Set(Bounds().left, c.y), e.Set(2, 0);
	}

	if (rangeMin != rangeMax)
	{
		if (a*p < rangeMin*Ratio())
			a++;
		
		if (b*p > rangeMax*Ratio())
			b--;
	}
		
	SetLowColor(ViewColor());
	
	BString mark;
	BPoint w = B_ORIGIN;
	float divisions = Units::GetDivisions(units);
	bool skip_draw_mark = false;
	
	if (rangeMin != rangeMax)
	{
		mark.SetTo("") << 
		  (int32) (max_c(fabs(rangeMin), fabs(rangeMax)))/p/Ratio()/divisions;

		skip_draw_mark = (StringWidth(mark.String())/2.0) > 
			p*((divisions > 1.0)? divisions: 1);
	}
	
	for (int32 i = a; i <= b; i++, c += d, m += d)
	{
		if (divisions > 1.0 && fmod(i, divisions) != 0.0)
		{
			SetHighColor(tint_color(ViewColor(), B_DARKEN_2_TINT));
			StrokeLine(m, m + e);
			continue ;
		}
		
		SetHighColor(ui_color(B_MENU_ITEM_TEXT_COLOR));
		StrokeLine(m, m + e);

		if (skip_draw_mark)
			continue;

		mark.SetTo("") << (int32) (i/divisions);

		if (direction == B_HORIZONTAL)
			w.Set(-(StringWidth(mark.String())/2.0) + 1.0, 0);
		else
			w.Set(0, (StringWidth(mark.String())/2.0));

		DrawString(mark.String(), c + w);
	}
}

void Ruler::WindowActivated(bool state)
{
	if (!state)
		HideMark();
}

orientation Ruler::Orientation(void) const
{
	return fOrientation;
}
	
void Ruler::SetRatio(float ratio)
{
	if (ratio == 0.0)
	{
		PRINT(("Ruler incorrect ratio value, ratio can`t be zero\n"));
		return;
	}
	
	Ruler::ratio = ratio;
	
	HideMark();	
	Invalidate();
}

void Ruler::SetMark(float new_mark)
{
	if (mark == new_mark || new_mark == HIDE_MARK_VALUE || 
		(Window() && !Window()->IsActive()))
		return;

	PushState();
	SetDrawingMode(B_OP_INVERT);
	SetHighColor(MarkColor());
	SetLowColor(ViewColor());
	SetPenSize(1.0);

	if (Orientation() == B_HORIZONTAL)
	{
		if (mark != HIDE_MARK_VALUE)
			StrokeLine(BPoint(mark, 0.0), BPoint(mark, Bounds().Height()), 
				B_MIXED_COLORS);
		
		mark = new_mark;

		StrokeLine(BPoint(mark, 0.0), BPoint(mark, Bounds().Height()), 
			B_MIXED_COLORS);
	}
	else
	{
		if (mark != HIDE_MARK_VALUE)
			StrokeLine(BPoint(0.0, mark), BPoint(Bounds().Width(), mark), 
				B_MIXED_COLORS);
		
		mark = new_mark;

		StrokeLine(BPoint(0.0, mark), BPoint(Bounds().Width(), mark), 
			B_MIXED_COLORS);
	}
	
	PopState();
}

void Ruler::HideMark(void)
{
	if (mark == HIDE_MARK_VALUE)
		return ;

	PushState();
	SetDrawingMode(B_OP_INVERT);
	SetHighColor(MarkColor());
	SetLowColor(ViewColor());
	SetPenSize(1.0);
	
	if (Orientation() == B_HORIZONTAL)
		StrokeLine(BPoint(mark, 0.0), BPoint(mark, Bounds().Height()), 
			B_MIXED_COLORS);
	else
		StrokeLine(BPoint(0.0, mark), BPoint(Bounds().Width(), mark), 
			B_MIXED_COLORS);
	
	PopState();
	
	mark = HIDE_MARK_VALUE;
}

void Ruler::SetUnits(Units::Type u)
{
	units = u;
	Invalidate();
}
	
void Ruler::SetMarkColor(rgb_color color)
{
	// Hide mark
	// markcolor = color;
	// Show mark
}

int32 Ruler::Align(void) const
{
	int32 resizeMask = ResizingMode();
	int32 result = 0;
	
	if (Orientation() == B_HORIZONTAL)
	{
		if ((resizeMask & ~B_FOLLOW_LEFT_RIGHT) == B_FOLLOW_TOP)
			result = B_ALIGN_TOP;
		else
		
		if ((resizeMask & ~B_FOLLOW_LEFT_RIGHT) == B_FOLLOW_BOTTOM)
			result = B_ALIGN_BOTTOM;
		else
			PRINT(("Can`t find correct horizontal Ruler align (ResizingMode = 0x%x\n)", resizeMask));		
	}
	else
	{
		if ((resizeMask & ~B_FOLLOW_TOP_BOTTOM) == B_FOLLOW_LEFT)
			result = B_ALIGN_LEFT;
		else
		
		if ((resizeMask & ~B_FOLLOW_TOP_BOTTOM) == B_FOLLOW_RIGHT)
			result = B_ALIGN_RIGHT;
		else
			PRINT(("Can`t find correct vertical Ruler align (ResizingMode = 0x%x\n)", resizeMask));
	}
	
	return result;
}

void Ruler::SetRange(float a, float b, Units::Type u = Units::PIXELS)
{
	rangeMin = min_c(a, b);
	rangeMax = max_c(a, b);
	
	if (u != Units::PIXELS)
	{
		rangeMin = Units::ConvertUnitsToPixels(u, rangeMin);
		rangeMax = Units::ConvertUnitsToPixels(u, rangeMax);
	}
}

void Ruler::set_v_width(float width)
{
	V_RULER_WIDTH = width;
}

void Ruler::set_h_height(float height)
{
	H_RULER_HEIGHT = height;
}

float Ruler::v_width(void)
{
	return V_RULER_WIDTH;
}

float Ruler::h_height(void)
{
	return H_RULER_HEIGHT;
}
