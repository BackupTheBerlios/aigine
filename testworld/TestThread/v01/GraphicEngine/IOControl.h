// IOControl.h: Schnittstelle für die Klasse IOControl.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCONTROL_H__E26598EF_6B10_4744_9880_3C5BF684918F__INCLUDED_)
#define AFX_IOCONTROL_H__E26598EF_6B10_4744_9880_3C5BF684918F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class IOControl  
{
public:
	void keyboard(unsigned char key, int x, int y);
	IOControl();
	virtual ~IOControl();

};

#endif // !defined(AFX_IOCONTROL_H__E26598EF_6B10_4744_9880_3C5BF684918F__INCLUDED_)
