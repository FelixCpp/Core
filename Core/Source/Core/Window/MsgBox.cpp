#include <Core/Window/MsgBox.hpp>

#include <Windows.h>

namespace Core
{
	
	MsgBoxResult MsgBox::Show(const std::string & text, const std::string & caption, MsgBoxButton button, MsgBoxIcon icon, MsgBoxDefaultButton defaultButton, MsgBoxModality modality, MsgBoxOptions options, Windowhandle handle)
	{
		const UINT uType = (int)button | (int)icon | (int)defaultButton | (int)modality | (int)options;

		const int msgBoxID = MessageBoxA(
			handle,
			text.c_str(),
			caption.c_str(),
			uType
		);

		return (MsgBoxResult)msgBoxID;
	}

}