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

    wchar_t* uuid = nullptr;
    wchar_t* protSeq = nullptr;
    wchar_t* netAddr = nullptr;
    wchar_t* endpoint = nullptr;
    wchar_t* options = nullptr;
    status = RpcStringBindingParseW(strBinding, (unsigned short**)&uuid, (unsigned short**)&protSeq, (unsigned short**)&netAddr, (unsigned short**)&endpoint, (unsigned short**)&options);
    assert(!status);

    wprintf(L"Server protocol: %s, endpoint: %s\n", protSeq, endpoint);

    RpcStringFreeW((unsigned short**)&uuid);
    RpcStringFreeW((unsigned short**)&protSeq);
    RpcStringFreeW((unsigned short**)&netAddr);
    RpcStringFreeW((unsigned short**)&endpoint);
    RpcStringFreeW((unsigned short**)&options);

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
