@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem
REM Run auto smoke tests on hardware


REM create testresults directory
md c:\testresults
md e:\testresults
 

testexecute z:\smoketest\smoketest_system.script
move c:\logs\testexecute\smoketest_system.htm e:\testresults\smoketest_system.htm

testexecute z:\smoketest\setup_smoketest_agenda.script
move c:\logs\testexecute\setup_smoketest_agenda.htm e:\testresults\setup_smoketest_agenda.htm

testexecute z:\smoketest\smoketest_agenda.script
move c:\logs\testexecute\smoketest_agenda.htm e:\testresults\smoketest_agenda.htm

testexecute z:\smoketest\setup_smoketest_contacts.script
move c:\logs\testexecute\setup_smoketest_contacts.htm e:\testresults\setup_smoketest_contacts.htm

testexecute z:\smoketest\smoketest_contacts.script
move c:\logs\testexecute\smoketest_contacts.htm e:\testresults\smoketest_contacts.htm

testexecute z:\smoketest\setup_smoketest_mess.script
move c:\logs\testexecute\setup_smoketest_mess.htm e:\testresults\setup_smoketest_mess.htm

testexecute z:\smoketest\smoketest_mess.script
move c:\logs\testexecute\smoketest_mess.htm e:\testresults\smoketest_mess.htm

testexecute z:\smoketest\setup_smoketest_timew.script
move c:\logs\testexecute\setup_smoketest_timew.htm e:\testresults\setup_smoketest_timew.htm

testexecute z:\smoketest\smoketest_timew.script
move c:\logs\testexecute\smoketest_timew.htm e:\testresults\smoketest_timew.htm

testexecute z:\smoketest\smoketest_phone.script
move c:\logs\testexecute\smoketest_phone.htm e:\testresults\smoketest_phone.htm

testexecute z:\smoketest\smoketest_syncmlapp.script
move c:\logs\testexecute\smoketest_syncmlapp.htm e:\testresults\smoketest_syncmlapp.htm

 