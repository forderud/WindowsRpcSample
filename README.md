Sample Windows RPC code. Demonstrates how to perform Remote Procedure Calls (RPC) to call functions across process boundaries.


Using the [`ncalrpc`](https://learn.microsoft.com/en-us/windows/win32/midl/ncalrpc) (ALPC) transport, which is the same as COM uses for inter-process calls.

[Process Explorer](https://learn.microsoft.com/en-us/sysinternals/downloads/process-explorer) can be used to inspect the RPC ports exposed by the server:  
<img width="786" height="288" alt="image" src="https://github.com/user-attachments/assets/49abed8f-81eb-4a95-968f-ab8f7cde5f56" />


## Acknowledgement
Based on the Microsoft [RPC Tutorial](https://learn.microsoft.com/en-us/windows/win32/rpc/tutorial) with MicrosoftDocs [#2096](https://github.com/MicrosoftDocs/win32/pull/2096) and [#2097](https://github.com/MicrosoftDocs/win32/pull/2097), [#2098](https://github.com/MicrosoftDocs/win32/pull/2098), [#2099](https://github.com/MicrosoftDocs/win32/pull/2099), [#2100](https://github.com/MicrosoftDocs/win32/pull/2100) fixes.
