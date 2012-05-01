#define W_WIDTH 320
#define W_HEIGHT 500

#include <ListView.h>
#include <TranslationUtils.h>
#include <Bitmap.h>
#include "TestWinAndApp.h"
#include "TitledListItem.h"
#include "BitmapListItem.h"
#include "SimpleListView.h"

int main(int argc, char *argv[])
{
	App *app = new App();
	
	if (app)
	{
		if (app->win)
		{
			SimpleListView *list = new SimpleListView(app->win->Bounds(), "list");
			
			if (list)
			{		
				TitledListItem *item = new TitledListItem("test 1");
				item->SetHeight(40);
				item->SetTitlePosition(BPoint(40, 20));
				item->SetEditable();
				list->AddItem(item);
				
				list->AddItem(item = new TitledListItem("test 2"));
				item->SetHeight(40);
				item->SetTitlePosition(BPoint(10, 10));
				
				list->AddItem(item = new TitledListItem("test 3"));
				item->SetHeight(40);
				item->SetTitlePosition(BPoint(10, 10));
				
				list->AddItem(item = new TitledListItem("test 4"));
				item->SetHeight(40);
				item->SetTitlePosition(BPoint(10, 10));
				
				list->AddItem(item = new TitledListItem("test 5"));
				item->SetHeight(40);
				item->SetTitlePosition(BPoint(10, 10));
				item->SetEditable();
				
				list->AddItem(item = new TitledListItem("test 6"));
				item->SetHeight(40);
				item->SetTitlePosition(BPoint(10, 10));

				BitmapListItem *bitmapListItem;

				list->AddItem(bitmapListItem = new BitmapListItem("Bitmap Test"));
				bitmapListItem->SetHeight(40);
				bitmapListItem->SetTitlePosition(BPoint(45, 10));
				bitmapListItem->SetEditable();
				
				BBitmap *bitmap = BTranslationUtils::GetBitmapFile("../Cat_and_Fly");
				bitmapListItem->SetBitmap(bitmap, BRect(), true);

				list->AddItem(item = new TitledListItem("test 7"));
				item->SetHeight(40);
				item->SetTitlePosition(BPoint(10, 10));

				list->AddItem(bitmapListItem = new BitmapListItem("Bitmap Test 2"));
				bitmapListItem->SetHeight(49);
				bitmapListItem->SetTitlePosition(BPoint(50, 10));
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