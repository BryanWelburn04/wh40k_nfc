#pragma once

class AcsReaderException
{
public:
	virtual const char* what();	
	
	AcsReaderException();
	AcsReaderException(long errorNumber);
	AcsReaderException(char* errorString);
	virtual ~AcsReaderException();

protected:
	const char* _message;
};