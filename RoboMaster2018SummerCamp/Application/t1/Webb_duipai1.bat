@echo off  
:loop  
    data_rand.exe %random% > data.in
    std.exe < data.in > std.out
    Webb_t1.exe < data.in > Webb_t1.out
    fc Webb_t1.out std.out 
if not errorlevel 1 goto loop  
pause
goto loop