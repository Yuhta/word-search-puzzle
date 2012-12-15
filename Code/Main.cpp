#include "UserInterface.h"

USER_INTERFACE userI;

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
	switch( msg ) {
		case WM_CREATE:
			userI.CreateList( hWnd );
			return 0;
		case WM_PAINT:
			userI.Update( hWnd );
			return 0;
		case WM_COMMAND:
			if( HIWORD( wParam ) == LBN_SELCHANGE ) {
				userI.Change( );
				InvalidateRect( hWnd, 0, 0 );
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT ) {
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle( 0 ), 0, 0, 0, 0, L"WND_CLASS", 0 };
	userI.Initialize( &wc, L"WND_CLASS" );
	UnregisterClass( L"WND_CLASS", wc.hInstance );
	return 0;
}
