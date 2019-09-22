# CopyShell
+ ATL Shell Extension for File Context Menu

### Environment
+ `MSVC 2010`
+ `Windows 10 1803 x64`

### Register
```bash
# Build x64 Register platform

cd x64/Register/
regsvr32 CopyShell.dll
# Use RegMenu.reg

# Unregister
regsvr32 /u CopyShell.dll
# Use UnregMenu.reg
```

### Tips
+ Tutorial see [tutorial.md](https://github.com/Aoi-hosizora/CopyShell/blob/master/tutorial.md)
+ Main code see [CopyMenu.h](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyMenu.h) and [CopyMenu.cpp](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyMenu.cpp)
+ CLSID setting see [CopyShell.idl](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyShell.idl) and [CopyMenu.rgs](https://github.com/Aoi-hosizora/CopyShell/blob/master/CopyMenu.rgs)
+ Use `wstring` and `LPWSTR` rather than `string` and `LPSTR` to support other code page
+ `lnk` file `QueryContextMenu` should register twice (`*` and `.lnk`) to filter the exact paths

### Screenshot
![Screenshot1](./assets/Screenshot1.jpg)

### References
+ [Windows Shell编程实现右键菜单](https://blog.csdn.net/zaishaoyi/article/details/38475553)
+ [ショットカットメニュー ハンドラ](http://eternalwindows.jp/shell/shellex/shellex08.html)
+ [クリップボード](http://wisdom.sakura.ne.jp/system/winapi/win32/win90.html)
+ [Working with Strings](https://docs.microsoft.com/en-us/windows/win32/learnwin32/working-with-strings)
+ [Windows Shell Extension doesn't give exact file paths](https://stackoverflow.com/questions/21848694/windows-shell-extension-doesnt-give-exact-file-paths)