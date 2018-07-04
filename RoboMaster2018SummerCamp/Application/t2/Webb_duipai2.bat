@echo off  
:loop  
    data_rand.exe %random% > data.in
    std.exe < data.in > std.out
    Webb_t2.exe < data.in > Webb_t2.out
    fc Webb_t2.out std.out 
if not errorlevel 1 goto loop  
pause
goto loop