// ValidateAddressTest.cpp

#include "stdafx.h"

using namespace System;
using namespace Avalara::AvaTax::Adapter;
using namespace Avalara::AvaTax::Adapter::AddressService;

int main()
{
	String^ accountNumber = "1234567890";
	String^ licenseKey = "A1B2C3D4E5F6G7H8";
	String^ serviceURL = "https://development.avalara.net";
	try
	{
		AddressSvc^ addressSvc = gcnew AddressSvc;

		// Header Level Parameters
		// Required Header Parameters
		addressSvc->Configuration->Security->Account = accountNumber;
		addressSvc->Configuration->Security->License = licenseKey;
		addressSvc->Configuration->Url = serviceURL;
		addressSvc->Configuration->ViaUrl = serviceURL;
		addressSvc->Profile->Client = "AvaTaxSample";

		Address^ address = gcnew Address;
		
		// Required Address Parameters
		address->Line1 = "118 N Clark St";
		address->City = "Chicago";
		address->Region = "IL";

		// Optional Address Parameters
		address->Line2 = "Suite 100";
		address->Line3 = "ATTN Accounts Payable";
		address->Country = "US"; 
		address->PostalCode = "60602";
		
		ValidateRequest^ validateRequest = gcnew ValidateRequest;
		
		// Required Request Parameters
		validateRequest->Address = address;

		// Optional Request Parameters
		validateRequest->Coordinates = true;
		validateRequest->Taxability = true;
		validateRequest->TextCase = TextCase::Upper;
				
		ValidateResult^ validateResult = addressSvc->Validate(validateRequest);	

		Console::WriteLine("Result: {0}", validateResult->ResultCode.ToString());		
		Console::WriteLine();
		if(validateResult->ResultCode != SeverityLevel::Success)
		{
			for each(Message^ message in validateResult->Messages)
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
            ValidAddress^ validatedAddress = validateResult->Addresses[0];
            Console::WriteLine("Validated Address: ");
			Console::WriteLine("{0}\n{1}\n{2}\n{3}, {4} {5}\nCountry: {6}", 
				validatedAddress->Line1,
				validatedAddress->Line2,
				validatedAddress->Line3,
				validatedAddress->City,
				validatedAddress->Region,
				validatedAddress->PostalCode,
				validatedAddress->Country);
			Console::WriteLine("Latitude: {0}\nLongitude: {1}", 
				validatedAddress->Latitude,
				validatedAddress->Longitude);            
        }
	}
	catch(Exception^ ex)
	{
		Console::WriteLine("Exception: {0}", ex->Message);
		Console::ReadLine();
		return -1;
	}

	Console::ReadLine();
	return 0;
}
