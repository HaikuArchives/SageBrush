#ifndef MINIMENUFIELD_H
#define MINIMENUFIELD_H

#include <PictureButton.h>
#include <PopUpMenu.h>
#include <Menu.h>

class MiniMenuField: public BPictureButton, public BPopUpMenu
{
  public:
	MiniMenuField(BRect frame, const char *name, const char *label, 
		BMenu *menu, uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, 
		uint32 flags = B_WILL_DRAW | B_NAVIGABLE);
	~MiniMenuField();
	
	virtual void MouseDown(BPoint point);
	virtual void SetLabel(const char *string);
	virtual void AttachedToWindow(void);
};

#endif /* MINIMENUFIELD_H */