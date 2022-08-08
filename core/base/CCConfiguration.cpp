/****************************************************************************
Copyright (c) 2010      Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
Copyright (c) 2020-2021 C4games Ltd

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

#include "base/CCConfiguration.h"
#include "platform/CCFileUtils.h"
#include "base/CCEventCustom.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "renderer/backend/Device.h"

NS_AX_BEGIN

extern const char* axysVersion();

Configuration* Configuration::s_sharedConfiguration = nullptr;

const char* Configuration::CONFIG_FILE_LOADED = "config_file_loaded";

Configuration::Configuration()
    : _maxModelviewStackDepth(0)
    , _supportsPVRTC(false)
    , _supportsETC1(false)
    , _supportsETC2(false)
    , _supportsS3TC(false)
    , _supportsATITC(false)
    , _supportsASTC(false)
    , _supportsNPOT(false)
    , _supportsBGRA8888(false)
    , _supportsDiscardFramebuffer(false)
    , _supportsShareableVAO(false)
    , _supportsOESMapBuffer(false)
    , _supportsOESDepth24(false)
    , _supportsOESPackedDepthStencil(false)
    , _maxDirLightInShader(1)
    , _maxPointLightInShader(1)
    , _maxSpotLightInShader(1)
    , _animate3DQuality(Animate3DQuality::QUALITY_LOW)
{
    _loadedEvent = new EventCustom(CONFIG_FILE_LOADED);
}

bool Configuration::init()
{
    _valueDict["axys.version"] = Value(axysVersion());

#if AX_ENABLE_PROFILERS
    _valueDict["axys.compiled_with_profiler"] = Value(true);
#else
    _valueDict["axys.compiled_with_profiler"]       = Value(false);
#endif

#if AX_ENABLE_GL_STATE_CACHE == 0
    _valueDict["axys.compiled_with_gl_state_cache"] = Value(false);
#else
    _valueDict["axys.compiled_with_gl_state_cache"] = Value(true);
#endif

#if _AX_DEBUG
    _valueDict["axys.build_type"] = Value("DEBUG");
#else
    _valueDict["axys.build_type"]                   = Value("RELEASE");
#endif

    return true;
}

Configuration::~Configuration()
{
    AX_SAFE_DELETE(_loadedEvent);
}

std::string Configuration::getInfo() const
{
    // And Dump some warnings as well
#if AX_ENABLE_PROFILERS
    AXLOG(
        "cocos2d: **** WARNING **** AX_ENABLE_PROFILERS is defined. Disable it when you finish profiling (from "
        "ccConfig.h)\n");
#endif

#if AX_ENABLE_GL_STATE_CACHE == 0
    AXLOG(
        "cocos2d: **** WARNING **** AX_ENABLE_GL_STATE_CACHE is disabled. To improve performance, enable it (from "
        "ccConfig.h)\n");
#endif

    // Dump
    Value forDump = Value(_valueDict);
    return forDump.getDescription();
}

void Configuration::gatherGPUInfo()
{
    auto _deviceInfo = backend::Device::getInstance()->getDeviceInfo();
    AXLOG("Supported extensions: %s", _deviceInfo->getExtension());

    _valueDict["vendor"]   = Value(_deviceInfo->getVendor());
    _valueDict["renderer"] = Value(_deviceInfo->getRenderer());
    _valueDict["version"]  = Value(_deviceInfo->getVersion());

    _valueDict["max_texture_size"]      = Value(_deviceInfo->getMaxTextureSize());
    _valueDict["max_vertex_attributes"] = Value(_deviceInfo->getMaxAttributes());
    _valueDict["max_texture_units"]     = Value(_deviceInfo->getMaxTextureUnits());
    _valueDict["max_samples_allowed"]   = Value(_deviceInfo->getMaxSamplesAllowed());

    _supportsNPOT               = true;
    _valueDict["supports_NPOT"] = Value(_supportsNPOT);

    _supportsETC1               = _deviceInfo->checkForFeatureSupported(backend::FeatureType::ETC1);
    _valueDict["supports_ETC1"] = Value(_supportsETC1);

    _supportsETC2               = _deviceInfo->checkForFeatureSupported(backend::FeatureType::ETC2);
    _valueDict["supports_ETC2"] = Value(_supportsETC2);

    _supportsS3TC               = _deviceInfo->checkForFeatureSupported(backend::FeatureType::S3TC);
    _valueDict["supports_S3TC"] = Value(_supportsS3TC);

    _supportsATITC               = _deviceInfo->checkForFeatureSupported(backend::FeatureType::AMD_COMPRESSED_ATC);
    _valueDict["supports_ATITC"] = Value(_supportsATITC);

    _supportsASTC               = _deviceInfo->checkForFeatureSupported(backend::FeatureType::ASTC);
    _valueDict["supports_ASTC"] = Value(_supportsASTC);

    _supportsPVRTC               = _deviceInfo->checkForFeatureSupported(backend::FeatureType::PVRTC);
    _valueDict["supports_PVRTC"] = Value(_supportsPVRTC);

    _supportsBGRA8888               = _deviceInfo->checkForFeatureSupported(backend::FeatureType::IMG_FORMAT_BGRA8888);
    _valueDict["supports_BGRA8888"] = Value(_supportsBGRA8888);

    _supportsDiscardFramebuffer = _deviceInfo->checkForFeatureSupported(backend::FeatureType::DISCARD_FRAMEBUFFER);
    _valueDict["supports_discard_framebuffer"] = Value(_supportsDiscardFramebuffer);

    _supportsOESPackedDepthStencil = _deviceInfo->checkForFeatureSupported(backend::FeatureType::PACKED_DEPTH_STENCIL);
    _valueDict["supports_OES_packed_depth_stencil"] = Value(_supportsOESPackedDepthStencil);

    _supportsShareableVAO                      = _deviceInfo->checkForFeatureSupported(backend::FeatureType::VAO);
    _valueDict["supports_vertex_array_object"] = Value(_supportsShareableVAO);

    _supportsOESMapBuffer                 = _deviceInfo->checkForFeatureSupported(backend::FeatureType::MAPBUFFER);
    _valueDict["supports_OES_map_buffer"] = Value(_supportsOESMapBuffer);

    _supportsOESDepth24                = _deviceInfo->checkForFeatureSupported(backend::FeatureType::DEPTH24);
    _valueDict["supports_OES_depth24"] = Value(_supportsOESDepth24);

    _glExtensions = _deviceInfo->getExtension();
}

Configuration* Configuration::getInstance()
{
    if (!s_sharedConfiguration)
    {
        s_sharedConfiguration = new Configuration();
        s_sharedConfiguration->init();
    }

    return s_sharedConfiguration;
}

void Configuration::destroyInstance()
{
    AX_SAFE_RELEASE_NULL(s_sharedConfiguration);
}

bool Configuration::checkForGLExtension(std::string_view searchName) const
{
    return _glExtensions.find(searchName) != std::string::npos;
}

//
// getters for specific variables.
// Maintained for backward compatibility reasons only.
//
int Configuration::getMaxTextureSize() const
{
    auto _deviceInfo = backend::Device::getInstance()->getDeviceInfo();
    return _deviceInfo->getMaxTextureSize();
}

int Configuration::getMaxModelviewStackDepth() const
{
    return _maxModelviewStackDepth;
}

int Configuration::getMaxTextureUnits() const
{
    auto _deviceInfo = backend::Device::getInstance()->getDeviceInfo();
    return _deviceInfo->getMaxTextureUnits();
}

bool Configuration::supportsNPOT() const
{
    return _supportsNPOT;
}

bool Configuration::supportsPVRTC() const
{
    return _supportsPVRTC;
}

bool Configuration::supportsETC1() const
{
    return _supportsETC1;
}

bool Configuration::supportsETC2() const
{
    return _supportsETC2;
}

bool Configuration::supportsS3TC() const
{
    return _supportsS3TC;
}

bool Configuration::supportsATITC() const
{
    return _supportsATITC;
}

bool Configuration::supportsASTC() const
{
    return _supportsASTC;
}

bool Configuration::supportsBGRA8888() const
{
    return _supportsBGRA8888;
}

bool Configuration::supportsDiscardFramebuffer() const
{
    return _supportsDiscardFramebuffer;
}

bool Configuration::supportsShareableVAO() const
{
#if AX_TEXTURE_ATLAS_USE_VAO
    return _supportsShareableVAO;
#else
    return false;
#endif
}

bool Configuration::supportsMapBuffer() const
{
    // Fixes Github issue #16123
    //
    // XXX: Fixme. Should check GL ES and not iOS or Android
    // For example, linux could be compiled with GL ES. Or perhaps in the future Android will
    // support OpenGL. This is because glMapBufferOES() is an extension of OpenGL ES. And glMapBuffer()
    // is always implemented in OpenGL.

    // XXX: Warning. On iOS this is always `true`. Avoiding the comparison.
#if defined(AX_USE_GLES) && !defined(__APPLE__)
    return _supportsOESMapBuffer;
#else
    return true;
#endif
}

bool Configuration::supportsOESDepth24() const
{
    return _supportsOESDepth24;
}
bool Configuration::supportsOESPackedDepthStencil() const
{
    return _supportsOESPackedDepthStencil;
}

int Configuration::getMaxSupportDirLightInShader() const
{
    return _maxDirLightInShader;
}

int Configuration::getMaxSupportPointLightInShader() const
{
    return _maxPointLightInShader;
}

int Configuration::getMaxSupportSpotLightInShader() const
{
    return _maxSpotLightInShader;
}

Animate3DQuality Configuration::getAnimate3DQuality() const
{
    return _animate3DQuality;
}

//
// generic getters for properties
//
const Value& Configuration::getValue(std::string_view key, const Value& defaultValue) const
{
    auto iter = _valueDict.find(key);
    if (iter != _valueDict.cend())
        return iter->second;

    return defaultValue;
}

void Configuration::setValue(std::string_view key, const Value& value)
{
    hlookup::set_item(_valueDict, key, value);  // _valueDict[key] = value;
}

//
// load file
//
void Configuration::loadConfigFile(std::string_view filename)
{
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(filename);
    AXASSERT(!dict.empty(), "cannot create dictionary");

    // search for metadata
    bool validMetadata = false;
    auto metadataIter  = dict.find("metadata");
    if (metadataIter != dict.cend() && metadataIter->second.getType() == Value::Type::MAP)
    {

        const auto& metadata = metadataIter->second.asValueMap();
        auto formatIter      = metadata.find("format");

        if (formatIter != metadata.cend())
        {
            int format = formatIter->second.asInt();

            // Support format: 1
            if (format == 1)
            {
                validMetadata = true;
            }
        }
    }

    if (!validMetadata)
    {
        AXLOG("Invalid config format for file: %s", filename.data());
        return;
    }

    auto dataIter = dict.find("data");
    if (dataIter == dict.cend() || dataIter->second.getType() != Value::Type::MAP)
    {
        AXLOG("Expected 'data' dict, but not found. Config file: %s", filename.data());
        return;
    }

    // Add all keys in the existing dictionary

    const auto& dataMap = dataIter->second.asValueMap();
    for (const auto& dataMapIter : dataMap)
    {
        if (_valueDict.find(dataMapIter.first) == _valueDict.cend())
            _valueDict[dataMapIter.first] = dataMapIter.second;
        else
            AXLOG("Key already present. Ignoring '%s'", dataMapIter.first.c_str());
    }

    // light info
    std::string name = "axys.3d.max_dir_light_in_shader";
    if (_valueDict.find(name) != _valueDict.end())
        _maxDirLightInShader = _valueDict[name].asInt();
    else
        _valueDict[name] = Value(_maxDirLightInShader);

    name = "axys.3d.max_point_light_in_shader";
    if (_valueDict.find(name) != _valueDict.end())
        _maxPointLightInShader = _valueDict[name].asInt();
    else
        _valueDict[name] = Value(_maxPointLightInShader);

    name = "axys.3d.max_spot_light_in_shader";
    if (_valueDict.find(name) != _valueDict.end())
        _maxSpotLightInShader = _valueDict[name].asInt();
    else
        _valueDict[name] = Value(_maxSpotLightInShader);

    name = "axys.3d.animate_quality";
    if (_valueDict.find(name) != _valueDict.end())
        _animate3DQuality = (Animate3DQuality)_valueDict[name].asInt();
    else
        _valueDict[name] = Value((int)_animate3DQuality);

    Director::getInstance()->getEventDispatcher()->dispatchEvent(_loadedEvent);
}

int Configuration::getMaxAttributes() const
{
    auto _deviceInfo = backend::Device::getInstance()->getDeviceInfo();
    return _deviceInfo->getMaxAttributes();
}

NS_AX_END
