#include "DX12RALResource.h"
#include "DX12RALDevice.h"
#include "RALCommandList.h"
#include "DX12RALCommandList.h"
#include <d3d12.h>
#include <iostream>

// DX12RALRenderTargetView析构函数实现
DX12RALRenderTargetView::~DX12RALRenderTargetView()
{
	if (mDevice && mRtvCPUHandle.ptr != 0)
	{
		mDevice->ReleaseRTVDescriptor(mRtvCPUHandle, mRtvIndex, mRtvHeap.Get());
	}
}

// DX12RALDepthStencilView析构函数实现
DX12RALDepthStencilView::~DX12RALDepthStencilView()
{
	if (mDevice && mDsvCPUHandle.ptr != 0)
	{
		mDevice->ReleaseDSVDescriptor(mDsvCPUHandle, mDsvIndex, mDsvHeap.Get());
	}
}

// DX12RALShaderResourceView析构函数实现
DX12RALShaderResourceView::~DX12RALShaderResourceView()
{
	if (mDevice && mSrvCPUHandle.ptr != 0)
	{
		mDevice->ReleaseSRVDescriptor(mSrvCPUHandle, mSrvIndex, mSrvHeap.Get());
	}
}
