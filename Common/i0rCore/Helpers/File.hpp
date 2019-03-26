#pragma once

#include "String.hpp"

#define FILE_READ( x )                   *( x* )ptr; FILE_MOVE_POINTER( sizeof( x ) )
#define FILE_READ_SIZE( x, y )           *( x* )ptr; FILE_MOVE_POINTER( y )
#define FILE_MOVE_POINTER( x )           ptr += x
#define FILE_MOVE_POINTER_RECURSIVE( x ) FILE_MOVE_POINTER( x ); i += x
#define FILE_READ_SIZE_RECURSIVE( x, y ) *( x* )ptr; FILE_MOVE_POINTER_RECURSIVE( y )
#define FILE_READ_RECURSIVE( x )         *( x* )ptr; FILE_MOVE_POINTER_RECURSIVE( sizeof( x ) )
#define DICTIONARY_FIND_KEY( x )         dictionaryIterator = dictionary.find( x );\
								                                 if( dictionaryIterator != dictionary.end() )

/*====================================
	DoesFileExist
		Checks if the specified file exits
		NOTE: this is only meant to be used outside the assets archives (e.g. for library check or locals loading)
			const char* fileName: name of the file to check; the path is relative
====================================*/
INLINE static bool DoesFileExist( const char* fileName ) {
	struct stat fileStat = {};
	return ( !stat( fileName, &fileStat ) );
}

/*====================================
ReadPadding
	Skip padding bytes in a binary file
		char* &pointer: pointer to the active buffer
		u32 &pos: secondary pointer
		const u32 padding = 16: base of the binary file
====================================*/
INLINE static void ReadPadding( char* &pointer, u32 &pos, const u32 padding = 16 ) {
	while( pos % padding != 0 ) {
		pointer++;
		pos++;
	}
}

/*====================================
GetFileSize
	Return the size of a file (outside the vfs)
	std::istream& stream: active filestream
	const std::streamoff curOffset = 0: current offset (to backup if required)
====================================*/
static std::streamoff GetFileSize( std::istream& stream, const std::streamoff curOffset = 0 ) {
	stream.seekg( 0, std::ios_base::end );
	const std::streamoff fileSize = stream.tellg();
	stream.seekg( curOffset, std::ios_base::beg );
	return fileSize;
}

/*====================================
GetFileExtensionFromPath
	Return the file extension from a path
		const std::string &x: string containing the path
====================================*/
static std::string GetFileExtensionFromPath( const std::string &x ) {
	const size_t dotPosition = x.find_last_of( '.' );

	if( dotPosition != std::string::npos ) {
		return x.substr( dotPosition + 1, x.length() - dotPosition );
	}

	return "";
}

/*====================================
FText
	Text file
====================================*/
class FText {
	public:
		/*====================================
		Write
			Write a Text file
				const char* fileName: name of the text file
				const char* content:  text to write
		====================================*/
		static void Write( const char* fileName, const char* content ) {
			std::ofstream fileStream( fileName, std::ofstream::out | std::ofstream::app );

			if( fileStream.is_open() ) {
				fileStream << content;
			}

			fileStream.close();
		}
};

/*====================================
FDictionary
	Dictionary file
====================================*/
class FDictionary {
	public:
		/*====================================
		Read
			Read a Dictionary file
				const char* fileName: name of the dictionary
		====================================*/
		static std::map<std::string, std::string> Read( const char* fileName ) {
			std::ifstream fileStream( fileName );

			std::map<std::string, std::string> data            = {};

			std::string                        fileLine        = "", 
								                              dictionaryKey   = "", 
								                              dictionaryValue = "";

			if( fileStream.is_open() ) {
				while( fileStream.good() ) {
					std::getline( fileStream, fileLine );

					const std::size_t separator       = fileLine.find_first_of( '=' );
					const std::size_t commentPosition = fileLine.find_first_of( ';' );

					if( commentPosition != -1 ) {
						fileLine.erase( fileLine.begin() + commentPosition, fileLine.end() );
					}

					if( !fileLine.empty() && separator != std::string::npos ) {
						dictionaryKey = fileLine.substr( 0, separator );
						dictionaryValue = fileLine.substr( separator + 1 );

						TrimString( dictionaryKey );
						TrimString( dictionaryValue );

						if( !dictionaryValue.empty() ) {
							data.insert( std::make_pair( dictionaryKey, dictionaryValue ) );
						}
					}
				}
			}

			fileStream.close();
			return data;
		}
};
