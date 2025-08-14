#include <cassert>
#include <stdio.h>
#include <windows.h>
#include "../RpcServer/MyRpc_h.h"

#pragma comment(lib, "Rpcrt4.lib")

/** WIP code for trying to get the process ID from a RPC binding. */
static void InspectServerBinding(RPC_BINDING_HANDLE binding) {
    RPC_WSTR strBinding = nullptr;
    RPC_STATUS status = RpcBindingToStringBindingW(binding, &strBinding);
    assert(!status);

    RPC_WSTR uuid = nullptr;
    RPC_WSTR protSeq = nullptr;
    RPC_WSTR netAddr = nullptr;
    RPC_WSTR endpoint = nullptr;
    RPC_WSTR options = nullptr;
    status = RpcStringBindingParseW(strBinding, &uuid, &protSeq, &netAddr, &endpoint, &options);
    assert(!status);

    wprintf(L"Server protocol: %s, endpoint: %s\n", (wchar_t*)protSeq, (wchar_t*)endpoint);

    RpcStringFreeW(&uuid);
    RpcStringFreeW(&protSeq);
    RpcStringFreeW(&netAddr);
    RpcStringFreeW(&endpoint);
    RpcStringFreeW(&options);

    RpcStringFreeW(&strBinding);
}

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

    InspectServerBinding(serverHandle);

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
    return 0;
}


void* midl_user_allocate(size_t len) {
    return malloc(len);
}

void midl_user_free(void __RPC_FAR* ptr) {
    free(ptr);
}
