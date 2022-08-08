/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2022 Bytedance Inc.

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

#ifndef __tests__VideoPlayerTest__
#define __tests__VideoPlayerTest__

#include "../UIScene.h"

DEFINE_TEST_SUITE(VideoPlayerTests);

class VideoPlayerTest : public UIScene
{
public:
    CREATE_FUNC(VideoPlayerTest);

    virtual bool init() override;

    void menuCloseCallback(axis::Ref* sender);

    void menuRatioCallback(axis::Ref* sender);
    void menuResourceVideoCallback(axis::Ref* sender);
    void menuOnlineVideoCallback(axis::Ref* sender);

    void menuFullScreenCallback(axis::Ref* sender);
    void menuPauseCallback(axis::Ref* sender);
    void menuResumeCallback(axis::Ref* sender);
    void menuStopCallback(axis::Ref* sender);
    void menuHintCallback(axis::Ref* sender);
    void menuLoopCallback(axis::Ref* sender);

    void sliderCallback(axis::Ref* sender, axis::ui::Slider::EventType eventType);

    void videoEventCallback(axis::Ref* sender, axis::ui::VideoPlayer::EventType eventType);

    void onEnter() override;
    void onExit() override;

private:
    void createVideo();
    void createSlider();

    axis::ui::VideoPlayer* _videoPlayer = nullptr;
    axis::Label* _videoStateLabel;
    axis::Label* _loopStatusLabel;
    axis::Rect _visibleRect;
};

class SimpleVideoPlayerTest : public UIScene
{
public:
    CREATE_FUNC(SimpleVideoPlayerTest);

    SimpleVideoPlayerTest();

    virtual bool init() override;

    void menuCloseCallback(axis::Ref* sender);
    void switchStyleCallback(axis::Ref* sender);
    void switchUserInputCallback(axis::Ref* sender);

    void onEnter() override;
    void onExit() override;

private:
    void createVideo();

    axis::Rect _visibleRect;
    axis::ui::VideoPlayer* _videoPlayer = nullptr;

    axis::MenuItemFont* _switchUserInputEnabled;
    axis::MenuItemFont* _switchStyle;

    bool _userInputEnabled;
    axis::ui::VideoPlayer::StyleType _style;

    void updateButtonsTexts();
};

#endif  // __tests__VideoPlayerTest__
