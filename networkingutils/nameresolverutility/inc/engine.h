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
// pingmodel.h - icmp echo client engine
//



/**
 @internalComponent
*/
#ifndef __ENGINE_H
#define __ENGINE_H

#include <es_sock.h>
#include <in_sock.h>

//#include <nifmbuf.h>

const TInt KHostNameLimit = 80;

class CNslookupContainer;

const TUint KPingQuiet			= 0x01;
const TUint KPingVerbose		= 0x02;
const TUint KPingPackLimit		= 0x04;
const TUint KPingDebug			= 0x08;

//Contains engine's saved preferences
class TPreferences
	{
public:
	TBuf<80> iHostname;
	};


class CNslookup : public CActive
	{
public:
	CNslookup();
	~CNslookup();
	
	void ConstructL(const TPreferences& aPref);

	//return the current preferences
	void GetPreferences(TPreferences &aPref) const;
	static void DefaultPreferences(TPreferences &aPref);
	void SetConsole(CNslookupContainer *aConsole);
	const TDesC* GetHostName() const;
	void SetHostName(const TDes& ahostname);
	
	void EndNslookup();
	TBool IsRunning() const;
	void BeginL();

protected:
	void RunL();    //Segment of code to execute
	void DoCancel();        //Stops Execution of Ping

private:
	void DisableRemoteLink();
	void Error(const TDesC& string, TInt error);
	void Stop();

	/**
	* Outputs some text about the Resolver Query() response to the console
	*/
	void QueryResponse();

private:
	
    CNslookupContainer *iConsole;
	RSocketServ iSockServ;			// Socket Server
	RHostResolver iHostResolv;		// Resolver
	TBuf<KHostNameLimit> iHostname;	// Contains name to resolve
	TInetAddr iAddress;				// Address to reverse
	TInt iCount;					// Successful resolves.
	TNameEntry iEntry;	//contains address resolution data
	TBuf8<1024> iResult;
	
public:
	TInetAddr iHostAddr;		//Address to Ping
	TUint		iQueryType;		//< Query type select from the dialog choice list.
	TBool		iShowSource;	//< Show source address for each resolved address.
	};

#endif
