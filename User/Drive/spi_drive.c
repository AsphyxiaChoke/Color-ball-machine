#include "spi_drive.h"

HAL_StatusTypeDef Write_spi(uint16_t addr, uint8_t *pdata, uint32_t size)
{
    HAL_StatusTypeDef err;
    uint8_t buf[128], len;
    buf[0] = 0x06;
    if ((err = HAL_SPI_Transmit(&hspi1, &buf[0], sizeof(buf[0]), 5000)) != HAL_OK)
    {
        return err;
    }
    buf[0] = 0x02;
    buf[1] = (uint8_t)(addr >> 8);
    buf[2] = (uint8_t)addr;
    for (len = 0; len < size; len++, pdata++)
    {
        buf[3 + len] = *pdata;
    }
    err = HAL_SPI_Transmit(&hspi1, buf, len + 3, 5000);
    return err;
}

HAL_StatusTypeDef Read_spi(uint16_t addr, uint8_t *pdata, uint32_t size)
{
    HAL_StatusTypeDef err;
    uint8_t txbuf[128], rxbuf[128], i;
    txbuf[0] = 0x03;
    txbuf[1] = (uint8_t)(addr >> 8);
    txbuf[2] = (uint8_t)addr;
    for (i = 0; i < size; i++)
    {
        txbuf[3 + i] = 0xff;
    }
    if ((err = HAL_SPI_TransmitReceive(&hspi1, txbuf, rxbuf, 3 + size, 5000)) != HAL_OK)
    {
        return err;
    }
    for(i=0;i<size;i++,pdata++){
        *pdata = rxbuf[3+i];
    }
    return err;
}

HAL_StatusTypeDef Read_spi_station(uint8_t *pdata, uint32_t size)
{
    HAL_StatusTypeDef err;
    uint8_t buf[2], rxbuf[2];
    buf[0] = 0x05;
    buf[1] = 0xff;
    if ((err = HAL_SPI_TransmitReceive(&hspi1, buf, rxbuf, 2, 5000)) != HAL_OK)
    {
        return err;
    }
    *pdata = rxbuf[1];
    return err;
}
