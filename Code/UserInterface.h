#pragma once

#include "WordSrch.h"
#include <windows.h>

class USER_INTERFACE {
  public:
	USER_INTERFACE( );
	~USER_INTERFACE( ) { delete[] lighted; }
	VOID Initialize( const WNDCLASSEX *pWc, LPCWSTR wndClassName );
	VOID CreateList( HWND hWnd );
	VOID Update( HWND hWnd );
	VOID Change( );
  private:
	Puzzle puzzle;
	BOOL *lighted;
	HWND hList;
	UINT width, height, listWidth;
	HPEN transparentPen;
	HBRUSH backBrush;
	HFONT hFont;
};