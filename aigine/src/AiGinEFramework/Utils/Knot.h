// Knot.h: interface for the Knot class.
//
//////////////////////////////////////////////////////////////////////

#ifndef KNOT_H
#define KNOT_H

class Knot  
{
public:
	Knot();
	Knot* left;
	Knot* right;
	void* data;
	virtual ~Knot();

};

#endif //KNOT_H



