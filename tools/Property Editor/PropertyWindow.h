/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#ifndef PROPERTY_WINDOW_H
#define PROPERTY_WINDOW_H

#include <Window.h>
#include <OutlineListView.h>
#include "SimpleListView.h"

class PropertyWindow: public BWindow
{
	SimpleListView<BOutlineListView> *list;
	
  public:
	PropertyWindow();
	~PropertyWindow();

	virtual void FrameMoved(BPoint origin);
	virtual void FrameResized(float width, float height);
	virtual void MessageReceived(BMessage *message);
};

#endif /* PROPERTY_WINDOW_H */