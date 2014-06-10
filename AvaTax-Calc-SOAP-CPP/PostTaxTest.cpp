#include "stdafx.h"
#include "PostTaxTest.h"

using namespace System;
using namespace Avalara::AvaTax::Adapter;
using namespace Avalara::AvaTax::Adapter::TaxService;

PostTaxTest::PostTaxTest(){}

void PostTaxTest::Test()
{
	String^ accountNumber = "1234567890";
	String^ licenseKey = "A1B2C3D4E5F6G7H8";
	String^ serviceURL = "https://development.avalara.net";
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