OESS User Agent
===============

MANUAL
------
OESS-Client is a tool for utilizing the OESS (Open Exchange Software
Suite) web API. It is designed to access services supported by Indiana
University's Global Research NOC (http://www.globalnoc.iu.edu).


Table Of Contents
-----------------

1. [Simple Usage](#simple-usage)
2. [Install](#install)
3. [Input File](#input-file)
4. [Design](#design)
5. [Future Features](#future-features)
6. [OESS Services](#oess-services)
7. [Contact](#contact)

-----------------


Simple Usage
---------------

Synopsis:

    oess-client inFile [outFile]

Description:

inFile is the path to the input file. This file should contain the 
properly formatted action you wish to perform. See the Input File section for details 
on how to format.

To run the example:

    oess-client example.in

Install
----------

__Build__

Build and install the following dependencies:

    libcurl 7.35.0 (build with SSL feature enabled)
    iperf 3-CURRENT

Build the application:

    make

Input File
-------------

The inFile is parsed to extract the method name, service type, and all 
parameters for the method call. Inorder for the file to be parsed correctly the 
inFile must be in the following format:

    service=<data|provisioning|measurement|monitoring|remote>
    action=<any method available in the OESS API under the service type set above>
    parameter=<parameter name listed in documentation>
    value=<value of the parameter>

__Example__

```
# A more extensive example can be found in the example.in file.

# Lines starting with '#' are comments

## Method Call Definition
# service=<data|provisioning|measurement|monitoring|remote>
# action=<any method available in the OESS API under the service type set above>
# <parameter>=<value>, where parameter is any parameter name listed in the api 
# for the method and value is the corresponding value.

# Action 1
service=data
action=get_maps
# get_maps does not take any parameters so they do not need to be defined

# Action 2
#service=data
#action=get_existing_circuits
#workgroup_id=1
```

Design
---------

__Parser__

The parser extracts all the method call information from the inFile. It currently can
only handle one method call per file (see future features).

    Input: file with a method call
    Output: URL to pass the method call and parameters to the correct cgi script. 
Eg.

    data.cgi?action=get_existing_circuits&parameter=workgroup_id&value=411&

__Client__

The client send requests to the OESS services and receives the response, storing the 
result into the outFile. It also stores the HTTP header response in the heardRecv file to 
help with debugging.

    Input: URL for a method call (from the parser)
    Output: The service response store in the outFile.

Future Features
------------------

    * Extend the parser to handle multiple method calls per file.
    * Possibly develop a more user-friendly format for the method definition file.  
    * Add base service URL to input file to work with more OESS host's
    * Include a way to view the method call results other than opening the file.

OESS Services
----------------

Please see http://globalnoc.iu.edu/sdn/oess.html to find the most current API
documentation from the Global Research Network Operations Center.

Contact
----------

If you have problems, questions, ideas, or suggestions please contact me at blearn@psc.edu.
