/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#ifndef EDITABLE_LIST_ITEM_H
#define EDITABLE_LIST_ITEM_H

#include <Point.h>
#include <Rect.h>
#include <ListItem.h>

class EditableListItem: public BStringItem
{
	BPoint position;
	bool editable;
	
  public:
	EditableListItem(const char *text, uint32 outlineLevel = 0, 
		bool expanded = true);
	EditableListItem(BMessage *data);
	
	~EditableListItem();
	
	virtual	status_t Archive(BMessage *data, bool deep = true) const;
	static	BArchivable	*Instantiate(BMessage *data);
	
	virtual void DrawItem(BView *view, BRect itemRect, bool drawEverything);
	virtual void Update(BView *owner, const BFont *font);
	
	inline bool IsEditable(void) const;
	void SetEditable(bool Editable = true);
	
	void SetTextPosition(BPoint point);
	inline BPoint TextPosition(void);

  private:
	EditableListItem(const BListItem &);
	EditableListItem	&operator=(const BListItem &);
};

inline bool EditableListItem::IsEditable(void) const
{
	return editable;
}

inline BPoint EditableListItem::TextPosition(void)
{
	return position;
}

#endif /* EDITABLE_LIST_ITEM_H */