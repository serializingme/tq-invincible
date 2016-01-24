/**
 * Copyright (C) 2016 Duarte Silva
 *
 * This file is part of TQ-Invincible.
 *
 * TQ-Invincible is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * TQ-Invincible is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TQ-Invincible. If not, see <http://www.gnu.org/licenses/>.
 */
#include <windows.h>

/**
 * Pointer to the exported function in the GameEngine class that returns the
 * current player.
 */
typedef PVOID(__thiscall *GetMainPlayerProto)(PVOID pThis);
/**
 * Pointer to the exported function that changes the invincibility flag in
 * Character classes.
 */
typedef PVOID(__thiscall *SetInvincibleProto)(PVOID pThis, BOOL);

void makeinvincible()
{
	HMODULE hGame = GetModuleHandle(TEXT("Game.dll"));

	if (hGame == NULL) {
		return;
	}

	// The GameEngine class instance pointer.
	PDWORD pGameEngine = (PDWORD) GetProcAddress(hGame, "?gGameEngine@GAME@@3PAVGameEngine@1@A");

	if (pGameEngine != NULL && ((PVOID) * pGameEngine) != NULL) {
		// Pointer to the location in memory of the main player, the one
		// that is to be made invincible.
		GetMainPlayerProto fpMainPlayer = (GetMainPlayerProto)
			GetProcAddress(hGame, "?GetMainPlayer@GameEngine@GAME@@QBEPAVPlayer@2@XZ");

		if (fpMainPlayer == NULL) {
			return;
		}

		// Pointer to the exported function that set the invincibility
		// flag.
		SetInvincibleProto fpSetInvincible = (SetInvincibleProto)
			GetProcAddress(hGame, "?SetInvincible@Character@GAME@@QAEX_N@Z");

		if (fpSetInvincible == NULL) {
			return;
		}

		PVOID pMainPlayer = fpMainPlayer((PVOID) *pGameEngine);

		if (pMainPlayer != NULL) {
			fpSetInvincible(pMainPlayer, TRUE);
		}
	}
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, PVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH) {
		makeinvincible();
	}

	return FALSE;
}
