/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */

#include <Application.h>
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
#include "SimpleTextFilter.h"

void add_type_menu_item(BMenu *menu, int typeConstant)
{
	BString name = getTypeName(typeConstant);
	
	if (name.IsEmpty())
		return;

	menu->AddItem(new BMenuItem(name.String(), new BMessage(typeConstant)));
}

AddPropertyItemWindow::AddPropertyItemWindow():
	BWindow(BRect(0, 0, 1, 1), "Add Property...", 
	B_MODAL_WINDOW,
	B_ASYNCHRONOUS_CONTROLS | B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_AUTO_UPDATE_SIZE_LIMITS)
{
	BMenu *menu = new BMenu("type_menu");
	
	BMenuField *menu_field = new BMenuField(BRect(), "menu_field", 
		"Type", menu);
		
	BTextControl *text_control1 = new BTextControl(BRect(), 
		"text_control", "", "", new BMessage('tchg'));

	BTextControl *text_control2 = new BTextControl(BRect(), 
		"text_control", "", "", new BMessage('tchg'));

	BButton *button1 = new BButton(BRect(), "button1", "Cancel", 
		new BMessage('cncl'));
		
	BButton *button2 = new BButton(BRect(), "button2", "Add", 
		new BMessage('addi'));
	
	BStringView *string_view1 = new BStringView(BRect(), "string_view1", 
		"Type:");

	BStringView *string_view2 = new BStringView(BRect(), "string_view2", 
		"Name:");

	BStringView *string_view3 = new BStringView(BRect(), "string_view3", 
		"Value:");
	
	BBox *border = new BBox(BRect(0, 0, 0, 0));

	add_type_menu_item(menu, B_ALIGNMENT_TYPE);
	add_type_menu_item(menu, B_RECT_TYPE);
	add_type_menu_item(menu, B_POINT_TYPE);
	add_type_menu_item(menu, B_SIZE_TYPE);
	add_type_menu_item(menu, B_STRING_TYPE);
	add_type_menu_item(menu, B_INT8_TYPE);
	add_type_menu_item(menu, B_UINT8_TYPE);
	add_type_menu_item(menu, B_INT16_TYPE);
	add_type_menu_item(menu, B_UINT16_TYPE);
	add_type_menu_item(menu, B_INT32_TYPE);
	add_type_menu_item(menu, B_UINT32_TYPE);
	add_type_menu_item(menu, B_INT64_TYPE);
	add_type_menu_item(menu, B_UINT64_TYPE);
	add_type_menu_item(menu, B_BOOL_TYPE);
	add_type_menu_item(menu, B_FLOAT_TYPE);
	add_type_menu_item(menu, B_DOUBLE_TYPE);
	add_type_menu_item(menu, B_POINTER_TYPE);
	add_type_menu_item(menu, B_MESSENGER_TYPE);
	add_type_menu_item(menu, B_REF_TYPE);
	add_type_menu_item(menu, B_MESSAGE_TYPE);
	add_type_menu_item(menu, B_RAW_TYPE);
	
	menu->SetLabelFromMarked(true);
	menu->SetTriggersEnabled(true);
	
	string_view1->SetAlignment(B_ALIGN_RIGHT);
	string_view2->SetAlignment(B_ALIGN_RIGHT);

	text_control1->TextView()->AddFilter(new BMessageFilter(B_KEY_DOWN, filterName));
		
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
			.Add(text_control1, 1, 2)
			.Add(string_view3, 0, 3)
			.Add(text_control2, 1, 3)
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
		
		case 'tchg':
			break;
		
		default:
			BWindow::MessageReceived(message);
	}
	
}
