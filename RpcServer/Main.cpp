// based on https://learn.microsoft.com/en-us/windows/win32/rpc/the-server-application
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "hello_h.h"

#pragma comment(lib, "Rpcrt4.lib")


int main() {
    RPC_STATUS status = RpcServerUseProtseqEpA((unsigned char*)"ncacn_np",
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,
        (unsigned char*)"\\pipe\\hello",
        nullptr); // Security
    if (status) exit(status);

    unsigned int    cMinCalls = 1;
    unsigned int    fDontWait = FALSE;
    status = RpcServerRegisterIf(hello_IfHandle, // TODO: hello_ServerIfHandle in doc
        NULL,
        NULL);
    if (status) exit(status);

    status = RpcServerListen(cMinCalls,
        RPC_C_LISTEN_MAX_CALLS_DEFAULT,
        fDontWait);
    if (status) exit(status);
}

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len) {
    return malloc(len);
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr) {
    free(ptr);
}
