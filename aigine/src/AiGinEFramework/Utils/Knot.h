// Knot.h: interface for the Knot class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KNOT_H__71E37B80_F2AC_429D_9154_6CA61AE87BF5__INCLUDED_)
#define AFX_KNOT_H__71E37B80_F2AC_429D_9154_6CA61AE87BF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Knot  
{
public:
	Knot();
	Knot* left;
	Knot* right;
	void* data;
	virtual ~Knot();

};

#endif // !defined(AFX_KNOT_H__71E37B80_F2AC_429D_9154_6CA61AE87BF5__INCLUDED_)



