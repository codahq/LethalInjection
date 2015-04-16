/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod
 * Copyright (C) 2004-2007 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id: detourhelpers.h 248 2008-08-27 00:56:22Z pred $
 */

#ifndef _INCLUDE_SOURCEMOD_DETOURHELPERS_H_
#define _INCLUDE_SOURCEMOD_DETOURHELPERS_H_

//#define DETOURHELPERS_LOG(...) L4D_DEBUG_LOG(__VA_ARGS__)
#define DETOURHELPERS_LOG(...)

#include <windows.h>

struct patch_t
{
	patch_t()
	{
		patch[0] = 0;
		bytes = 0;
	}
	unsigned char patch[20];
	size_t bytes;
};

inline void ProtectMemory(void *addr, int length, int prot)
{
	DETOURHELPERS_LOG("Protecting memory...");
	DWORD old_prot;
	VirtualProtect(addr, length, prot, &old_prot);
}

inline void SetMemPatchable(void *address, size_t size)
{
	ProtectMemory(address, (int)size, PAGE_EXECUTE_READWRITE);
}

inline void DoGatePatch(unsigned char *target, void *callback)
{
	SetMemPatchable(target, 20);

	target[0] = 0xFF;	/* JMP */
	target[1] = 0x25;	/* MEM32 */
	*(void **)(&target[2]) = callback;
}

inline void ApplyPatch(void *address, int offset, const patch_t *patch, patch_t *restore)
{
	ProtectMemory(address, 20, PAGE_EXECUTE_READWRITE);

	unsigned char *addr = (unsigned char *)address + offset;
	if (restore)
	{
		for (size_t i=0; i<patch->bytes; i++)
		{
			restore->patch[i] = addr[i];
		}
		restore->bytes = patch->bytes;
	}

	DETOURHELPERS_LOG("Copying new patch into memory...");
	for (size_t i=0; i<patch->bytes; i++)
	{
		DETOURHELPERS_LOG("Copying new patch into memory byte %d/%d...", i+1, patch->bytes);
		addr[i] = patch->patch[i];
	}
}

#endif