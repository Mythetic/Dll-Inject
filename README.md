# Dll-Inject
借助win32远程线程实现DLL的注射，可完成DLL注射工具的开发。
具体过程如下：
1.获取目标进程的句柄，借助OpenProcess函数
2.计算DLL路径的长度，考虑结尾\0要叫1
3. 在目标进程分配内存，使用VirtualAllocEx（此函数可以在当前进程来为别的进程分配内存，可以说是Windows的不安全的体现，可以借助此函数完成很多猥琐的操作）
   lpAllocAddr = VirtualAllocEx(hProcess,NULL,dwLength, MEM_COMMIT, PAGE_READWRITE);
4. 拷贝DLL路径到目标进程的内存，使用WriteProcessMemory（此函数可以向进程内存进行写操作）
   bRet = WriteProcessMemory(hProcess, lpAllocAddr, szDllPathName, dwLength, NULL);
5.获取（KERNEL32.dll）模块地址。
   win32的进程都会包含KERNEL32.dll和ntdll。dll，本注射方法就借助了这个原理。LoadLibrary函数就在KERNEL32.dll模块中
6.获取LoadLibrary函数地址
   dwLoadAddr = (DWORD)GetProcAddress(hModule, "LoadLibraryA");
7. 创建远程线程，加载dll
   hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)dwLoadAddr, lpAllocAddr, 0, NULL);
   
   这样就可以实现DLL文件的注射。
