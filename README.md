# WsaStrerr
## 简介
这是一个DLL, 里面提供下面的接口, 用于获取WSA(Windows Sockets Error)错误码的描述信息, 类似于linux系统errno.h的strerr(errno)  
对于Windows下的socket编程很有用
```c++
const char* WSAStrError(unsigned long ulErrno)
```
## 用法
```c++
typedef const char* (*WSAStrErrFunc)(unsigned long ulErrno);

int main(int argc, char* argv[])
{
    /* 加载dll */
    WSAStrErrFunc pfWsaStrErr = nullptr;
    HMODULE  hDllHandle = LoadLibrary(L"wsaerror.dll");
    if (hDllHandle == nullptr)
    {
        printf("wsaerror.dll load failed!\n");
        return -1;
    }
    else
    {
        pfWsaStrErr = (WSAStrErrFunc)GetProcAddress(hDllHandle, "WSAStrError");
    }
    
    WSAData stWSAData{};
    if (0 != WSAStartup(MAKEWORD(2, 2), &stWSAData))
    {
        unsigned long ulWsaErrno = WSAGetLastError();
        printf("WSAStartup failed! %s(%ul)\n", pfWsaStrErr(ulWsaErrno));
    }
	return 0;
}
```
