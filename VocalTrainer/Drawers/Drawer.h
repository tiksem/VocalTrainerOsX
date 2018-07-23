//
// Created by Semyon Tikhonenko on 6/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#ifndef VOCALTRAINER_DRAWER_H
#define VOCALTRAINER_DRAWER_H

#include <array>
#include <vector>
#include "RoundedRect.h"
#include "Point.h"
#include "DrawerColor.h"
#include <string>

class Drawer {
public:
    enum LineJoin {
        BEVEL, ROUND, MITER
    };

    enum TextAlign {
        CENTER, LEFT, RIGHT
    };

    enum TextBaseline {
        TOP, BOTTOM, MIDDLE
    };

    typedef DrawerColor Color;

    virtual void clear() = 0;

    virtual void translate(float x, float y) = 0;
    virtual float getTranslateX() = 0;
    virtual float getTranslateY() = 0;
    virtual void translateTo(float x, float y) = 0;

    virtual void beginFrame(float width, float height, float devicePixelRatio) = 0;
    virtual void endFrame() = 0;
    virtual void moveTo(float x, float y) = 0;
    virtual void lineTo(float x, float y) = 0;
    virtual void arcTo(float x1, float y1, float x2, float y2, float radius) = 0;
    virtual void setStrokeColor(int color);
    virtual void setFillColor(int color);
    virtual void setStrokeColor(const Color& color) = 0;
    virtual void setFillColor(const Color& color) = 0;
    virtual void setStrokeWidth(float strokeWidth) = 0;
    virtual void stroke() = 0;
    virtual void fill() = 0;
    virtual void beginPath() = 0;
    virtual void closePath() = 0;
    virtual void bezierCurveTo(float c1x, float c1y, float c2x, float c2y, float x, float y) = 0;
    virtual void quadraticCurveTo(float cpx, float cpy, float x, float y) = 0;
    virtual void lineJoin(LineJoin type) = 0;
    virtual void rotate(float angle) = 0;
    virtual void scale(float x, float y) = 0;
    virtual void rect(float x, float y, float w, float h) = 0;
    virtual void fillRect(float x, float y, float w, float h) = 0;
    virtual void drawLine(float x1, float y1, float x2, float y2);
    virtual void drawVerticalLine(float x, float y, float height);

    virtual void roundedRect(float x, float y, float w, float h, float r);
    virtual void roundedRectDifferentCorners(float x, float y, float w,
            float h, float radiusLeftTop,
            float radiusRightTop, float radiusBottomRight, float radiusBottomLeft);
    void roundedRect(const CppUtils::RoundedRectF& roundedRect);
    virtual void lineTo(const CppUtils::PointF& point);
    virtual void moveTo(const CppUtils::PointF& point);

    virtual void setTextFont(const std::string& fontFamily, int fontSize);
    virtual void setTextAlign(TextAlign align);
    virtual void setTextBaseline(TextBaseline baseline);
    virtual void fillText(const std::string &text, float x, float y) = 0;

    virtual ~Drawer() = default;

protected:
    std::string fontFamily;
    int fontSize = 14;
    TextBaseline textBaseline = MIDDLE;
    TextAlign textAlign = LEFT;
};


#endif //VOCALTRAINER_DRAWER_H