/*
 * Authors:
 *  Vladislav Burundukov <vlad.be@gmail.com>
 */
#define W_WIDTH 320
#define W_HEIGHT 500

#include <ListView.h>
#include <TranslationUtils.h>
#include <Bitmap.h>
#include <ListView.h>
#include <Debug.h>
#include "TestWinAndApp.h"
#include "EditableListItem.h"
#include "BitmapListItem.h"
#include "SimpleListView.h"

int main(int argc, char *argv[])
{
	App *app = new App();
	
	if (app)
	{
		if (app->win)
		{
			SimpleListView<BListView> *list = new SimpleListView<BListView>(app->win->Bounds(), "list");
//			SimpleListView *list = new SimpleListView(app->win->Bounds(), "list");
			
			if (list)
			{		
				EditableListItem *item = new EditableListItem("test 1");
				item->SetHeight(40);
				item->SetTextPosition(BPoint(40, 20));
				item->SetEditable();
				list->AddItem(item);
				
				list->AddItem(item = new EditableListItem("test 2"));
				item->SetHeight(40);
				item->SetTextPosition(BPoint(10, 10));
				
				list->AddItem(item = new EditableListItem("test 3"));
				item->SetHeight(40);
				item->SetTextPosition(BPoint(10, 10));
				
				list->AddItem(item = new EditableListItem("test 4"));
				item->SetHeight(40);
				item->SetTextPosition(BPoint(10, 10));
				
				list->AddItem(item = new EditableListItem("test 5"));
				item->SetHeight(40);
				item->SetTextPosition(BPoint(10, 10));
				item->SetEditable();
				
				list->AddItem(item = new EditableListItem("test 6"));
				item->SetHeight(40);
				item->SetTextPosition(BPoint(10, 10));

				BitmapListItem *bitmapListItem;

				list->AddItem(bitmapListItem = new BitmapListItem("Bitmap Test"));
				bitmapListItem->SetHeight(40);
				bitmapListItem->SetTextPosition(BPoint(45, 10));
				bitmapListItem->SetEditable();
				
				BBitmap *bitmap = BTranslationUtils::GetBitmapFile("../Cat_and_Fly");
				bitmapListItem->SetBitmap(bitmap, BRect(), true);

				list->AddItem(item = new EditableListItem("test 7"));
				item->SetHeight(40);
				item->SetTextPosition(BPoint(10, 10));

				list->AddItem(bitmapListItem = new BitmapListItem("Bitmap Test 2"));
				bitmapListItem->SetHeight(49);
				bitmapListItem->SetTextPosition(BPoint(50, 10));
				bitmapListItem->SetEditable();
				
				bitmap = BTranslationUtils::GetBitmapFile("../Cat_and_Fly");
				bitmapListItem->SetBitmap(bitmap, BRect(), true);
				
				
				list->SetViewColor(167, 68, 190);
				app->win->AddChild(list);				
			}
		}
		
		app->Run();
		delete app;
	}
	
	return 0;
}