#include "WordSrch.h"
#include <algorithm>
#include <string>

namespace WordSrch {

bool isPrefix( const string & prefix, const string & word ) {
    if( word.length( ) < prefix.length( ) )
        return false;
    for( unsigned i = 0; i < prefix.length( ); i++ )
        if( prefix[ i ] != word[ i ] )
            return false;
    return true;
}

vector<char> toVec( const string & str ) {
    vector<char> v( str.length( ) );
    for( unsigned i = 0; i < str.length( ); i++ )
        v[ i ] = str[ i ];
    return v;
}

}  // namespace WordSrch

void Puzzle::solvePuzzle( ) {
    for( unsigned r = 0; r < theBoard.numrows( ); r++ )
        for( unsigned c = 0; c < theBoard.numcols( ); c++ )
            for( int rd = -1; rd <= 1; rd++ )
                for( int cd = -1; cd <= 1; cd++ )
                    if( rd != 0 || cd != 0 ) solveDirection( r, c, rd, cd );
}

WORDLIST::WORDLIST( string _word, unsigned _r0, unsigned _c0, unsigned _r1, unsigned _c1 ): r0(_r0), c0(_c0), r1(_r1), c1(_c1) {
	UINT len = MultiByteToWideChar( CP_UTF8, 0, _word.c_str( ), _word.length( ), NULL, 0 ) + 1;
	word = new WCHAR[ len ];
	ZeroMemory( word, sizeof( WCHAR ) * len );
	MultiByteToWideChar( CP_UTF8, 0, _word.c_str( ), _word.length( ), word, len );
}

void Puzzle::solveDirection( int baseRow, int baseCol, int rowDelta, int colDelta ) {
    string word;
	word = theBoard[ baseRow ][ baseCol ];
    for( int i = baseRow + rowDelta, j = baseCol + colDelta;
            i >= 0 && j >= 0 && ( unsigned )i < theBoard.numrows( ) && ( unsigned )j < theBoard.numcols( );
                i += rowDelta, j += colDelta ) {
        word += theBoard[ i ][ j ];
        vector<string>::const_iterator itr;
        itr = lower_bound( theWords.begin( ), theWords.end( ), word );
        if( itr == theWords.end( ) || !WordSrch::isPrefix( word, *itr ) ) break;
		if( *itr == word ) {
			if ( maxLength < word.length( ) ) maxLength = word.length( );
			output.push_back( WORDLIST( word, baseRow, baseCol, i, j ) );
		}
    }
}

void Puzzle::openFile( LPCWSTR filename, ifstream & inFile ) {
	inFile.clear( );
	inFile.open( filename );
	unsigned len = wcslen( filename ) + 13;
	LPWSTR msg = new WCHAR[ len ];
	wcscpy_s( msg, len, L"Cannot find " );
	wcscat_s( msg, len, filename );
	if( !inFile ) {
		MessageBox( 0, msg, 0, 0 );
		exit( 1 );
	}
}

void Puzzle::readPuzzle( ) {
    string oneLine;
	if( getline( puzzleStream, oneLine ).eof( ) ) return;
    int columns = oneLine.length( );
    theBoard.push_back( WordSrch::toVec( oneLine ) );
    while( !( getline( puzzleStream, oneLine ) ).eof( ) )
		if( oneLine.length( ) != columns )
			MessageBox( 0, L"Puzzle is not rectangular", 0, 0 );
		else theBoard.push_back( WordSrch::toVec( oneLine ) );
}

void Puzzle::readWords( ) {
    string thisWord;
    for( int numEntries = 0; wordStream >> thisWord; numEntries++ ) {
		if( numEntries != 0 && thisWord < theWords[ numEntries - 1 ] ) {
			MessageBox( 0, L"Dictionary is not sorted... skipping", 0, 0 );
			--numEntries;
		} else theWords.push_back( thisWord );
    }
}

Puzzle::Puzzle( ): theBoard( 0, 0 ), maxLength( 0 ) {
	openFile( L"puzzle.txt", puzzleStream );
    openFile( L"words.txt", wordStream );
    readPuzzle( );
    readWords( );
	solvePuzzle( );
}

Puzzle::~Puzzle( ) {
	for ( UINT i = 0; i < output.size( ); delete[] output[ i++ ].word );
}