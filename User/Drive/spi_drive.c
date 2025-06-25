#include "spi_drive.h"

static uint8_t g_spi_tx_buffer[350];
static uint8_t g_spi_rx_buffer[350];
static uint8_t g_spi_write_buffer[350];

HAL_StatusTypeDef Write_spi(uint16_t addr, uint8_t *pdata, uint32_t size)
{
    HAL_StatusTypeDef err;
    uint32_t len;
    g_spi_write_buffer[0] = 0x06;
    if ((err = HAL_SPI_Transmit(&hspi1, &g_spi_write_buffer[0], sizeof(g_spi_write_buffer[0]), 5000)) != HAL_OK)
    {
        return err;
    }
    g_spi_write_buffer[0] = 0x02;
    g_spi_write_buffer[1] = (uint8_t)(addr >> 8);
    g_spi_write_buffer[2] = (uint8_t)addr;
    for (len = 0; len < size; len++, pdata++)
    {
        g_spi_write_buffer[3 + len] = *pdata;
    }
    err = HAL_SPI_Transmit(&hspi1, g_spi_write_buffer, len + 3, 5000);
    return err;
}

HAL_StatusTypeDef Read_spi(uint16_t addr, uint8_t *pdata, uint32_t size)
{
    HAL_StatusTypeDef err;
    uint32_t i;
    g_spi_tx_buffer[0] = 0x03;
    g_spi_tx_buffer[1] = (uint8_t)(addr >> 8);
    g_spi_tx_buffer[2] = (uint8_t)addr;
     for (i = 0; i < size; i++)
    {
        g_spi_tx_buffer[3 + i] = 0xff;
    }
    if ((err = HAL_SPI_TransmitReceive(&hspi1, g_spi_tx_buffer, g_spi_rx_buffer, 3 + size, 5000)) != HAL_OK)
    {
        return err;
    }
    for(i=0;i<size;i++,pdata++){
        *pdata = g_spi_rx_buffer[3+i];
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
