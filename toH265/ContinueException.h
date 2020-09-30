#pragma once
ref class ContinueException : public System::Exception
{
public:
	ContinueException(System::String^ message) : Exception(message){}
};

