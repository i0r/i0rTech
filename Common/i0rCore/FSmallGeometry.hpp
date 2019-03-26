#pragma once

#include "IFileSystem.hpp"

struct file_header_sge_t {
	u32 Magic;

	u8  VersionMajor;
	u8  VersionMinor;
	u8  VersionPatch;
	u8  __PADDING__;

	u32 VboSize;
	u32 VaoSize;
};

struct file_small_geometry_t {
	explicit file_small_geometry_t()  : BaseAddress( nullptr ) {
		memset( &Infos, 0, sizeof( file_header_sge_t ) );
	}

	~file_small_geometry_t() {
		if( BaseAddress ) {
			Instance.FileSystem->FreeUncompressedFile( BaseAddress );
		}

		BaseAddress = nullptr;
		memset( &Infos, 0, sizeof( file_header_sge_t ) );
	}

	char*             BaseAddress;
	file_header_sge_t Infos;
};
