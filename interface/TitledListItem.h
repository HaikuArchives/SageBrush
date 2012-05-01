#ifndef TITLED_LIST_ITEM_H
#define TITLED_LIST_ITEM_H

#include <Point.h>
#include <Rect.h>
#include <ListItem.h>

class TitledListItem: public BStringItem
{
	BPoint position;
	bool editable;
	
  public:
	TitledListItem(const char *text, uint32 outlineLevel = 0, 
		bool expanded = true);
	TitledListItem(BMessage *data);
	
	~TitledListItem();
	
	virtual	status_t Archive(BMessage *data, bool deep = true) const;
	static	BArchivable	*Instantiate(BMessage *data);
	
	virtual void DrawItem(BView *view, BRect itemRect, bool drawEverything);
	virtual void Update(BView *owner, const BFont *font);
	
	inline bool IsEditable(void) const;
	void SetEditable(bool Editable = true);
	
	void SetTitlePosition(BPoint point);
	inline BPoint TitlePosition(void);

  private:
	TitledListItem(const BListItem &);
	TitledListItem	&operator=(const BListItem &);
};

inline bool TitledListItem::IsEditable(void) const
{
	return editable;
}

inline BPoint TitledListItem::TitlePosition(void)
{
	return position;
}

#endif /* TITLED_LIST_ITEM_H */