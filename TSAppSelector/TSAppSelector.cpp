#include "stdafx.h"

TCHAR *TITLE = L"TS App Selector";
WCHAR INIPATH[MAX_PATH];

bool isM2TS(LPCTSTR filename);
void launchPlayer(LPCTSTR filePath);
void launchEditor(LPCTSTR filePath);
void launch(LPCTSTR exePath, LPCTSTR filePath);

int APIENTRY _tWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	// set INIPATH
	GetModuleFileName(NULL, INIPATH, MAX_PATH);
	WCHAR drive[_MAX_DRIVE];
	WCHAR dir[_MAX_DIR];
	WCHAR fname[_MAX_FNAME];
	_wsplitpath_s(INIPATH, drive, sizeof(drive), dir, sizeof(dir), fname, sizeof(fname), NULL, NULL);
	wsprintf(INIPATH, L"%s%s%s.ini\0", drive, dir, fname);

	if ( 2 == __argc ) {
		if (isM2TS(__targv[1])) {
			launchPlayer(__targv[1]);
		} else {
			launchEditor(__targv[1]);
		}

		return TRUE;
	}

	MessageBox(NULL, L"Please specify the path to .ts file", TITLE, MB_OK | MB_ICONINFORMATION);
	return TRUE;
}

bool isM2TS(LPCTSTR filename)
{
	unsigned char readHead[1];

	FILE *fp = NULL;

	if (
		0 != _tfopen_s(&fp, filename, L"rb") ||
		1 != fread(readHead, 1, 1, fp)
	) {
		MessageBox(NULL, L"Failed to read", TITLE, MB_OK | MB_ICONERROR);
		return FALSE;
	}
	fclose(fp);

	if (0x47 != readHead[0]) {
		return FALSE;
	}

	return TRUE;

}

void launchPlayer(LPCTSTR filePath)
{
	WCHAR exePath[256];
	
	GetPrivateProfileString(L"APP", L"PLAYER", L"wmplayer.exe", exePath, sizeof(exePath), INIPATH);

	launch(exePath, filePath);
}

void launchEditor(LPCTSTR filePath)
{
	WCHAR exePath[256];

	GetPrivateProfileString(L"APP", L"EDITOR", L"notepad.exe", exePath, sizeof(exePath), INIPATH);

	launch(exePath, filePath);
}

void launch(LPCTSTR exePath, LPCTSTR filePath)
{
	ShellExecute(NULL, NULL, exePath, filePath, NULL, SW_SHOW);
}
