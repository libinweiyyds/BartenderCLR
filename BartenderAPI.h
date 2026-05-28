#pragma once
#include <string>
#include <vector>

#include <vcclr.h>

#using "C:\\Program Files (x86)\\Seagull\\BarTender Suite\\SDK\\Assemblies\\Seagull.BarTender.Print.dll"

using namespace Seagull::BarTender::Print;

class BartenderAPI
{
public:

	BartenderAPI();

	~BartenderAPI();

public:

	bool start();

	void stop();

	std::string getLastError();

	void setLastError(const std::string& error);

	std::vector<std::string> getPrinterNames();

	bool openLabelFormatDocument(const char* absFilePath);
	bool openLabelFormatDocumentW(const wchar_t* absFilePath);

	void closeLabelFormatDocument();

	std::vector<std::string>
		currentDocumentSubStrings();

	bool setSubStringValue(const char* name, const char* value);
	bool setSubStringValueW(const wchar_t* name, const wchar_t* value);

	std::string getSubStringValue(const char* name);
	std::wstring getSubStringValueW(const wchar_t* name);

	int print(
		const char* printerName,
		int waitMs,
		std::vector<std::string>& msgs);

private:

	gcroot<Engine^> m_engine;

	gcroot<LabelFormatDocument^> m_document;

	std::string m_lastError;
};