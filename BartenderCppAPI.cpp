#include "pch.h"

#include "BartenderCppAPI.h"

#include "BartenderAPI.h"

#include <vector>

#include <cstring>

#include <msclr/marshal_cppstd.h>
#include <windows.h> 

extern "C" {

BARTENDER_API void* CreateBartenderAPI()
{
	try
	{
		BartenderAPI* api =
			new BartenderAPI();

		return static_cast<void*>(api);
	}
	catch (...)
	{
		return nullptr;
	}
}

BARTENDER_API void DeleteBartenderAPI(
	void* handle)
{
	if (handle)
	{
		BartenderAPI* api =
			static_cast<BartenderAPI*>(handle);

		delete api;
	}
}

BARTENDER_API int StartEngine(void* handle)
{
	if (!handle)
	{
		return 0;
	}

	try
	{
		BartenderAPI* api =
			static_cast<BartenderAPI*>(handle);

		return api->start() ? 1 : 0;
	}
	catch (const std::exception& e)
	{
		BartenderAPI* api =
			static_cast<BartenderAPI*>(handle);
		api->setLastError(e.what());
		return 0;
	}
	catch (System::Exception^ ex)
	{
		BartenderAPI* api =
			static_cast<BartenderAPI*>(handle);
		api->setLastError(msclr::interop::marshal_as<std::string>(ex->ToString()));
		return 0;
	}
	catch (...)
	{
		BartenderAPI* api =
			static_cast<BartenderAPI*>(handle);
		api->setLastError("Unknown exception in StartEngine");
		return 0;
	}
}

BARTENDER_API void StopEngine(void* handle)
{
	if (handle)
	{
		BartenderAPI* api =
			static_cast<BartenderAPI*>(handle);

		api->stop();
	}
}

BARTENDER_API const char* GetLastErrorMessage(
	void* handle)
{
	static char lastError[4096] = { 0 };

	if (!handle)
	{
		return "";
	}

	BartenderAPI* api =
		static_cast<BartenderAPI*>(handle);

	std::string err =
		api->getLastError();

	strcpy_s(
		lastError,
		sizeof(lastError),
		err.c_str());

	return lastError;
}

BARTENDER_API int GetPrinterNames(
	void* handle,
	char** names,
	int maxCount)
{
	if (!handle || !names || maxCount <= 0)
	{
		return 0;
	}

	BartenderAPI* api =
		static_cast<BartenderAPI*>(handle);

	std::vector<std::string> printers =
		api->getPrinterNames();

	int count =
		(int)printers.size();

	if (count > maxCount)
	{
		count = maxCount;
	}

	for (int i = 0; i < count; ++i)
	{
		unsigned int len =
			(unsigned int)printers[i].size() + 1;

		names[i] = new char[len];

		strcpy_s(
			names[i],
			len,
			printers[i].c_str());
	}

	return count;
}

BARTENDER_API void FreePrinterNames(
	char** names,
	int count)
{
	if (!names)
	{
		return;
	}

	for (int i = 0; i < count; ++i)
	{
		delete[] names[i];
	}
}

BARTENDER_API int OpenLabelFormatDocument(
	void* handle,
	const char* absFilePath)
{
	if (!handle || !absFilePath)
	{
		return 0;
	}

	BartenderAPI* api = static_cast<BartenderAPI*>(handle);

	int len = static_cast<int>(strlen(absFilePath));
	bool hasNonAscii = false;
	for (int i = 0; i < len; i++)
	{
		if (static_cast<unsigned char>(absFilePath[i]) > 127)
		{
			hasNonAscii = true;
			break;
		}
	}

	wchar_t* widePath = nullptr;
	if (hasNonAscii)
	{
		int utf8Len = MultiByteToWideChar(CP_UTF8, 0, absFilePath, -1, nullptr, 0);
		if (utf8Len > 0)
		{
			widePath = new wchar_t[utf8Len];
			MultiByteToWideChar(CP_UTF8, 0, absFilePath, -1, widePath, utf8Len);
		}
	}

	if (!widePath)
	{
		int acpLen = MultiByteToWideChar(CP_ACP, 0, absFilePath, -1, nullptr, 0);
		widePath = new wchar_t[acpLen];
		MultiByteToWideChar(CP_ACP, 0, absFilePath, -1, widePath, acpLen);
	}

	int result = api->openLabelFormatDocumentW(widePath) ? 1 : 0;
	delete[] widePath;

	return result;
}

BARTENDER_API void CloseLabelFormatDocument(
	void* handle)
{
	if (handle)
	{
		BartenderAPI* api =
			static_cast<BartenderAPI*>(handle);

		api->closeLabelFormatDocument();
	}
}

BARTENDER_API int SetSubStringValue(
	void* handle,
	const char* name,
	const char* value)
{
	if (!handle || !name || !value)
	{
		return 0;
	}

	BartenderAPI* api = static_cast<BartenderAPI*>(handle);

	int nameWideLen = MultiByteToWideChar(CP_UTF8, 0, name, -1, nullptr, 0);
	wchar_t* wideName = new wchar_t[nameWideLen];
	MultiByteToWideChar(CP_UTF8, 0, name, -1, wideName, nameWideLen);

	int valueWideLen = MultiByteToWideChar(CP_UTF8, 0, value, -1, nullptr, 0);
	wchar_t* wideValue = new wchar_t[valueWideLen];
	MultiByteToWideChar(CP_UTF8, 0, value, -1, wideValue, valueWideLen);

	int result = api->setSubStringValueW(wideName, wideValue) ? 1 : 0;
	delete[] wideName;
	delete[] wideValue;

	return result;
}

BARTENDER_API int GetSubStringValue(
	void* handle,
	const char* name,
	char* buffer,
	int bufferSize)
{
	if (!handle || !name || !buffer || bufferSize <= 0)
	{
		return 0;
	}

	BartenderAPI* api =
		static_cast<BartenderAPI*>(handle);

	std::string value = api->getSubStringValue(name);
	
	if (value.empty())
	{
		return 0;
	}

	size_t len = value.size();
	if (len >= static_cast<size_t>(bufferSize))
	{
		len = static_cast<size_t>(bufferSize) - 1;
	}

	strcpy_s(buffer, bufferSize, value.c_str());
	return static_cast<int>(value.size());
}

BARTENDER_API int GetDocumentSubStrings(
	void* handle,
	char** names,
	int maxCount)
{
	if (!handle || !names || maxCount <= 0)
	{
		return 0;
	}

	BartenderAPI* api =
		static_cast<BartenderAPI*>(handle);

	std::vector<std::string> subs =
		api->currentDocumentSubStrings();

	int count = static_cast<int>(subs.size());
	if (count > maxCount)
	{
		count = maxCount;
	}

	for (int i = 0; i < count; ++i)
	{
		unsigned int len = static_cast<unsigned int>(subs[i].size()) + 1;
		names[i] = new char[len];
		strcpy_s(names[i], len, subs[i].c_str());
	}

	return count;
}

BARTENDER_API void FreeSubStrings(
	char** names,
	int count)
{
	if (!names)
	{
		return;
	}

	for (int i = 0; i < count; ++i)
	{
		delete[] names[i];
	}
}

BARTENDER_API int PrintLabel(
	void* handle,
	const char* printerName,
	int waitMs,
	char** msgs,
	int maxMsgCount)
{
	if (!handle || !printerName)
	{
		return 0;
	}

	BartenderAPI* api =
		static_cast<BartenderAPI*>(handle);

	std::vector<std::string> msgVec;

	int result =
		api->print(
			printerName,
			waitMs,
			msgVec);

	if (msgs && maxMsgCount > 0)
	{
		int count =
			(int)msgVec.size();

		if (count > maxMsgCount)
		{
			count = maxMsgCount;
		}

		for (int i = 0; i < count; ++i)
		{
			unsigned int len =
				(unsigned int)msgVec[i].size() + 1;

			msgs[i] = new char[len];

			strcpy_s(
				msgs[i],
				len,
				msgVec[i].c_str());
		}
	}

	return result;
}

BARTENDER_API void FreePrintMessages(
	char** msgs,
	int count)
{
	if (!msgs)
	{
		return;
	}

	for (int i = 0; i < count; ++i)
	{
		delete[] msgs[i];
	}
}

}