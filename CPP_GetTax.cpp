// CPP_GetTax.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace Avalara::AvaTax::Adapter;
using namespace Avalara::AvaTax::Adapter::TaxService;
using namespace Avalara::AvaTax::Adapter::AddressService;

int main(array<System::String ^> ^args)
{
	if(args->Length != 4)
	{
		Console::WriteLine("Usage: CPP_GetTax <url> <account> <key> <company code>");
		return -1;
	}

	TaxSvc ^taxSvc = gcnew TaxSvc();
	GetTaxRequest ^getTaxRequest = gcnew GetTaxRequest();
	GetTaxResult ^getTaxResult;
	Address ^documentOrigAddress = gcnew Address();
	Address ^documentDestAddress = gcnew Address();
	Line ^lItem = gcnew Line();

	// Profile->Client is required - pattern is ERPName,majver,minver[,connectorname,majver,minver]
	taxSvc->Profile->Client = "CPP_GetTax,1,0";

	taxSvc->Configuration->Url = args[0];
	taxSvc->Configuration->Security->Account = args[1];
	taxSvc->Configuration->Security->License = args[2];	
	getTaxRequest->CompanyCode = args[3];
	
	// Default (header) Origin Address for GetTax
	documentOrigAddress->Line1 = "100 Ravine Lane NE";
	documentOrigAddress->Line2 = "Suite 220";
	documentOrigAddress->City = "Bainbridge Island";
	documentOrigAddress->Region = "WA";
	documentOrigAddress->PostalCode = "98110";
	documentOrigAddress->Country = "US";

	// Default (header) Destination Address for GetTax
	documentDestAddress->Line1 = "7462 Kearny St";
	documentDestAddress->Line2 = "";
	documentDestAddress->City = "Commerce City";
	documentDestAddress->Region = "CO";
	documentDestAddress->PostalCode = "80022-1335";
	documentDestAddress->Country = "US";
	getTaxRequest->DestinationAddress = documentDestAddress;
	getTaxRequest->OriginAddress = documentOrigAddress;
	
	/* DetailLevel sets the amount of information returned in the GetTaxResult
	 * these are in order of detail complexity
	 * Summary		//returns total tax for each jurisdiction without line detail
	 * Document		//returns tax for complete order
	 * Line			//returns tax for each line
	 * Tax			//returns jurisdictional tax details
	 * Diagnostic	//ONLY FOR USE ON TEST SYSTEMS - returns all details
	*/
	getTaxRequest->DetailLevel = DetailLevel::Tax;
	
	/* DocType determines whether the document is an order or an invoice
	 * Orders are temporary - invoices are permanent (saved)
	 * SalesOrder, SalesInvoice, PurchaseOrder, PurchaseInvoice, ReturnOrder, ReturnInvoice */
	getTaxRequest->DocType = DocumentType::SalesOrder;
	/* Change the DocumentType to SalesInvoice to save this document to the Admin Console*/

	getTaxRequest->DocDate = DateTime::Now;
	
	getTaxRequest->Commit = false;	//set to true to commit document to reports
	// ***Commit may also be done separately with PostTax() or CommitTax() methods - see SDK Best Practices.
    
	getTaxRequest->CustomerCode = "UniqueCustomerID";

	// DocCode is the order or invoice number and should be unique
	// The sample uses a GUID - you should use a number or string that matches the order in your accounting system
	getTaxRequest->DocCode = "SAMPLE_" + DateTime::UtcNow;

	// The sample uses a static two-line example - you will want to consider how to build a loop that will
	// allow any number of lines.  Also, please refer to the .NET SDK adapter documentation for other
	// line variables - we have only used the minimum here.
	lItem->No = "0001";
	lItem->ItemCode = "SDK Documentation";
	lItem->Description = "A box of SDK Documentation";
	lItem->Qty = Convert::ToDouble(1);
	lItem->Amount = Convert::ToDecimal(100);
	getTaxRequest->Lines->Add(lItem);

	lItem = gcnew Line();
	lItem->No = "0002";
	lItem->ItemCode = "Shipping";
	lItem->Description = "Shipping Item";
	lItem->TaxCode = "FR020100";
	lItem->Qty = Convert::ToDouble(1);
	lItem->Amount = Convert::ToDecimal(10);
	getTaxRequest->Lines->Add(lItem);

	try
	{
		getTaxResult = taxSvc->GetTax(getTaxRequest);
	}
	catch(Exception ^ex)
	{
		Console::WriteLine("GetTax threw this exception: " + ex->Message);
		return -1;
	}

	Console::WriteLine("GetTax Result: " + getTaxResult->ResultCode.ToString());
	if(getTaxResult->ResultCode != SeverityLevel::Success)
	{
		for(int i = 0; i < getTaxResult->Messages->Count; i++)
		{
			Console::WriteLine(getTaxResult->Messages[i]->Name);
			Console::WriteLine(getTaxResult->Messages[i]->Summary);
			Console::WriteLine(getTaxResult->Messages[i]->Details);
		}
	}
	else
	{
		Console::WriteLine("GetTax For Invoice #" + getTaxRequest->DocCode+ " Succeeded");                
		Console::WriteLine("Total Tax: " + getTaxResult->TotalTax.ToString());
		Console::WriteLine();
        
		if (getTaxRequest->DetailLevel > DetailLevel::Document)
		{
			Console::WriteLine("Line Taxes:");
		}
		for(int tli = 0; tli < getTaxResult->TaxLines->Count; tli++)              
		{
			TaxLine ^taxLine = getTaxResult->TaxLines[tli];
			Console::WriteLine("     Line " + taxLine->No + " Total Taxable: " + taxLine->Taxable.ToString());
			Console::WriteLine("     Line " + taxLine->No + " Total Tax: " + taxLine->Tax.ToString());

			
			for(int tdi = 0; tdi < taxLine->TaxDetails->Count; tdi++)
			{	
				TaxDetail ^taxDetail = taxLine->TaxDetails->default[tdi];
				Console::WriteLine("=============================");
				Console::WriteLine("          Jurisdiction Name: " + taxDetail->JurisName->ToString());
				Console::WriteLine("          Jurisdiction Tax Name: " + taxDetail->TaxName->ToString());
				Console::WriteLine("          Jurisdiction Type: " + taxDetail->JurisType.ToString());
				Console::WriteLine("          Jurisdiction Rate: " + taxDetail->Rate.ToString());
				Console::WriteLine("          Jurisdiction NonTaxable: " + taxDetail->NonTaxable.ToString());
				Console::WriteLine("          Jurisdiction Taxable: " + taxDetail->Taxable.ToString());
				Console::WriteLine("          Jurisdiction Tax: " + taxDetail->Tax.ToString());
				Console::WriteLine("          Jurisdiction Code: " + taxDetail->JurisCode->ToString());
			}//end for
			Console::WriteLine("=============================");
		}//end for
		//to display diagnostic details in the console
		for(int i = 0; i < getTaxResult->Messages->Count; i++)
		{
			Console::WriteLine(getTaxResult->Messages[i]->Name);
			Console::WriteLine(getTaxResult->Messages[i]->Summary);
			Console::WriteLine(getTaxResult->Messages[i]->Details);
		}
	}//end else

	return 0;
}
