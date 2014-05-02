// GetTaxHistoryTest.cpp

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

		GetTaxHistoryRequest^ getTaxHistoryRequest = gcnew GetTaxHistoryRequest;

		// Required Request Parameters
        getTaxHistoryRequest->CompanyCode = "APITrialCompany";
        getTaxHistoryRequest->DocType = DocumentType::SalesInvoice;
        getTaxHistoryRequest->DocCode = "INV001";
		getTaxHistoryRequest->DetailLevel = DetailLevel::Tax;

        // Optional Request Parameters
        // getTaxHistoryRequest->DocId = "123412341234";

		GetTaxHistoryResult^ getTaxHistoryResult = taxSvc->GetTaxHistory(getTaxHistoryRequest);	

		Console::WriteLine("Result: {0}", getTaxHistoryResult->ResultCode.ToString());
		if(getTaxHistoryResult->ResultCode != SeverityLevel::Success)
		{
			for each(Message^ message in getTaxHistoryResult->Messages)			
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
			Console::WriteLine("Document Code: {0} Total Tax: {1}",
				getTaxHistoryResult->GetTaxRequest->DocCode,
				getTaxHistoryResult->GetTaxResult->TotalTax.ToString());
			
			for each(TaxLine^ taxLine in getTaxHistoryResult->GetTaxResult->TaxLines)
			{
				Console::WriteLine("    Line {0} Tax: {1}", taxLine->No, taxLine->Tax.ToString());
			
				for each(TaxDetail^ taxDetail in taxLine->TaxDetails)
				{	
					Console::WriteLine("        Jurisdiction: {0} Tax: {1}", 
						taxDetail->JurisName->ToString(),
						taxDetail->Tax.ToString());
				}
			}
			//to display diagnostic details in the console
			for each(Message^ message in getTaxHistoryResult->GetTaxResult->Messages)
			{
				Console::WriteLine("{0}\n{1}\n{2}",
					message->Name,
					message->Summary,
					message->Details);
				Console::ReadLine();
			}
		}
	}
	catch(Exception ^ex)
	{
		Console::WriteLine("Exception: " + ex->Message);
		Console::ReadLine();
		return -1;
	}

	Console::ReadLine();

	return 0;
}
