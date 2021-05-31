#include "common.h"

// ======================================================
//  find pattern function / gave unresolved extern
// ======================================================
//context ctx::;

// Original function by learn_more -- https://github.com/learn-more/findpattern-bench
#define in_range(x, a, b) (x >= a && x <= b) 
#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

namespace memory {
	std::uint8_t* find_pattern(void* module, const char* signature)
	{
		static auto pattern_to_byte = [](const char* pattern) {
			auto bytes = std::vector<int>{};
			auto start = const_cast<char*>(pattern);
			auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; ++current) {
				if (*current == '?') {
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else {
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

		auto dosHeader = (PIMAGE_DOS_HEADER)module;
		auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

		auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
		auto patternBytes = pattern_to_byte(signature);
		auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

		auto s = patternBytes.size();
		auto d = patternBytes.data();

		for (auto i = 0ul; i < sizeOfImage - s; ++i) {
			bool found = true;
			for (auto j = 0ul; j < s; ++j) {
				if (scanBytes[i + j] != d[j] && d[j] != -1) {
					found = false;
					break;
				}
			}
			if (found) {
				return &scanBytes[i];
			}
		}
		return nullptr;
	}
	DWORD find_pattern2(std::string moduleName, BYTE* Mask, char* szMask)
	{ 
		auto WaitOnModuleHandle = [](std::string moduleName) -> DWORD
		{
			DWORD ModuleHandle = NULL;
			while (!ModuleHandle)
			{
				ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
				if (!ModuleHandle)
					Sleep(50);
			}
			return ModuleHandle;
		};
		auto bCompare= [](const BYTE* Data, const BYTE* Mask, const char* szMask)
		{
			for (; *szMask; ++szMask, ++Mask, ++Data)
			{
				if (*szMask == 'x' && *Mask != *Data)
				{
					return false;
				}
			}
			return (*szMask) == 0;
		};
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;
		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), Mask, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		std::cout << "error" << std::endl;
		return 0;
	}
	uintptr_t occurence( std::string module, std::string pattern ) {
		auto get_handle = [ & ] ( ) -> HMODULE {
			HMODULE mod = nullptr;

			while ( !mod ) {
				mod = ( GetModuleHandleA( module.c_str( ) ) );
				std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
			}

			return mod;
		};

		auto find_pattern = [ ] ( const uintptr_t& start_address, const uintptr_t& end_address, const char* target_pattern ) -> uintptr_t {
			const char* pattern = target_pattern;

			uintptr_t first_match = 0;

			for ( uintptr_t position = start_address; position < end_address; position++ ) {
				if ( !*pattern )
					return first_match;

				const uint8_t pattern_current = *reinterpret_cast< const uint8_t* >( pattern );
				const uint8_t memory_current = *reinterpret_cast< const uint8_t* >( position );

				if ( pattern_current == '\?' || memory_current == get_byte( pattern ) ) {
					if ( !first_match )
						first_match = position;

					if ( !pattern [ 2 ] )
						return first_match;

					pattern += pattern_current != '\?' ? 3 : 2;
				}
				else {
					pattern = target_pattern;
					first_match = 0;
				}
			}

			return 0;
		};

		MODULEINFO module_info = { 0 };

		if ( !K32GetModuleInformation( GetCurrentProcess( ), get_handle( ), &module_info, sizeof( MODULEINFO ) ) ) {
			return -1;
		}

		const uintptr_t start_address = uintptr_t( module_info.lpBaseOfDll );
		const uintptr_t end_address = start_address + module_info.SizeOfImage;

		uintptr_t address = find_pattern( start_address, end_address, pattern.c_str( ) );

		return address;
	}
};