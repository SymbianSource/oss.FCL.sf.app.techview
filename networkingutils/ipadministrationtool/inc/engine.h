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
// engine.h - IP administration tool engine
//



/**
 @internalComponent
*/

#ifndef __ENGINE_H
#define __ENGINE_H

#include <es_sock.h>
#include <in_sock.h>
#include <coemain.h>
#include <coecntrl.h>
#include <s32file.h>

class CIpAdmView;

class CIpAdmEngine : public CBase
	{
public:
	CIpAdmEngine(CIpAdmView *aView) : iAppView(aView) {}
	~CIpAdmEngine();
	void ConstructL();
	void HandleCommandL(TInt aCommand);
private:
	CIpAdmView *iAppView;
	void Show(TInt aVersion, const TSoInetInterfaceInfo &aInfo);
	void Show(TInt aVersion, const TSoInetRouteInfo &aInfo);
	void ListInterfaces(TInt aVersion, const TDesC &aProtocol);
	void ListRoutes(TInt aVersion, const TDesC &aProtocol);
	void AddRoutes(const TDesC &aProtocol);
public:
	RFs iFS;
	RSocketServ iSS;
	void CheckResultL(const TDesC &aText, TInt aResult);
	TInt CheckResult(const TDesC &aText, TInt aResult);
	void ShowText(const TDesC &aText);
	};

#endif
