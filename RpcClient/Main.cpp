// based on https://learn.microsoft.com/en-us/windows/win32/rpc/the-client-application
#include <stdio.h>
#include <windows.h>
#include "../RpcServer/MyRpc_h.h"

#pragma comment(lib, "Rpcrt4.lib")


int main() {
    unsigned char* stringBinding = nullptr;
    {
        RPC_SERVER_INTERFACE* ifc = (RPC_SERVER_INTERFACE*)MyRpc_v1_0_c_ifspec;
        RPC_STATUS status = RpcStringBindingComposeA(nullptr, // Uuid
            ifc->RpcProtseqEndpoint->RpcProtocolSequence,
            nullptr, // NetworkAddress
            ifc->RpcProtseqEndpoint->Endpoint,
            nullptr, // options
            &stringBinding);
        if (status)
            exit(status);
    }

    RPC_BINDING_HANDLE serverHandle = nullptr;
    RPC_STATUS status = RpcBindingFromStringBindingA(stringBinding, &serverHandle);
    if (status)
        exit(status);

    {
        // clean up string
        status = RpcStringFreeA(&stringBinding);
        if (status)
            exit(status);
        stringBinding = nullptr;
    }

    wprintf(L"Calling RPC function...\n");
    PrintMessage(serverHandle, L"Hi, there!");

    wprintf(L"Requesting server shutdown...\n");
    Shutdown(serverHandle);

    status = RpcBindingFree(&serverHandle);
    if (status)
        exit(status);

    wprintf(L"[done]\n");
    exit(0);
}


void* midl_user_allocate(size_t len) {
    return malloc(len);
}

void midl_user_free(void __RPC_FAR* ptr) {
    free(ptr);
}
