#ifndef SIMPLE_TRIANGLE_SLIDER_H
#define SIMPLE_TRIANGLE_SLIDER_H

#include <View.h>
#include <Slider.h>

class SimpleTriangleSlider: public BView
{
	BSlider *slider;
  public:
	SimpleTriangleSlider(BRect frame, const char *name, BMessage *msg, 
		int32 minValue, int32 maxValue, 
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, 
		uint32 flags = B_FRAME_EVENTS | B_WILL_DRAW | B_NAVIGABLE);	
	
	~SimpleTriangleSlider();
			
	virtual void SetValue(int32 value);
	virtual int32 Value(void) const;
};

#endif /* SIMPLE_TRIANGLE_SLIDER_H */