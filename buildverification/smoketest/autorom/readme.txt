24/11/2008 - Temporary AutoRom solution
========================================

Code
=====
The code in this directory is temporary code which enables smoke testing while a fully automated replacement for testdriver is being implemented.

The code changes made are:

Additon of directory common\testtools\smoketest\autorom containing the files:
build_smoketest_code.mbc, smoketests.oby, smoketests.bat, dummytest.txt, autoexec.bat, smoketest_romspec_9.5.xml, smoketest_romspec_9.5.xml and smoketest_romspec_future.xml

The autorom building is invoked by changes to 
//EPOC/master/cedar/generic/utils/Symbian_OS_v9.5.xml
//EPOC/master/cedar/generic/utils/Symbian_OS_v9.6.xml
//EPOC/master/cedar/generic/utils/Symbian_OS_vFuture.xml

( <!--Build Smoke test auto roms (temp) -->
<Execute Component="Build code for auto roms (Temp)" Cwd="%BuildDir%\src\common\testtools\smoketest\autorom\" CommandLine="metabld build_smoketest_code.mbc > %LogsDir%\smoketest_autorom_build.log"/>
<Execute Component="Build Smoketest Auto Roms (Temp)" Cwd="%BuildDir%\src\cedar\generic\base\e32\rombuild\" CommandLine="perl %SourceDir%\cedar\generic\tools\romkit\utils\metarombuild.pl -romspec %SourceDir%\common\testtools\smoketest\autorom\smoketest_romspec_9.6.xml -logdir %LogsDir% -buildnum %BuildNumber%_smoketest_autorom_temp"/>)


Location of Roms
================
The resulting roms can be found in :
src\cedar\generic\base\e32\rombuild

H4 Core : h4hrp_smoketest_autorom_temp.img
H4 Rofs : h4hrp_smoketest_autorom_temp.rofs.img
H6 Core : h6_smoketest_autorom_temp.img
H6 Rofs : h6_smoketest_autorom_temp.rofs.img
Naviengine :  NE1S_smoketest_autorom_temp.img


Location of build logs
==========================
The log for building the smoketest code is located in:
logs\cedar\smoketest_autorom_build.log

The rom building logs can be found in :
logs\cedar\techviewroms%buildnum%_smoketest_autorom_temp.log

Executing the smoke tests
==========================
Put the roms onto an MMC card in the normal way. Tests will execute automatically and write results to the MMC card (E:\testresults)
Retrieve the results from the MMC card.
Check that all tests have passed by checking each html results file in testresults.
Update the ESR with the results.

