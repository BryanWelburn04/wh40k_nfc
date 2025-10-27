// PcscProvider.h: interface for the PcscProvider class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCSCPROVIDER_H__9262CABA_1823_45E4_AEDB_73B0798391DE__INCLUDED_)
#define AFX_PCSCPROVIDER_H__9262CABA_1823_45E4_AEDB_73B0798391DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PcscProvider  
{
public:
	char* getSCardErrorMsg(long errCode);
	PcscProvider();
	virtual ~PcscProvider();

};

#endif // !defined(AFX_PCSCPROVIDER_H__9262CABA_1823_45E4_AEDB_73B0798391DE__INCLUDED_)
