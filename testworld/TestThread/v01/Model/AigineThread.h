// AigineThread.h: Schnittstelle für die Klasse AigineThread.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIGINETHREAD_H__EE629AF7_5629_4CBD_8B43_DF1EE95C922C__INCLUDED_)
#define AFX_AIGINETHREAD_H__EE629AF7_5629_4CBD_8B43_DF1EE95C922C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AigineThread : public JThread 
{
private:
	char * _name;
public:
	AigineThread();

	//konstruktor für benennung des threads
	AigineThread(char* name);

	//getter und setter Methoden	
	char* getName() ;
	void setName(char* name) ;
	
	void* Thread();


	virtual ~AigineThread();

};

#endif // !defined(AFX_AIGINETHREAD_H__EE629AF7_5629_4CBD_8B43_DF1EE95C922C__INCLUDED_)
