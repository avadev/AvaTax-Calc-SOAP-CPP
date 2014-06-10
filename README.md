AvaTax-SOAP-CPP
=====================

This is a sample for C++/CLI demonstrating the [AvaTax SOAP API](http://developer.avalara.com/api-docs/soap) methods.

For more information on the use of these methods and the AvaTax product, please visit our [developer site](http://developer.avalara.com/) or [home page](http://www.avalara.com/)
 
The Visual Studio solution uses a main.cpp file to run the Avalara AvaTax methods in order. Some calls may not be necessary for your integration.
 
Requirements:
----------
- If you do not have an AvaTax account, a free trial account can be acquired through our [developer site](http://developer.avalara.com/api-get-started)
- Enter your account credentials in each test file (e.g. \Source Files\PingTest.cpp) you would like to run. You can pass your free trial Username and Password in the accountNumber and licenseKey fields, otherwise use your actual account and key.
- If you're using a paid account instead of a free trial account, you will also need to change the CompanyCode in some files. The default value we have entered is APITrialCompany.
  
Contents:
----------
 
<table>
<th colspan="2" align=left>Sample Projects</th>
<tr><td>PingTest</td><td>Demonstrates a ping call to verify connectivity and credentials.</td></tr>
<tr><td>GetTaxTest</td><td>Demonstrates the GetTax method used for product- and line- specific <a href="http://developer.avalara.com/api-docs/api-reference/gettax">calculation</a>.</td></tr>
<tr><td>PostTaxTest</td><td>Demonstrates the PostTax method used to <a href="http://developer.avalara.com/api-docs/api-reference/posttax-and-committax">post or commit</a> a previously recorded document.</td></tr>
<tr><td>CommitTaxTest</td><td>Demonstrates the CommitTax method used to <a href="http://developer.avalara.com/api-docs/api-reference/posttax-and-committax">commit</a> a previously posted document.</td></tr>
<tr><td>GetTaxHistoryTest</td><td>Demonstrates a GetTaxHistory call to retrieve document details for a saved transaction.</td></tr>
<tr><td>AdjustTaxTest</td><td>Demonstrates an AdjustTax call to modify a previously committed transaction. If it's not committed continue to use GetTax to update the document.</td></tr>
<tr><td>CancelTaxTest</td><td>Demonstrates the CancelTax method used to <a href="http://developer.avalara.com/api-docs/api-reference/canceltax">void a document</a>.</td></tr>
<tr><td>ValidateAddressTest</td><td>Demonstrates the Validate method to <a href="http://developer.avalara.com/api-docs/api-reference/address-validation">normalize an address</a>.</td></tr>
<th colspan="2" align=left>Core Classes</th>
<tr><td>Avalara.AvaTax.Adapter.dll</td><td>-</td></tr>
<th colspan="2" align=left>Other Files</th>
<tr><td>.gitattributes</td><td>-</td></tr>
<tr><td>.gitignore</td><td>-</td></tr>
<tr><td>AvaTax-SOAP-CPP.sln</td><td>-</td></tr>
<tr><td>LICENSE.md</td><td>-</td></tr>
<tr><td>README.md</td><td>-</td></tr>
</table>

Dependencies:
-----------
- .NET 2.0 or later
- Avalara.AvaTax.Adapter.DLL (included in sample)



