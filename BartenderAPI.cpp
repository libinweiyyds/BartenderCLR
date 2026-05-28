#include "pch.h"

#include "BartenderAPI.h"

#include <msclr/marshal_cppstd.h>

#using <System.Drawing.dll>

using namespace msclr::interop;

using namespace System;
using namespace System::Drawing::Printing;

BartenderAPI::BartenderAPI()
{
    m_engine = nullptr;
    m_document = nullptr;
}

BartenderAPI::~BartenderAPI()
{
    stop();
}

bool BartenderAPI::start()
{
    try
    {
        Engine^ engine = m_engine;
        if (engine != nullptr)
        {
            return true;
        }

        m_engine = gcnew Engine(true);

        return true;
    }
    catch (Exception^ ex)
    {
        m_lastError =
            marshal_as<std::string>(
                ex->ToString());

        return false;
    }
}

void BartenderAPI::stop()
{
    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc != nullptr)
        {
            doc->Close(
                SaveOptions::DoNotSaveChanges);
            m_document = nullptr;
        }

        Engine^ engine = m_engine;
        if (engine != nullptr)
        {
            engine->Stop();
            m_engine = nullptr;
        }
    }
    catch (Exception^ ex)
    {
        m_lastError =
            marshal_as<std::string>(
                ex->ToString());
    }
}

std::string BartenderAPI::getLastError()
{
    return m_lastError;
}

void BartenderAPI::setLastError(const std::string& error)
{
    m_lastError = error;
}

std::vector<std::string>
BartenderAPI::getPrinterNames()
{
    std::vector<std::string> result;

    try
    {
        for each(String^ printerName
            in PrinterSettings::InstalledPrinters)
        {
            result.push_back(
                marshal_as<std::string>(
                    printerName));
        }
    }
    catch (Exception^ ex)
    {
        m_lastError =
            marshal_as<std::string>(
                ex->ToString());
    }

    return result;
}

bool BartenderAPI::openLabelFormatDocument(
    const char* absFilePath)
{
    try
    {
        Engine^ engine = m_engine;
        if (engine == nullptr)
        {
            m_lastError = "Engine not started";
            return false;
        }

        if (!absFilePath || strlen(absFilePath) == 0)
        {
            m_lastError = "Invalid file path";
            return false;
        }

        String^ path = gcnew String(absFilePath);

        m_document = engine->Documents->Open(path);

        return true;
    }
    catch (Exception^ ex)
    {
        m_lastError = marshal_as<std::string>(ex->ToString());
        return false;
    }
}

bool BartenderAPI::openLabelFormatDocumentW(const wchar_t* absFilePath)
{
    try
    {
        Engine^ engine = m_engine;
        if (engine == nullptr)
        {
            m_lastError = "Engine not started";
            return false;
        }

        if (!absFilePath || wcslen(absFilePath) == 0)
        {
            m_lastError = "Invalid file path";
            return false;
        }

        String^ path = gcnew String(absFilePath);

        m_document = engine->Documents->Open(path);

        return true;
    }
    catch (Exception^ ex)
    {
        m_lastError = marshal_as<std::string>(ex->ToString());
        return false;
    }
}

void BartenderAPI::closeLabelFormatDocument()
{
    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc != nullptr)
        {
            doc->Close(
                SaveOptions::DoNotSaveChanges);
            m_document = nullptr;
        }
    }
    catch (Exception^ ex)
    {
        m_lastError =
            marshal_as<std::string>(
                ex->ToString());
    }
}

std::vector<std::string>
BartenderAPI::currentDocumentSubStrings()
{
    std::vector<std::string> result;

    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc == nullptr)
        {
            return result;
        }

        SubStrings^ subStrings =
            doc->SubStrings;

        for each(SubString^ sub in subStrings)
        {
            result.push_back(
                marshal_as<std::string>(
                    sub->Name));
        }
    }
    catch (Exception^ ex)
    {
        m_lastError =
            marshal_as<std::string>(
                ex->ToString());
    }

    return result;
}

bool BartenderAPI::setSubStringValue(const char* name, const char* value)
{
    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc == nullptr)
        {
            m_lastError = "No document opened";
            return false;
        }

        if (!name || !value)
        {
            m_lastError = "Invalid parameters";
            return false;
        }

        String^ nameStr = gcnew String(name);
        String^ valueStr = gcnew String(value);

        SubStrings^ subStrings = doc->SubStrings;
        
        for (int i = 0; i < subStrings->Count; i++)
        {
            SubString^ sub = subStrings[i];
            if (sub->Name->Equals(nameStr))
            {
                sub->Value = valueStr;
                return true;
            }
        }
        
        m_lastError = "SubString not found: " + std::string(name);
        return false;
    }
    catch (Exception^ ex)
    {
        m_lastError = marshal_as<std::string>(ex->ToString());
        return false;
    }
}

bool BartenderAPI::setSubStringValueW(const wchar_t* name, const wchar_t* value)
{
    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc == nullptr)
        {
            m_lastError = "No document opened";
            return false;
        }

        if (!name || !value)
        {
            m_lastError = "Invalid parameters";
            return false;
        }

        String^ nameStr = gcnew String(name);
        String^ valueStr = gcnew String(value);

        SubStrings^ subStrings = doc->SubStrings;
        
        for (int i = 0; i < subStrings->Count; i++)
        {
            SubString^ sub = subStrings[i];
            if (sub->Name->Equals(nameStr))
            {
                sub->Value = valueStr;
                return true;
            }
        }
        
        m_lastError = "SubString not found";
        return false;
    }
    catch (Exception^ ex)
    {
        m_lastError = marshal_as<std::string>(ex->ToString());
        return false;
    }
}

std::string BartenderAPI::getSubStringValue(const char* name)
{
    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc == nullptr)
        {
            m_lastError = "No document opened";
            return "";
        }

        if (!name)
        {
            m_lastError = "Invalid parameter";
            return "";
        }

        String^ nameStr = gcnew String(name);
        SubStrings^ subStrings = doc->SubStrings;
        
        for (int i = 0; i < subStrings->Count; i++)
        {
            SubString^ sub = subStrings[i];
            if (sub->Name->Equals(nameStr))
            {
                return marshal_as<std::string>(sub->Value);
            }
        }
        
        m_lastError = "SubString not found: " + std::string(name);
        return "";
    }
    catch (Exception^ ex)
    {
        m_lastError = marshal_as<std::string>(ex->ToString());
        return "";
    }
}

std::wstring BartenderAPI::getSubStringValueW(const wchar_t* name)
{
    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc == nullptr)
        {
            m_lastError = "No document opened";
            return L"";
        }

        if (!name)
        {
            m_lastError = "Invalid parameter";
            return L"";
        }

        String^ nameStr = gcnew String(name);
        SubStrings^ subStrings = doc->SubStrings;
        
        for (int i = 0; i < subStrings->Count; i++)
        {
            SubString^ sub = subStrings[i];
            if (sub->Name->Equals(nameStr))
            {
                return marshal_as<std::wstring>(sub->Value);
            }
        }
        
        m_lastError = "SubString not found";
        return L"";
    }
    catch (Exception^ ex)
    {
        m_lastError = marshal_as<std::string>(ex->ToString());
        return L"";
    }
}

int BartenderAPI::print(
    const char* printerName,
    int waitMs,
    std::vector<std::string>& msgs)
{
    try
    {
        LabelFormatDocument^ doc = m_document;
        if (doc == nullptr)
        {
            return 0;
        }

        String^ printer =
            gcnew String(printerName);

        doc->PrintSetup->PrinterName =
            printer;

        Messages^ btMsgs = nullptr;

        Result result =
            doc->Print(
                "QtPrintJob",
                waitMs,
                btMsgs);

        if (btMsgs != nullptr)
        {
            for each(Message^ msg in btMsgs)
            {
                msgs.push_back(
                    marshal_as<std::string>(
                        msg->Text));
            }
        }

        return result == Result::Success ? 1 : 0;
    }
    catch (Exception^ ex)
    {
        m_lastError =
            marshal_as<std::string>(
                ex->ToString());

        return 0;
    }
}