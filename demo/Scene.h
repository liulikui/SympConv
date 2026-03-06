#ifndef SCENE_H
#define SCENE_H

#include "Primitive.h"
#include "TRefCountPtr.h"
#include "RALResource.h"
#include <vector>
#include <memory>
#include <DirectXMath.h>
#include "SympConv/PhysicWorld.h"

// 前向声明
class IRALDevice;

// 为了方便使用，定义一个简化的命名空间别名
namespace dx = DirectX;

class Scene
{
public:
    // 构造函数
    Scene();
    
    // 初始化场景，创建根签名
    // 参数：
    //   pDevice - IRALDevice对象指针
    // 返回值：
    //   初始化是否成功
    bool Initialize(IRALDevice* pDevice);
    
    // 析构函数
    ~Scene();
    
    // 获取场景中Primitive对象的数量
    size_t GetPrimitiveCount() const
    {
        return mPrimitives.size();
    }

    // 更新场景中所有Primitive对象的状态
    void Update(float deltaTime);

    // 渲染场景
   // 参数：
   //   commandList - 命令队列
   //   viewMatrix - 视图矩阵
   //   projectionMatrix - 投影矩阵
    void Render(const dx::XMMATRIX& viewMatrix, const dx::XMMATRIX& projectionMatrix);

    // 添加一个Primitive对象到场景中
    // 参数：
    //   primitive - 要添加的Primitive对象指针
    // 返回值：
    //   添加是否成功
    bool AddPrimitive(Primitive* primitive);

    // 从场景中移除一个Primitive对象
    // 参数：
    //   primitive - 要移除的primitive对象指针
    // 返回值：
    //   移除是否成功
    bool RemovePrimitive(Primitive* primitive);

    // 清空场景中的所有对象
    void Clear();

    // 设置场景的背景颜色
    void SetBackgroundColor(const dx::XMFLOAT4& color)
    {
        mBackgroundColor = color;
    }

    // 获取场景的背景颜色
    const dx::XMFLOAT4& GetBackgroundColor() const
    {
        return mBackgroundColor;
    }

    // 设置场景的光源位置
    void SetLightPosition(const dx::XMFLOAT3& position)
    {
        mLightPosition = position;
    }

    // 获取场景的光源位置
    const dx::XMFLOAT3& GetLightPosition() const 
    {
        return mLightPosition;
    }

    // 设置场景的光源颜色
    void SetLightDiffuseColor(const dx::XMFLOAT4& color)
    {
        mLightDiffuseColor = color;
    }

    // 获取场景的光源颜色
    const dx::XMFLOAT4& GetLightDiffuseColor() const 
    { 
        return mLightDiffuseColor; 
    }
    
    // 设置场景的光源方向（自动归一化）
    void SetLightDirection(const dx::XMFLOAT3& direction);

    // 获取场景的光源方向
    const dx::XMFLOAT3& GetLightDirection() const 
    { 
        return mLightDirection; 
    }
    
    // 设置场景的环境光颜色
    void SetLightAmbientColor(const dx::XMFLOAT4& color)
    { 
        mLightAmbientColor = color; 
    }
    
    // 获取场景的环境光颜色
    const dx::XMFLOAT4& GetLightAmbientColor() const 
    { 
        return mLightAmbientColor; 
    }
    
    // 调整渲染资源大小
    void Resize(uint32_t width, uint32_t height);

private:
    struct AddPrimitiveRequest
    {
        Primitive* mPrimitive;
    };

    struct PrimitiveInfo
    {
        Primitive* mPrimitive;
        dx::XMMATRIX mWorldMatrix;
        bool mVisible;
        dx::XMFLOAT3 mDiffuseColor;
        dx::XMFLOAT3 mSpecularColor;
        float mShininess;
        TRefCountPtr<IRALVertexBuffer> mVertexBuffer;
        TRefCountPtr<IRALIndexBuffer> mIndexBuffer;
        TRefCountPtr<IRALConstBuffer> mConstBuffer;
    };

    void UpdatePrimitiveRequests();
    void UpdateSceneConstBuffer(IRALGraphicsCommandList* commandList, const dx::XMMATRIX& viewMatrix, const dx::XMMATRIX& projectionMatrix);
	void UpdatePrimitiveConstBuffer(IRALGraphicsCommandList* commandList, PrimitiveInfo* primitiveInfo);

    // 初始化延迟着色相关资源
    bool InitializeDeferredRendering();
    // 清理延迟着色相关资源
    void CleanupDeferredRendering();
    // 创建渲染目标、深度模板和相关视图资源
    bool CreateRenderingResources(uint32_t width, uint32_t height);
    // 释放所有渲染资源
    void ReleaseRenderingResources();
    // 初始化
    bool CreatePhysicWorld();
    void DestroyPhysicWorld();
    
    // 创建全屏四边形
    void CreateFullscreenQuad();

    // 执行几何阶段
    void ExecuteGeometryPass(const dx::XMMATRIX& viewMatrix, const dx::XMMATRIX& projectionMatrix);
    // 执行光照阶段
    void ExecuteLightingPass();
    // 执行GBuffer Resolve阶段
    void ExecuteResolvePass();
	// 执行色调映射阶段
    void ExecuteTonemappingPass();

private:
    // 渲染设备
    IRALDevice* mDevice;

    // 物理世界
    SympConv::PhysicWorld* mPhysicWorld;

    // 添加Primitive的请求列表
    std::vector<AddPrimitiveRequest> mAddPrimitiveRequests;

    // 场景中的所有Mesh对象
    std::vector<PrimitiveInfo> mPrimitives;

    // 场景的背景颜色
    dx::XMFLOAT4 mBackgroundColor; // 默认浅灰色背景

    // 光源属性
    dx::XMFLOAT3 mLightPosition; // 默认光源位置
    dx::XMFLOAT3 mLightDirection; // 默认光源方向
    dx::XMFLOAT4 mLightDiffuseColor;       // 默认光源颜色（白色）
    dx::XMFLOAT4 mLightSpecularColor;       // 默认光源颜色（白色）
    dx::XMFLOAT4 mLightAmbientColor;        // 默认环境光颜色

    // 延迟着色相关 - 几何阶段管道状态
    TRefCountPtr<IRALGraphicsPipelineState> mGbufferPipelineState;
    TRefCountPtr<IRALVertexShader> mGbufferVertexShader;
    TRefCountPtr<IRALPixelShader> mGbufferPixelShader;
    TRefCountPtr<IRALRootSignature> mGbufferRootSignature;

    // 延迟着色相关 - 光照阶段管道状态
    TRefCountPtr<IRALGraphicsPipelineState> mLightPipelineState;
    TRefCountPtr<IRALVertexShader> mLightVertexShader;
    TRefCountPtr<IRALPixelShader> mLightPixelShader;
    TRefCountPtr<IRALRootSignature> mLightRootSignature;
    
    // 延迟着色相关 - Resolve阶段管道状态
    TRefCountPtr<IRALGraphicsPipelineState> mResolvePipelineState;
    TRefCountPtr<IRALVertexShader> mResolveVertexShader;
    TRefCountPtr<IRALPixelShader> mResolvePixelShader;
    TRefCountPtr<IRALRootSignature> mResolveRootSignature;

    // GBuffer相关
    TRefCountPtr<IRALRenderTarget> mGbufferA; // RRG为世界空间法线
    TRefCountPtr<IRALRenderTarget> mGbufferB; // Metallic, Specular, Roughness
    TRefCountPtr<IRALRenderTarget> mGbufferC; // BaseColor RGB
    TRefCountPtr<IRALDepthStencil> mGbufferDepthStencil;
    
    // GBuffer对应的视图
    TRefCountPtr<IRALRenderTargetView> mGbufferARTV;
    TRefCountPtr<IRALRenderTargetView> mGbufferBRTV;
    TRefCountPtr<IRALRenderTargetView> mGbufferCRTV;
    TRefCountPtr<IRALDepthStencilView> mGbufferDSV;
    
    // GBuffer对应的SRV（用于光照阶段采样）
    TRefCountPtr<IRALShaderResourceView> mGbufferASRV;
    TRefCountPtr<IRALShaderResourceView> mGbufferBSRV;
    TRefCountPtr<IRALShaderResourceView> mGbufferCSRV;
    TRefCountPtr<IRALShaderResourceView> mGbufferDepthSRV;
    
    // 光照结果RT（用于保存Diffuse和Specular的光照计算结果）
    TRefCountPtr<IRALRenderTarget> mDiffuseLightRT; // 保存Diffuse光照计算结果
    TRefCountPtr<IRALRenderTarget> mSpecularLightRT; // 保存Specular光照计算结果
    
    // 光照结果RT对应的视图
    TRefCountPtr<IRALRenderTargetView> mDiffuseLightRTV;
    TRefCountPtr<IRALRenderTargetView> mSpecularLightRTV;
    TRefCountPtr<IRALShaderResourceView> mDiffuseLightSRV;
    TRefCountPtr<IRALShaderResourceView> mSpecularLightSRV;
    
    // HDR场景颜色渲染目标（用于延迟着色Resolve结果）
    TRefCountPtr<IRALRenderTarget> mHDRSceneColor;        // HDR场景颜色RT
    TRefCountPtr<IRALRenderTargetView> mHDRSceneColorRTV; // HDR场景颜色RTV
    TRefCountPtr<IRALShaderResourceView> mHDRSceneColorSRV; // HDR场景颜色SRV
    
    // 色调映射相关
    TRefCountPtr<IRALRootSignature> mTonemappingRootSignature;          // 色调映射根签名
    TRefCountPtr<IRALGraphicsPipelineState> mTonemappingPipelineState;   // 色调映射管线状态
    TRefCountPtr<IRALVertexShader> mTonemappingVS;                       // 色调映射顶点着色器
    TRefCountPtr<IRALPixelShader> mTonemappingPS;                        // 色调映射像素着色器
    
    TRefCountPtr<IRALConstBuffer> mLightPassConstBuffer;
    TRefCountPtr<IRALVertexBuffer> mFullscreenQuadVB;
    TRefCountPtr<IRALIndexBuffer> mFullscreenQuadIB;

    // 场景相关常量
    TRefCountPtr<IRALConstBuffer> mSceneConstBuffer;
};

#endif // SCENE_H