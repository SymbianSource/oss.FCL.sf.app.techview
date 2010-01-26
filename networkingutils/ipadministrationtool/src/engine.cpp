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
// engine.cpp - IP administration tool engine
//

#include <eikenv.h>
#include "ipadm.h"
#include "engine.h"

#include "uniload.h"

#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <in_sock_internal.h>
#endif

#ifndef __SECURE_DATA__
#define DEFAULT_ROUTES_FILE	_L("C:\\Data\\IpAdm\\route.ini")
#else
#define DEFAULT_ROUTES_FILE	_L("route.ini")
#endif

LOCAL_C void AppendInterfaceStatus(TDes& buf, const TInt aStatus)
	{
	switch (aStatus)
		{
		case EIfPending:	buf.Append(_L("PEND ")); break;
		case EIfUp:			break;
		case EIfBusy:		buf.Append(_L("BUSY ")); break;
		case EIfDown:		buf.Append(_L("DOWN ")); break;
		default:			buf.Append(_L("???? ")); break;
		}
	}

LOCAL_C void AppendInterfaceName(TDes& buf, const TName& aName)
	{
	if (aName.Length() > 0)
		{
		buf.Append(aName);
		}
	else
		{
		buf.Append(_L("default"));
		}
	}

LOCAL_C void AppendRouteType(TDes& buf, const TInt aType)
	{
	switch (aType)
		{
		case ERtNormal:		buf.Append(_L(" ")); break;
		case ERtUser:		buf.Append(_L("u ")); break;
		case ERtIcmpAdd:	buf.Append(_L("- ")); break; // for IPv6, this is ND entry
		case ERtIcmpDel:	buf.Append(_L("d ")); break;
		default:			buf.Append(_L("? ")); break;
		}
	}

LOCAL_C void AppendRouteState(TDes& buf, const TInt aState)
	{
	switch (aState)
		{
		case ERtNone:		buf.Append(_L("NONE ")); break;
		case ERtPending:	buf.Append(_L("PEND ")); break;
		case ERtBusy:		buf.Append(_L("BUSY ")); break;
		case ERtReady:		break;
		case ERtDown:		buf.Append(_L("DOWN ")); break;
		default:			buf.Append(_L("???? ")); break;
		}
	}

//
//
// Just count the number of rightmost zeroes in an IPv6 address
// and return (128 - count).
//
LOCAL_C TInt MaskLength(const TIp6Addr &aAddr)
	{
	TInt count = 0;
	for (TInt i = sizeof(aAddr.u.iAddr8) / sizeof(aAddr.u.iAddr8[0]); --i >= 0; )
		{
		if (aAddr.u.iAddr8[i])
			{
			TUint8 nonZeroByte = aAddr.u.iAddr8[i];
			while ((nonZeroByte & 1) == 0)
				{
				count += 1;
				nonZeroByte >>= 1;
				}
			break;
			}
		count += 8;
		}
	return 128-count;
	}


LOCAL_C TInt MaskLength(TUint32 aAddr)
	{
	TInt count = 0;
	// obviously, this is "brute force" counting
	while (aAddr & 0x80000000)
		{
		count++;
		aAddr <<= 1;
		}
	return count;
	}


// TRawAddr
// *********
// Lightweight internal help class for handling the link layer addresses
//
class TRawAddr : public TSockAddr
	{
public:

	void Output(TDes& aBuf) const
		{
		TUint8* p = (TUint8*)UserPtr();
		TInt len = ((TSockAddr *)this)->GetUserLen();	//lint !e1773 // it's a kludge but works anyway

		aBuf.SetLength(0);
		if (len == 0)
			return;
		if (aBuf.MaxLength() < len * 4)
			{
			aBuf.Fill('*', aBuf.MaxLength());
			return;
			}
		if (len == 0)
			return;
		for (;;)
			{
			aBuf.AppendFormat(_L("%02X"), *p++ & 0xFF);
			if (--len == 0)
				break;
			aBuf.Append(TChar(':'));
			}
		}

	inline static TRawAddr& Cast(const TSockAddr& aAddr)
		{
		return *((TRawAddr *)&aAddr); //lint !e1773 // standard way to implement Cast
		}
	inline static TRawAddr& Cast(const TSockAddr* aAddr)
		{
		return *((TRawAddr *)aAddr); //lint !e1773 // standard way to implement Cast
		}
	};

//
CIpAdmEngine::~CIpAdmEngine()
	{
	//
	// Ordering is important... Do not close iSS before
	// all sockets associated with it have been closed!
	//
	iFS.Close();
	iSS.Close();
	}

void CIpAdmEngine::ConstructL()
	{
	//
	// Start Socket Reader activity
	//
	CheckResultL(_L("Active Scheduler"), CActiveScheduler::Current() == NULL);
	CheckResultL(_L("Connect to File server"), iFS.Connect());
	CheckResultL(_L("Connect to Socket server"), iSS.Connect());
	}


LOCAL_C void AppendIpAddress(TDes &buf, const TDesC &aLabel, const TInetAddr &aAddr, TInt aSkip = 1)
	{
	if (aSkip && aAddr.IsUnspecified())
		return;
	TBuf<64> out;
	buf.Append(aLabel);
#ifdef HURRICANE_INSOCK
	aAddr.Output(out);
#else
	aAddr.OutputWithScope(out);
#endif
	buf.Append(out);
	}

LOCAL_C void AppendRawAddress(TDes &buf, const TDesC &aLabel, const TSockAddr &aAddr)
	{
	if (aAddr.Family() == KAFUnspec)
		return;
	TBuf<100> out;
	buf.Append(aLabel);
	TRawAddr::Cast(aAddr).Output(out);
	buf.Append(out);
	}

LOCAL_C void AppendIpMask(TDes &buf, const TInetAddr &aAddr)
	{
	if (aAddr.Family() == KAfInet6)
		buf.AppendFormat(_L("/%d"), MaskLength(aAddr.Ip6Address()));
	else
		buf.AppendFormat(_L("/%d"), MaskLength(aAddr.Address())); 
	}

void CIpAdmEngine::Show(TInt /*aVersion*/, const TSoInetInterfaceInfo &aInfo)
	{
	TBuf<1000> buf;
	buf = _L("if ");
	AppendInterfaceStatus(buf, aInfo.iState);
	AppendInterfaceName(buf, aInfo.iName);
	AppendIpAddress(buf, _L(" addr="), (const TInetAddr &)aInfo.iAddress, 0);
	AppendIpMask(buf, (const TInetAddr &)aInfo.iNetMask);
	AppendIpAddress(buf, _L(" bcast="), (const TInetAddr &)aInfo.iBrdAddr);
	AppendIpAddress(buf, _L(" gw="), (const TInetAddr &)aInfo.iDefGate);
	AppendIpAddress(buf, _L(" ns1="), (const TInetAddr &)aInfo.iNameSer1);
	AppendIpAddress(buf, _L(" ns2="), (const TInetAddr &)aInfo.iNameSer2);
	AppendRawAddress(buf, _L(" hwa="), aInfo.iHwAddr);
	buf.AppendFormat(_L(" Mtu=%d, SM=%d, F=%x"), aInfo.iMtu, aInfo.iSpeedMetric, (TUint)aInfo.iFeatures);
	ShowText(buf);
	}

void CIpAdmEngine::Show(TInt /*aVersion*/, const TSoInetRouteInfo &aInfo)
	{
	TBuf<1000> buf;
	buf = _L("rt");
	AppendRouteType(buf, aInfo.iType);
	AppendRouteState(buf, aInfo.iState);
	AppendIpAddress(buf, _L(""), (const TInetAddr &)aInfo.iDstAddr, 0);
	AppendIpMask(buf, (const TInetAddr &)aInfo.iNetMask);
	// the iIfAddr is actually the src address to be used for this route destination
	AppendIpAddress(buf, _L(" src="), (const TInetAddr &)aInfo.iIfAddr);
	if (aInfo.iGateway.Family() == KAfInet6 || aInfo.iGateway.Family() == KAfInet)
		AppendIpAddress(buf, _L(" gw="), (const TInetAddr &)aInfo.iGateway);
	else
		AppendRawAddress(buf, _L(" hwa="), aInfo.iGateway);
	buf.AppendFormat(_L(" M=%d"), aInfo.iMetric);
	ShowText(buf);
	}

void CIpAdmEngine::ListInterfaces(TInt aVersion, const TDesC &aProtocol)
	{
	RSocket socket;

	if (CheckResult(aProtocol, socket.Open(iSS, aProtocol)) != KErrNone)
		return;
	if (socket.SetOpt(KSoInetEnumInterfaces, KSolInetIfCtrl) == KErrNone)
		{
		TPckgBuf<TSoInetInterfaceInfo> opt;
		while (socket.GetOpt(KSoInetNextInterface, KSolInetIfCtrl, opt) == KErrNone)
			Show(aVersion, opt());
		}
	socket.Close();
	}

void CIpAdmEngine::ListRoutes(TInt aVersion, const TDesC &aProtocol)
	{
	RSocket socket;

	if (CheckResult(aProtocol, socket.Open(iSS, aProtocol)) != KErrNone)
		return;
	if (socket.SetOpt(KSoInetEnumRoutes, KSolInetRtCtrl) == KErrNone)
		{
		TPckgBuf<TSoInetRouteInfo> opt;
		while (socket.GetOpt(KSoInetNextRoute, KSolInetRtCtrl, opt) == KErrNone)
			Show(aVersion, opt());
		}
	socket.Close();
	}

class TParser : public TLex
	{
public:
	TParser(const TDesC &aStr);
	void NextToken();
	int SkipSpaceAndMark();
	TInt ParameterValue(const TDesC &aKey, TInt &aValue, const TInt aDefault = 0);
	TInt ParameterValue(const TDesC &aKey, TInetAddr &aValue);
	TLexMark iLine;
	TLexMark iCurrent;
	TPtrC iLastLine;
	TPtrC Line();
	inline void MarkLine() { iCurrent = iLine; iOpen = 1; }
	TPtrC iToken;
	TInt iOpen;
	TInt iFirst;	// Non-zero, if next token is first in line
	};

#pragma warning (disable:4097)
TParser::TParser(const TDesC &aStr) : TLex(aStr), iLine(), iCurrent(), iLastLine(), iToken(), iOpen(0), iFirst(1)
	{
	Mark(iLine);
	MarkLine();
	}
#pragma warning (default:4097)

//
// Return current line as a descriptor
// (and skip to the next line, if not fully parsed)
//
TPtrC TParser::Line()
	{
	if (iOpen)
		{
		// Line not fully parsed yet, search to the EOL
		while (!Eos())
			{
			TChar ch = Get();
			if (ch == '\n' || ch == '\r')
				{
				iLastLine.Set(MarkedToken(iCurrent));
				iOpen = 0;
				Mark(iLine);
				break;
				}
			}
		}
	return iLastLine;
	}
//
// Skip white space and mark, including comments!
//
int TParser::SkipSpaceAndMark()
	{
	TChar ch;
	TInt comment = 0;
	TInt newline = 0;

	while (!Eos())
		{
		ch = Get();
		if (ch =='\n')
			{
			comment = 0;
			newline = 1;
			if (iOpen)
				{
				iLastLine.Set(MarkedToken(iCurrent));
				iOpen = 0;
				}
			Mark(iLine);
			}
		else if (comment || ch == '#')
			comment = 1;
		else if (!ch.IsSpace())
			{
			UnGet();
			break;
			}
		}
	Mark();
	return newline;
	}

//
// Extract Next token and return
//
void TParser::NextToken()
	{
	if (SkipSpaceAndMark())
		iFirst = 1;		// New line!
	if (Eos())
		{
		iFirst = -1;
		return;
		}
	while (!Eos())
		{
		TChar ch = Peek();
		if (ch == '#' || ch.IsSpace())
			break;
		Inc();
		}
	iToken.Set(MarkedToken());
	iFirst = SkipSpaceAndMark();
	}

TInt TParser::ParameterValue(const TDesC &aKey, TInt &aValue, const TInt aDefault)
	{
	if (aKey.Compare(iToken) != 0)
		return KErrNotFound;		// Doesn't match, do nothing
	//
	// When the keyword matches, return is always KErrNone
	// (caller must deduce errors by aDefault.
	//
	if (iFirst)
		aValue = aDefault;
	else if (Val(aValue) == KErrNone)
		iFirst = SkipSpaceAndMark();
	else
		aValue = aDefault;
	return KErrNone;
	}

TInt TParser::ParameterValue(const TDesC &aKey, TInetAddr &aValue)
	{
	if (aKey.Compare(iToken) != 0)
		return KErrNotFound;		// Doesn't match, do nothing
	//
	// When the keyword matches, return is always KErrNone
	// (caller must deduce errors by aDefault.
	//
	if (!iFirst)
		{
		NextToken();
		if (aValue.Input(iToken) == KErrNone)
			return KErrNone;
		}
	aValue.SetFamily(0);
	return KErrNone;
	}


void CIpAdmEngine::AddRoutes(const TDesC &aProtocol)
	{
	TInt err = KErrNone;
	RSocket socket;

	if (CheckResult(aProtocol, socket.Open(iSS, aProtocol)) != KErrNone)
		return;

	HBufC *buf = NULL;
#ifndef __DATA_CAGING__
	TRAP(err, buf = UnicodeLoad::LoadL(iFS, DEFAULT_ROUTES_FILE));
	if (CheckResult(DEFAULT_ROUTES_FILE, err))
#else // __DATA_CAGING__
// get private path
	TFileName filename;
	RFs theFS;

	theFS.Connect();
	theFS.PrivatePath(filename);
	theFS.Close();
	filename.Append(DEFAULT_ROUTES_FILE);

	TRAP(err, buf = UnicodeLoad::LoadL(iFS, filename));
	if (CheckResult(filename, err))
#endif // __DATA_CAGING__
		{
		socket.Close();
		return;
		}
	//lint -save -e613 Trapped above
	TParser inifile(*buf); //lint -restore
	TInt route_count = 0;
	TInt if_count = 0;
	while (!err)
		{
		// Skip until first token in line
		while (inifile.iFirst == 0)
			inifile.NextToken();
		if (inifile.iFirst < 0)
			break;

		//
		// The route file is a simple list of lines with following format
		//
		inifile.NextToken();
		inifile.MarkLine();
		if ((inifile.iToken.Compare(_L("route-add")) == 0))
			{
			//    route-add destination netmask metric gateway interface
			//
			// all except the "metric" are assumed to be addresses
			//
			TPckgBuf<TSoInetRouteInfo> opt;
			opt().iType = ERtUser;
			opt().iState = ERtReady;
			opt().iIfAddr.SetFamily(KAFUnspec);
			for (int i = 0; !err && inifile.iFirst == 0; ++i)
				{
				switch (i)
					{
					case 0:	// Destination
						inifile.NextToken();
						err = (TInetAddr::Cast(opt().iDstAddr)).Input(inifile.iToken);
						break;
					case 1: // Netmask
						inifile.NextToken();
						err = (TInetAddr::Cast(opt().iNetMask)).Input(inifile.iToken);
						break;
					case 2:	// metric
						err = inifile.Val(opt().iMetric);
						break;
					case 3:	// Gateway
						inifile.NextToken();
						if (inifile.iToken.Compare(_L("-")) == 0)
							opt().iGateway.SetFamily(KAFUnspec);
						else
							err = (TInetAddr::Cast(opt().iGateway)).Input(inifile.iToken);
						break;
					case 4: // Interface
						inifile.NextToken();
						if (inifile.iToken.Compare(_L("-")) == 0)
							opt().iIfAddr.SetFamily(KAFUnspec);
						else
							err = (TInetAddr::Cast(opt().iIfAddr)).Input(inifile.iToken);
						break;
					default:
						inifile.NextToken();
						break;
					}
				inifile.SkipSpaceAndMark();
				}
			if (err)
				{
				ShowText(_L("Syntax error on line"));
				ShowText(inifile.Line());
				break;
				}
			else if (CheckResult(inifile.Line(), socket.SetOpt(KSoInetAddRoute, KSolInetRtCtrl, opt)) == KErrNone)
				route_count++;
			}
		else if ((inifile.iToken.Compare(_L("ifconfig")) == 0))
			{
			//
			// For now only simple format
			//
			//	ifconfig interface [address [remote]] [parameters]
			//
			//  perameters can be
			//
			//	prefix n	netmask (ip4)/prefix (ip6) [default 32/128]. This *IS* not same
			//				as Unix ifconfig. Here the value tells whether a "single address"
			//				or netmask/prefix configuration is to be performed.
			//
			//		*NOTE*	prefix splits the bits in the address into two sections: (prefix,id)
			//				if prefix part is non-ZERO, it will be added as if router had sent
			//				RA with prefix option with L=1, A=1
			//				if id part is non-ZERO, it will be processed as interface id (alias
			//				keyword will control whether this is primary or additional).
			//
			//		*NOTE*	prefix 128 => id part is zero length, assumed ZERO
			//				prefix 0 => prefix part is zero length, and treated as ZERO
			//
			//		*NOTE*	If configuring for single address, do not specify prefix!
			//
			//	alias		specify additional network address ([address] required)
			//				'alias' is not present, but [address] is, then the primary
			//				network address is configured.
			//	delete		remove specified network address
			//	down		mark interface down
			//	up			mark interface up
			//	metric n	set metric to n
			//	mtu n		set MTU to n
			//	ns1 address	nameserver 1 address
			//	ns2 address	nameserver 2 address
			//	proxy		speficy address as proxy
			//	anycast		specify address as anycast
			//

			TPckgBuf<TSoInet6InterfaceInfo> opt;
#if 1
			opt().iDoState = 0;
			opt().iDoId = 0;
			opt().iDoPrefix = 0;
			opt().iDoAnycast = 0;
#ifndef HURRICANE_INSOCK
			opt().iDoProxy = 0;
#endif
			opt().iAlias = 0;
			opt().iDelete = 0;
			opt().iAddress.SetFamily(0);
			opt().iDefGate.SetFamily(0);
			opt().iNetMask.SetFamily(0);
			opt().iNameSer1.SetFamily(0);
			opt().iNameSer2.SetFamily(0);
			opt().iMtu = 0;
			opt().iSpeedMetric = 0;
#else
			// unfortunately, this does not work.. "has initialized data" in MARM compile!
			static const TSoInet6InterfaceInfo init_opt;
			opt() = init_opt;
#endif
			TInt prefix = -1;
			for (int i = 0; !err && inifile.iFirst == 0; ++i)
				{
				inifile.NextToken();
				switch (i)
					{
					case 0:	// Interface Name
						opt().iName = inifile.iToken;
						opt().iTag = inifile.iToken;
						break;
					case 1: // Address
						if ((TInetAddr::Cast(opt().iAddress)).Input(inifile.iToken) != KErrNone)
							{
							i = 2;	// We won't have remote address either!
							goto parameters; //lint !e801 // no clean way to do it without goto
							}
						break;
					case 2: // Remote Address
						if ((TInetAddr::Cast(opt().iDefGate)).Input(inifile.iToken) == KErrNone)
							break;
						//lint -fallthrough
					default:
parameters:
						if (opt().iDoState == 0)
							{
							if (inifile.iToken.Compare(_L("down")) == 0)
								{
								opt().iState = EIfDown;
								opt().iDoState = 1;
								break;
								}
							if (inifile.iToken.Compare(_L("up")) == 0)
								{
								opt().iState = EIfUp;
								opt().iDoState = 1;
								break;
								}
							}
						if (opt().iDoAnycast == 0 &&
#ifndef HURRICANE_INSOCK
							opt().iDoProxy == 0 &&
#endif
							prefix < 0)
							{
							// Only one of 'proxy' or 'anycast' can be present. Also, prefix must not
							// be present.
							if (inifile.iToken.Compare(_L("anycast")) == 0)
								{
								opt().iDoAnycast = 1;
								break;
								}
							else if (inifile.iToken.Compare(_L("proxy")) == 0)
								{
#ifndef HURRICANE_INSOCK
								opt().iDoProxy = 1;
#endif
								break;
								}
							}
						if (opt().iNameSer1.Family() == 0 && inifile.ParameterValue(_L("ns1"), opt().iNameSer1) == KErrNone)
							break;
						if (opt().iNameSer2.Family() == 0 && inifile.ParameterValue(_L("ns2"), opt().iNameSer2) == KErrNone)
							break;
						if (opt().iDelete == 0 && inifile.iToken.Compare(_L("delete")) == 0)
							{
							opt().iDelete = 1;
							break;
							}
						if (opt().iAlias == 0 && inifile.iToken.Compare(_L("alias")) == 0)
							{
							opt().iAlias = 1;
							break;
							}
						if (opt().iMtu == 0 && inifile.ParameterValue(_L("mtu"), opt().iMtu) == KErrNone)
							break;
						if (opt().iSpeedMetric == 0 && inifile.ParameterValue(_L("metric"), opt().iSpeedMetric) == KErrNone)
							break;
						if (opt().iDoAnycast == 0 &&
#ifndef HURRICANE_INSOCK
							opt().iDoProxy == 0 &&
#endif
							prefix < 0 &&
							inifile.ParameterValue(_L("prefix"), prefix, 129) == KErrNone)
							{
							// prefix == 129, if value is missing => Error
							if (prefix > 128)
								err = KErrArgument;
							break;
							}
						err = KErrGeneral;
						break;
					}
					inifile.SkipSpaceAndMark();
				}
			if (!err)
				{
				// Munge the prefix information into TSoInet6InterfaceInfo
				if (prefix < 0)
					{
					// No prefix present, iNetMask is left unspecified,
					// request "single address" processing (prefix = 128)
					opt().iDoId = 1;
					opt().iDoPrefix = 1;
					}
				else if (opt().iAddress.Family() == KAfInet && prefix <= 32)
					{
					// IPv4 processing -- defines the netmask
					const TUint32 mask = ~0UL << (32 - prefix);
					const TUint32 addr = opt().iAddress.Address();
					TInetAddr::Cast(opt().iNetMask).SetAddress(mask);
					if (mask & addr)
						opt().iDoPrefix = 1;
					if ((~mask) & addr)
						opt().iDoId = 1;
					}
				else if (opt().iAddress.Family() == KAfInet6 && prefix <= 128)
					{
					// IPv6 processing
					TInetAddr p;
					// Is Prefix part all zeroes?
					p.Prefix(TInetAddr::Cast(opt().iAddress), prefix);
					if (!p.Ip6Address().IsUnspecified())
						opt().iDoPrefix = 1;
					TInetAddr::Cast(opt().iNetMask).PrefixMask(prefix);
					// Is Id part all zeroes?
					p.SetAddress(TInetAddr::Cast(opt().iAddress).Ip6Address());
					const TIp6Addr addr = p.Ip6Address();
					const TIp6Addr mask = TInetAddr::Cast(opt().iNetMask).Ip6Address();
					if ((addr.u.iAddr32[0] & ~mask.u.iAddr32[0]) != 0 ||
						(addr.u.iAddr32[1] & ~mask.u.iAddr32[1]) != 0 ||
						(addr.u.iAddr32[2] & ~mask.u.iAddr32[2]) != 0 ||
						(addr.u.iAddr32[3] & ~mask.u.iAddr32[3]) != 0)
						opt().iDoId = 1;
					}
				else
					{
					// incorrect of prefix value
					err = KErrArgument;
					}
				}
			if (err)
				{
				ShowText(_L("Syntax error on line"));
				ShowText(inifile.Line());
				break;
				}
			else if (CheckResult(inifile.Line(), socket.SetOpt(KSoInetConfigInterface, KSolInetIfCtrl, opt)) == KErrNone)
				if_count++;
			}
#ifndef HURRICANE_INSOCK
		else if ((inifile.iToken.Compare(_L("setscope")) == 0))
			{
			//
			// For now only simple format
			//
			//	setscope interface interface2 level
			//
			//  perameters can be
			//
			//	interface	the name of the interface to be change
			//	interface2	the name of the interface to used as a source for the scope values
			//	scope		the scope of the join point [2..16]

			TPckgBuf<TSoInetIfQuery> opt1;
			TPckgBuf<TSoInetIfQuery> opt2;
			TInt scope = -1;
			TInt i = 0;
			for (i = 0; !err && inifile.iFirst == 0; ++i)
				{
				inifile.NextToken();
				switch (i)
					{
					case 0:	// Interface Name (to modify)
						opt1().iName = inifile.iToken;
						err = CheckResult(inifile.iToken, socket.GetOpt(KSoInetIfQueryByName, KSolInetIfQuery, opt1));
						break;
					case 1: // Interface Name (the source)
						opt2().iName = inifile.iToken;
						err = CheckResult(inifile.iToken, socket.GetOpt(KSoInetIfQueryByName, KSolInetIfQuery, opt2));
						break;
					case 2: // Scope Level
						err = inifile.ParameterValue(_L("level"), scope);
						break;
					default:
						err = KErrArgument;
						break;
					}
				inifile.SkipSpaceAndMark();
				}
			if (err || scope < 2 || scope > 16)
				{
				ShowText(_L("Syntax error on line"));
				ShowText(inifile.Line());
				break;
				}
			//
			// Build a new scope id vector
			//
			scope -= 1; // scope array is indexed from 0
#if 0
			for (i = 0; ++i < scope;)
				opt1().iZone[i] = ~opt1().iZone[0];
			for ( ;scope < STATIC_CAST(TInt, sizeof(opt1().iZone) / sizeof(opt1().iZone[0])); ++scope)
				opt1().iZone[scope] = opt2().iZone[scope];
#else
			opt1().iZone[scope] = opt2().iZone[scope];
#endif
			opt1().iIndex = opt1().iZone[0];
			err = CheckResult(inifile.Line(), socket.SetOpt(KSoInetIfQuerySetScope, KSolInetIfQuery, opt1));
			}
#endif
		}
	delete buf;
	TBuf<80> text;
	text.Format(_L("Added %d routes, configured %d interfaces"), route_count, if_count);
	ShowText(text);
	socket.Close();
	}

void CIpAdmEngine::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
		case EIpAdmInterfaces:
			ListInterfaces(4,_L("udp"));
//			ListInterfaces(6,_L("udp6"));
			break;
		case EIpAdmRoutes:
			ListRoutes(4, _L("udp"));
//			ListRoutes(6, _L("udp6"));
			break;
		case EIpAdmAddRoutes:
			AddRoutes(_L("udp"));
			break;
		default:
			break;
		}
	}


//
// CIpAdmEngine::CheckResult
//	Output success or fail message, returns the error code
//
TInt CIpAdmEngine::CheckResult(const TDesC &aText, TInt aResult)
	{
	if (aResult == KErrNone)
		return KErrNone;

	TBuf<100> err;
	CEikonEnv::Static()->GetErrorText(err, aResult);

	TBuf<200> str(aText);
	str.AppendFormat(_L(" returned with [%s] "), err.PtrZ());
	iAppView->Write(str);

	return aResult;
	}
//
// CIpAdmEngine::CheckResultL
//	Output success or fail message, and Leave if the code is not
//	KErrNone.
//
void CIpAdmEngine::CheckResultL(const TDesC &aText, TInt aResult)
	{
	if (CheckResult(aText, aResult) != KErrNone)
		User::Leave(aResult);
	}

void CIpAdmEngine::ShowText(const TDesC &aText)
	{
	iAppView->Write(aText);
	}

