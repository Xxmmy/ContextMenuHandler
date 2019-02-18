#include "Global.h"
#include "settings.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

Global::Global() :
	ModRefCount{ 0 },
	ModHandle{ nullptr },
	ModPath{},
	ProdName{ PRODUCT_NAME_W },
	ProdClassId{ PRODUCT_CLSID_W },
	ProdMenuItem{ PRODUCT_MENU_W },
	ProdClassRoots{ PRODUCT_HKCR_W },

#ifdef PRODUCT_SUBMENU_W
	ProdSubMenuItems{ PRODUCT_SUBMENU_W },
#else
	ProdSubMenuItems{},
#endif

#ifdef PRODUCT_MUTEX_W
	ProdMutexName{ PRODUCT_MUTEX_W },
#else
	ProdMutexName{},
#endif

#ifdef PRODUCT_ICON_A
	ProdIconId{ PRODUCT_ICON_N },
#else
	ProdIcon{ 0 },
#endif

	ProdMultiSelect{ PRODUCT_MULTISELECT_B }
{
}
