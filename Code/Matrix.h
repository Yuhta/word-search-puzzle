#pragma once

#include <vector>
using namespace std;

template <class Object>
class matrix
{
  public:
    matrix( int rows, int cols ) : array( rows )
    {
        for( int i = 0; i < rows; i++ )
            array[ i ].resize( cols );
    }

    matrix( const matrix & rhs ) : array( rhs.array ) { }

    const vector<Object> & operator[]( int row ) const
      { return array[ row ]; }
    vector<Object> & operator[]( int row )
      { return array[ row ]; }
    unsigned numrows( ) const
      { return array.size( ); }
    unsigned numcols( ) const
      { return numrows( ) > 0 ? array[ 0 ].size( ) : 0; }

    void push_back( const vector<Object> & newRow )
      { array.push_back( newRow ); }

    private:
       vector< vector<Object> > array;
};