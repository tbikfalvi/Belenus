@echo off

set currdate=%date%
set currtime=%time%

set currdate=%currdate:.=%
set currdate=%currdate:/=%
set currdate=%currdate:-=%
set currdate=%currdate: =%

set currtime=%currtime::=%
set currtime=%currtime:.=%
set currtime=%currtime:,=%
set currtime=%currtime: =%

cmd /c process.bat > "%BelenusStartup%/update_%currdate%_%currtime%.log"

