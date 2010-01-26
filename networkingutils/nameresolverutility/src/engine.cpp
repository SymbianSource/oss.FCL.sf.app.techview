// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// engine.cpp - nslookup client engine
//

#include <e32math.h>
#include <e32std.h>
#include <eikenv.h>
#include <plpsess.h>	//Used for RRemoteLink
#include <networking/dnd_err.h>
#include <dns_ext.h>
#include <dns_qry.h>
#include "engine.h"
#include <nslookup.rsg>
#include "nslookup.h"

CNslookup::CNslookup(): CActive(EPriorityStandard)
	{
	CActiveScheduler::Add(this);	//Adds itself to the scheduler only the first time
	}

//Sets the remote link to off
void CNslookup::DisableRemoteLink()
	{
	RRemoteLink link;
	TRemoteLinkStatus state;

	TInt err=link.Open();
	if (err==KErrNone)
		{
		if (link.Status(state)!=KErrNone)
			return;
		if (state.iStatus!=TRemoteLinkStatus::EDisabled)
			{
			iConsole->WriteLine(_L("Disabling Remote link\n"));
			link.Disable(); 
			}
		link.Close();
		}
	}

//Sets all default values. Actually it's no more a L function
void CNslookup::ConstructL(const TPreferences& aPref)
	{

	// Base class second-phase construction.

	iHostname=aPref.iHostname;
	}

//return the current preferences
void CNslookup::GetPreferences(TPreferences &aPref) const
	{	
	aPref.iHostname=iHostname;		//Address to Ping
	}

void CNslookup::DefaultPreferences(TPreferences &aPref)
	{	
	aPref.iHostname=_L("127.0.0.1");
	}


const TDesC* CNslookup::GetHostName() const
	{
	return &iHostname;
	}

void CNslookup::SetHostName(const TDes& aHostname)
	{
	iHostname = aHostname;
	}

void CNslookup::SetConsole(CNslookupContainer* aConsole)
	{
	iConsole = aConsole;
	}

CNslookup::~CNslookup()
	{
	Cancel();
	}

//Shows the error and set the application as not running. 
//Requires a return after calling it!

void CNslookup::Error(const TDesC& string,TInt error)
	{
	TBuf<150> aux;
	TBuf<100> errtxt;

	CEikonEnv::Static()->GetErrorText( errtxt,error);
	aux.Append(string);	
	aux.Append(_L(": "));
	aux.Append(errtxt);
	aux.AppendFormat(_L(" (%d)\n"), error);
	iConsole->WriteLine(aux);
	}


void CNslookup::Stop()
	{
	iHostResolv.Close();
	iSockServ.Close();
	CEikonEnv::Static()->BusyMsgCancel();
	}


void CNslookup::BeginL()
	{
	TInt err=0;

	if (IsRunning())	// There's another instance running
		return;

	//INITIALIZATION

	DisableRemoteLink();

	iConsole->WriteHostL(iHostname);
		
	//connecting the Socket Server
	err = iSockServ.Connect();	//KESockDefaultMessageSlots
	if (err!=KErrNone)
		{
		Error(_L("Socket Server Error (Connect)"),err);
		return;
		}

	err = iHostResolv.Open(iSockServ, KAfInet, KProtocolInetUdp);	// Address Resolver 
	if (err != KErrNone)
		{
		Error(_L("Resolver Error (Open)"),err);
		iSockServ.Close();
		return;
		}
	iConsole->WriteLine(_L("\nResolving...\n"));	
	CEikonEnv::Static()->BusyMsgL(R_RESOLVING_NAME);

	TUint16 querytype = KDnsRRTypeInvalid;

	// This lengthy code checks what the user selected as query type in UI dialog
	switch (iQueryType)
		{
	case 0:		// Default (GetName)
		break;
	
	case 1:		// Query A
		querytype = KDnsRRTypeA;
		break;
		
	case 2:		// Query NS
		querytype = KDnsRRTypeNS;
		break;
	
	case 3:		// Query CNAME
		querytype = KDnsRRTypeCNAME;
		break;
	
	case 4:		// Query WKS
		querytype = KDnsRRTypeWKS;
		break;
	
	case 5:		// Query PTR
		querytype = KDnsRRTypePTR;
		break;
	
	case 6:		// Query HINFO
		querytype = KDnsRRTypeHINFO;
		break;
	
	case 7:		// Query MX
		querytype = KDnsRRTypeMX;
		break;
	
	case 8:		// Query TXT
		querytype = KDnsRRTypeTXT;
		break;
	
	case 9:		// Query AAAA
		querytype = KDnsRRTypeAAAA;
		break;
		
	case 10:	// Query SRV
		querytype = KDnsRRTypeSRV;
		break;
	
	case 11:	// Query NAPTR
		querytype = KDnsRRTypeNAPTR;
		break;
	
	case 12:	// Query Any
		querytype = KDnsQTypeANY;
		break;

#ifdef DND_DCM_EXTENSION				
	case 13:	// Cache Clear
		querytype = KDnsQTypeCacheClear;
		break;
#endif

	default:
		break;
		}
		
	// If query type was something else than default using GetName(), use the new
	// Query() interface instead
	if (querytype != KDnsRRTypeInvalid)
		{
#ifdef __RHOSTRESOLV_QUERY_IF
		TBuf8<KHostNameLimit> name8;
		name8.Copy(iHostname);
		TDnsQuery query(name8, querytype);
		TPckgC<TDnsQuery> querybuf(query);	

		// Hmm... Esock seems to use the current length when allocating a buffer for result
		// output. It should use MaxLength() instead, and now we have to do this to get
		// over with it
		iResult.SetLength(iResult.MaxLength());
		iHostResolv.Query(querybuf, iResult, iStatus);
#else
		TBuf<80> aux;
		aux.Copy(_L("Query() interface not supported\n"));
		iConsole->WriteLine(aux);
		iQueryType = 0;
		Stop();
		return;
#endif
		}
	else
		{
		if (iAddress.Input(iHostname) == KErrNone)
			{
			iHostResolv.GetByAddress(iAddress, iEntry, iStatus);
			}
		else
			{
			iHostResolv.GetByName(iHostname, iEntry, iStatus);
			}
		}

	iCount = 0;
	SetActive();	//Sets the object as Active.
	}

static TPtrC ErrorTextL(const TInt aCode)
	{
	switch (aCode)
		{
		case KErrNotFound:			return _L("Name not found");

//		case KErrDndTimedOut:		return _L("timed out");
//		case KErrDndNoHost:			return _L("no host");	// [dubious error code -- eliminate? ]
//		case KErrDndNoMemorySend:	return _L("Out of memory on send");
//		case KErrDndNotSent:		return _L("Query not sent");
		case KErrDndCache:			return _L("Cache error");

		case KErrDndFormat:			return _L("Bad DNS reply format");
		case KErrDndServerFailure:	return _L("DNS server failed");
		case KErrDndBadName:		return _L("Name does not exist in DNS");
		case KErrDndNotImplemented:	return _L("Query not implemented by DNS server");
		case KErrDndRefused:		return _L("DNS Server refused query");

//		case KErrDndNoMemoryProc:	return _L("Insufficient memory - can not process the response");
		case KErrDndNoRecord:		return _L("No record found of the desired type and class"); 
		case KErrDndNameTooBig:		return _L("Buffer overflow");
		case KErrDndUnknown:		return _L("Error in DND");
		case KErrDndServerUnusable:	return _L("No answer available from current server");
		default:
			break;

		}
	User::Leave(aCode);
	// NOTREACHED
	return _L("");  // to make compiler happy
	}

void CNslookup::RunL()
	{
	TInt src_addr = 0;

	TBuf<100> textIPaddress;	//text address to be displayed
	TBuf<356> aux;

	if (iStatus != KErrNone)
		{
		if (iCount == 0)
			{
			// An error message is only shown, if the primary query
			// failes, and not for subsequent Next() operations.
			TBuf<100> msg(iHostname);

			TRAPD(err,
				TPtrC errText = ErrorTextL(iStatus.Int());
				msg.AppendFormat(_L(" - %S\n"), &errText));
			if (err == KErrNone)
				iConsole->WriteLine(msg);
			else
				Error(msg, iStatus.Int());
			}
		Stop();
		return;
		}	

	// Check if we are using the query interface instead of GetName()
	if (iQueryType != KDnsRRTypeInvalid)
		{
#ifdef __RHOSTRESOLV_QUERY_IF
		QueryResponse();
		
		// See explanation of this SetLength() hack above
		iResult.SetLength(iResult.MaxLength());
		iHostResolv.QueryGetNext(iResult, iStatus);
		iCount++;
		SetActive();
#endif		
		return;
		}

	aux.Append(iEntry().iName);   // maybe the main name is not the entered
	switch (iEntry().iAddr.Family())
		{
	case KAfInet:
	case KAfInet6:
		aux.Append(_L(" is "));
		iHostAddr = TInetAddr::Cast(iEntry().iAddr);	//host address
		iHostAddr.Output(textIPaddress);
		aux.Append(textIPaddress);
		src_addr = iShowSource && !iHostAddr.IsUnspecified();
		break;
	case KAfDns:
		{
		SDnsRR &rr = TInetDnsRR::Cast(iEntry().iAddr).RR();
		if (rr.iClass == 1 /* IN */)
			{
			if (rr.iType == 2 /* NS */)
				{
				aux.Append(_L(" NS"));
				break;
				}
			else if (rr.iType == 6 /* SOA */)
				{
				aux.AppendFormat(_L(" SOA serial=%u refresh=%u retry=%u expire=%u min=%u"),
					(TUint)rr.iSOA.iSerial,
					(TUint)rr.iSOA.iRefresh,
					(TUint)rr.iSOA.iRetry,
					(TUint)rr.iSOA.iExpire,
					(TUint)rr.iSOA.iMinimum);
				break;
				}
			else if (rr.iType == 15 /* MX */)
				{
				aux.AppendFormat(_L(" MX preference=%d"), (TInt)rr.iMX.iPreference);
				break;
				}
			else if (rr.iType == 33 /* SRV */)
				{
				aux.AppendFormat(_L(" SRV port=%d priority=%d weight=%d"),
					(TInt)iEntry().iAddr.Port(), (TInt)rr.iSRV.iPriority, (TInt)rr.iSRV.iWeight);
				break;
				}
			else if (rr.iType == 35 /* NAPTR */)
				{
				TPtrC replacement = rr.iNAPTR.REPLACEMENT(iEntry().iName);
				TPtrC services = rr.iNAPTR.SERVICES(iEntry().iName);
				TPtrC regexp = rr.iNAPTR.REGEXP(iEntry().iName);

				aux.AppendFormat(_L(" NAPTR order=%d, preference=%d repl=%S services=%S regexp=%S"),
					(TInt)rr.iNAPTR.iOrder,
					(TInt)rr.iNAPTR.iPreference,
					&replacement,
					&services,
					&regexp);

				break;
				}
			}
		aux.AppendFormat(_L(" class=%d type=%d"), (TInt)rr.iClass, (TInt)rr.iType);
		}
		break;
	default:
		break;
		}

	const TUint flags = iEntry().iFlags;
	if (flags)
		{
		aux.Append(_L(" ( "));
		if (flags & EDnsAlias)
			aux.Append(_L("Alias "));
		if (flags & EDnsAuthoritive)
			aux.Append(_L("Authoritative "));
		if (flags & EDnsHostsFile)
			aux.Append(_L("Hostfile "));
		if (flags & EDnsServer)
			aux.Append(_L("DNS "));
		if (flags & EDnsHostName)
			aux.Append(_L("MyName "));
		if (flags & EDnsCache)
			aux.Append(_L("Cached "));
		aux.Append(')');
		}
	aux.Append(_L("\n"));
	iConsole->WriteLine(aux);
	iCount++;
	if (src_addr)
		{
		// Show a matching SRC address.
		RSocket socket;
		TPckgBuf<TSoInetIfQuery> opt;
		opt().iDstAddr = iHostAddr;
		if (socket.Open(iSockServ, KAfInet, KSockDatagram, KProtocolInetUdp) == KErrNone)
			{
			_LIT(KIsVIF, " (VIF)");
			_LIT(KIsIF, "");
			_LIT(KNoRoute, "(no route)");

			(void)socket.GetOpt(KSoInetIfQueryByDstAddr, KSolInetIfQuery, opt);
			socket.Close();
			opt().iSrcAddr.OutputWithScope(textIPaddress);
			aux.Format(_L("   src= %S @ %S%S\n"), &textIPaddress, &opt().iName,
				opt().iName.Length() == 0 ? &KNoRoute() : opt().iIsUp ? &KIsIF() : &KIsVIF());
			iConsole->WriteLine(aux);
			}
		else
			iConsole->WriteLine(_L("cannot find src, UDP socket open failed\n"));
		}
	//
	// Get next address
	//
	iHostResolv.Next(iEntry, iStatus);
	SetActive();
	}

void CNslookup::DoCancel()
	{
	// Called only from Cancel() if there is pending resolve going on... (IsActive())
	iHostResolv.Cancel();
	}


// Outputs some text about the Query() response to the console
void CNslookup::QueryResponse()
	{
	TBuf<256> aux;
	TBuf<128> addrbuf;

	if (iResult.Length() < (TInt)sizeof(TDnsQryRespBase))
		{
		aux.AppendFormat(_L("Malformed response (length: %d)\n"), iResult.Length());
		return;
		}

	TDnsQryRespBase *respbase = (TDnsQryRespBase *)iResult.Ptr(); //lint !e826 // area length checked above
	aux.Append(iHostname);
	aux.AppendFormat(_L(": cl: %d  ttl: %d  "),
			respbase->RRClass(), respbase->RRTtl());

	// ugh... I'll do this the hard way: temporary buffer for converting from 8-bit
	// DNS result descriptors to 16-bit descriptor and the append to output buffer.
	// A better solution will follow a bit later...
	switch(respbase->RRType())
		{
	case KDnsRRTypeA:
		{
		const TDnsRespA *const respA = (TDnsRespA *) respbase;
		respA->HostAddress().Output(addrbuf);
		aux.Append(_L("type: A  "));
		aux.Append(_L("addr: "));
		aux.Append(addrbuf);
		}
		break;
		
	case KDnsRRTypeAAAA:
		{
		const TDnsRespAAAA *const respAAAA = (TDnsRespAAAA *) respbase;
		respAAAA->HostAddress().Output(addrbuf);
		aux.Append(_L("type: AAAA  "));
		aux.Append(_L("addr: "));
		aux.Append(addrbuf);
		}
		break;

	case KDnsRRTypePTR:
		{
		const TDnsRespPTR *const respPTR = (TDnsRespPTR *) respbase;
		aux.Append(_L("type: PTR  "));
		aux.Append(_L("name: "));
		addrbuf.Copy(respPTR->HostName());
		aux.Append(addrbuf);
		}
		break;
		
	case KDnsRRTypeMX:
		{
		const TDnsRespMX *const respMX = (TDnsRespMX *) respbase;
		aux.Append(_L("type: MX  "));
		aux.AppendFormat(_L("pref: %d  "), respMX->Pref());
		aux.Append(_L("name: "));
		addrbuf.Copy(respMX->HostName());
		aux.Append(addrbuf);
		}
		break;
		
	case KDnsRRTypeSRV:
		{
		const TDnsRespSRV *const respSRV = (TDnsRespSRV *) respbase;
		aux.Append(_L("type: SRV  "));
		aux.AppendFormat(_L("prio: %d  wght: %d  port: %d  targ: "),
				respSRV->Priority(), respSRV->Weight(), respSRV->Port());
		addrbuf.Copy(respSRV->Target());
		aux.Append(addrbuf);
		}
		break;
		
	case KDnsRRTypeNAPTR:
		{
		const TDnsRespNAPTR *const respNAPTR = (TDnsRespNAPTR *) respbase;
		aux.Append(_L("type: NAPTR  "));
		aux.AppendFormat(_L("ordr: %d  pref: %d  flag: "));
		addrbuf.Copy(respNAPTR->Flags());
		aux.Append(addrbuf);
		aux.Append(_L("  serv: "));
		addrbuf.Copy(respNAPTR->Service());
		aux.Append(addrbuf);
		aux.Append(_L("  regx: "));
		addrbuf.Copy(respNAPTR->Regexp());
		aux.Append(addrbuf);
		aux.Append(_L("  repl: "));
		addrbuf.Copy(respNAPTR->Replacement());
		aux.Append(addrbuf);
		}
		break;
		
#ifdef DND_DCM_EXTENSION	
	case KDnsQTypeCacheClear:
		{
		aux.Append(_L("  OK"));
		break;
		}
#endif

	default:
		aux.AppendFormat(_L("Unknown response type: %d"), respbase->RRType());
		break;
		}
			
	aux.Append(_L("\n"));
	iConsole->WriteLine(aux);			
	}

// Stops NSLOOKUP

void CNslookup::EndNslookup()
	{	
	Cancel();
	Stop();
	}

// Just checks if sending packets from a previous ping

TBool CNslookup::IsRunning() const
	{
	return IsActive();
	}
