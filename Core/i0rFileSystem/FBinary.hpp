#pragma once

struct file_bloc_generic_t {
	u32 Magic;
	u32 Count;
	u32 Size;
	u32 __PADDING__;
};

class FBinary {
	public:
		static size_t ReadFast( const char* file, void* &dataPtr ) {
			if( !::DoesFileExist( file ) ) {
				return 0;
			}
		
			std::streamoff fileSize = 0;

			std::ifstream fileStream( file, std::ios::binary );

			if( fileStream.is_open() ) {
				fileSize = GetFileSize( fileStream );
				dataPtr  = malloc( fileSize + 1 );
				memset( dataPtr, 0, fileSize + 1 );
				fileStream.read( (char*)dataPtr, fileSize );
			} 
			
			fileStream.close();

			return fileSize;
		}

		static INLINE void Free( void* blockPointer ) {
			free( blockPointer );
		}
};