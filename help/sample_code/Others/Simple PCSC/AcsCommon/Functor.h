// Functor.h: interface for the Functor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTOR_H__AABA870B_50C0_4FBC_8366_C384B409A85F__INCLUDED_)
#define AFX_FUNCTOR_H__AABA870B_50C0_4FBC_8366_C384B409A85F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Functor  
{
public:
	/* Only constructor - stores the given data */							
	Functor()
	{
	
	}

	void setFunctor(void (*pFunc)(const uint8* data, const int len)) 
	{ 
		m_pFunc = pFunc; 
	}
	
	void operator () (const uint8* data, const int len)
	{
		if (m_pFunc != NULL)
		{
			(*m_pFunc)(data, len);
		}
	}
						
private:																	
	void (*m_pFunc)(const uint8* data, const int len);	/* Function pointer */						
};

#endif // !defined(AFX_FUNCTOR_H__AABA870B_50C0_4FBC_8366_C384B409A85F__INCLUDED_)
