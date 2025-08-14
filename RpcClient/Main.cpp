// based on https://learn.microsoft.com/en-us/windows/win32/rpc/the-client-application
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "../RpcServer/hello_h.h"

#pragma comment(lib, "Rpcrt4.lib")


int main() {
    unsigned char* pszOptions = NULL;
    unsigned char* pszStringBinding = NULL;

    RPC_STATUS status = RpcStringBindingComposeA(nullptr, // Uuid
        (unsigned char*)"ncacn_np",
        nullptr, // NetworkAddress
        (unsigned char*)"\\pipe\\hello",
        pszOptions,
        &pszStringBinding);
    if (status) exit(status);


    RPC_BINDING_HANDLE hello_ClientIfHandle = nullptr;
    status = RpcBindingFromStringBindingA(pszStringBinding, &hello_ClientIfHandle);
    if (status) exit(status);

    RpcTryExcept
    {
        unsigned char* pszString = (unsigned char*)"hello, world";
        HelloProc(pszString);
        Shutdown();
    }
        RpcExcept(1)
    {
        unsigned long ulCode = RpcExceptionCode();
        printf("Runtime reported exception 0x%lx = %ld\n", ulCode, ulCode);
    }
    RpcEndExcept

    status = RpcBindingFree(&hello_ClientIfHandle);
    if (status) exit(status);

    status = RpcStringFreeA(&pszStringBinding);
    if (status) exit(status);

    status = RpcBindingFree(&hello_IfHandle);
    if (status) exit(status);

    exit(0);
}

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len) {
    return malloc(len);
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr) {
    free(ptr);
}
