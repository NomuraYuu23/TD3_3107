#include "InputLayoutManager.h"

using namespace Microsoft::WRL;

std::array<D3D12_INPUT_LAYOUT_DESC, kInputLayoutIndexOfCount> InputLayoutManager::inputLayouts_;

std::array< std::vector<D3D12_INPUT_ELEMENT_DESC>, kInputLayoutIndexOfCount> InputLayoutManager::inputElementDescs_;

InputLayoutManager* InputLayoutManager::GetInstance()
{
	static InputLayoutManager instance;
	return &instance;
}

void InputLayoutManager::Initialize()
{

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (uint32_t i = 0; i < 3; i++) {
		inputElementDescs_[kInputLayoutIndexNormal].push_back(inputElementDescs[i]);
	}

	inputLayouts_[kInputLayoutIndexNormal].pInputElementDescs = inputElementDescs_[kInputLayoutIndexNormal].data();
	inputLayouts_[kInputLayoutIndexNormal].NumElements = static_cast<uint32_t>(inputElementDescs_[kInputLayoutIndexNormal].size());

	//Model
	D3D12_INPUT_ELEMENT_DESC inputElementModelDescs[10] = {};
	inputElementModelDescs[0].SemanticName = "POSITION";
	inputElementModelDescs[0].SemanticIndex = 0;
	inputElementModelDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementModelDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementModelDescs[1].SemanticName = "TEXCOORD";
	inputElementModelDescs[1].SemanticIndex = 0;
	inputElementModelDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementModelDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementModelDescs[2].SemanticName = "NORMAL";
	inputElementModelDescs[2].SemanticIndex = 0;
	inputElementModelDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementModelDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementModelDescs[3].SemanticName = "BLENDWEIGHT";
	inputElementModelDescs[3].SemanticIndex = 0;
	inputElementModelDescs[3].Format = DXGI_FORMAT_R32_FLOAT; // float
	inputElementModelDescs[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementModelDescs[4].SemanticName = "BLENDWEIGHT";
	inputElementModelDescs[4].SemanticIndex = 1;
	inputElementModelDescs[4].Format = DXGI_FORMAT_R32_FLOAT; // float
	inputElementModelDescs[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementModelDescs[5].SemanticName = "BLENDWEIGHT";
	inputElementModelDescs[5].SemanticIndex = 2;
	inputElementModelDescs[5].Format = DXGI_FORMAT_R32_FLOAT; // float
	inputElementModelDescs[5].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementModelDescs[6].SemanticName = "BLENDINDICES";
	inputElementModelDescs[6].SemanticIndex = 0;
	inputElementModelDescs[6].Format = DXGI_FORMAT_R32_UINT;
	inputElementModelDescs[6].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementModelDescs[7].SemanticName = "BLENDINDICES";
	inputElementModelDescs[7].SemanticIndex = 1;
	inputElementModelDescs[7].Format = DXGI_FORMAT_R32_UINT;
	inputElementModelDescs[7].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementModelDescs[8].SemanticName = "BLENDINDICES";
	inputElementModelDescs[8].SemanticIndex = 2;
	inputElementModelDescs[8].Format = DXGI_FORMAT_R32_UINT;
	inputElementModelDescs[8].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementModelDescs[9].SemanticName = "BLENDINDICES";
	inputElementModelDescs[9].SemanticIndex = 3;
	inputElementModelDescs[9].Format = DXGI_FORMAT_R32_UINT;
	inputElementModelDescs[9].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (uint32_t i = 0; i < 10; i++) {
		inputElementDescs_[kInputLayoutIndexModel].push_back(inputElementModelDescs[i]);
	}

	inputLayouts_[kInputLayoutIndexModel].pInputElementDescs = inputElementDescs_[kInputLayoutIndexModel].data();
	inputLayouts_[kInputLayoutIndexModel].NumElements = static_cast<uint32_t>(inputElementDescs_[kInputLayoutIndexModel].size());

	//Line
	D3D12_INPUT_ELEMENT_DESC inputElementLineDescs[2] = {};
	inputElementLineDescs[0].SemanticName = "POSITION";
	inputElementLineDescs[0].SemanticIndex = 0;
	inputElementLineDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementLineDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementLineDescs[1].SemanticName = "COLOR";
	inputElementLineDescs[1].SemanticIndex = 0;
	inputElementLineDescs[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementLineDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (uint32_t i = 0; i < 2; i++) {
		inputElementDescs_[kInputLayoutIndexLine].push_back(inputElementLineDescs[i]);
	}

	inputLayouts_[kInputLayoutIndexLine].pInputElementDescs = inputElementDescs_[kInputLayoutIndexLine].data();
	inputLayouts_[kInputLayoutIndexLine].NumElements = static_cast<uint32_t>(inputElementDescs_[kInputLayoutIndexLine].size());

}
