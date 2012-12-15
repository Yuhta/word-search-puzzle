#pragma once

#include "Matrix.h"
#include <vector>
#include <fstream>
#include <windows.h>

struct WORDLIST {
	LPWSTR word;
	unsigned r0, c0, r1, c1;
	WORDLIST( string _word, unsigned _r0, unsigned _c0, unsigned _r1, unsigned _c1 );
};

class Puzzle {
  public:
	matrix<char> theBoard;
	vector<WORDLIST> output;
	unsigned maxLength;
	Puzzle( );
	~Puzzle( );
  private:
	vector<string> theWords;
	ifstream puzzleStream, wordStream;
	void openFile( LPCWSTR filename, ifstream & inFile );
	void readPuzzle( );
	void readWords( );
	void solvePuzzle( );
	void solveDirection( int baseRow, int baseCol, int rowDelta, int colDelta );
};