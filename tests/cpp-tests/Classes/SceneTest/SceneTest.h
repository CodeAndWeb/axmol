/****************************************************************************
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

#ifndef _SCENE_TEST_H_
#define _SCENE_TEST_H_

////----#include "cocos2d.h"
#include "../BaseTest.h"

DEFINE_TEST_SUITE(SceneTests);

class SceneTestLayer1 : public axis::Layer
{
public:
    CREATE_FUNC(SceneTestLayer1);

    SceneTestLayer1();
    ~SceneTestLayer1();

    virtual void onEnter() override;
    virtual void onEnterTransitionDidFinish() override;

    void testDealloc(float dt);
    void onPushScene(axis::Ref* sender);
    void onPushSceneTran(axis::Ref* sender);
    void onQuit(axis::Ref* sender);
};

class SceneTestLayer2 : public axis::Layer
{
    float _timeCounter;

public:
    CREATE_FUNC(SceneTestLayer2);

    SceneTestLayer2();

    void testDealloc(float dt);
    void onGoBack(axis::Ref* sender);
    void onReplaceScene(axis::Ref* sender);
    void onReplaceSceneTran(axis::Ref* sender);
};

class SceneTestLayer3 : public axis::LayerColor
{
public:
    CREATE_FUNC(SceneTestLayer3);

    SceneTestLayer3();

    virtual bool init() override;
    virtual void testDealloc(float dt);
    void item0Clicked(axis::Ref* sender);
    void item1Clicked(axis::Ref* sender);
    void item2Clicked(axis::Ref* sender);
    void item3Clicked(axis::Ref* sender);
};

class SceneTestScene : public TestCase
{
public:
    static SceneTestScene* create(int testIndex = 1);
};

#endif
