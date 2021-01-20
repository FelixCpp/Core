#include <Core/Rendering/Renderers/BitmapRenderer.hpp> // Core::BitmapRenderer
#include <Core/Rendering/FactoryManager.hpp>
#include <Core/System/Logger.hpp> // CORE_ERROR

namespace Core
{

	BitmapRenderer::BitmapRenderer() :
		dcRenderTarget(nullptr),
		bitmapRenderTarget(nullptr),
		hdc(nullptr)
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
		this->hdc = GetDC(handle);

		// Get the windows rectangle
		RECT wndRect = {};
		if (!GetClientRect(handle, &wndRect))
		{
			CORE_ERROR("Failed to get the windows size");
			return false;
		}

		// bind the dc to the DCRenderTarget
		this->dcRenderTarget->BindDC(this->hdc, &wndRect);

		// Create a BitmapRenderTarget
		hr = this->dcRenderTarget->CreateCompatibleRenderTarget(&this->bitmapRenderTarget);

		// error handling
		if (FAILED(hr))
		{
			CORE_ERROR("Failed to create a BitmapRenderTarget");
			return false;
		}
		return true;
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

	}

	ID2D1RenderTarget * BitmapRenderer::GetRenderTarget() const
	{
		return this->bitmapRenderTarget.Get();
	}

} // namespace Core