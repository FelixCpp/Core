#include <Core/Rendering/Renderers/BitmapRenderer.hpp> // Core::BitmapRenderer
#include <Core/Rendering/FactoryManager.hpp>
#include <Core/System/Logger.hpp> // CORE_ERROR

namespace Core
{

	BitmapRenderer::BitmapRenderer() :
		dcRenderTarget(nullptr),
		bitmapRenderTarget(nullptr),
		windowHandle(nullptr)
	{
	}

	bool BitmapRenderer::Initialize(Windowhandle handle)
	{
		// Create a PixelFormat object
		const D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_PREMULTIPLIED
		);

		// Create the RenderTarget properties
		const D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT, pixelFormat, 0.f, 0.f, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT
		);

		// Create a DC RenderTarget
		HRESULT hr = FactoryManager::d2dFactory->CreateDCRenderTarget(&properties, &this->dcRenderTarget);
		
		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a DCRenderTarget");
			return false;
		}

		// get a DC from the window
		HDC hdc = GetDC(handle);

		// Get the windows rectangle
		RECT wndRect = {};
		if (!GetClientRect(handle, &wndRect))
		{
			CORE_ERROR("Failed to get the windows size");
			return false;
		}

		// bind the dc to the DCRenderTarget
		this->dcRenderTarget->BindDC(hdc, &wndRect);

		// Create a BitmapRenderTarget
		hr = this->dcRenderTarget->CreateCompatibleRenderTarget(&this->bitmapRenderTarget);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a BitmapRenderTarget");
			return false;
		}

		this->windowHandle = handle;

		return true;
	}

	void BitmapRenderer::Destroy()
	{
		this->dcRenderTarget.Reset();
		this->bitmapRenderTarget.Reset();
	}

	void BitmapRenderer::BeginDraw()
	{
		this->bitmapRenderTarget->BeginDraw();
	}

	void BitmapRenderer::EndDraw()
	{
		this->bitmapRenderTarget->EndDraw();

		// get the bitmap
		Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap = nullptr;
		this->bitmapRenderTarget->GetBitmap(&bitmap);
	
		this->dcRenderTarget->BeginDraw();
		this->dcRenderTarget->DrawBitmap(bitmap.Get());
		this->dcRenderTarget->EndDraw();
	}

	void BitmapRenderer::ResizeViewport(u32_t width, u32_t height)
	{
		// Get a DC from the Windowhandle
		HDC hdc = GetDC(this->windowHandle);

		// Get the size of the window
		RECT windowBoundary = {};
		GetClientRect(this->windowHandle, &windowBoundary);

		// bind the dc and the size
		this->dcRenderTarget->BindDC(hdc, &windowBoundary);

		// Recreate the BitmapRenderTarget
		this->dcRenderTarget->CreateCompatibleRenderTarget(&this->bitmapRenderTarget);
		
		// Release the DC
		ReleaseDC(this->windowHandle, hdc);
	}

	ID2D1RenderTarget * BitmapRenderer::GetRenderTarget() const
	{
		return this->bitmapRenderTarget.Get();
	}

} // namespace Core