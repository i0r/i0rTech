#pragma once

/*====================================
BoolToString
	Convert a boolean to a litteral string
			const bool b : boolean to convert
====================================*/
INLINE std::string BoolToString( const bool b ) {
	return b ? "TRUE" : "FALSE";
}

/*====================================
TrimString
	Removes spaces, new lines, tabulations from a string
		std::string &string : string to trim
====================================*/
static void TrimString( std::string &string ) {
	std::size_t charPos = string.find_first_not_of( " \t\n" );

	if( charPos != std::string::npos ) {
		string.erase( 0, charPos );
	}

	charPos = string.find_last_not_of( " \t\n" );

	if( charPos != std::string::npos ) {
		string.erase( charPos + 1 );
	}
}

/*====================================
RemoveSpaces
	Removes spaces from a string
		std::string &string : string to edit
====================================*/
INLINE void RemoveSpaces( std::string &string ) {
	string.erase( std::remove_if( string.begin(), string.end(), isspace ), string.end() );
}

/*====================================
WideToMultibyte
	Converts a wstring to a string
			const std::wstring &wideString: wide string to convert
====================================*/
INLINE std::string WideToMultibyte( const std::wstring &wideString ) {
	return std::string( wideString.begin(), wideString.end() );
}

/*====================================
ToLower
	Convert a string to lowercase
		std::string &string: string to lower
====================================*/
INLINE void ToLower( std::string &string ) {
	std::transform( string.begin(), string.end(), string.begin(), ::tolower );
}

/*====================================
StringToBool
	Convert a string to a boolean
	const std::string &string : return a string litteral
====================================*/
static bool StringToBool( const std::string &string ) {
	std::string strCpy( string );
	ToLower( strCpy );

	return strCpy == "true" || strCpy == "1" || strCpy == "y" || strCpy == "yes";
}

/*====================================
TextFileToString
	Concate a string vector to a single string
		const std::vector<std::string> &fileText : string vector to concate
====================================*/
static std::string TextFileToString( const std::vector<std::string> &fileText ) {
	std::string catString = "";

	for( std::string line : fileText ) {
		catString.append( line + "\r\n" );
	}

	return catString;
}

/*====================================
RemoveWord
	Remove a specific word from a string
		std::string &string : string to edit
		const std::string &word : word to remove
====================================*/
static void RemoveWord( std::string &string, const std::string &word ) {
	std::size_t wordIndex = 0;

	while( ( wordIndex = string.find( word ) ) != std::string::npos ) {
		string.replace( wordIndex, word.size(), "" );
	}
}

/*====================================
ReplaceWord
	Replace a specific word with another word from a string
		std::string &string : string to edit
		const std::string &word : word to replace
		const std::string &newWord : new word
====================================*/
static void ReplaceWord( std::string &string, const std::string &word, const std::string &newWord ) {
	std::size_t wordIndex = 0;

	while( ( wordIndex = string.find( word ) ) != std::string::npos ) {
		string.replace( wordIndex, word.size(), newWord );
	}
}

/*====================================
RemoveFileExtension
	Remove dotted extension from a string
		const std::string &x: string to edit
====================================*/
static std::string RemoveFileExtension( const std::string &x ) {
	const size_t dotPosition = x.find_last_of( '.' );

	if( dotPosition != std::string::npos ) {
		return x.substr( 0, dotPosition );
	}

	return x;
}

/*====================================
RemoveSpecialCharacters
	Remove new line and carriage return from a string
		std::string &str : string to edit
====================================*/
static void RemoveSpecialCharacters( std::string &str ) {
	str.erase( std::remove( str.begin(), str.end(), '\r' ), str.end() );
	str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
}

/*====================================
StringToStringArray
	Split a string into a vector of string
		const std::string str: string to split
====================================*/
static std::vector<std::string> StringToStringArray( const std::string &str ) {
	std::stringstream strCpy( str );

	std::vector<std::string> strArr = {};
	std::string              strCur = "";

	while( std::getline( strCpy, strCur, '\n' ) ) {
		RemoveSpecialCharacters( strCur );
		strArr.push_back( strCur );
		strCur.clear();
	}

	return strArr;
}

/*====================================
SplitString
	Split a string into a vector of string using a special character as a delimiter
		const std::string str: string to split
		const char splitChar: delimiter
====================================*/
static std::vector<std::string> SplitString( const std::string &str, const char splitChar ) {
	std::stringstream strCpy( str );

	std::vector<std::string> strArr = {};
	std::string              strCur = "";

	while( std::getline( strCpy, strCur, splitChar ) ) {
		strArr.push_back( strCur );
	}

	return strArr;
}
