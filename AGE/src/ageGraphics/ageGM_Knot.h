// Knot.h: interface for the Knot class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEGM_KNOT_H
#define AGEGM_KNOT_H

class ageGM_Knot  
{
public:
	ageGM_Knot();
	ageGM_Knot* left;
	ageGM_Knot* right;
	void* data;
	virtual ~ageGM_Knot();

};

#endif //AGEGM_KNOT_H



