# Music Player
[VB Script Plugin](https://www.hex-rays.com/products/ida/support/freefiles/vb.idc "IDA Plugin")  
[Reversing Reference For VBScript](https://blog.talosintelligence.com/2014/08/discovering-dynamically-loaded-api-in.html)

winmm.dll is a module for the Windows Multimedia API, which contains low-level audio and joystick functions. 
comdlg32.dll is a module that contains Common dialog boxes used by Windows applications, such as the standard Open File form. 

[VB Decompiler lite](https://reverseengineering.stackexchange.com/questions/1597/reverse-engineering-a-visual-basic-p-code-binary)  
VB Decompiler give good understanding of how each functionality of Forms, After Understanding few of functionality I am able to findout `Code -> FrmMain -> Proc_0_10_403370` which does What we are looking for

|![Reference Image](https://github.com/loggervicky69/ctf/blob/main/image/VB_Decompiler0.png)|
|:--:|
|**Decompiled Program**|

```asm
  loc_00403515: mov eax, arg_C
  loc_00403518: mov ecx, 0000003Ch
  loc_0040351D: cdq
  loc_0040351E: idiv ecx
  loc_00403520: push 00000001h
  loc_00403522: push 00000001h
  loc_00403524: lea eax, var_6C
  loc_00403527: lea ecx, var_8C
  loc_0040352D: mov var_6C, 00000003h
  loc_00403534: mov var_64, edx  ; calculating modulo of current time with 60 
  loc_00403537: lea edx, var_7C
  loc_0040353A: push edx
  loc_0040353B: push eax
  loc_0040353C: push ecx
  loc_0040353D: call [00401030h] ; rtcVarFromFormatVar
```


