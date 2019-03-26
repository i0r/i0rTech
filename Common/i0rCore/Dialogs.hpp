#pragma once

/*====================================
	dialog_flag_t
		Flags used to create dialog (cross-platform)
	====================================*/
enum dialog_flag_t {
	DIALOG_FLAG_NONE          = 0,
	DIALOG_FLAG_BUTTON_CANCEL = 0x1 << 0, // Cancel button (optional; since the window control should be available)
	DIALOG_FLAG_BUTTON_RETRY  = 0x1 << 1, // Retry button
};

#ifdef FLAG_WINDOWS
	/*====================================
		DisplayErrorDialog
			Displays a error dialog
				const wchar_t* title:  Window title
				const wchar_t* header: Dialog header text
				const wchar_t* body:   Dialog body text
				const wchar_t* infos:  Dialog details text
				const u32 flags:    Dialog Building flags (see dialog_flag_t)
	====================================*/
	static i32 DisplayErrorDialog( const wchar_t* title, const wchar_t* header, const wchar_t* body, const wchar_t* infos, 
								                        const u32 flags = DIALOG_FLAG_NONE ) {
		i32 selection = -1;

		TASKDIALOGCONFIG tdc       = {};
		tdc.cbSize                 = sizeof( tdc );
		tdc.dwFlags                = TDF_ALLOW_DIALOG_CANCELLATION | TDF_EXPAND_FOOTER_AREA;
		tdc.pszWindowTitle         = title;
		tdc.pszMainIcon            = TD_ERROR_ICON;
		tdc.pszMainInstruction     = header;
		tdc.pszContent             = body;
		tdc.pszExpandedInformation = infos;
		tdc.dwCommonButtons        = 0;

		if( flags & DIALOG_FLAG_BUTTON_CANCEL ) {
			tdc.dwCommonButtons |= TDCBF_RETRY_BUTTON;
		}

		if( flags & DIALOG_FLAG_BUTTON_RETRY ) {
			tdc.dwCommonButtons |= TDCBF_CANCEL_BUTTON;
		}

		TaskDialogIndirect( &tdc, &selection, NULL, NULL );

		return ( selection == IDCANCEL ) ? 0 : selection;
	}
#endif
