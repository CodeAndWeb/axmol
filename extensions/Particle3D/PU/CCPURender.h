/****************************************************************************
 Copyright (C) 2013 Henry van Merode. All rights reserved.
 Copyright (c) 2015-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 https://axys1.github.io/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#pragma once

#include <vector>

#include "base/CCRef.h"
#include "math/CCMath.h"
#include "extensions/Particle3D/CCParticle3DRender.h"
#include "renderer/CCRenderState.h"
#include "renderer/backend/Types.h"
#include "renderer/backend/Buffer.h"

NS_AX_BEGIN

// particle render for quad
struct PUParticle3D;

class AX_EX_DLL PURender : public Particle3DRender
{
public:
    virtual void prepare(){};
    virtual void unPrepare(){};
    virtual void updateRender(PUParticle3D* particle, float deltaTime, bool firstParticle);

    std::string_view getRenderType() const { return _renderType; };
    void setRenderType(std::string_view observerType) { _renderType = observerType; };

    virtual PURender* clone() = 0;
    void copyAttributesTo(PURender* render);

public:
    bool autoRotate;  // Means that the objects to render automatically rotate if the node to which the particle system
                      // is attached to rotates.

protected:
    std::string _renderType;
};

class AX_EX_DLL PUParticle3DEntityRender : public PURender
{
public:
    void copyAttributesTo(PUParticle3DEntityRender* render);
    virtual void reset() override;
    PUParticle3DEntityRender();
    virtual ~PUParticle3DEntityRender();

protected:
    bool initRender(std::string_view texFile);

    void onBeforeDraw();

    void onAfterDraw();

protected:
    struct VertexInfo
    {
        Vec3 position;
        Vec2 uv;
        Vec4 color;
    };

    MeshCommand _meshCommand;

    RenderState::StateBlock _stateBlock;
    Texture2D* _texture                  = nullptr;
    backend::ProgramState* _programState = nullptr;
    backend::Buffer* _indexBuffer        = nullptr;  // index buffer
    backend::Buffer* _vertexBuffer       = nullptr;  // vertex buffer

    std::vector<VertexInfo> _vertices;
    std::vector<uint16_t> _indices;

    std::string _texFile;

    backend::UniformLocation _locColor;
    backend::UniformLocation _locTexture;
    backend::UniformLocation _locPMatrix;

    // renderer state cache variables
    bool _rendererDepthTestEnabled                 = true;
    backend::CompareFunction _rendererDepthCmpFunc = backend::CompareFunction::LESS;
    backend::CullMode _rendererCullMode            = backend::CullMode::BACK;
    backend::Winding _rendererWinding              = backend::Winding::COUNTER_CLOCK_WISE;
    bool _rendererDepthWrite                       = false;
};

class AX_EX_DLL PUParticle3DQuadRender : public PUParticle3DEntityRender
{
public:
    enum Type
    {
        POINT,
        ORIENTED_COMMON,
        ORIENTED_SELF,
        ORIENTED_SHAPE,
        PERPENDICULAR_COMMON,
        PERPENDICULAR_SELF,
    };

    enum Origin
    {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        CENTER_LEFT,
        CENTER,
        CENTER_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT
    };

    enum RotateType
    {
        TEXTURE_COORDS,
        VERTEX
    };

    static PUParticle3DQuadRender* create(std::string_view texFile = "");

    void setType(Type type);
    Type getType() const { return _type; }
    void setOrigin(Origin origin) { _origin = origin; }
    Origin getOrigin() const { return _origin; }
    void setRotateType(RotateType type) { _rotateType = type; }
    RotateType getRotateType() const { return _rotateType; }
    void setCommonDirection(const Vec3& dir) { _commonDir = dir; }
    const Vec3& getCommonDirection() const { return _commonDir; }
    void setCommonUp(const Vec3& up) { _commonUp = up; }
    const Vec3& getCommonUp() const { return _commonUp; }

    unsigned short getTextureCoordsRows() const;
    void setTextureCoordsRows(unsigned short textureCoordsRows);
    unsigned short getTextureCoordsColumns() const;
    void setTextureCoordsColumns(unsigned short textureCoordsColumns);
    unsigned int getNumTextureCoords();

    virtual void render(Renderer* renderer, const Mat4& transform, ParticleSystem3D* particleSystem) override;

    virtual PUParticle3DQuadRender* clone() override;
    void copyAttributesTo(PUParticle3DQuadRender* render);

    PUParticle3DQuadRender();
    virtual ~PUParticle3DQuadRender();

protected:
    void getOriginOffset(int& offsetX, int& offsetY);
    void determineUVCoords(PUParticle3D* particle);
    void fillVertex(unsigned short index, const Vec3& pos, const Vec4& color, const Vec2& uv);
    void fillTriangle(unsigned short index, unsigned short v0, unsigned short v1, unsigned short v2);

protected:
    Type _type;
    Origin _origin;
    RotateType _rotateType;
    Vec3 _commonDir;
    Vec3 _commonUp;

    unsigned short _textureCoordsRows;
    unsigned short _textureCoordsColumns;
    float _textureCoordsRowStep;
    float _textureCoordsColStep;
};

// particle render for MeshRenderer
class AX_EX_DLL PUParticle3DModelRender : public PURender
{
public:
    static PUParticle3DModelRender* create(std::string_view modelFile, std::string_view texFile = "");

    virtual void render(Renderer* renderer, const Mat4& transform, ParticleSystem3D* particleSystem) override;

    virtual PUParticle3DModelRender* clone() override;
    void copyAttributesTo(PUParticle3DModelRender* render);

    virtual void reset() override;
    PUParticle3DModelRender();
    virtual ~PUParticle3DModelRender();

protected:
    std::vector<MeshRenderer*> _meshList;
    std::string _modelFile;
    std::string _texFile;
    Vec3 _meshSize;
};

class AX_EX_DLL PUParticle3DBoxRender : public PUParticle3DEntityRender
{
public:
    static PUParticle3DBoxRender* create(std::string_view texFile = "");

    virtual void render(Renderer* renderer, const Mat4& transform, ParticleSystem3D* particleSystem) override;

    virtual PUParticle3DBoxRender* clone() override;

    PUParticle3DBoxRender();
    virtual ~PUParticle3DBoxRender();

protected:
    void reBuildIndices(unsigned short count);
};

class AX_EX_DLL PUSphereRender : public PUParticle3DEntityRender
{
public:
    static PUSphereRender* create(std::string_view texFile = "");

    virtual void render(Renderer* renderer, const Mat4& transform, ParticleSystem3D* particleSystem) override;

    virtual PUSphereRender* clone() override;
    void copyAttributesTo(PUSphereRender* render);

    PUSphereRender();
    virtual ~PUSphereRender();

protected:
    void buildBuffers(unsigned short count);

protected:
    unsigned short _numberOfRings;
    unsigned short _numberOfSegments;
    std::vector<VertexInfo> _vertexTemplate;
};

NS_AX_END
