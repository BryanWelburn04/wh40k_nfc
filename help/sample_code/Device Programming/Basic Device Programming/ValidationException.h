#if !defined(AFX_VALIDATIONEXCEPTION_H__FADC75E8_6BF4_47FC_85E1_EFFF42957F6F__INCLUDED_)
#define AFX_VALIDATIONEXCEPTION_H__FADC75E8_6BF4_47FC_85E1_EFFF42957F6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <exception>
#include <string>

using namespace std;

class ValidationException : public exception
{
public:
	explicit ValidationException(const char* pMessage) : sMessage_(pMessage) {}
	explicit ValidationException(const string& sMessage) : sMessage_(sMessage) {}

	virtual ~ValidationException() throw (){}
	virtual const char* what() const throw () { return sMessage_.c_str(); }

protected:
	string sMessage_;
};

#endif // !defined(AFX_VALIDATIONEXCEPTION_H__FADC75E8_6BF4_47FC_85E1_EFFF42957F6F__INCLUDED_)
