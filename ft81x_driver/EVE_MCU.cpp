#include "spi.h"
#include "gio.h"

#include <EVE2.h>
#include <MCU.h>

#define EVE_SPI spiREG4

static spiDAT1_t dataconfig =
{
    false,          // CS_HOLD
    false,          // WDEL
    SPI_FMT_0,      // Data format
    SPI_CS_NONE     // Chip select (manual CS)
};

void MCU_Init(void)
{
    gioInit();
    spiInit();

    uint32_t dirA = 0U;

    dirA |= (1U << 0);   // GIOA[0] -> CS
    dirA |= (1U << 1);   // GIOA[1] -> PD

    gioSetDirection(gioPORTA, dirA);

    MCU_CShigh();
    MCU_PDhigh();
}

void MCU_Deinit(void)
{

}

void MCU_Setup(void)
{

}

////////////////////////////////////////
//// GPIO CONTROL
////////////////////////////////////////

void MCU_CSlow(void)
{
    gioSetBit(gioPORTA, 0, 0);
}

void MCU_CShigh(void)
{
    gioSetBit(gioPORTA, 0, 1);
}

void MCU_PDlow(void)
{
    gioSetBit(gioPORTA, 1, 0);
}

void MCU_PDhigh(void)
{
    gioSetBit(gioPORTA, 1, 1);
}

////////////////////////////////////////
//// SPI CORE
////////////////////////////////////////

uint8_t MCU_SPIReadWrite8(uint8_t val)
{
    uint16 tx = val & 0xFF;
    uint16 rx = 0;

    spiTransmitAndReceiveData(EVE_SPI, &dataconfig, 1, &tx, &rx);

    return (uint8_t)(rx & 0xFF);
}

uint16_t MCU_SPIReadWrite16(uint16_t DataToWrite)
{
    uint16 DataRead = 0;
    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 0) & 0xFF) << 0;
    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 8) & 0xFF) << 8;

    return DataRead;
}

uint32_t MCU_SPIReadWrite24(uint32_t DataToWrite)
{
    uint32_t DataRead = 0;

    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 0) & 0xFF) << 0;
    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 8) & 0xFF) << 8;
    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 16) & 0xFF) << 16;

    return DataRead;
}

uint32_t MCU_SPIReadWrite32(uint32_t DataToWrite)
{
    uint32_t DataRead = 0;

    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 0) & 0xFF) << 0;
    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 8) & 0xFF) << 8;
    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 16) & 0xFF) << 16;
    DataRead |= MCU_SPIReadWrite8((DataToWrite >> 24) & 0xFF) << 24;

    return DataRead;
}

////////////////////////////////////////
//// DELAYS
////////////////////////////////////////

void MCU_Delay_20ms(void)
{
    for(volatile uint32 i=0;i<200000;i++);
}

void MCU_Delay_500ms(void)
{
    for(volatile uint32 i=0;i<5000000;i++);
}

////////////////////////////////////////
//// SPI API
////////////////////////////////////////

uint8_t MCU_SPIRead8(void)
{
    return MCU_SPIReadWrite8(0);
}

void MCU_SPIWrite8(uint8_t DataToWrite)
{
    MCU_SPIReadWrite8(DataToWrite);
}

uint16_t MCU_SPIRead16(void)
{
    return MCU_SPIReadWrite16(0);
}

void MCU_SPIWrite16(uint16_t DataToWrite)
{
    MCU_SPIReadWrite16(DataToWrite);
}

uint32_t MCU_SPIRead24(void)
{
    return MCU_SPIReadWrite24(0);
}

void MCU_SPIWrite24(uint32_t DataToWrite)
{
    MCU_SPIReadWrite24(DataToWrite);
}

uint32_t MCU_SPIRead32(void)
{
    return MCU_SPIReadWrite32(0);
}

void MCU_SPIWrite32(uint32_t DataToWrite)
{
    MCU_SPIReadWrite32(DataToWrite);
}

void MCU_SPIWrite(const uint8_t *DataToWrite, uint32_t length)
{
    for(uint32_t i=0;i<length;i++)
    {
        MCU_SPIWrite8(DataToWrite[i]);
    }
}

void MCU_SPIRead(uint8_t *DataToRead, uint32_t length)
{
    for(uint32_t i=0;i<length;i++)
    {
        DataToRead[i] = MCU_SPIRead8();
    }
}

////////////////////////////////////////
//// ENDIAN
////////////////////////////////////////

uint16_t MCU_htobe16(uint16_t h){ return __builtin_bswap16(h); }
uint32_t MCU_htobe32(uint32_t h){ return __builtin_bswap32(h); }

uint16_t MCU_htole16(uint16_t h){ return h; }
uint32_t MCU_htole32(uint32_t h){ return h; }

uint16_t MCU_be16toh(uint16_t h){ return __builtin_bswap16(h); }
uint32_t MCU_be32toh(uint32_t h){ return __builtin_bswap32(h); }

uint16_t MCU_le16toh(uint16_t h){ return h; }
uint32_t MCU_le32toh(uint32_t h){ return h; }
