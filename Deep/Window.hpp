#ifndef DEP_WINDOW_HPP
#define DEEP_WINDOW_HPP

#include <Windows.h>
#include <d3d11.h>

#include <string>

#include "Debug.hpp"

namespace deep
{
	class Window
	{
	public:
		Window(unsigned int width, unsigned int height, const std::string& title = "Default Title") :
			m_uWidth(width), m_uHeight(height), m_sTitle(title), m_hInstance(GetModuleHandle(NULL))
		{
			initializeWnd(m_hInstance, SW_SHOW);
			initializeDx();
			ZeroMemory(&m_Msg, sizeof(MSG));
		}

		void display()
		{
			m_SwapChain->Present(0, 0);
		}

		bool isOpen() const
		{
			if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
			{
				if (m_Msg.message == WM_QUIT || m_bUserClose)  
					return false;

				TranslateMessage(&m_Msg);   

				DispatchMessage(&m_Msg);
			}


			return true;
		}

		void close()
		{
			m_bUserClose = true;
		}

		~Window()
		{
			m_SwapChain->Release();
			m_d3d11Device->Release();
			m_d3d11DevCon->Release();
		}

	private:
		WNDCLASSEX createClass(HINSTANCE hInstance)
		{
			WNDCLASSEX wc;

			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = CS_VREDRAW | CS_HREDRAW;
			wc.lpfnWndProc = WndProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = m_sTitle.c_str();
			wc.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);

			return wc;
		}

		bool initializeWnd(HINSTANCE hInstance, int showWnd)
		{
			WNDCLASSEX wc = createClass(hInstance);

			if (!RegisterClassEx(&wc))
			{
				deep::Console::LogError("Could not register window class.");
				return false;
			}

			m_hWnd = CreateWindowEx(0, m_sTitle.c_str(), m_sTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
				CW_USEDEFAULT, m_uWidth, m_uHeight, nullptr, nullptr, hInstance, nullptr);
			
			if (!m_hWnd)
			{
				deep::Console::LogError("Could not create window.");
				return false;
			}

			ShowWindow(m_hWnd, showWnd);
			UpdateWindow(m_hWnd);

#ifndef _DEBUG
			ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

			return true;
		}

		void initializeDx()
		{
			HRESULT hr;

			//Describe our Buffer
			DXGI_MODE_DESC bufferDesc;

			ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

			bufferDesc.Width = m_uWidth;
			bufferDesc.Height = m_uHeight;
			bufferDesc.RefreshRate.Numerator = 60;
			bufferDesc.RefreshRate.Denominator = 1;
			bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

			//Describe our SwapChain
			DXGI_SWAP_CHAIN_DESC swapChainDesc;

			ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

			swapChainDesc.BufferDesc = bufferDesc;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 1;
			swapChainDesc.OutputWindow = m_hWnd;
			swapChainDesc.Windowed = TRUE;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


			//Create our SwapChain
			hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
				D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_d3d11Device, nullptr, &m_d3d11DevCon);

			//Create our BackBuffer
			ID3D11Texture2D* BackBuffer;
			hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

			//Create our Render Target
			hr = m_d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &m_renderTargetView);
			BackBuffer->Release();

			//Set our Render Target
			m_d3d11DevCon->OMSetRenderTargets(1, &m_renderTargetView, NULL);
		}

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			}

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

	//private:
	public:
		HINSTANCE		m_hInstance;
		HWND			m_hWnd;
		mutable MSG		m_Msg;

		unsigned int	m_uWidth;
		unsigned int	m_uHeight;
		bool			m_bUserClose = false;
		
		IDXGISwapChain*			m_SwapChain;
		ID3D11Device*			m_d3d11Device;
		ID3D11DeviceContext*	m_d3d11DevCon;
		ID3D11RenderTargetView* m_renderTargetView;

		std::string		m_sTitle;
	};
}

#endif // DEEP_WINDOW_HPP