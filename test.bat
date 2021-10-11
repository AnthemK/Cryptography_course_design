@echo off
set /a s1=%time:~6,2%
set /a m1=%time:~9,2%
if not "%3"=="" (
 set s=%2
 set e=%3
) else if not "%2"=="" (
 set s=%2
 set e=%2
) else (
 set s=1
 set e=10
)   
for /L %%a in (%s%,1,%e%) do (
  if exist %%a.in (
	%1 <%%a.in > a.out
	fc a.out %%a.out
  )
)
set /a s2=%time:~6,2%
set /a m2=%time:~9,2%

set /a diff=( %s2% - %s1% )*1000 + %m2% - %m1%
echo time:%diff%ms


