#include <Core/Rendering/FrameSaver.hpp>
#include <Core/Rendering/Renderers/Renderer.hpp>
#include <Core/System/Logger.hpp>
#include <Core/System/FinalAction.hpp>

/// <summary>
/// Windows API
/// </summary>
#include <Windows.h>
#include <gdiplus.h>

/// <summary>
/// C++ / STL
/// </summary>
#include <vector>
#include <algorithm>
#include <thread>

namespace Core
{
	
	/// <summary>
	/// Returns the Clsid of the given format
	/// </summary>
	/// <param name="format">
	/// The Encoders format:
	///		image/bmp
	///		image/jpeg
	///		image/gif
	///		image/tiff
	///		image/png
	/// </param>
	/// <param name="pClsid">the clsid to store the value in</param>
	bool GetEncoderClsid(const std::string & format, OUT CLSID * pClsid)
	{
		const std::wstring wFormat(format.begin(), format.end());

		UINT num = 0;  // number of image encoders
		UINT size = 0; // size of the image encoder array in bytes

		Gdiplus::Status status = Gdiplus::GetImageEncodersSize(&num, &size);
		if (status != Gdiplus::Status::Ok || size == 0)
		{
			CORE_ERROR("Failed to get the image encoders size");
			return false;
		}

		std::vector<Gdiplus::ImageCodecInfo> pImageCodecInfo(size);

		status = Gdiplus::GetImageEncoders(num, size, &pImageCodecInfo[0]);
		if (status != Gdiplus::Status::Ok)
		{
			CORE_ERROR("Failed to get the image encoders");
			return false;
		}

		for (UINT i = 0; i < num; i++)
		{
			if (wcscmp(pImageCodecInfo[i].MimeType, wFormat.c_str()) == 0)
			{
				*pClsid = pImageCodecInfo[i].Clsid;
				return true;  // Success
			}
		}

		CORE_ERROR("Unknown format \"%s\"", format.c_str());
		return false;  // Failure	
	}

    FrameSaver::FrameSaver(LateRef<Renderer> renderer, LateRef<Windowhandle> windowHandle) :
		renderer(renderer),
		handle(windowHandle)
    {
	}

	bool FrameSaver::SaveFrame(const std::string & filepath)
	{
		// error handling
		if (!this->handle)
		{
			CORE_ERROR("There is no Windowhandle to grab the image from");
			return false;
		}

		// get the device context of the screen
		HDC hScreenDC = GetDC(this->handle);
		if (!hScreenDC)
		{
			CORE_ERROR("Failed to get the windows DeviceContext");
			return false;
		}

		const FinalAction hScreenDCRelease = [&]()
		{
			DeleteDC(hScreenDC);
		};

		// and a device context to put it in
		HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
		if (!hMemoryDC)
		{
			CORE_ERROR("Failed to create a compatible DeviceContext");
			return false;
		}

		const FinalAction hMemoryDCRelease = [&]()
		{
			DeleteDC(hMemoryDC);
		};

		// get the screen size
		RECT rect = {};
		if (!GetClientRect(this->handle, &rect))
		{
			CORE_ERROR("Failed to get the windows dimensions");
			return false;
		}

		const int width = rect.right - rect.left;
		const int height = rect.bottom - rect.top;

		// maybe worth checking these are positive values
		HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
		if (!hBitmap)
		{
			CORE_ERROR("Failed to create a compatible bitmap");
			return false;
		}

		// get a new bitmap
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);
		if (!hOldBitmap)
		{
			CORE_ERROR("Failed to select an object from memory as a Bitmap");
			return false;
		}

		// copy data into the dc
		if (!BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY))
		{
			CORE_ERROR("Failed to copy the data into another DC");
			return false;
		}

		// select another object as current
		hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);
		if (!hBitmap)
		{
			CORE_ERROR("Failed to select an object from the memory as a Bitmap");
			return false;
		}

		// now your image is held in hBitmap. You can save it or do whatever with it
		Gdiplus::Bitmap bitmap(hBitmap, nullptr);

		// find the last '.' character from the filepath
		const std::size_t dotPosition = filepath.find_last_of('.');
		if (dotPosition == std::string::npos)
		{
			CORE_ERROR("\"%s\" has no file extension like .png, .jpg etc.", filepath.c_str());
			return false;
		}

		// get the extension 'png', 'jpg' etc.
		std::string extension = filepath.substr(dotPosition + 1);

		// make the extension lowercased
		std::transform(extension.begin(), extension.end(), &extension[0], std::tolower);

		// create the clsid format
		const std::string clsidFormat = "image/" + extension;

		// Variable to store the encoder in
		CLSID encoder = {};
		if (!GetEncoderClsid(clsidFormat, &encoder))
		{
			// we don't need to log anything since GetEncoderClsid does that for us
			return false;
		}

		// convert the cstring to wstring
		const std::wstring wFilepath(filepath.begin(), filepath.end());

		// save the bitmap
		const Gdiplus::Status status = bitmap.Save(wFilepath.c_str(), &encoder);
		if (status != Gdiplus::Status::Ok)
		{
			CORE_ERROR("Failed to save the data to \"%s\"", filepath.c_str());
			return false;
		}

		return true;
	}

} // namespace Core