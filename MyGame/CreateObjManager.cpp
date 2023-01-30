#include "CreateObjManager.h"

#include <tchar.h>
CreateObjManager::~CreateObjManager()
{
}


void CreateObjManager::OpenCsvFile(CString filename)
{
	CString default=_T("C:\\K020G1414\\NowProj\\MatsuEngines\\MyGame\\Param_CSV\\");

	ShellExecute(nullptr, _T("open"), default+ filename, nullptr, nullptr, SW_SHOW);

}
