#pragma once

#define MODULE_GETTER( x ) extern "C" {                                     \
								                    DLL_EXPORT x* Get##x##Instance() {     \
								                     return ::_##x;                                 \
								                    }                                               \
								                   }

#define MODULE( x ) x _##x##Instance = {};     \
								            x* _##x = &_##x##Instance; \
								            MODULE_GETTER( x )
