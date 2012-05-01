#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <View.h>
#include <ScrollView.h>
#include <Cursor.h>

class Viewport: public BView
{
	float fRatio, minRatio, maxRatio;
	bool action, apply_cursor;
	const BCursor *cursor;
	
	virtual void update_scrollbars(void);
	virtual void update_cursor(void);
  public:
  	enum { RATIO_CHANGED = 'rtcg' };
  
	Viewport(BRect frame, const char *name, 
		uint32 resizingMode = B_FOLLOW_ALL_SIDES,
		uint32 flags = B_WILL_DRAW | B_FRAME_EVENTS);
	
	~Viewport();
	
	virtual void AttachedToWindow(void);
	virtual void Draw(BRect updateRect);
	virtual void FrameResized(float width, float height);
	virtual void ScrollTo(BPoint where);
	virtual void MessageReceived(BMessage *message);
	virtual void MouseMoved(BPoint where, uint32 transit, const BMessage *msg);
	virtual void MouseDown(BPoint where);
	virtual void MouseUp(BPoint where);
	virtual	void WindowActivated(bool state);
	
	BRect SourceRect(void) const;
	BRect DestinationRect(void) const;
	BPoint ConvertToTarget(BPoint point) const;
	BPoint ConvertFromTarget(BPoint point) const;
	
	inline float Ratio(void) const;
	void SetRatio(float ratio);
	void SetRatioLimits(float min, float max);
	void GetRatioLimits(float &min, float &max) const;
	
	virtual bool CreateAction(BPoint where) = 0;
	virtual void AppendToAction(BPoint where) = 0;
	virtual void FinishAction(void) = 0;
	
	virtual float Width(void) const = 0;
	virtual float Height(void) const = 0;
	
	virtual void DrawTarget(BRect updateRect) = 0;

	void SetViewportCursor(const BCursor *new_cursor);
};

inline float Viewport::Ratio(void) const
{
	return fRatio;
}

#endif /* VIEWPORT_H */