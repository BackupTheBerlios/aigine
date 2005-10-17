// Knot.h: interface for the Knot class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEG_KNOT_H
#define AGEG_KNOT_H

class ageG_Knot  
{
public:
	ageG_Knot();
	ageG_Knot* left;
	ageG_Knot* right;
	void* data;
	virtual ~ageG_Knot();

};

#endif //AGEG_KNOT_H



