/****************************************************************************
 Copyright (c) 2013-2016 Chukong Technologies Inc.
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

#include "2d/CCNode.h"
#include "renderer/CCGroupCommand.h"
#include "renderer/CCCustomCommand.h"

NS_AX_BEGIN

class GridBase;
/**
 *  @addtogroup _2d
 *  @{
 */

/**
 * @brief Base class for Grid Node.
 */

class AX_DLL NodeGrid : public Node
{
public:
    /** Create a Grid Node.
     *
     * @return An autorelease Grid Node.
     */
    static NodeGrid* create();

    static NodeGrid* create(const Rect& rect);

    /** Get a Grid Node.
     *
     * @return Return a GridBase.
     */
    GridBase* getGrid() { return _nodeGrid; }
    /**
     * @js NA
     */
    const GridBase* getGrid() const { return _nodeGrid; }

    /**
     * Changes a grid object that is used when applying effects.
     *
     * @param grid  A Grid object that is used when applying effects.
     */
    void setGrid(GridBase* grid);

    /** Set the Grid Target.
     *
     * @param target A Node is used to set the Grid Target.
     */
    void setTarget(Node* target);

    /**
     * @brief Set the effect grid rect.
     * @param gridRect The effect grid rect.
     */
    void setGridRect(const Rect& gridRect) { _gridRect = gridRect; }
    /**
     * @brief Get the effect grid rect.
     * @return Return the effect grid rect.
     */
    const Rect& getGridRect() const { return _gridRect; }

    // overrides
    virtual void visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

    NodeGrid();
    virtual ~NodeGrid();

protected:
    void onGridBeginDraw();
    void onGridEndDraw();

    Node* _gridTarget   = nullptr;
    GridBase* _nodeGrid = nullptr;
    GroupCommand _groupCommand;
    CustomCommand _gridBeginCommand;
    CustomCommand _gridEndCommand;

    Rect _gridRect = Rect::ZERO;

private:
    AX_DISALLOW_COPY_AND_ASSIGN(NodeGrid);
};
/** @} */
NS_AX_END
