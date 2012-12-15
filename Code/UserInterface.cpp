#include "UserInterface.h"

#define WS_CUSTOMWINDOWSTYLE ( WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX )
#define WS_CUSTOMLISTSTYLE ( LBS_NOTIFY | LBS_SORT | LBS_DISABLENOSCROLL | WS_VSCROLL | WS_CHILD | WS_VISIBLE )
#define HEIGHT_OF_TEXT 40

USER_INTERFACE::USER_INTERFACE( ) {
	transparentPen = CreatePen( PS_NULL, 0, 0 );
	backBrush = CreateSolidBrush( 0x00290F2D );
	hFont = CreateFont( HEIGHT_OF_TEXT, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, PROOF_QUALITY, 0, L"Impact" );
	UINT maxLength = puzzle.maxLength;
	if( maxLength < 6 ) maxLength = 6;
	listWidth = ( maxLength + 4 ) * LOWORD( GetDialogBaseUnits( ) );
	width = HEIGHT_OF_TEXT * ( puzzle.theBoard.numcols( ) + 3 ) + listWidth, height = HEIGHT_OF_TEXT * ( puzzle.theBoard.numrows( ) + 2 );
	lighted = new BOOL[ puzzle.theBoard.numcols( ) * puzzle.theBoard.numrows( ) ];
	ZeroMemory( lighted, sizeof( BOOL ) * puzzle.theBoard.numcols( ) * puzzle.theBoard.numrows( ) );
}

VOID USER_INTERFACE::Initialize( const WNDCLASSEX *pWc, LPCWSTR wndClassName ) {
	RegisterClassEx( pWc );
	HWND hWnd = CreateWindow( wndClassName, L"Word Search Puzzle", WS_CUSTOMWINDOWSTYLE, CW_USEDEFAULT, 0, width, height, 0, 0, pWc->hInstance, 0 );
	RECT cRect, wRect;
	GetClientRect( hWnd, &cRect );
	GetWindowRect( hWnd, &wRect );
	DWORD wWidth = wRect.right - wRect.left - cRect.right + width, wHeight = wRect.bottom - wRect.top - cRect.bottom + height;
	MoveWindow(hWnd, wRect.left, wRect.top, wWidth, wHeight, 0);
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );
	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	while( msg.message != WM_QUIT )
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
}

VOID USER_INTERFACE::CreateList( HWND hWnd ) {
	DWORD listHeight = height - HEIGHT_OF_TEXT * 2, listX = width - listWidth - HEIGHT_OF_TEXT;
	hList = CreateWindow( L"LISTBOX", L"", WS_CUSTOMLISTSTYLE, listX, HEIGHT_OF_TEXT, listWidth, listHeight, hWnd, 0, 0, 0 );
	SendMessage( hList, LB_ADDSTRING, 0, ( LPARAM )L"(none)" );
	UINT pos;
	for( UINT i = 0; i < puzzle.output.size( ); SendMessage( hList, LB_SETITEMDATA, pos, i++ ) )
		pos = SendMessage( hList, LB_ADDSTRING, 0, ( LPARAM )puzzle.output[ i ].word );
	SendMessage( hList, LB_SETCURSEL, 0, 0 );
}

VOID USER_INTERFACE::Update( HWND hWnd ) {
	PAINTSTRUCT ps;
	BeginPaint( hWnd, &ps );
	SelectObject( ps.hdc, transparentPen );
	SelectObject( ps.hdc, backBrush );
	Rectangle( ps.hdc, 0, 0, width + 1, height + 1);
	SelectObject( ps.hdc, hFont );
	SetBkColor( ps.hdc, 0x002D0F29 );
	WCHAR ch;
	UINT i, j;
	for( i = 0; i < puzzle.theBoard.numrows( ); ++i )
		for( j = 0; j < puzzle.theBoard.numcols( ); ++j ) {
			if( lighted[ i * puzzle.theBoard.numcols( ) + j ] ) SetTextColor( ps.hdc, 0x000249FE );
			else SetTextColor( ps.hdc, 0x0070EAF2 );
			MultiByteToWideChar( CP_UTF8, 0, &puzzle.theBoard[ i ][ j ], 1, &ch, 1 );
			TextOut( ps.hdc, ( j + 1 ) * HEIGHT_OF_TEXT, ( i + 1 ) * HEIGHT_OF_TEXT, &ch, 1 );
		}
	EndPaint( hWnd, &ps);
}

VOID USER_INTERFACE::Change( ) {
	ZeroMemory( lighted, sizeof( BOOL ) * puzzle.theBoard.numcols( ) * puzzle.theBoard.numrows( ) );
	UINT pos = SendMessage( hList, LB_GETCURSEL, 0, 0 );
	if( pos ) {
		UINT i = SendMessage( hList, LB_GETITEMDATA, pos, 0 );
		INT rDelta = puzzle.output[ i ].r1 - puzzle.output[ i ].r0, cDelta = puzzle.output[ i ].c1 - puzzle.output[ i ].c0;
		rDelta = rDelta > 0 ? 1 : rDelta < 0 ? -1 : 0, cDelta = cDelta > 0 ? 1 : cDelta < 0 ? -1 : 0;
		for( UINT k = 0, r = puzzle.output[ i ].r0, c = puzzle.output[ i ].c0; k < wcslen( puzzle.output[ i ].word ); ++k, r += rDelta, c += cDelta )
			lighted[ r * puzzle.theBoard.numcols( ) + c ] = TRUE;
	}
}