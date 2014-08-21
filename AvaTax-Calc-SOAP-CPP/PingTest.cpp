#include "stdafx.h"
#include "PingTest.h"

using namespace System;
using namespace System::Configuration;
using namespace Avalara::AvaTax::Adapter;
using namespace Avalara::AvaTax::Adapter::TaxService;

PingTest::PingTest(){}

void PingTest::Test()
{
	String^ accountNumber = ConfigurationManager::AppSettings["AvaTax:AccountNumber"];
	String^ licenseKey = ConfigurationManager::AppSettings["AvaTax:LicenseKey"];
	String^ serviceURL = ConfigurationManager::AppSettings["AvaTax:ServiceUrl"];

	try
	{
		TaxSvc^ taxSvc = gcnew TaxSvc;

		// Header Level Parameters
		// Required Header Parameters
		taxSvc->Configuration->Security->Account = accountNumber;
		taxSvc->Configuration->Security->License = licenseKey;
		taxSvc->Configuration->Url = serviceURL;
		taxSvc->Configuration->ViaUrl = serviceURL;
		taxSvc->Profile->Client = "AvaTaxSample";

		PingResult^ pingResult = taxSvc->Ping("");	

		Console::WriteLine("PingTest Result: {0}\nVersion: {1}",
			pingResult->ResultCode.ToString(), pingResult->Version);
		if(pingResult->ResultCode != SeverityLevel::Success)
		{
			for each(Message^ message in pingResult->Messages)			
			{
				Console::WriteLine("{0}\n{1}\n{2}\n{3}", 
					message->Name,
					message->Summary,
					message->RefersTo,
					message->Details);
			}
		}
	}
	catch(Exception^ ex)
	{
		Console::WriteLine("Exception: {0}", ex->Message);
	}	
	Console::WriteLine("----------------------------------------------");
}