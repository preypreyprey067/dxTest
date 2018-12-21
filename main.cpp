#include "Deep\Window.hpp"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

deep::Window wnd{ 800, 600, "My Window" };

ID3D11Buffer* vertBuffer;
ID3D11VertexShader* vertShader;
ID3D11PixelShader* pxShader;
ID3D10Blob* vsBuffer;
ID3D10Blob* psBuffer;
ID3D11InputLayout* inLayout;
ID3D11RasterizerState* rasterizerStateSolidNoCulling;

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

bool InitScene()
{
	D3DCompileFromFile(L"Shaders.hlsl", nullptr, nullptr, "VS", "vs_4_0", 0, 0, &vsBuffer, 0);
	D3DCompileFromFile(L"Shaders.hlsl", nullptr, nullptr, "PS", "ps_4_0", 0, 0, &psBuffer, 0);

	wnd.m_d3d11Device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), nullptr, &vertShader);
	wnd.m_d3d11Device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), nullptr, &pxShader);
	
	wnd.m_d3d11DevCon->VSSetShader(vertShader, nullptr, 0);
	wnd.m_d3d11DevCon->PSSetShader(pxShader, nullptr, 0);

	float vpositions[] =
	{
		0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	D3D11_BUFFER_DESC bufDesc;
	ZeroMemory(&bufDesc, sizeof(bufDesc));

	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof(vpositions);
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;
	bufDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vBufData;
	ZeroMemory(&vBufData, sizeof(vBufData));

	vBufData.pSysMem = vpositions;
	wnd.m_d3d11Device->CreateBuffer(&bufDesc, &vBufData, &vertBuffer);

	UINT stride = 3 * sizeof(float);
	UINT offset = 0;
	wnd.m_d3d11DevCon->IASetVertexBuffers(0, 1, &vertBuffer, &stride, &offset);

	wnd.m_d3d11Device->CreateInputLayout(layout, 1,
		vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &inLayout);

	wnd.m_d3d11DevCon->IASetInputLayout(inLayout);
	wnd.m_d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.Width = wnd.m_uWidth;
	viewport.Height = wnd.m_uHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	wnd.m_d3d11DevCon->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC RasterizerDesc;
	ZeroMemory(&RasterizerDesc, sizeof(RasterizerDesc));
	RasterizerDesc.FrontCounterClockwise = FALSE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0.0f;
	RasterizerDesc.SlopeScaledDepthBias = 0.0f;
	RasterizerDesc.DepthClipEnable = FALSE;
	RasterizerDesc.ScissorEnable = FALSE;
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;

	// solid unculled
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	RasterizerDesc.CullMode = D3D11_CULL_NONE;
	wnd.m_d3d11Device->CreateRasterizerState(&RasterizerDesc, &rasterizerStateSolidNoCulling);

	wnd.m_d3d11DevCon->RSSetState(rasterizerStateSolidNoCulling);

	return true;
}

void UpdateScene()
{
	
}

int main()
{
	
	
	while (wnd.isOpen())
	{
		float bgColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		wnd.m_d3d11DevCon->ClearRenderTargetView(wnd.m_renderTargetView, bgColor);

		wnd.m_d3d11DevCon->Draw(3, 0);

		UpdateScene();
		wnd.display();
	}

	vertBuffer->Release();
	vertShader->Release();
	pxShader->Release();
	vsBuffer->Release();
	psBuffer->Release();
	inLayout->Release();


	return 0;
}
