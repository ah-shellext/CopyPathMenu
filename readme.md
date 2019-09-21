# CopyShell
+ ATL Shell Extension for File Context Menu

### Environment
+ `MSVC 2010`
+ `Windows 10 1803`

### Tips
+ Tutorial see [tutorial.md](https://github.com/Aoi-hosizora/CopyShell/blob/master/tutorial.md)
+ Main code see [CopyMenu.h](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyMenu.h) and [CopyMenu.cpp](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyMenu.cpp)
+ CLSID setting see [CopyShell.idl](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyShell.idl) and [CopyMenu.rgs](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyMenu.rgs)

### Problem
+ `Shift-JIS` code page could not support Chinese character

### Screenshot
![Screenshot1](./assets/Screenshot1.jpg)

### References
+ [Windows Shell编程实现右键菜单](https://blog.csdn.net/zaishaoyi/article/details/38475553)
+ [ショットカットメニュー ハンドラ](http://eternalwindows.jp/shell/shellex/shellex08.html)
+ [クリップボード](http://wisdom.sakura.ne.jp/system/winapi/win32/win90.html)