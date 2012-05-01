#ifndef SCROLLVIEWWITHRULERS_H
#define SCROLLVIEWWITHRULERS_H

#include <View.h>
#include <ScrollView.h>
#include <PictureButton.h>
#include <MessageFilter.h>
#include "Units.h"
#include "Ruler.h"

class ScrollViewWithRulers: public BScrollView
{
	BPictureButton *rulerSettings;
	Ruler *ruler_v, *ruler_h;
	BMessageFilter *filter_h, *filter_v;
  public:
  
	enum 
	{ 
		SET_INCHES = 'stin', SET_CENTIMETERS = 'stcm', SET_PICAS = 'stpc',
		SET_PIXELS = 'stpx', HIDE_RULERS = 'hide' 
	};

	ScrollViewWithRulers(const char *name, BView *target, 
		uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = 0,
		bool horizontal = false, bool vertical = false,
		border_style border = B_FANCY_BORDER);
	
	~ScrollViewWithRulers();
	
	inline bool HasRulers(void) const;
	inline Ruler* GetRuler(orientation ruler);
	
	void ShowRulers(bool show = true);
	
	virtual void MessageReceived(BMessage *message);
};

inline bool ScrollViewWithRulers::HasRulers(void) const
{
	return (rulerSettings != 0);
}

inline Ruler* ScrollViewWithRulers::GetRuler(orientation ruler)
{
	return (HasRulers()? ((ruler == B_HORIZONTAL)? ruler_h: ruler_v): 0);
}

#endif /* SCROLLVIEWWITHRULERS_H */