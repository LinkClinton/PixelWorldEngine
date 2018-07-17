#include "DebugLayer.hpp"
#include "Utility.hpp"

void PixelWorldEngine::DebugLayer::ReportMessage(std::string message) 
{
#ifdef _DEBUG
	std::cout << message << std::endl;
#endif // DEBUG
}

void PixelWorldEngine::DebugLayer::ReportError(Error error, std::string exMessage = " ")
{
	ReportMessage(messageTemplate[(int)error] + exMessage);
}

bool PixelWorldEngine::DebugLayer::Assert(bool test, Error error, std::string exMessage = " ")
{
	if (test == false) return false;

	ReportMessage(messageTemplate[(int)error] + exMessage);

	return true;
}

bool PixelWorldEngine::DebugLayer::Assert(bool test, Error error, std::string target, std::string type)
{
	return Assert(test, error, " Target = " + target + " Type = " + type);
}
