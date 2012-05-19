/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include <InterfaceDefs.h>
#include "SimpleTriangleSlider.h"

SimpleTriangleSlider::SimpleTriangleSlider(BRect frame, const char *name, 
	BMessage *msg, int32 minValue, int32 maxValue, 
	uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, 
	uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW | B_NAVIGABLE):
	BView(frame, name, resizingMode, flags), slider(0)
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	slider = new BSlider(frame, ":BSlider", "", msg, 
		minValue, maxValue, B_TRIANGLE_THUMB, resizingMode, flags);
	
	if (slider)
	{
		AddChild(slider);
		slider->SetHashMarks(B_HASH_MARKS_NONE);
		slider->SetBarThickness(2.0);
		slider->MoveTo(0.0, 
			-slider->BarFrame().top + Bounds().Height()/2.0 - 2.0);
	}
}
	
SimpleTriangleSlider::~SimpleTriangleSlider()
{
}

void SimpleTriangleSlider::SetValue(int32 value)
{
	if (slider)
		slider->SetValue(value);
}
			
int32 SimpleTriangleSlider::Value(void) const
{
	return slider? slider->Value(): 0;
}
