#pragma once

#include <string>

#include <Core/System/Datatypes.hpp>
#include <Core/System/StaticObject.hpp>

#include <Core/Window/MsgBoxButton.hpp>
#include <Core/Window/MsgBoxDefaultButton.hpp>
#include <Core/Window/MsgBoxIcon.hpp>
#include <Core/Window/MsgBoxModality.hpp>
#include <Core/Window/MsgBoxOptions.hpp>
#include <Core/Window/MsgBoxResult.hpp>

namespace Core
{

	struct MsgBox : StaticObject {
	
		/// <summary>
		/// Displays a modal dialog box that contains a system icon, a set of buttons, and a brief
		/// application-specific message, such as status or error information. The message box returns an
		/// result-type that indicates which button the user clicked.
		/// </summary>
		///
		/// <param name="text">
		/// The message to be displayed. If the string consists of more than one line,
		/// you can separate the lines using a carriage return and/or linefeed character
		/// between each line.
		/// </param>
		///
		/// <param name="caption">
		/// The dialog box title. If this parameter is NULL, the default title is Error.
		/// </param>
		///
		/// <param name="button">
		/// To indicate the buttons displayed in the message box, specify one of the following values.
		/// </param>
		///
		/// <param name="icon">
		/// To display an icon in the message box, specify one of the following values.
		/// </param>
		///
		/// <param name="defaultButton">
		/// To indicate the default button, specify one of the following values.
		/// </param>
		///
		/// <param name="modality">
		/// To indicate the modality of the dialog box, specify one of the following values.
		/// </param>
		///
		/// <param name="options">
		/// To specify other options, use one or more of the following values.
		/// </param>
		///
		/// <param name="handle">
		/// A handle to the owner window of the message box to be created.
		/// If this parameter is NULL, the message box has no owner window.
		/// </param>
		///
		/// <remark>
		/// Adding two right-to-left marks (RLMs), represented by Unicode formatting
		/// character U+200F, in the beginning of a MessageBox display string is
		/// interpreted by the MessageBox rendering engine so as to cause the reading
		/// order of the MessageBox to be rendered as right-to-left (RTL).
		/// 
		/// When you use a system - modal message box to indicate that the
		/// system is low on memory, the strings pointed to by the text and
		/// caption parameters should not be taken from a resource file because
		/// an attempt to load the resource may fail.
		/// 
		/// If you create a message box while a dialog box is present,
		/// use a handle to the dialog box as the hWnd parameter. The handle
		/// parameter should not identify a child window, such as a
		/// control in a dialog box.
		/// </remark>
		///
		/// <returns>
		/// If a message box has a Cancel button, the function returns the MsgBoxResult::Cancel
		/// value if either the ESC key is pressed or the Cancel button is selected.
		/// If the message box has no Cancel button, pressing ESC will no effect-unless
		/// an MsgBoxButton::Ok button is present. If an MsgBoxButton::Ok button
		/// is displayed and the user presses ESC, the return value will be MsgBoxResult::Ok.
		/// 
		/// If the function fails, the return value is MsgBoxResult::Invalid.
		/// To get extended error information, call GetLastError.
		/// 
		/// If the function succeeds, the return value is one
		/// of the following menu - item values.
		/// </returns>
		static MsgBoxResult Show(const std::string & text,
								 const std::string & caption = "Error",
								 MsgBoxButton button = MsgBoxButton::Ok,
								 MsgBoxIcon icon = MsgBoxIcon::Error,
								 MsgBoxDefaultButton defaultButton = MsgBoxDefaultButton::Button1,
								 MsgBoxModality modality = MsgBoxModality::AppModal,
								 MsgBoxOptions options = MsgBoxOptions::SetForeground,
								 Windowhandle handle = nullptr);

	};

}