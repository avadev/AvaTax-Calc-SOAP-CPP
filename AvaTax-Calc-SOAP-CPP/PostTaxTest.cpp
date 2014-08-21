#include "stdafx.h"
#include "PostTaxTest.h"

using namespace System;
using namespace System::Configuration;
using namespace Avalara::AvaTax::Adapter;
using namespace Avalara::AvaTax::Adapter::TaxService;

PostTaxTest::PostTaxTest(){}

void PostTaxTest::Test()
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

		PostTaxRequest^ postTaxRequest = gcnew PostTaxRequest;

		// Required Request Parameters
		postTaxRequest->CompanyCode = "APITrialCompany";
		postTaxRequest->DocType = DocumentType::SalesInvoice;
		postTaxRequest->DocCode = "INV001";
		postTaxRequest->Commit = false;
		postTaxRequest->DocDate = DateTime::Parse("2014-01-01");
		postTaxRequest->TotalTax = Convert::ToDecimal(14.27);
		postTaxRequest->TotalAmount = 175;

		// Optional Request Parameters
		// postTaxRequest->NewDocCode = "INV001-1";

		PostTaxResult^ postTaxResult = taxSvc->PostTax(postTaxRequest);	

		Console::WriteLine("PostTaxTest Result: {0}", postTaxResult->ResultCode.ToString());		
		if(postTaxResult->ResultCode != SeverityLevel::Success)
		{
			for each(Message^ message in postTaxResult->Messages)			
			{
				Console::WriteLine("{0}\n{1}\n{2}\n{3}",
					message->Name,
					message->Summary,
					message->RefersTo,
					message->Details);
			}
		}
		else
		{
			Console::WriteLine("Document Posted.");
		}
	}
	catch(Exception ^ex)
	{
		Console::WriteLine("Exception: " + ex->Message);
	}
	Console::WriteLine("----------------------------------------------");
}