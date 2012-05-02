#include <Rect.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ScrollView.h>
#include <ScrollBar.h>
#include <OutlineListView.h>
#include "PropertyWindow.h"
#include "PropertyListItem.h"

PropertyWindow::PropertyWindow():
	BWindow(BRect(40, 40, 400, 320), "Property Editor", B_TITLED_WINDOW,
	B_QUIT_ON_WINDOW_CLOSE),
	list(NULL)
{
	BMenuBar *menubar = new BMenuBar(BRect(0, 0, 10, 10), "MenuBar");
	
	AddChild(menubar);

	BMenu *file = new BMenu("File");
	
	if (file)
	{
		menubar->AddItem(file);
	
		file->AddItem(new BMenuItem("Open...", new BMessage('open'), 'O'));
		
		file->AddItem(new BMenuItem("Save", new BMessage('save'), 'S'));
		
		file->AddItem(new BMenuItem("Save As...", new BMessage('s_as')));
		
		file->AddSeparatorItem();
		
		file->AddItem(new BMenuItem("About Property Editor...", 
			new BMessage(B_ABOUT_REQUESTED)));
		
		file->AddSeparatorItem();
		
		file->AddItem(new BMenuItem("Quit", 
			new BMessage(B_QUIT_REQUESTED), 'Q'));
	}
	
	BMenu *edit = new BMenu("Edit");
	
	if (edit)
	{
		menubar->AddItem(edit);
	
		edit->AddItem(new BMenuItem("Add Child", new BMessage('addc')));
		
		edit->AddItem(new BMenuItem("Delete Item", new BMessage('deli')));	
	}

	BRect frame = menubar->Frame();
	
	menubar->RemoveSelf();
	
	frame = BRect(frame.left, frame.bottom, 
		frame.right - B_V_SCROLL_BAR_WIDTH - 1, Bounds().bottom + 1);

	SimpleListView<BOutlineListView> *list = 
		new SimpleListView<BOutlineListView>(frame, "list",
		B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL_SIDES);

	BScrollView *scroll = new BScrollView("scroll", list,
		B_FOLLOW_ALL_SIDES, 0, false, true, B_NO_BORDER);
		
	list->SetViewColor(ui_color(B_MENU_BACKGROUND_COLOR));
	
	AddChild(scroll);
	AddChild(menubar);

	// ---- Cut here :-)
//	list->AddItem(new BStringItem("test"));
	PropertyListItem *t = NULL;
	PropertyListItem *k = NULL;

	list->AddItem(new PropertyListItem("test1 super test"));
	list->AddItem(new PropertyListItem("test2 super test"));
	list->AddItem(t = new PropertyListItem("test3 super test"));
	
	list->AddUnder(new PropertyListItem("test3 super test"), t);
	list->AddUnder(new PropertyListItem("test3 super test"), t);
	list->AddUnder(k = new PropertyListItem("test3 super test"), t);

	list->AddUnder(new PropertyListItem("test4.5 super test"), k);
	list->AddUnder(new PropertyListItem("test4.5 super test"), k);
	list->AddUnder(new PropertyListItem("test4.5 super test"), k);
	
	list->AddItem(new PropertyListItem("test4 super test"));
	
}

PropertyWindow::~PropertyWindow()
{
}

void PropertyWindow::FrameMoved(BPoint origin)
{
}

void PropertyWindow::FrameResized(float width, float height)
{
}

void PropertyWindow::MessageReceived(BMessage *message)
{
}
