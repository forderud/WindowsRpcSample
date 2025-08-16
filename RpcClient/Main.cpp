/* file: helloc.c */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "../RpcServer/hello_h.h" 
#include <windows.h>

#pragma comment(lib, "Rpcrt4.lib")


int main()
{
    RPC_STATUS status;
    unsigned char* pszUuid = NULL;
    unsigned char* pszProtocolSequence = (unsigned char*)"ncacn_np";
    unsigned char* pszNetworkAddress = NULL;
    unsigned char* pszEndpoint = (unsigned char*)"\\pipe\\hello";
    unsigned char* pszOptions = NULL;
    unsigned char* pszStringBinding = NULL;
    unsigned char* pszString = (unsigned char*)"hello, world";
    unsigned long ulCode;

    status = RpcStringBindingComposeA(pszUuid,
        pszProtocolSequence,
        pszNetworkAddress,
        pszEndpoint,
        pszOptions,
        &pszStringBinding);
    if (status) exit(status);

    status = RpcBindingFromStringBindingA(pszStringBinding, &hello_IfHandle);

    if (status) exit(status);

    RpcTryExcept
    {
        HelloProc(pszString);
        Shutdown();
    }
        RpcExcept(1)
    {
        ulCode = RpcExceptionCode();
        printf("Runtime reported exception 0x%lx = %ld\n", ulCode, ulCode);
    }
    RpcEndExcept

        status = RpcStringFreeA(&pszStringBinding);

    if (status) exit(status);

    status = RpcBindingFree(&hello_IfHandle);

    if (status) exit(status);

    exit(0);
}

/******************************************************/
/*         MIDL allocate and free                     */
/******************************************************/

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr)
{
    free(ptr);
}