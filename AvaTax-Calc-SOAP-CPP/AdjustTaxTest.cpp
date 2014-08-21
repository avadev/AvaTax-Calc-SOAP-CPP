#include "stdafx.h"
#include "AdjustTaxTest.h"

using namespace System;
using namespace System::Configuration;
using namespace Avalara::AvaTax::Adapter;
using namespace Avalara::AvaTax::Adapter::TaxService;
using namespace Avalara::AvaTax::Adapter::AddressService;

AdjustTaxTest::AdjustTaxTest(){}

void AdjustTaxTest::Test()
{
	String^ accountNumber = ConfigurationManager::AppSettings["AvaTax:AccountNumber"];
	String^ licenseKey = ConfigurationManager::AppSettings["AvaTax:LicenseKey"];
	String^ serviceURL = ConfigurationManager::AppSettings["AvaTax:ServiceUrl"];

	try
	{
		TaxSvc^ taxSvc = gcnew TaxSvc();

		// Header Level Parameters
		// Required Header Parameters
		taxSvc->Configuration->Security->Account = accountNumber;
		taxSvc->Configuration->Security->License = licenseKey;
		taxSvc->Configuration->Url = serviceURL;
		taxSvc->Configuration->ViaUrl = serviceURL;
		taxSvc->Profile->Client = "AvaTaxSample";

		GetTaxRequest^ getTaxRequest = gcnew GetTaxRequest();
	
		// Document Level Parameters

		// Required Request Parameters
		getTaxRequest->CustomerCode = "ABC4335";
		getTaxRequest->DocDate = DateTime::Parse("2014-01-01");
		//// getTaxRequest->Lines is also required, 
		//// and is presented later in this file.

		// Best Practice Request Parameters
		getTaxRequest->CompanyCode = "APITrialCompany";
		getTaxRequest->DocCode = "INV001";
		getTaxRequest->DetailLevel = DetailLevel::Tax;
		getTaxRequest->DocType = DocumentType::SalesInvoice;
		getTaxRequest->Commit = true;
	
		// Situational Request Parameters	
		// getTaxRequest->BusinessIdentificationNo = "234243";
		// getTaxRequest->CustomerUsageType = "G";
		// getTaxRequest->ExemptionNo = "12345";
		// getTaxRequest->Discount = 50;
		// getTaxRequest->LocationCode = "01";
		// getTaxRequest->ServiceMode = ServiceMode::Automatic;	
		// getTaxRequest->TaxOverride->TaxOverrideType = TaxOverrideType::TaxDate;
		// getTaxRequest->TaxOverride->Reason = "Adjustment for return";
		// getTaxRequest->TaxOverride->TaxDate = DateTime::Parse("2013-07-01");    
		
		// Optional Request Parameters
		getTaxRequest->PurchaseOrderNo = "PO123456";
		getTaxRequest->ReferenceCode = "ref123456";
		getTaxRequest->PosLaneCode = "09";
		getTaxRequest->CurrencyCode = "USD";
		getTaxRequest->ExchangeRate = Convert::ToDecimal(1.0);
		getTaxRequest->ExchangeRateEffDate = DateTime::Parse("2013-01-01");
		getTaxRequest->SalespersonCode = "Bill Sales";
	
		Address^ address1 = gcnew Address();
		address1->Line1 = "45 Fremont Street";	
		address1->City = "San Francisco";
		address1->Region = "CA";

		Address^ address2 = gcnew Address();
		address2->Line1 = "118 N Clark St";
		address2->Line2 = "Suite 100";
		address2->Line3 = "ATTN Accounts Payable";
		address2->City = "Chicago";
		address2->Region = "IL";
		address2->PostalCode = "60602";
		address2->Country = "US";	

		Address^ address3 = gcnew Address();
		address3->Latitude = "47.627935";
		address3->Longitude = "-122.51702";

		getTaxRequest->OriginAddress = address1;
		getTaxRequest->DestinationAddress = address3;

		// Line Data
    
		// Required Parameters
		Line^ line1 = gcnew Line();
		line1->No = "0001";
		line1->ItemCode = "N543";	
		line1->Qty = Convert::ToDouble(1);
		line1->Amount = Convert::ToDecimal(10);
		line1->OriginAddress = address1;
		line1->DestinationAddress = address2;
	
		// Best Practice Request Parameters
		line1->Description = "Red Size 7 Widget";
		line1->TaxCode = "NT";

		// Situational Request Parameters
		// line1->CustomerUsageType = "L";
		// line1->ExemptionNo = "12345";
		// line1->Discounted = true;
		// line1->TaxIncluded = true;
		// line1->TaxOverride->TaxOverrideType = TaxOverrideType::TaxDate;
		// line1->TaxOverride->Reason = "Adjustment for return";
		// line1->TaxOverride->TaxDate = DateTime::Parse("2013-07-01");

		// Optional Request Parameters
		line1->Ref1 = "ref123";
		line1->Ref2 = "ref456";

		getTaxRequest->Lines->Add(line1);

		Line^ line2 = gcnew Line();
		line2->No = "0002";
		line2->ItemCode = "T345";		
		line2->Qty = Convert::ToDouble(4);
		line2->Amount = Convert::ToDecimal(200);
		line2->OriginAddress = address1;
		line2->DestinationAddress = address3;
		line2->Description = "Size 10 Green Running Shoe";
		line2->TaxCode = "PC030147";
		getTaxRequest->Lines->Add(line2);

		Line^ line3 = gcnew Line();
		line3->No = "0002-FR";
		line3->ItemCode = "FREIGHT";		
		line3->Qty = Convert::ToDouble(1);
		line3->Amount = Convert::ToDecimal(15);
		line3->OriginAddress = address1;
		line3->DestinationAddress = address3;
		line3->Description = "Shipping Charge";
		line3->TaxCode = "FR";
		getTaxRequest->Lines->Add(line3);

		AdjustTaxRequest^ adjustTaxRequest = gcnew AdjustTaxRequest();
		adjustTaxRequest->GetTaxRequest = getTaxRequest;
		adjustTaxRequest->AdjustmentReason = 4; // quantity change
		//adjustTaxRequest->AdjustmentDescription = "Transaction Adjusted for Testing";
		
		AdjustTaxResult^ adjustTaxResult = taxSvc->AdjustTax(adjustTaxRequest);	

		Console::WriteLine("AdjustTaxTest Result: {0}", adjustTaxResult->ResultCode.ToString());
		if(adjustTaxResult->ResultCode != SeverityLevel::Success)
		{
			for each(Message^ message in adjustTaxResult->Messages)
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
			Console::WriteLine("Document Code: {0}\nTotal Tax: {1}\nDocument Version: {2}", 
				getTaxRequest->DocCode, 
				adjustTaxResult->TotalTax.ToString(),
				adjustTaxResult->Version.ToString());
			
			for each(TaxLine^ taxLine in adjustTaxResult->TaxLines)
			{
				Console::WriteLine("    Line {0} Tax: {1}",	
					taxLine->No, taxLine->Tax.ToString());
			
				for each(TaxDetail^ taxDetail in taxLine->TaxDetails)
				{	
					Console::WriteLine("        {0} Tax: {1}",
						taxDetail->JurisName->ToString(),
						taxDetail->Tax.ToString());
				}
			}
		}
	}
	catch(Exception ^ex)
	{
		Console::WriteLine("Exception: {0}", ex->Message);
	}
	Console::WriteLine("----------------------------------------------");
}