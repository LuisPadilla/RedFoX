/* soapStub.h
   Generated by gSOAP 2.8.7 from vatCheckService.h

Copyright(C) 2000-2011, Robert van Engelen, Genivia Inc. All Rights Reserved.
The generated code is released under one of the following licenses:
1) GPL or 2) Genivia's license for commercial use.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
*/

#ifndef soapStub_H
#define soapStub_H
#define SOAP_NAMESPACE_OF_ns2	"urn:ec.europa.eu:taxud:vies:services:checkVat:types"
#include "stdsoap2.h"
#if GSOAP_H_VERSION != 20807
# error "GSOAP VERSION MISMATCH IN GENERATED CODE: PLEASE REINSTALL PACKAGE"
#endif


/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_ns2__matchCode
#define SOAP_TYPE_ns2__matchCode (13)
/* ns2:matchCode */
enum ns2__matchCode {ns2__matchCode__1 = 1, ns2__matchCode__2 = 2};
#endif

/******************************************************************************\
 *                                                                            *
 * Types with Custom Serializers                                              *
 *                                                                            *
\******************************************************************************/


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#if 0 /* volatile type: do not declare here, declared elsewhere */

#endif

#ifndef SOAP_TYPE__ns2__checkVat
#define SOAP_TYPE__ns2__checkVat (8)
/* ns2:checkVat */
class SOAP_CMAC _ns2__checkVat
{
public:
	char *countryCode;	/* required element of type xsd:string */
	char *vatNumber;	/* required element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 8; } /* = unique id SOAP_TYPE__ns2__checkVat */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns2__checkVat() { _ns2__checkVat::soap_default(NULL); }
	virtual ~_ns2__checkVat() { }
};
#endif

#ifndef SOAP_TYPE__ns2__checkVatResponse
#define SOAP_TYPE__ns2__checkVatResponse (9)
/* ns2:checkVatResponse */
class SOAP_CMAC _ns2__checkVatResponse
{
public:
	char *countryCode;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
	char *vatNumber;	/* required element of type xsd:string */
	char *requestDate;	/* required element of type xsd:date */
	bool valid;	/* required element of type xsd:boolean */
	char *name;	/* optional element of type xsd:string */
	char *address;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 9; } /* = unique id SOAP_TYPE__ns2__checkVatResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns2__checkVatResponse() { _ns2__checkVatResponse::soap_default(NULL); }
	virtual ~_ns2__checkVatResponse() { }
};
#endif

#ifndef SOAP_TYPE__ns2__checkVatApprox
#define SOAP_TYPE__ns2__checkVatApprox (10)
/* ns2:checkVatApprox */
class SOAP_CMAC _ns2__checkVatApprox
{
public:
	char *countryCode;	/* required element of type xsd:string */
	char *vatNumber;	/* required element of type xsd:string */
	char *traderName;	/* optional element of type xsd:string */
	char *traderCompanyType;	/* optional element of type ns2:companyTypeCode */
	char *traderStreet;	/* optional element of type xsd:string */
	char *traderPostcode;	/* optional element of type xsd:string */
	char *traderCity;	/* optional element of type xsd:string */
	char *requesterCountryCode;	/* optional element of type xsd:string */
	char *requesterVatNumber;	/* optional element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 10; } /* = unique id SOAP_TYPE__ns2__checkVatApprox */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns2__checkVatApprox() { _ns2__checkVatApprox::soap_default(NULL); }
	virtual ~_ns2__checkVatApprox() { }
};
#endif

#ifndef SOAP_TYPE__ns2__checkVatApproxResponse
#define SOAP_TYPE__ns2__checkVatApproxResponse (11)
/* ns2:checkVatApproxResponse */
class SOAP_CMAC _ns2__checkVatApproxResponse
{
public:
	char *countryCode;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
	char *vatNumber;	/* required element of type xsd:string */
	char *requestDate;	/* required element of type xsd:date */
	bool valid;	/* required element of type xsd:boolean */
	char *traderName;	/* optional element of type xsd:string */
	char *traderCompanyType;	/* optional element of type ns2:companyTypeCode */
	char *traderAddress;	/* optional element of type xsd:string */
	char *traderStreet;	/* optional element of type xsd:string */
	char *traderPostcode;	/* optional element of type xsd:string */
	char *traderCity;	/* optional element of type xsd:string */
	enum ns2__matchCode *traderNameMatch;	/* optional element of type ns2:matchCode */
	enum ns2__matchCode *traderCompanyTypeMatch;	/* optional element of type ns2:matchCode */
	enum ns2__matchCode *traderStreetMatch;	/* optional element of type ns2:matchCode */
	enum ns2__matchCode *traderPostcodeMatch;	/* optional element of type ns2:matchCode */
	enum ns2__matchCode *traderCityMatch;	/* optional element of type ns2:matchCode */
	char *requestIdentifier;	/* required element of type xsd:string */
	struct soap *soap;	/* transient */
public:
	virtual int soap_type() const { return 11; } /* = unique id SOAP_TYPE__ns2__checkVatApproxResponse */
	virtual void soap_default(struct soap*);
	virtual void soap_serialize(struct soap*) const;
	virtual int soap_put(struct soap*, const char*, const char*) const;
	virtual int soap_out(struct soap*, const char*, int, const char*) const;
	virtual void *soap_get(struct soap*, const char*, const char*);
	virtual void *soap_in(struct soap*, const char*, const char*);
	         _ns2__checkVatApproxResponse() { _ns2__checkVatApproxResponse::soap_default(NULL); }
	virtual ~_ns2__checkVatApproxResponse() { }
};
#endif

#ifndef SOAP_TYPE___ns1__checkVat
#define SOAP_TYPE___ns1__checkVat (20)
/* Operation wrapper: */
struct __ns1__checkVat
{
public:
	_ns2__checkVat *ns2__checkVat;	/* optional element of type ns2:checkVat */
};
#endif

#ifndef SOAP_TYPE___ns1__checkVatApprox
#define SOAP_TYPE___ns1__checkVatApprox (24)
/* Operation wrapper: */
struct __ns1__checkVatApprox
{
public:
	_ns2__checkVatApprox *ns2__checkVatApprox;	/* optional element of type ns2:checkVatApprox */
};
#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (25)
/* SOAP Header: */
struct SOAP_ENV__Header
{
#ifdef WITH_NOEMPTYSTRUCT
private:
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (26)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
public:
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (28)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
public:
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (31)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
public:
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif

#endif

#ifndef WITH_NOGLOBAL

#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (32)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
public:
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif

#endif

/******************************************************************************\
 *                                                                            *
 * Typedefs                                                                   *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE_xsd__date
#define SOAP_TYPE_xsd__date (7)
typedef char *xsd__date;
#endif

#ifndef SOAP_TYPE_ns2__companyTypeCode
#define SOAP_TYPE_ns2__companyTypeCode (12)
typedef char *ns2__companyTypeCode;
#endif


/******************************************************************************\
 *                                                                            *
 * Externals                                                                  *
 *                                                                            *
\******************************************************************************/


#endif

/* End of soapStub.h */