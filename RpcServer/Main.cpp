// based on https://learn.microsoft.com/en-us/windows/win32/rpc/the-server-application
#include <stdio.h>
#include <windows.h>
#include "hello_h.h"

#pragma comment(lib, "Rpcrt4.lib")


int main() {
    {
        RPC_SERVER_INTERFACE* ifc = (RPC_SERVER_INTERFACE*)hello_v1_0_s_ifspec;
        RPC_STATUS status = RpcServerUseProtseqEpA(
            ifc->RpcProtseqEndpoint->RpcProtocolSequence,
            RPC_C_LISTEN_MAX_CALLS_DEFAULT,
            ifc->RpcProtseqEndpoint->Endpoint,
            nullptr); // Security
        if (status)
            exit(status);
    }

    RPC_STATUS status = RpcServerRegisterIf(hello_v1_0_s_ifspec, NULL, NULL);
    if (status)
        exit(status);

    // listen to clients (blocking call)
    unsigned int cMinCalls = 1;
    unsigned int fDontWait = FALSE;
    status = RpcServerListen(cMinCalls, RPC_C_LISTEN_MAX_CALLS_DEFAULT, fDontWait);
    if (status)
        exit(status);
}


/* RPC interface function */
void HelloProc(/*string*/ const wchar_t* msg) {
    wprintf(L"Received message: %s\n", msg);
}

/* RPC interface function */
void Shutdown() {
    wprintf(L"Receiving shutdown request.\n");

    RPC_STATUS status;
    status = RpcMgmtStopServerListening(NULL);
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
