#pragma once

#ifdef FLAG_DEBUG
	/*===========================
	DumpRegistersToFile
		Dump CPU registers to a file
	===========================*/
	static void DumpRegistersToFile() {
		//  ASSERT( false, "FUNCTION IS NOT IMPLEMENTED" )
	}

	/*===========================
	DumpRegistersToConsole
		Dump CPU registers to the debug console
	===========================*/
	static void DumpRegistersToConsole() {
		jmp_buf rDump = {};
		_setjmp( rDump );

		CONSOLE_PRINT_INFO( "\n" );
		CONSOLE_PRINT_INFO( "================================================\n" );
		CONSOLE_PRINT_INFO( "DumpRegistersToConsole\n" );
		CONSOLE_PRINT_INFO( "================================================\n" );

		#ifdef FLAG_64
			CONSOLE_PRINT_INFO( "RAX: 0x%08x\t RBX: 0x%08x\t RCX: 0x%08x\t RDX: 0x%08x\n RSI: 0x%08x\t RDI: 0x%08x\t RBP:"
								               " 0x%08x\t RSP: 0x%08x\n\t R8: 0x%08x\t  R9: 0x%08x\t R10: 0x%08x\t R11: 0x%08x\n R12: 0x%08x\t"
								               " R13: 0x%08x\t R14: 0x%08x\t R15: 0x%08x\n",
								               rDump[0], rDump[1],  rDump[2],  rDump[3],  rDump[4],  rDump[5],  rDump[6], rDump[7], rDump[8], 
								               rDump[9], rDump[10], rDump[11], rDump[12], rDump[13], rDump[14], rDump[15] );
		#endif

		CONSOLE_PRINT_INFO( "\n" );
	}
#endif
