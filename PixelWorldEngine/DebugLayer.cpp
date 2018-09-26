#include "DebugLayer.hpp"
#include "Utility.hpp"

void PixelWorldEngine::DebugLayer::ReportMessage(const std::string &message) 
{
#ifdef _DEBUG
	std::cout << message << std::endl;
#endif // DEBUG
}

void PixelWorldEngine::DebugLayer::ReportError(Error error,const std::string &exMessage = " ")
{
	ReportMessage(messageTemplate[(int)error] + exMessage);
}

bool PixelWorldEngine::DebugLayer::Assert(bool test, Error error,const std::string &exMessage = " ")
{
	if (test == false) return false;

	ReportMessage(messageTemplate[(int)error] + exMessage);

	return true;
}

bool PixelWorldEngine::DebugLayer::Assert(bool test, Error error,const std::string &target,const std::string &type)
{
	return Assert(test, error, " Target = " + target + " Type = " + type);
}
