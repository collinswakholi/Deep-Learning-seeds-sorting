//-----------------------------------------------------------------------------------------
//File Name:UniDAQ.h
//Update date:2014/03/18 by Dan Huang
//Support Language: Visual C++/Visual C.NET/Borland C++ Builder
//UniDAQ SDK User Manual: http://ftp.icpdas.com/pub/cd/iocard/pci/napdos/pci/unidaq/manual/
//-----------------------------------------------------------------------------------------
#include "Windows.h"

#ifndef EXPORTS
#ifdef  __cplusplus
	#define EXPORTS extern "C" __declspec (dllimport)
#else
	#define EXPORTS __declspec (dllimport)
#endif 
#endif

#ifndef __Uni_DAQ_HEADER__
#define __Uni_DAQ_HEADER__

#define MAX_BOARD_NUMBER 32
#define MAX_EVENT_NUMBER 20
#define MAX_AO_CHANNEL   32

//Option Mode
#define MULTIPROCESSOR 1
#define ENABLEMEMIO    2
#define ENABLEDMAAI   16

//ModelNumber
#define PIOD56		0x800140
#define PEXD56		0x800140
#define PIOD48		0x800130
#define PEXD48		0x800130
#define PIOD64		0x800120
#define PIOD96		0x800110
#define PIOD144		0x800100
#define PIOD168		0x800150
#define PIODA		0x800400
#define PEXDA		0x800400
#define PIO821		0x800310

#define PISOP16R16U 0x1800FF
#define PEXP16R16	0x1800FF
#define PEXP8R8	    0x1800FF
#define PISOC64		0x800800
#define PEXC64		0x800800
#define PISOP64		0x800810
#define PEXP64		0x800810
#define PISOA64		0x800850
#define PISO1730	0x800820
#define PISOP32C32	0x800820
#define PEXP32C32	0x800820
#define PISOP32A32	0x800870
#define PISOP8R8	0x800830
#define PISO730		0x800840
#define PISO730A	0x800880
#define PISO725		0x8008FF
#define PISODA2		0x800B00

#define PISO813		0x800A00

#define PCITMC12	0xDF2962
#define PCIM1024	0xDEA074
#define PCIM512		0xDE9562
#define PCIM256		0xDE92A6
#define PCIM128		0xDE9178
#define PCIFC16		0xB13017 
#define PCID64		0x2C90B8
#define PCI822		0xDE3823
#define	PCI826		0xDE3827
#define PCI827		0xDE3828
#define PCI2602     0x2CB656 

#define PCI100x		0x341002
#define PEX100x		0x341002
#define PCI1202		0x345672
#define PEX1202		0x345672
#define PCI1602		0x345676
#define PCI180x		0x345678
#define PCIP8R8		0xD6102B
#define PEXP8POR8	0xD6102B
#define PCIP16R16	0xD61E39
#define PEXP16POR16	0xD61E39


#define MAX_CFGCODE_NUMBER 24//User definition the total number of the AI ConfigCodeTotal

//User Config Code
#define IXUD_BI_20V		23	//Bipolar +/- 20V
#define IXUD_BI_10V		 0	//Bipolar +/- 10V
#define IXUD_BI_5V		 1	//Bipolar +/-  5V
#define IXUD_BI_2V5		 2	//Bipolar +/-  2.5V
#define IXUD_BI_1V25	 3	//Bipolar +/-  1.25V
#define IXUD_BI_0V625	 4	//Bipolar +/-  0.625V
#define IXUD_BI_0V3125	 5	//Bipolar +/-  0.3125V
#define IXUD_BI_0V5		 6	//Bipolar +/-  0.5V
#define IXUD_BI_0V05	 7	//Bipolar +/-  0.05V
#define IXUD_BI_0V005	 8	//Bipolar +/-  0.005V
#define IXUD_BI_1V		 9	//Bipolar +/-  1V
#define IXUD_BI_0V1		10	//Bipolar +/-  0.1V
#define IXUD_BI_0V01	11	//Bipolar +/-  0.01V
#define IXUD_BI_0V001	12	//Bipolar +/-  0.001V
#define IXUD_UNI_20V	13	//Unipolar 0 ~ 20V
#define IXUD_UNI_10V	14	//Unipolar 0 ~ 10V
#define IXUD_UNI_5V		15	//Unipolar 0 ~  5V
#define IXUD_UNI_2V5	16	//Unipolar 0 ~  2.5V
#define IXUD_UNI_1V25	17	//Unipolar 0 ~  1.25V
#define IXUD_UNI_0V625	18	//Unipolar 0 ~  0.625V
#define IXUD_UNI_1V		19	//Unipolar 0 ~  1V
#define IXUD_UNI_0V1	20	//Unipolar 0 ~  0.1V
#define IXUD_UNI_0V01	21	//Unipolar 0 ~  0.01V
#define IXUD_UNI_0V001	22	//Unipolar 0 ~  0.001V


#define MAX_AOCFGCODE_NUMBER 6//User definition the total number of the AO ConfigCodeTotal

//User AO Config Code for Voltage
#define IXUD_AO_UNI_5V		 0	//0  ~  5V
#define IXUD_AO_BI_5V		 1	//+/-   5V
#define IXUD_AO_UNI_10V		 2	//0  ~ 10V
#define IXUD_AO_BI_10V       3  //+/-  10V
#define IXUD_AO_UNI_20V      4  //0  ~ 20V
#define IXUD_AO_BI_20V       5  //+/-  20V

//User AO Config Code for Current
#define IXUD_AO_I_0_20_MA	 16	//0 ~ 20mA
#define IXUD_AO_I_4_20_MA	 17	//4 ~ 20mA

//Interrupt Setting
#define IXUD_HARDWARE_INT  1 // Hardware Interrupt
#define IXUD_APC_READY_INT 2 // APC Ready Interrupt
#define IXUD_ACTIVE_LOW    4 // Active Low Trigger
#define IXUD_ACTIVE_HIGH   8 // Active High Trigger

//Analog Trigger Mode Setting
#define IXUD_ANALOGTRIGGER_ABOVE   1 //Above high
#define IXUD_ANALOGTRIGGER_BELOW   2 //Below low
#define IXUD_ANALOGTRIGGER_LEAVE   3 //Leave region
#define IXUD_ANALOGTRIGGER_ENTRY   4 //Entry region

// Return code
#define Ixud_NoErr                       0 //Correct
#define Ixud_OpenDriverErr               1 //Open driver error
#define Ixud_PnPDriverErr				 2 //Plug & Play error
#define Ixud_DriverNoOpen                3 //The driver was not open
#define Ixud_GetDriverVersionErr         4 //Receive driver version error
#define Ixud_ExceedBoardNumber			 5 //Board number error
#define Ixud_FindBoardErr				 6 //No board found
#define Ixud_BoardMappingErr			 7 //Board Mapping error
#define Ixud_DIOModesErr				 8 //Digital input/output mode setting error
#define Ixud_InvalidAddress				 9 //Invalid address
#define Ixud_InvalidSize				10 //Invalid size
#define	Ixud_InvalidPortNumber			11 //Invalid port number
#define Ixud_UnSupportedModel			12 //This board model is not supported
#define Ixud_UnSupportedFun				13 //This function is not supported
#define Ixud_InvalidChannelNumber		14 //Invalid channel number
#define Ixud_InvalidValue				15 //Invalid value
#define Ixud_InvalidMode				16 //Invalid mode
#define Ixud_GetAIStatusTimeOut			17 //A timeout occurred while receiving the status of the analog input
#define Ixud_TimeOutErr					18 //Timeout error
#define Ixud_CfgCodeIndexErr			19 //A compatible configuration code table index could not be found
#define Ixud_ADCCTLTimeoutErr			20 //ADC controller a timeout error
#define Ixud_FindPCIIndexErr			21 //A compatible PCI table index value could not be found
#define Ixud_InvalidSetting				22 //Invalid setting value
#define Ixud_AllocateMemErr	    		23 //Error while allocating the memory space
#define Ixud_InstallEventErr			24 //Error while installing the interrupt event
#define Ixud_InstallIrqErr				25 //Error while installing the interrupt IRQ
#define Ixud_RemoveIrqErr				26 //Error while removing the interrupt IRQ
#define Ixud_ClearIntCountErr			27 //Error while the clear interrupt count
#define Ixud_GetSysBufferErr			28 //Error while retrieving the system buffer
#define Ixud_CreateEventErr				29 //Error while create the event
#define Ixud_UnSupportedResolution		30 //Resolution not supported
#define Ixud_CreateThreadErr			31 //Error while create the thread
#define Ixud_ThreadTimeOutErr			32 //Thread timeout error
#define Ixud_FIFOOverFlowErr			33 //FIFO overflow error
#define Ixud_FIFOTimeOutErr				34 //FIFO timeout error
#define Ixud_GetIntInstStatus			35 //Retrieves the status of the interrupt installation
#define Ixud_GetBufStatus				36 //Retrieves the status of the system buffer
#define Ixud_SetBufCountErr			    37 //Error while setting the buffer count
#define Ixud_SetBufInfoErr              38 //Error while setting the buffer data
#define Ixud_FindCardIDErr				39 //Card ID code could not be found
#define Ixud_EventThreadErr				40 //Event Thread error
#define Ixud_AutoCreateEventErr			41 //Error while automatically creating an event
#define Ixud_RegThreadErr				42 //Register Thread error
#define Ixud_SearchEventErr				43 //Search Event error
#define Ixud_FifoResetErr				44 //Error while resetting the FIFO
#define Ixud_InvalidBlock				45 //Invalid EEPROM block
#define Ixud_InvalidAddr				46 //Invalid EEPROM address
#define Ixud_AcqireSpinLock				47 //Error while acquiring spin lock
#define Ixud_ReleaseSpinLock			48 //Error while releasing spin lock
#define	Ixud_SetControlErr				49 //Analog input setting error
#define Ixud_InvalidChannels			50 //Invalid channel number
#define Ixud_SearchCardErr				51 //Invalid model number
#define Ixud_SetMapAddressErr			52 //Error while setting the mapping address
#define Ixud_ReleaseMapAddressErr		53 //Error while releasing the mapping address
#define Ixud_InvalidOffset				54 //Invalid memory offset
#define Ixud_ShareHandleErr				55 //Open the share memory fail
#define Ixud_InvalidDataCount			56 //Invalid data count
#define Ixud_WriteEEPErr				57 //Error while writing the EEPROM
#define Ixud_CardIOErr                  58 //CardIO error
#define Ixud_IOErr                      59 //MemoryIO error
#define Ixud_SetScanChannelErr			60 //Set channel scan number error
#define Ixud_SetScanConfigErr           61 //Set channel scan config error
#define Ixud_GetMMIOMapStatus			62 //

typedef struct _IXUD_DEVICE_INFO_
{
	DWORD dwSize;			//Structure Size
	
	WORD wVendorID;			//Vendor ID
	WORD wDeviceID;			//Device ID
	
	WORD wSubVendorID;		//Sub Vendor ID
	WORD wSubDeviceID;		//Sub Device ID

	DWORD dwBAR[6];			//PCI Bar 0 ~ 5

	UCHAR BusNo;			//the PCI Bus No. in mainboard
	UCHAR DevNo;			//the Slot No. in PCI bus
	UCHAR IRQ;				//IRQ No.
	UCHAR Aux;				//Sub Aux for Tiger series,0xFF=N/A

	DWORD dwBarVirtualAddress[6];   //PCI Bar Virtual Address(For MMIO)

}IXUD_DEVICE_INFO,*PIXUD_DEVICE_INFO;

//extern IXUD_DEVICE_INFO DevInfo[MAX_BOARD_NUMBER];

typedef struct _IXUD_CARD_INFO_
{
	DWORD dwSize;			//Structure size
	DWORD dwModelNo;		//Model Number
	
	//CardID is update when calling the function each time.
    UCHAR CardID;			//for new cards, 0xFF=N/A
	UCHAR wSingleEnded;		//for new cards,1:S.E 2:D.I.F,0xFF=N/A
	WORD wAIOResolution;	//AI/AO Resolution High byte is AI, Low byte is AO

	WORD wAIChannels;		//Number of AI channels(AD)
	WORD wAOChannels;		//Number of AO channels(DA)

	WORD wDIPorts;			//Number of DI ports
	WORD wDOPorts;			//Number of DO ports

	WORD wDIOPorts;			//Number of DIO ports
	WORD wDIOPortWidth;		//The width is 8/16/32 bit.

	WORD wCounterChannels;	//Number of Timers/Counters
	WORD wMemorySize;		//PCI-M512==>512, Units in KB.

	DWORD dwReserved1[6];   //Reserver 

}IXUD_CARD_INFO,*PIXUD_CARD_INFO;
//extern IXUD_CARD_INFO CardInfo[MAX_BOARD_NUMBER];

EXPORTS WORD  CALLBACK  Ixud_GetDllVersion(DWORD *dwDLLVer);
        
// Driver functions
EXPORTS WORD  CALLBACK  Ixud_OptionMode(DWORD dwMode);
EXPORTS WORD  CALLBACK  Ixud_DriverInit(WORD *wTotalBoards);
EXPORTS WORD  CALLBACK  Ixud_DriverClose(void);
EXPORTS WORD  CALLBACK  Ixud_SearchCard(WORD *wTotalBoards,DWORD dwModelNo);	
EXPORTS WORD  CALLBACK  Ixud_GetBoardNoByCardID(WORD *wBoardNo,DWORD dwModelNumber,WORD wCardID);		  
EXPORTS WORD  CALLBACK	Ixud_GetCardInfo(WORD wBoardNo, PIXUD_DEVICE_INFO sDevInfo,PIXUD_CARD_INFO sCardInfo, char *szModelName);
		  
EXPORTS WORD  CALLBACK	Ixud_ReadPort(DWORD dwAddress,WORD wSize,DWORD* dwVal);				  
EXPORTS WORD  CALLBACK	Ixud_WritePort(DWORD dwAddress,WORD wSize,DWORD  dwVal);				  
EXPORTS WORD  CALLBACK	Ixud_ReadPort32(DWORD dwAddress,DWORD* dwLow,DWORD* dwHigh);				  
EXPORTS WORD  CALLBACK	Ixud_WritePort32(DWORD dwAddress,DWORD dwLow,DWORD  dwHigh);
EXPORTS WORD  CALLBACK	Ixud_WritePhyMemory(DWORD dwAddress,WORD wSize,DWORD dwValue);
EXPORTS WORD  CALLBACK	Ixud_ReadPhyMemory(DWORD dwAddress,WORD wSize,DWORD* dwValue);

//Digital I/O functions			
EXPORTS WORD  CALLBACK	Ixud_SetDIOModes32(WORD wBoardNo,DWORD dwDioMode);			  
EXPORTS WORD  CALLBACK	Ixud_SetDIOMode(WORD wBoardNo,WORD wPortNo,WORD wDioMode);
EXPORTS WORD  CALLBACK	Ixud_ReadDI(WORD wBoardNo,WORD wPortNo,DWORD *dwDIVal);				  
EXPORTS WORD  CALLBACK	Ixud_WriteDO(WORD wBoardNo,WORD wPortNo,DWORD dwDOVal);
EXPORTS WORD  CALLBACK	Ixud_ReadDI32(WORD wBoardNo,WORD wPortN,DWORD* dwLow,DWORD* dwHigh);				  
EXPORTS WORD  CALLBACK	Ixud_WriteDO32(WORD wBoardNo,WORD wPortNo,DWORD dwLow ,DWORD dwHigh);
EXPORTS WORD  CALLBACK	Ixud_ReadDIBit(WORD wBoardNo,WORD wPortNo, WORD wBitNo, WORD *wDIVal);				  
EXPORTS WORD  CALLBACK	Ixud_WriteDOBit(WORD wBoardNo,WORD wPortNo, WORD wBitNo, WORD wDOVal);
EXPORTS WORD  CALLBACK  Ixud_SoftwareReadbackDO(WORD wBoardNo,WORD wPortNo,DWORD *dwDOVal);
EXPORTS WORD  CALLBACK  Ixud_SetPWMOutput(WORD wBoardNo,WORD wChannel,WORD wMode,float fFrequency,float fDutyRate,float fDelayUS);
EXPORTS WORD  CALLBACK  Ixud_StartDO(WORD wBoardNo, WORD wPortNo,DWORD dwMode, float fSamplingRate, DWORD dwDataCount,DWORD dwCycleNum, DWORD dwDOVal[]);
EXPORTS WORD  CALLBACK  Ixud_StopDO(WORD wBoardNo,WORD wPortNo);
EXPORTS WORD  CALLBACK  Ixud_StartDI(WORD wBoardNo, WORD wPortNo,DWORD dwMode, float fSamplingRate, DWORD dwDataCount);
EXPORTS WORD  CALLBACK  Ixud_StopDI(WORD wBoardNo,WORD wPortNo);
EXPORTS WORD  CALLBACK	Ixud_GetDIBufferH(WORD wBoardNo,WORD wPortNo,DWORD dwDataCount,DWORD hValue[]);

//Callback functions
EXPORTS WORD  CALLBACK  Ixud_SetEventCallback(WORD wBoardNo,WORD wEventType,WORD wInterruptSource,HANDLE *hEvent,PVOID CallbackFun,DWORD dwCallBackParameter);
EXPORTS WORD  CALLBACK  Ixud_RemoveEventCallback(WORD wBoardNo,WORD wInterruptSource);

//Interrupt functions
EXPORTS WORD  CALLBACK  Ixud_InstallIrq(WORD wBoardNo,DWORD dwInterruptMask);
EXPORTS WORD  CALLBACK  Ixud_RemoveIrq(WORD wBoardNo);

//Timer/Counter functions
EXPORTS WORD  CALLBACK  Ixud_ReadCounter(WORD wBoardNo,WORD wChannel, DWORD *dwValue);		  
EXPORTS WORD  CALLBACK  Ixud_SetCounter(WORD wBoardNo,WORD wChannel,WORD wMode, DWORD dwValue);		  
EXPORTS WORD  CALLBACK  Ixud_DisableCounter(WORD wBoardNo,WORD wChannel);
EXPORTS WORD  CALLBACK  Ixud_SetFCChannelMode(WORD wBoardNo,WORD wChannel,WORD wMode,WORD wDelayMs);
EXPORTS WORD  CALLBACK  Ixud_ReadFrequency(WORD wBoardNo,WORD wChannel,float *fValue,DWORD dwTimeOutMs,WORD *wStatus);

//Memory I/O functions
EXPORTS WORD  CALLBACK  Ixud_ReadMemory(WORD wBoardNo,DWORD dwOffsetByte,WORD wSize, DWORD *dwValue);
EXPORTS WORD  CALLBACK  Ixud_WriteMemory(WORD wBoardNo,DWORD dwOffsetByte,WORD wSize, DWORD dwValue);
EXPORTS WORD  CALLBACK  Ixud_ReadMemory32(WORD wBoardNo,DWORD dwOffsetByte,DWORD *dwLow, DWORD *dwHigh);
EXPORTS WORD  CALLBACK  Ixud_WriteMemory32(WORD wBoardNo,DWORD dwOffsetByte,DWORD dwLow, DWORD dwHigh);

//Analog Input functions
EXPORTS WORD  CALLBACK  Ixud_ReadAI(WORD wBoardNo,WORD wChannel,WORD wConfig,float *fValue);
EXPORTS WORD  CALLBACK  Ixud_ReadAIH(WORD wBoardNo,WORD wChannel,WORD wConfig,DWORD *dwValue);
EXPORTS WORD  CALLBACK  Ixud_PollingAI(WORD wBoardNo,WORD wChannel,WORD wConfig,DWORD dwDataCount,float fValue[]);
EXPORTS WORD  CALLBACK  Ixud_PollingAIH(WORD wBoardNo,WORD wChannel,WORD wConfig,DWORD dwDataCount,DWORD dwValue[]);
EXPORTS WORD  CALLBACK  Ixud_PollingAIScan(WORD wBoardNo,WORD wChannels,WORD wChannelList[],WORD wConfigList[],DWORD dwDataCountPerChannel,float fValue[]);
EXPORTS WORD  CALLBACK  Ixud_PollingAIScanH(WORD wBoardNo,WORD wChannels,WORD wChannelList[],WORD wConfigList[],DWORD dwDataCountPerChannel,DWORD dwValue[]);
EXPORTS WORD  CALLBACK  Ixud_ConfigAI(WORD wBoardNo,WORD wFIFOSizeKB,DWORD BufferSizeCount,WORD wCardType,WORD wDelaySettlingTime);
EXPORTS WORD  CALLBACK  Ixud_ConfigAIEx(WORD wBoardNo,WORD wFIFOSizeKB,DWORD BufferSizeCount,WORD wCardType,WORD wDelaySettlingTime,DWORD dwMode);
EXPORTS WORD  CALLBACK  Ixud_ClearAIBuffer(WORD wBoardNo);
EXPORTS WORD  CALLBACK  Ixud_GetBufferStatus(WORD wBoardNo,WORD *wBufferStatus, DWORD *dwDataCount);
EXPORTS WORD  CALLBACK  Ixud_StartAI(WORD wBoardNo,WORD wChannel,WORD wConfig,float fSamplingRate,DWORD dwDataCount);
EXPORTS WORD  CALLBACK  Ixud_StartExtAI(WORD wBoardNo,WORD wActive,WORD wChannel,WORD wConfig,float fSamplingRate,DWORD dwPostDataCount,DWORD dwPreDataCount);
EXPORTS WORD  CALLBACK	Ixud_StartExtAnalogTrigger(WORD wBoardNo, WORD wActive, WORD wChannel, WORD wConfig, float fSamplingRate, DWORD dwDataCount, DWORD dwReserved ,float fAboveTrgVoltage, float fBelowTrgVoltage);
EXPORTS WORD  CALLBACK  Ixud_StartExtAIScan(WORD wBoardNo,WORD wChannels,WORD wActive,WORD wChannelList[],WORD wConfigList[],float fSamplingRate,DWORD dwPostDataCountPerChannel,DWORD dwPreDataCountPerChannel);
EXPORTS WORD  CALLBACK  Ixud_StartAIScan(WORD wBoardNo,WORD wChannels,WORD wChannelList[],WORD wConfigList[],float fSamplingRate, DWORD dwDataCountPerChannel);
EXPORTS WORD  CALLBACK  Ixud_GetAIBuffer(WORD wBoardNo,DWORD dwDataCount,float fValue[]);
EXPORTS WORD  CALLBACK  Ixud_GetAIBufferH(WORD wBoardNo,DWORD dwDataCount,DWORD hValue[]);
EXPORTS WORD  CALLBACK  Ixud_StopAI(WORD wBoardNo);

//Analog Output functions
EXPORTS WORD  CALLBACK  Ixud_ConfigAO(WORD wBoardNo, WORD wChannel, WORD wCfgCode);
EXPORTS WORD  CALLBACK  Ixud_WriteAOVoltage(WORD wBoardNo, WORD wChannel, float fValue);
EXPORTS WORD  CALLBACK  Ixud_WriteAOVoltageH(WORD wBoardNo, WORD wChannel, DWORD hValue);
EXPORTS WORD  CALLBACK  Ixud_WriteAOCurrent(WORD wBoardNo, WORD wChannel, float fValue);
EXPORTS WORD  CALLBACK  Ixud_WriteAOCurrentH(WORD wBoardNo, WORD wChannel, DWORD hValue);
EXPORTS WORD  CALLBACK  Ixud_StartAOVoltage(WORD wBoardNo, WORD wChannel,WORD wConfig, float fSamplingRate, DWORD dwDataCount,DWORD dwCycleNum, float fValue[]);
EXPORTS WORD  CALLBACK  Ixud_StartAOVoltageH(WORD wBoardNo, WORD wChannel,WORD wConfig, float fSamplingRate, DWORD dwDataCount,DWORD dwCycleNum, DWORD dwValue[]);
EXPORTS WORD  CALLBACK  Ixud_StopAO(WORD wBoardNo, WORD wChannel);

#endif // end define __Uni_DAQ_HEADER__
 