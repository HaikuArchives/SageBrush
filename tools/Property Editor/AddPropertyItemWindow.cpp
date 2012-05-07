#include <Application.h>
#include <Message.h>
#include <SupportDefs.h>
#include <Button.h>
#include <MenuField.h>
#include <Menu.h>
#include <MenuItem.h>
#include <TextControl.h>
#include <Box.h>
#include <GroupLayout.h>
#include <GridLayout.h>
#include <GroupLayoutBuilder.h>
#include <GridLayoutBuilder.h>
#include <SpaceLayoutItem.h>
#include <View.h>
#include <StringView.h>
#include <Size.h>
#include "AddPropertyItemWindow.h"
#include "NameOfType.h"

AddPropertyItemWindow::AddPropertyItemWindow():
	BWindow(BRect(0, 0, 1, 1), "Add Property...", 
	B_MODAL_WINDOW,
	B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	BMenu *menu = new BMenu("type_menu");
	
	BMenuField *menu_field = new BMenuField(BRect(), "menu_field", 
		"Type", menu);
		
	BTextControl *text_control = new BTextControl(BRect(), 
		"text_control", "Value", "", new BMessage('tchg'));

	BButton *button1 = new BButton(BRect(), "button1", "Cancel", 
		new BMessage('cncl'));
		
	BButton *button2 = new BButton(BRect(), "button2", "Add", 
		new BMessage('addi'));
	
	BStringView *string_view1 = new BStringView(BRect(), "string_view1", 
		"Type:");

	BStringView *string_view2 = new BStringView(BRect(), "string_view2", 
		"Value:");
	
	BBox *border = new BBox(BRect(0, 0, 0, 0));
	
//	menu->AddItem(new BMenuItem("test 1 etst tetst tetst", new BMessage('tst1')));
//	menu->AddItem(new BMenuItem("test 2 super tetet sttdds", new BMessage('tst1')));

//	sizeof(name_of_type);

	for (int i = 0; i < sizeof(name_of_type)/sizeof(NameOfType); i++)
		menu->AddItem(new BMenuItem(name_of_type[i].name, 
			new BMessage(name_of_type[i].type)));
	
	menu->SetLabelFromMarked(true);
	menu->SetTriggersEnabled(true);
	
	string_view1->SetAlignment(B_ALIGN_RIGHT);
	string_view2->SetAlignment(B_ALIGN_RIGHT);
	
	SetLayout(new BGroupLayout(B_HORIZONTAL));

	AddChild(BGroupLayoutBuilder(B_VERTICAL, 10)
		.Add(border)
		.Add(
			BGridLayoutBuilder(10, 10)
			.SetInsets(5, 0, 5, 0)
			.Add(BSpaceLayoutItem::CreateGlue(), 0, 0)
			.Add(string_view1, 0, 1)
			.Add(menu_field, 1, 1)
			.Add(string_view2, 0, 2)
			.Add(text_control, 1, 2)
		)
		.Add(BGroupLayoutBuilder(B_HORIZONTAL, 10)
			.SetInsets(5, 5, 5, 5)
			.Add(button1)
			.Add(button2)
		)
	);
	
	CenterOnScreen();
	
	if (menu->ItemAt(0))
		menu->ItemAt(0)->SetMarked(true);
	
	BMenuItem *anItem = NULL;
	
	float width = 0, maxWidth = 0;

	for (int i = 0; (anItem = menu->ItemAt(i)); i++)
	{
		width = menu_field->StringWidth(anItem->Label());
		if (maxWidth < width) 
			maxWidth = width;		
	}
	
	border->SetExplicitMinSize(BSize(maxWidth*4, 1));
	border->SetExplicitMaxSize(BSize(maxWidth*4, 1));
	border->SetBorder(B_NO_BORDER);
	
	BWindow *appWin = be_app->WindowAt(0);
		
	if (appWin)
	{
		appWin->Lock();
		BRect frame = appWin->Frame();
		appWin->Unlock();
		
		MoveTo(frame.left + frame.Width()/2 - maxWidth*2, 
			frame.top + frame.Height()/2 - Bounds().Height()/2);
	}
}

AddPropertyItemWindow::~AddPropertyItemWindow()
{
}
	
void AddPropertyItemWindow::MessageReceived(BMessage *message)
{
	switch (message->what)
	{
		case 'cncl':
			Quit();
			break;
			
		case 'addi':
			Quit();
			break;
			
		default:
			BWindow::MessageReceived(message);
	}
	
}

