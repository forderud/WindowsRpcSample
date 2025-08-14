// based on https://learn.microsoft.com/en-us/windows/win32/rpc/the-client-application
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "../RpcServer/hello_h.h"

#pragma comment(lib, "Rpcrt4.lib")


int main() {
    unsigned short* pszOptions = NULL;
    unsigned short* pszStringBinding = NULL;
    RPC_STATUS status = RpcStringBindingComposeW(nullptr, // Uuid
        (unsigned short*)L"ncalrpc",
        nullptr, // NetworkAddress
        (unsigned short*)L"MyRpcServer", // endpoint
        pszOptions,
        &pszStringBinding);
    if (status)
        exit(status);

    status = RpcBindingFromStringBindingW(pszStringBinding, &hello_IfHandle);
    if (status)
        exit(status);

    {
        // call RPC functions
        HelloProc(L"hello, world");

        Shutdown();
    }

    status = RpcStringFreeW(&pszStringBinding);
    if (status)
        exit(status);

    status = RpcBindingFree(&hello_IfHandle);
    if (status)
        exit(status);

    exit(0);
}


void* midl_user_allocate(size_t len) {
    return malloc(len);
}

void midl_user_free(void __RPC_FAR* ptr) {
    free(ptr);
}
