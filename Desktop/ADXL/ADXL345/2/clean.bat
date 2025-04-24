@echo off  
echo 正在清理 KEIL 工程临时文件...  
del /f /s /q *.axf  
del /f /s /q *.hex  
del /f /s /q *.o  
del /f /s /q *.d  
del /f /s /q *.crf  
del /f /s /q *.htm  
del /f /s /q *.dep  
del /f /s /q *.map  
del /f /s /q *.bak  
rd /s /q Objects  
rd /s /q Listings  
rd /s /q OBJ （根据图中目录结构，增加对 `OBJ` 文件夹的清理）  
echo 清理完成！  
pause  