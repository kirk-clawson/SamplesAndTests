#pragma once

#include "DirectXHelper.h"
#include <DirectXMath.h>
#include <d2d1.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wincodec.h>
#include <wrl\client.h>

namespace DX
{
	// Provides an interface for an application that owns DeviceResources to be notified of the device being lost or created.
	class IDeviceNotify
	{
	public:
		virtual void OnDeviceLost() = 0;
		virtual void OnDeviceRestored() = 0;
	};

	// Controls all the DirectX device resources.
	class DeviceResources
	{
	public:
		DeviceResources();
		void SetWindow(HWND window);
		void SetLogicalSize(DX::Size logicalSize);
		void SetDpi(float dpi);
		void ValidateDevice();
		void HandleDeviceLost();
		void RegisterDeviceNotify(IDeviceNotify* deviceNotify);
		void Trim();
		void Present();

		// Device Accessors.
		DX::Size GetOutputSize() const { return m_outputSize; }
		DX::Size GetLogicalSize() const { return m_logicalSize; }

		// D3D Accessors.
		ID3D11Device2*			GetD3DDevice() const { return m_d3dDevice.Get(); }
		ID3D11DeviceContext2*	GetD3DDeviceContext() const { return m_d3dContext.Get(); }
		IDXGISwapChain1*		GetSwapChain() const { return m_swapChain.Get(); }
		D3D_FEATURE_LEVEL		GetDeviceFeatureLevel() const { return m_d3dFeatureLevel; }
		ID3D11RenderTargetView*	GetBackBufferRenderTargetView() const { return m_d3dRenderTargetView.Get(); }
		ID3D11DepthStencilView* GetDepthStencilView() const { return m_d3dDepthStencilView.Get(); }
		D3D11_VIEWPORT			GetScreenViewport() const { return m_screenViewport; }
		DirectX::XMFLOAT4X4		GetOrientationTransform3D() const { return m_orientationTransform3D; }

		// D2D Accessors.
		ID2D1Factory2*			GetD2DFactory() const { return m_d2dFactory.Get(); }
		ID2D1Device1*			GetD2DDevice() const { return m_d2dDevice.Get(); }
		ID2D1DeviceContext1*	GetD2DDeviceContext() const { return m_d2dContext.Get(); }
		ID2D1Bitmap1*			GetD2DTargetBitmap() const { return m_d2dTargetBitmap.Get(); }
		IDWriteFactory2*		GetDWriteFactory() const { return m_dwriteFactory.Get(); }
		IWICImagingFactory2*	GetWicImagingFactory() const { return m_wicFactory.Get(); }
		D2D1::Matrix3x2F		GetOrientationTransform2D() const { return m_orientationTransform2D; }

	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		DXGI_MODE_ROTATION ComputeDisplayRotation();

		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D11Device2>			m_d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext2>	m_d3dContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain1>			m_swapChain;

		// Direct3D rendering objects. Required for 3D.
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_d3dRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	m_d3dDepthStencilView;
		D3D11_VIEWPORT									m_screenViewport;

		// Direct2D drawing components.
		Microsoft::WRL::ComPtr<ID2D1Factory2>		m_d2dFactory;
		Microsoft::WRL::ComPtr<ID2D1Device1>		m_d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext1>	m_d2dContext;
		Microsoft::WRL::ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		// DirectWrite drawing components.
		Microsoft::WRL::ComPtr<IDWriteFactory2>		m_dwriteFactory;
		Microsoft::WRL::ComPtr<IWICImagingFactory2>	m_wicFactory;

		// Cached reference to the Window.
		HWND				m_window;

		// Cached device properties.
		D3D_FEATURE_LEVEL	m_d3dFeatureLevel;
		DX::Size			m_d3dRenderTargetSize;
		DX::Size			m_outputSize;
		DX::Size			m_logicalSize;
		float				m_dpi;

		// Transforms used for display orientation.
		D2D1::Matrix3x2F	m_orientationTransform2D;
		DirectX::XMFLOAT4X4	m_orientationTransform3D;

		// The IDeviceNotify can be held directly as it owns the DeviceResources.
		IDeviceNotify* m_deviceNotify;
	};
}