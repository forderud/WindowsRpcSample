#include <stdio.h>
#include <windows.h>
#include "../RpcServer/MyRpc_h.h"

#pragma comment(lib, "Rpcrt4.lib")


int main() {
    RPC_BINDING_HANDLE serverHandle = nullptr;
    {
        unsigned char* stringBinding = nullptr;
        auto* ifc = (RPC_SERVER_INTERFACE*)MyRpc_v1_0_c_ifspec;
        RPC_STATUS status = RpcStringBindingComposeA(nullptr, // uuid
            ifc->RpcProtseqEndpoint->RpcProtocolSequence,
            nullptr, // net addr.
            ifc->RpcProtseqEndpoint->Endpoint,
            nullptr, // options
            &stringBinding);
        if (status)
            exit(status);

        // create server handle (doesn't actually connect)
        status = RpcBindingFromStringBindingA(stringBinding, &serverHandle);
        if (status)
            exit(status);

        // clean up string
        status = RpcStringFreeA(&stringBinding);
        if (status)
            exit(status);
        stringBinding = nullptr;
    }

    wprintf(L"Calling RPC function...\n");
    PrintMessage(serverHandle, L"Hi, there!");

    auto sum = ComputeSum(serverHandle, 7, 8);
    wprintf(L"7 + 8 = %i\n", sum);

    wprintf(L"Requesting server shutdown...\n");
    RequestShutdown(serverHandle);

    RPC_STATUS status = RpcBindingFree(&serverHandle);
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
