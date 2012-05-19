/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#ifndef ADD_PROPERTY_ITEM_WINDOW_H
#define ADD_PROPERTY_ITEM_WINDOW_H

#include <Window.h>

class AddPropertyItemWindow: public BWindow
{
  	
  public:
	AddPropertyItemWindow();
	~AddPropertyItemWindow();
	
	virtual void MessageReceived(BMessage *message);
};

#endif /* ADD_PROPERTY_ITEM_WINDOW_H */