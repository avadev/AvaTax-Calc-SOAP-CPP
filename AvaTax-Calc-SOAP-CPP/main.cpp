// main.cpp : main project file.

#include "stdafx.h"
#include "PingTest.h"
#include "GetTaxTest.h"
#include "PostTaxTest.h"
#include "CommitTaxTest.h"
#include "GetTaxHistoryTest.h"
#include "AdjustTaxTest.h"
#include "CancelTaxTest.h"
#include "ValidateAddressTest.h"

using namespace System;

int main(array<System::String ^> ^args)
{
    PingTest pingTest;
	GetTaxTest getTaxTest;
    PostTaxTest postTaxTest;
    CommitTaxTest commitTaxTest;
    GetTaxHistoryTest getTaxHistoryTest;
    AdjustTaxTest adjustTaxTest;
    CancelTaxTest cancelTaxTest;
    ValidateAddressTest validateAddressTest;

	pingTest.Test();
    getTaxTest.Test();
    postTaxTest.Test();
    commitTaxTest.Test();
    getTaxHistoryTest.Test();
    adjustTaxTest.Test();
    cancelTaxTest.Test();
    validateAddressTest.Test();

	Console::ReadLine();

	return 0;
}
