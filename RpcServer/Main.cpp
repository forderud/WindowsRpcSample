#include <stdio.h>
#include <windows.h>
#include "MyRpc_h.h"

#pragma comment(lib, "Rpcrt4.lib")

/** Get client process ID (PID) during processing of incoming RPC call.
    Only supported for the "ncalrpc" local RPC protocol. */
static size_t GetClientProcessID(RPC_BINDING_HANDLE binding = nullptr) {
    RPC_CALL_ATTRIBUTES attribs{};
    attribs.Version = RPC_CALL_ATTRIBUTES_VERSION; // 3
    attribs.Flags = RPC_QUERY_CLIENT_PID;
    RPC_STATUS status = RpcServerInqCallAttributesW(binding, &attribs);
    if (status != RPC_S_OK)
        exit(status);

    return (size_t)attribs.ClientPID;
}


int main() {
    {
        auto* ifc = (RPC_SERVER_INTERFACE*)MyRpc_v1_0_s_ifspec;
        RPC_STATUS status = RpcServerUseProtseqEpA(
            ifc->RpcProtseqEndpoint->RpcProtocolSequence,
            RPC_C_LISTEN_MAX_CALLS_DEFAULT,
            ifc->RpcProtseqEndpoint->Endpoint,
            nullptr); // security
        if (status)
            exit(status);
    }

    RPC_STATUS status = RpcServerRegisterIf(MyRpc_v1_0_s_ifspec, NULL, NULL);
    if (status)
        exit(status);

    // listen to clients (blocking call)
    wprintf(L"Listening to client calls..\n");
    unsigned int cMinCalls = 1;
    unsigned int fDontWait = FALSE;
    status = RpcServerListen(cMinCalls, RPC_C_LISTEN_MAX_CALLS_DEFAULT, fDontWait);
    if (status) {
        wprintf(L"RpcServerListen failed with err=%u\n", status);
        exit(status);
    }

    wprintf(L"[done]\n");
    return 0;
}


/* RPC interface function */
void PrintMessage(handle_t handle, /*string*/const wchar_t* msg) {
    auto pid = GetClientProcessID(handle);
    wprintf(L"Client process PID: %zu\n", pid);

    wprintf(L"Message: %s\n", msg);
}

int ComputeSum(handle_t /*handle*/, int a, int b) {
    wprintf(L"Computing the sum of %i and %i...\n", a, b);
    return a + b;
}

/* RPC interface function */
void RequestShutdown(handle_t /*handle*/) {
    wprintf(L"Receiving shutdown request.\n");

    RPC_STATUS status = RpcMgmtStopServerListening(NULL);
    if (status)
        exit(status);

    status = RpcServerUnregisterIf(NULL, NULL, FALSE);
    if (status)
        exit(status);
}

void* midl_user_allocate(size_t len) {
    return malloc(len);
}

void midl_user_free(void __RPC_FAR* ptr) {
    free(ptr);
}
