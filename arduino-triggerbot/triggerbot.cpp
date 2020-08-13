#include "triggerbot.h"

void triggerbot::run()
{
	nbqmemory mem("csgo.exe", PROCESS_ALL_ACCESS);
	arduino duino("Arduino Leonardo");

	module client_module = { 0 };
	client_module = mem.get_module("client.dll");

	if (client_module.base)
	{
		for (;;)
		{
			DWORD my_player = mem.read_memory<DWORD>(client_module.base + m_dwLocalPlayer);
			if (my_player <= 0) { continue; }

			DWORD my_team = mem.read_memory<DWORD>(my_player + m_iTeamNum);
			DWORD my_cross = mem.read_memory<DWORD>(my_player + m_iCrossHairID);

			if (my_cross >= 1 && my_cross <= 64)
			{
				DWORD entity = mem.read_memory<DWORD>(client_module.base + m_dwEntityList + ((my_cross - 1) * 0x10));
				if (entity <= 0) { continue; }
	
				DWORD entity_health = mem.read_memory<DWORD>(entity + m_iHealth);
				DWORD entity_team = mem.read_memory<DWORD>(entity + m_iTeamNum);

				if (entity_health > 0 && entity_team != my_team && entity_team > 1)
				{
					char buffer[] = "-10";
					duino.send_data(buffer, sizeof(buffer));
				}
			}
			Sleep(1);
		}
	}
}

