//------------------------------------------------------------------------------
// Copyright (c) 2019 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include "InfoWidget.h"

using namespace djv;

void InfoWidget::_init(const std::shared_ptr<Core::Context>& context)
{
    ISettingsWidget::_init(context);

    _labels["SceneSizeW"] = UI::Label::create(context);
    _labels["SceneSizeW"]->setFont(AV::Font::familyMono);
    _labels["SceneSizeW"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneSizeW"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["SceneSizeH"] = UI::Label::create(context);
    _labels["SceneSizeH"]->setFont(AV::Font::familyMono);
    _labels["SceneSizeH"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneSizeH"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["SceneSizeD"] = UI::Label::create(context);
    _labels["SceneSizeD"]->setFont(AV::Font::familyMono);
    _labels["SceneSizeD"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneSizeD"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _layouts["SceneSize"] = UI::FormLayout::create(context);
    _layouts["SceneSize"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _layouts["SceneSize"]->setSpacing(UI::Layout::Spacing(UI::MetricsRole::SpacingSmall));
    _layouts["SceneSize"]->addChild(_labels["SceneSizeW"]);
    _layouts["SceneSize"]->addChild(_labels["SceneSizeH"]);
    _layouts["SceneSize"]->addChild(_labels["SceneSizeD"]);
    _bellows["SceneSize"] = UI::Bellows::create(context);
    _bellows["SceneSize"]->addChild(_layouts["SceneSize"]);

    _labels["SceneRangeMinX"] = UI::Label::create(context);
    _labels["SceneRangeMinX"]->setFont(AV::Font::familyMono);
    _labels["SceneRangeMinX"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneRangeMinX"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["SceneRangeMaxX"] = UI::Label::create(context);
    _labels["SceneRangeMaxX"]->setFont(AV::Font::familyMono);
    _labels["SceneRangeMaxX"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneRangeMaxX"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["SceneRangeMinY"] = UI::Label::create(context);
    _labels["SceneRangeMinY"]->setFont(AV::Font::familyMono);
    _labels["SceneRangeMinY"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneRangeMinY"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["SceneRangeMaxY"] = UI::Label::create(context);
    _labels["SceneRangeMaxY"]->setFont(AV::Font::familyMono);
    _labels["SceneRangeMaxY"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneRangeMaxY"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["SceneRangeMinZ"] = UI::Label::create(context);
    _labels["SceneRangeMinZ"]->setFont(AV::Font::familyMono);
    _labels["SceneRangeMinZ"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneRangeMinZ"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["SceneRangeMaxZ"] = UI::Label::create(context);
    _labels["SceneRangeMaxZ"]->setFont(AV::Font::familyMono);
    _labels["SceneRangeMaxZ"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["SceneRangeMaxZ"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _layouts["SceneRange"] = UI::FormLayout::create(context);
    _layouts["SceneRange"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _layouts["SceneRange"]->setSpacing(UI::Layout::Spacing(UI::MetricsRole::SpacingSmall));
    _layouts["SceneRange"]->addChild(_labels["SceneRangeMinX"]);
    _layouts["SceneRange"]->addChild(_labels["SceneRangeMaxX"]);
    _layouts["SceneRange"]->addChild(_labels["SceneRangeMinY"]);
    _layouts["SceneRange"]->addChild(_labels["SceneRangeMaxY"]);
    _layouts["SceneRange"]->addChild(_labels["SceneRangeMinZ"]);
    _layouts["SceneRange"]->addChild(_labels["SceneRangeMaxZ"]);
    _bellows["SceneRange"] = UI::Bellows::create(context);
    _bellows["SceneRange"]->addChild(_layouts["SceneRange"]);

    _labels["Primitives"] = UI::Label::create(context);
    _labels["Primitives"]->setFont(AV::Font::familyMono);
    _labels["Primitives"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["Primitives"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["PointCount"] = UI::Label::create(context);
    _labels["PointCount"]->setFont(AV::Font::familyMono);
    _labels["PointCount"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["PointCount"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _labels["FPS"] = UI::Label::create(context);
    _labels["FPS"]->setFont(AV::Font::familyMono);
    _labels["FPS"]->setTextHAlign(UI::TextHAlign::Left);
    _labels["FPS"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _layouts["Stats"] = UI::FormLayout::create(context);
    _layouts["Stats"]->setMargin(UI::Layout::Margin(UI::MetricsRole::MarginSmall));
    _layouts["Stats"]->setSpacing(UI::Layout::Spacing(UI::MetricsRole::SpacingSmall));
    _layouts["Stats"]->addChild(_labels["Primitives"]);
    _layouts["Stats"]->addChild(_labels["PointCount"]);
    _layouts["Stats"]->addChild(_labels["FPS"]);
    _bellows["Stats"] = UI::Bellows::create(context);
    _bellows["Stats"]->addChild(_layouts["Stats"]);

    auto layout = UI::VerticalLayout::create(context);
    layout->setSpacing(UI::Layout::Spacing(UI::MetricsRole::None));
    layout->addChild(_bellows["SceneSize"]);
    layout->addChild(_bellows["SceneRange"]);
    layout->addChild(_bellows["Stats"]);
    addChild(layout);

    auto weak = std::weak_ptr<InfoWidget>(std::dynamic_pointer_cast<InfoWidget>(shared_from_this()));
}

InfoWidget::InfoWidget()
{}

InfoWidget::~InfoWidget()
{}

std::shared_ptr<InfoWidget> InfoWidget::create(const std::shared_ptr<Core::Context>& context)
{
    auto out = std::shared_ptr<InfoWidget>(new InfoWidget);
    out->_init(context);
    return out;
}

void InfoWidget::setBBox(const Core::BBox3f& value)
{
    _bbox = value;
    _textUpdate();
}

void InfoWidget::setPrimitivesCount(size_t value)
{
    _primitivesCount = value;
    _textUpdate();
}

void InfoWidget::setPointCount(size_t value)
{
    _pointCount = value;
    _textUpdate();
}

void InfoWidget::setFPS(float value)
{
    _fps = value;
    _textUpdate();
}

void InfoWidget::_initEvent(Core::Event::Init&)
{
    _textUpdate();
}

void InfoWidget::_textUpdate()
{
    setTitle(_getText(DJV_TEXT("Information")));
    {
        std::stringstream ss;
        ss << _bbox.w();
        _labels["SceneSizeW"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.h();
        _labels["SceneSizeH"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.d();
        _labels["SceneSizeD"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.min.x;
        _labels["SceneRangeMinX"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.max.x;
        _labels["SceneRangeMaxX"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.min.y;
        _labels["SceneRangeMinY"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.max.y;
        _labels["SceneRangeMaxY"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.min.z;
        _labels["SceneRangeMinZ"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _bbox.max.z;
        _labels["SceneRangeMaxZ"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _primitivesCount;
        _labels["Primitives"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _pointCount;
        _labels["PointCount"]->setText(ss.str());
    }
    {
        std::stringstream ss;
        ss << _fps;
        _labels["FPS"]->setText(ss.str());
    }

    _layouts["SceneSize"]->setText(_labels["SceneSizeW"], _getText(DJV_TEXT("Width")) + ":");
    _layouts["SceneSize"]->setText(_labels["SceneSizeH"], _getText(DJV_TEXT("Height")) + ":");
    _layouts["SceneSize"]->setText(_labels["SceneSizeD"], _getText(DJV_TEXT("Depth")) + ":");
    _layouts["SceneRange"]->setText(_labels["SceneRangeMinX"], _getText(DJV_TEXT("Min X")) + ":");
    _layouts["SceneRange"]->setText(_labels["SceneRangeMaxX"], _getText(DJV_TEXT("Max X")) + ":");
    _layouts["SceneRange"]->setText(_labels["SceneRangeMinY"], _getText(DJV_TEXT("Min Y")) + ":");
    _layouts["SceneRange"]->setText(_labels["SceneRangeMaxY"], _getText(DJV_TEXT("Max Y")) + ":");
    _layouts["SceneRange"]->setText(_labels["SceneRangeMinZ"], _getText(DJV_TEXT("Min Z")) + ":");
    _layouts["SceneRange"]->setText(_labels["SceneRangeMaxZ"], _getText(DJV_TEXT("Max Z")) + ":");
    _layouts["Stats"]->setText(_labels["Primitives"], _getText(DJV_TEXT("Primitives")) + ":");
    _layouts["Stats"]->setText(_labels["PointCount"], _getText(DJV_TEXT("Point count")) + ":");
    _layouts["Stats"]->setText(_labels["FPS"], _getText(DJV_TEXT("FPS")) + ":");

    _bellows["SceneSize"]->setText(_getText(DJV_TEXT("Scene Size")));
    _bellows["SceneRange"]->setText(_getText(DJV_TEXT("Scene Range")));
    _bellows["Stats"]->setText(_getText(DJV_TEXT("Stats")));
}
