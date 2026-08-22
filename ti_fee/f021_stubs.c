/*
 * f021_stubs.c
 *
 * Stub implementations of the TI F021 Flash API (Fapi_*) functions.
 *
 * The real implementations live in the pre-compiled TI-format .lib files
 * (lib/F021_Flash_API_*.lib), which are only linkable by TI CGT. When
 * building with arm-none-eabi-gcc these stubs satisfy the linker.
 *
 * All functions return Fapi_Status_Success (0). Flash read/write/erase
 * operations are no-ops. If flash programming is required at runtime,
 * a proper GCC-compatible F021 implementation must replace these stubs.
 */

#include "F021.h"

/* ---- Flash State Machine commands ---------------------------------------- */

Fapi_StatusType Fapi_enableMainBankSectors(uint16_t u16SectorsEnables)
{
    (void)u16SectorsEnables;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_enableEepromBankSectors(uint32_t u32SectorsEnables_31_0,
                                              uint32_t u32SectorsEnables_63_32)
{
    (void)u32SectorsEnables_31_0;
    (void)u32SectorsEnables_63_32;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_enableFsmDoneEvent(void)
{
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_disableFsmDoneEvent(void)
{
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_initializeFlashBanks(uint32_t u32HclkFrequency)
{
    (void)u32HclkFrequency;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_setActiveFlashBank(Fapi_FlashBankType oNewFlashBank)
{
    (void)oNewFlashBank;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_enableBanksForOtpWrite(uint8_t u8Banks)
{
    (void)u8Banks;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_disableBanksForOtpWrite(void)
{
    return Fapi_Status_Success;
}

/* ---- Utility ------------------------------------------------------------- */

void Fapi_flushPipeline(void)
{
}

uint32_t* Fapi_remapEccAddress(uint32_t u32EccAddress)
{
    (void)u32EccAddress;
    return (uint32_t*)0U;
}

uint32_t Fapi_remapMainAddress(uint32_t u32MainAddress)
{
    (void)u32MainAddress;
    return 0U;
}

boolean_t Fapi_isAddressEcc(uint32_t u32Address)
{
    (void)u32Address;
    return (boolean_t)0U;
}

boolean_t Fapi_isAddressEEPROM(uint32_t u32Address)
{
    (void)u32Address;
    return (boolean_t)0U;
}

/* ---- Async commands ------------------------------------------------------- */

Fapi_StatusType Fapi_issueAsyncCommandWithAddress(
        Fapi_FlashStateCommandsType oCommand, uint32_t* pu32StartAddress)
{
    (void)oCommand;
    (void)pu32StartAddress;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_issueAsyncCommand(Fapi_FlashStateCommandsType oCommand)
{
    (void)oCommand;
    return Fapi_Status_Success;
}

/* ---- Info commands -------------------------------------------------------- */

Fapi_LibraryInfoType Fapi_getLibraryInfo(void)
{
    Fapi_LibraryInfoType oInfo;
    /* zero-initialize: version fields left at 0 */
    uint8_t *p = (uint8_t*)&oInfo;
    for (uint32_t i = 0; i < sizeof(oInfo); i++) p[i] = 0;
    return oInfo;
}

Fapi_DeviceInfoType Fapi_getDeviceInfo(void)
{
    Fapi_DeviceInfoType oInfo;
    uint8_t *p = (uint8_t*)&oInfo;
    for (uint32_t i = 0; i < sizeof(oInfo); i++) p[i] = 0;
    return oInfo;
}

Fapi_StatusType Fapi_getBankSectors(Fapi_FlashBankType oBank,
                                     Fapi_FlashBankSectorsType* poFlashBankSectors)
{
    (void)oBank;
    (void)poFlashBankSectors;
    return Fapi_Status_Success;
}

uint32_t Fapi_getNumberOfBankSectors(uint32_t u32Bank)
{
    (void)u32Bank;
    return 0U;
}

/* ---- Read commands -------------------------------------------------------- */

Fapi_StatusType Fapi_doBlankCheck(uint32_t* pu32StartAddress, uint32_t u32Length,
                                   Fapi_FlashStatusWordType* poFlashStatusWord)
{
    (void)pu32StartAddress;
    (void)u32Length;
    (void)poFlashStatusWord;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_doMarginRead(uint32_t* pu32StartAddress,
                                   uint32_t* pu32ReadBuffer, uint32_t u32Length,
                                   Fapi_FlashReadMarginModeType oReadMode)
{
    (void)pu32StartAddress;
    (void)pu32ReadBuffer;
    (void)u32Length;
    (void)oReadMode;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_doVerify(uint32_t* pu32StartAddress, uint32_t u32Length,
                               uint32_t* pu32CheckValueBuffer,
                               Fapi_FlashStatusWordType* poFlashStatusWord)
{
    (void)pu32StartAddress;
    (void)u32Length;
    (void)pu32CheckValueBuffer;
    (void)poFlashStatusWord;
    return Fapi_Status_Success;
}

uint32_t Fapi_calculatePsa(uint32_t* pu32StartAddress, uint32_t u32Length,
                            uint32_t u32PsaSeed,
                            Fapi_FlashReadMarginModeType oReadMode)
{
    (void)pu32StartAddress;
    (void)u32Length;
    (void)u32PsaSeed;
    (void)oReadMode;
    return 0U;
}

Fapi_StatusType Fapi_doPsaVerify(uint32_t* pu32StartAddress, uint32_t u32Length,
                                  uint32_t u32PsaValue,
                                  Fapi_FlashStatusWordType* poFlashStatusWord)
{
    (void)pu32StartAddress;
    (void)u32Length;
    (void)u32PsaValue;
    (void)poFlashStatusWord;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_doBlankCheckByByte(uint8_t* pu8StartAddress, uint32_t u32Length,
                                         Fapi_FlashStatusWordType* poFlashStatusWord)
{
    (void)pu8StartAddress;
    (void)u32Length;
    (void)poFlashStatusWord;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_doMarginReadByByte(uint8_t* pu8StartAddress,
                                         uint8_t* pu8ReadBuffer, uint32_t u32Length,
                                         Fapi_FlashReadMarginModeType oReadMode)
{
    (void)pu8StartAddress;
    (void)pu8ReadBuffer;
    (void)u32Length;
    (void)oReadMode;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_doVerifyByByte(uint8_t* pu8StartAddress, uint32_t u32Length,
                                     uint8_t* pu8CheckValueBuffer,
                                     Fapi_FlashStatusWordType* poFlashStatusWord)
{
    (void)pu8StartAddress;
    (void)u32Length;
    (void)pu8CheckValueBuffer;
    (void)poFlashStatusWord;
    return Fapi_Status_Success;
}

/* ---- Programming commands ------------------------------------------------- */

Fapi_StatusType Fapi_issueProgrammingCommand(uint32_t* pu32StartAddress,
                                              uint8_t* pu8DataBuffer,
                                              uint8_t u8DataBufferSizeInBytes,
                                              uint8_t* pu8EccBuffer,
                                              uint8_t u8EccBufferSizeInBytes,
                                              Fapi_FlashProgrammingCommandsType oMode)
{
    (void)pu32StartAddress;
    (void)pu8DataBuffer;
    (void)u8DataBufferSizeInBytes;
    (void)pu8EccBuffer;
    (void)u8EccBufferSizeInBytes;
    (void)oMode;
    return Fapi_Status_Success;
}

Fapi_StatusType Fapi_issueProgrammingCommandForEccAddresses(
        uint32_t* pu32StartAddress, uint8_t* pu8EccBuffer,
        uint8_t u8EccBufferSizeInBytes)
{
    (void)pu32StartAddress;
    (void)pu8EccBuffer;
    (void)u8EccBufferSizeInBytes;
    return Fapi_Status_Success;
}

/* ---- Utility commands ----------------------------------------------------- */

Fapi_StatusType Fapi_waitDelay(volatile uint32_t u32WaitDelay)
{
    (void)u32WaitDelay;
    return Fapi_Status_Success;
}

uint32_t Fapi_calculateFletcherChecksum(uint32_t u32Address, uint32_t u32Length)
{
    (void)u32Address;
    (void)u32Length;
    return 0U;
}

uint8_t Fapi_calculateEcc(uint32_t u32Address, uint64_t u64Data)
{
    (void)u32Address;
    (void)u64Data;
    return 0U;
}

/* ---- User-defined --------------------------------------------------------- */

Fapi_StatusType Fapi_serviceWatchdogTimer(void)
{
    return Fapi_Status_Success;
}
