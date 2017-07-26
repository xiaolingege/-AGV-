#include "flash.h"

//************************************
// FunctionName:  Flash_GetSector
// Returns:   uint16_t
// Qualifier:���ݵ�ַ�����ж�д�������λ�ò�����������
// Parameter: uint32_t Address
//************************************
uint16_t Flash_GetSector(uint32_t Address)
{
	uint16_t sector = 0;
	if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_Sector_0;
	}
	else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_Sector_1;
	}
	else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_Sector_2;
	}
	else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_Sector_3;
	}
	else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_Sector_4;
	}
	else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_Sector_5;
	}
	else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_Sector_6;
	}
	else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_Sector_7;
	}
	else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_Sector_8;
	}
	else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_Sector_9;
	}
	else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_Sector_10;
	}
	else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
	{
		sector = FLASH_Sector_11;
	}
	return sector;
}

//************************************
// FunctionName:  Flash_EraseSector
// Returns:   void
// Qualifier:���������Ų�������
// Parameter: uint16_t SectorNum
//************************************
void Flash_EraseSector(uint16_t SectorNum)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
		FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	if (FLASH_EraseSector(SectorNum, VoltageRange_3) != FLASH_COMPLETE) while (1);
	FLASH_Lock();
}

//************************************
// FunctionName:  Flash_Write32BitDatas
// Returns:   void
// Qualifier:�ڸ�����ַ�������ݳ��ȣ����ݲ���д����Ϣ
// Parameter: uint32_t address
// Parameter: uint16_t length
// Parameter: int32_t * data_32
//************************************
void Flash_Write32BitDatas(uint32_t address, uint16_t length, int32_t* data_32)
{
	uint16_t StartSector, EndSector, i;
	FLASH_Unlock(); //����FLASH�������FLASH��д���ݡ�
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
		FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	StartSector = Flash_GetSector(address);  //��ȡFLASH��Sector���
	EndSector = Flash_GetSector(address + 4 * length);
	for (i = StartSector; i < EndSector; i += 8)  //ÿ��FLASH�������8���ɲο��ϱ�FLASH Sector�Ķ��塣
	{
		if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE) while (1);
	}
	for (i = 0; i < length; i++)
	{
		if (FLASH_ProgramWord(address, data_32[i]) == FLASH_COMPLETE)   //��DATA_32д����Ӧ�ĵ�ַ��
		{
			address = address + 4;
		}
		else
		{
			while (1);
		}
	}
	FLASH_Lock();  //��FLASH����ҪFLASH���ڽ���״̬��
}

//************************************
// FunctionName:  Flash_Read32BitDatas
// Returns:   void
// Qualifier:��ȡ��Ӧ���ȵ�32λ���ݵ�����������ʼλ��Ϊ����λ��
// Parameter: uint32_t address
// Parameter: uint16_t length
// Parameter: int32_t * data_32
//************************************
void Flash_Read32BitDatas(uint32_t address, uint16_t length, int32_t* data_32)
{
	uint8_t i;
	for (i = 0; i < length; i++)
	{
		data_32[i] = *(__IO int32_t*)address;
		address = address + 4;
	}
}

void Flash_Write16BitDatas(uint32_t address, uint16_t length, int16_t* data_16)
{
	uint16_t StartSector, EndSector, i;
	FLASH_Unlock(); //����FLASH�������FLASH��д���ݡ�
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
		FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	StartSector = Flash_GetSector(address);  //��ȡFLASH��Sector���
	EndSector = Flash_GetSector(address + 2 * length);
	for (i = StartSector; i < EndSector; i += 8)  //ÿ��FLASH�������8���ɲο��ϱ�FLASH Sector�Ķ��塣
	{
		if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE) while (1);
	}
	for (i = 0; i < length; i++)
	{
		if (FLASH_ProgramHalfWord(address, data_16[i]) == FLASH_COMPLETE)
		{
			address = address + 2;
		}
		else
		{
			while (1);
		}
	}
	FLASH_Lock();  //��FLASH����ҪFLASH���ڽ���״̬��
}

void Flash_Read16BitDatas(uint32_t address, uint16_t length, int16_t* data_16)
{
	uint8_t i;
	for (i = 0; i < length; i++)
	{
		data_16[i] = *(__IO int16_t*)address;
		address = address + 2;
	}
}

void Flash_Write8BitDatas(uint32_t address, uint16_t length, int8_t* data_8)
{
	uint16_t StartSector, EndSector, i;
	FLASH_Unlock(); //����FLASH�������FLASH��д���ݡ�
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
		FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	StartSector = Flash_GetSector(address);  //��ȡFLASH��Sector���
	EndSector = Flash_GetSector(address + length);
	for (i = StartSector; i < EndSector; i += 8)  //ÿ��FLASH�������8���ɲο��ϱ�FLASH Sector�Ķ��塣
	{
		if (FLASH_EraseSector(i, VoltageRange_3) != FLASH_COMPLETE) while (1);
	}
	for (i = 0; i < length; i++)
	{
		if (FLASH_ProgramByte(address, data_8[i]) == FLASH_COMPLETE)
		{
			address++;
		}
		else
		{
			while (1);
		}
	}
	FLASH_Lock();  //��FLASH����ҪFLASH���ڽ���״̬��
}

void Flash_Read8BitDatas(uint32_t address, uint16_t length, int8_t* data_8)
{
	uint8_t i;
	for (i = 0; i < length; i++)
	{
		data_8[i] = *(__IO int8_t*)address;
		address++;
	}
}

