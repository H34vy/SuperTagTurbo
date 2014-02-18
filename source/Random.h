//=============================================================================
//	Random.h
//	Chris Prosser
//		This file defines a random number generation class
//=============================================================================

#include <stdlib.h>
#include <time.h>

class Random{
public:
	Random(int l=0, int h=1){
		srand(time(0));
		range(l, h);		
	}
	//Set range of random numbers (doubly inclusive)
	void range(int l, int h){
		if (l < h){ low = l; high = h; }
		else { low = h; high = l; }
	}

	int next(){ return rand() % ((high-low)+1) + low; }	
	
private:
	
	int low, high;

};