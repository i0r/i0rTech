#pragma once

#if defined( FLAG_DEBUG )
	enum file_watched_type_t {
		FILE_WATCHED_TYPE_UNKNOWN,
		FILE_WATCHED_TYPE_TEXTURE,
		FILE_WATCHED_TYPE_SHADER,
		FILE_WATCHED_TYPE_MESH,
	};

	struct file_watched_t {
		std::string         Name;
		file_watched_type_t Type;
	};

	class IFileWatchdog {
		DECLARE_MEMBER( protected, std::queue<file_watched_t*>, ToReload )
		DECLARE_MEMBER( protected, HANDLE,                      Watchdog )

		public:
			virtual ~IFileWatchdog() {}

			virtual bool Initialize()  = 0;
			virtual void Pump()        = 0;
	};
#endif
