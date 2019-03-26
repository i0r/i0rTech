#pragma once

static constexpr u32 CRC32_POLYNOMIAL = 0x82F63B78;

/*====================================
	HashStringCRC32
		Returns the CRC32 hashcode of a string
			char* string: string to be hashed
====================================*/
static u32 HashStringCRC32( const char* string ) {
	u32 crcHash = ~0;

	for( const char* character = string; *character; ++character ) {
		crcHash ^= *character;

		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
	}

	return ~crcHash;
}

/*====================================
	HashStringCRC32
		Returns the CRC32 hashcode of a string
			const std::string &string: string to be hashed
====================================*/
static u32 HashStringCRC32( const std::string &string ) {
	u32 crcHash = ~0;

	for( const char &character : string ) {
		crcHash ^= character;

		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
		crcHash = crcHash & 1 ? ( crcHash >> 1 ) ^ CRC32_POLYNOMIAL : crcHash >> 1;
	}

	return ~crcHash;
}
