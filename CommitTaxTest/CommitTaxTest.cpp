// CommitTaxTest.cpp

#include "stdafx.h"

using namespace System;
using namespace Avalara::AvaTax::Adapter;
using namespace Avalara::AvaTax::Adapter::TaxService;
using namespace Avalara::AvaTax::Adapter::AddressService;

int main()
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

		CommitTaxRequest^ commitTaxRequest = gcnew CommitTaxRequest;

		// Required Request Parameters
		commitTaxRequest->CompanyCode = "APITrialCompany";
		commitTaxRequest->DocType = DocumentType::SalesInvoice;
		commitTaxRequest->DocCode = "INV001";

		// Optional Request Parameters
		// commitTaxRequest->DocId = "123412341234";

		CommitTaxResult^ commitTaxResult = taxSvc->CommitTax(commitTaxRequest);	

		Console::WriteLine("Result: {0}", 
			commitTaxResult->ResultCode.ToString());

		if(commitTaxResult->ResultCode != SeverityLevel::Success)
		{
			for each(Message^ message in commitTaxResult->Messages)			
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
			Console::WriteLine("Document Committed.");
		}
	}
	catch(Exception ^ex)
	{
		Console::WriteLine("Exception: {0}", ex->Message);
		Console::ReadLine();
		return -1;
	}

	Console::ReadLine();

	return 0;
}
