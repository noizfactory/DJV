// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvViewApp/DebugWidget.h>

#include <djvViewApp/FileSystem.h>
#include <djvViewApp/Media.h>

#include <djvUIComponents/LineGraphWidget.h>
#include <djvUIComponents/ThermometerWidget.h>

#include <djvUI/Bellows.h>
#include <djvUI/EventSystem.h>
#include <djvUI/IconSystem.h>
#include <djvUI/Label.h>
#include <djvUI/RowLayout.h>
#include <djvUI/ScrollWidget.h>

#include <djvRender2D/FontSystem.h>
#include <djvRender2D/Render.h>

#include <djvAV/IO.h>
#include <djvAV/ThumbnailSystem.h>

#include <djvSystem/Context.h>
#include <djvSystem/TimerFunc.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewApp
    {
        namespace
        {
            class IDebugWidget : public UI::Widget
            {
            public:
                virtual ~IDebugWidget() = 0;
                
            protected:
                void _preLayoutEvent(System::Event::PreLayout&) override;
                void _layoutEvent(System::Event::Layout&) override;

                void _initEvent(System::Event::Init&) override;

                virtual void _widgetUpdate() = 0;

                std::map<std::string, std::shared_ptr<UI::Text::Label> > _labels;
                std::map<std::string, std::shared_ptr<UIComponents::LineGraphWidget> > _lineGraphs;
                std::map<std::string, std::shared_ptr<UIComponents::ThermometerWidget> > _thermometerWidgets;
                std::shared_ptr<UI::VerticalLayout> _layout;
                std::shared_ptr<System::Timer> _timer;
            };
            
            IDebugWidget::~IDebugWidget()
            {}

            void IDebugWidget::_preLayoutEvent(System::Event::PreLayout&)
            {
                _setMinimumSize(_layout->getMinimumSize());
            }

            void IDebugWidget::_layoutEvent(System::Event::Layout&)
            {
                _layout->setGeometry(getGeometry());
            }

            void IDebugWidget::_initEvent(System::Event::Init& event)
            {
                if (event.getData().text)
                {
                    _widgetUpdate();
                }
            }

            class GeneralDebugWidget : public IDebugWidget
            {
                DJV_NON_COPYABLE(GeneralDebugWidget);

            protected:
                void _init(const std::shared_ptr<System::Context>&);
                GeneralDebugWidget();

            public:
                static std::shared_ptr<GeneralDebugWidget> create(const std::shared_ptr<System::Context>&);

            protected:
                void _widgetUpdate() override;
            };

            void GeneralDebugWidget::_init(const std::shared_ptr<System::Context>& context)
            {
                IDebugWidget::_init(context);

                setClassName("djv::ViewApp::GeneralDebugWidget");

                _labels["FPS"] = UI::Text::Label::create(context);
                _labels["FPSValue"] = UI::Text::Label::create(context);
                _labels["FPSValue"]->setFontFamily(Render2D::Font::familyMono);
                _lineGraphs["FPS"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["FPS"]->setPrecision(2);

                _labels["TotalSystemTime"] = UI::Text::Label::create(context);
                _labels["TotalSystemTimeValue"] = UI::Text::Label::create(context);
                _lineGraphs["TotalSystemTime"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["TotalSystemTime"]->setPrecision(0);

                _labels["TopSystemTime"] = UI::Text::Label::create(context);
                _labels["TopSystemTimeValue"] = UI::Text::Label::create(context);
                _lineGraphs["TopSystemTime"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["TopSystemTime"]->setPrecision(0);

                _labels["ObjectCount"] = UI::Text::Label::create(context);
                _labels["ObjectCountValue"] = UI::Text::Label::create(context);
                _labels["ObjectCountValue"]->setFontFamily(Render2D::Font::familyMono);
                _lineGraphs["ObjectCount"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["ObjectCount"]->setPrecision(0);

                _labels["WidgetCount"] = UI::Text::Label::create(context);
                _labels["WidgetCountValue"] = UI::Text::Label::create(context);
                _labels["WidgetCountValue"]->setFontFamily(Render2D::Font::familyMono);
                _lineGraphs["WidgetCount"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["WidgetCount"]->setPrecision(0);

                _labels["Hover"] = UI::Text::Label::create(context);
                _labels["Grab"] = UI::Text::Label::create(context);
                _labels["KeyGrab"] = UI::Text::Label::create(context);
                _labels["TextFocus"] = UI::Text::Label::create(context);

                _labels["GlyphCache"] = UI::Text::Label::create(context);
                _labels["GlyphCacheValue"] = UI::Text::Label::create(context);
                _labels["GlyphCacheValue"]->setFontFamily(Render2D::Font::familyMono);
                _thermometerWidgets["GlyphCache"] = UIComponents::ThermometerWidget::create(context);

                _labels["ThumbnailInfoCache"] = UI::Text::Label::create(context);
                _labels["ThumbnailInfoCacheValue"] = UI::Text::Label::create(context);
                _labels["ThumbnailInfoCacheValue"]->setFontFamily(Render2D::Font::familyMono);
                _thermometerWidgets["ThumbnailInfoCache"] = UIComponents::ThermometerWidget::create(context);

                _labels["ThumbnailImageCache"] = UI::Text::Label::create(context);
                _labels["ThumbnailImageCacheValue"] = UI::Text::Label::create(context);
                _labels["ThumbnailInfoCacheValue"]->setFontFamily(Render2D::Font::familyMono);
                _thermometerWidgets["ThumbnailImageCache"] = UIComponents::ThermometerWidget::create(context);

                _labels["IconCache"] = UI::Text::Label::create(context);
                _labels["IconCacheValue"] = UI::Text::Label::create(context);
                _labels["IconCacheValue"]->setFontFamily(Render2D::Font::familyMono);
                _thermometerWidgets["IconCache"] = UIComponents::ThermometerWidget::create(context);

                for (auto& i : _labels)
                {
                    i.second->setTextHAlign(UI::TextHAlign::Left);
                }

                _layout = UI::VerticalLayout::create(context);
                _layout->setMargin(UI::MetricsRole::Margin);
                auto hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["FPS"]);
                hLayout->addChild(_labels["FPSValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["FPS"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["TotalSystemTime"]);
                hLayout->addChild(_labels["TotalSystemTimeValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["TotalSystemTime"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["TopSystemTime"]);
                hLayout->addChild(_labels["TopSystemTimeValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["TopSystemTime"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["ObjectCount"]);
                hLayout->addChild(_labels["ObjectCountValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["ObjectCount"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["WidgetCount"]);
                hLayout->addChild(_labels["WidgetCountValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["WidgetCount"]);
                _layout->addChild(_labels["Hover"]);
                _layout->addChild(_labels["Grab"]);
                _layout->addChild(_labels["KeyGrab"]);
                _layout->addChild(_labels["TextFocus"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["GlyphCache"]);
                hLayout->addChild(_labels["GlyphCacheValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_thermometerWidgets["GlyphCache"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["ThumbnailInfoCache"]);
                hLayout->addChild(_labels["ThumbnailInfoCacheValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_thermometerWidgets["ThumbnailInfoCache"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["ThumbnailImageCache"]);
                hLayout->addChild(_labels["ThumbnailImageCacheValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_thermometerWidgets["ThumbnailImageCache"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["IconCache"]);
                hLayout->addChild(_labels["IconCacheValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_thermometerWidgets["IconCache"]);
                addChild(_layout);

                _timer = System::Timer::create(context);
                _timer->setRepeating(true);
                auto weak = std::weak_ptr<GeneralDebugWidget>(std::dynamic_pointer_cast<GeneralDebugWidget>(shared_from_this()));
                _timer->start(
                    System::getTimerDuration(System::TimerValue::Medium),
                    [weak](const std::chrono::steady_clock::time_point&, const Time::Duration&)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_widgetUpdate();
                    }
                });
            }

            GeneralDebugWidget::GeneralDebugWidget()
            {}

            std::shared_ptr<GeneralDebugWidget> GeneralDebugWidget::create(const std::shared_ptr<System::Context>& context)
            {
                auto out = std::shared_ptr<GeneralDebugWidget>(new GeneralDebugWidget);
                out->_init(context);
                return out;
            }

            void GeneralDebugWidget::_widgetUpdate()
            {
                if (auto context = getContext().lock())
                {
                    const float fps = context->getFPSAverage();
                    const auto& systemTickTimes = context->getSystemTickTimes();
                    Time::Duration totalSystemTime = Time::Duration::zero();
                    for (const auto& i : systemTickTimes)
                    {
                        totalSystemTime += i.second;
                    }
                    std::string topSystemTime;
                    Time::Duration topSystemTimeValue = Time::Duration::zero();
                    if (systemTickTimes.size())
                    {
                        auto i = systemTickTimes.begin();
                        topSystemTime = i->first;
                        topSystemTimeValue = i->second;
                    }
                    const size_t objectCount = IObject::getGlobalObjectCount();
                    const size_t widgetCount = UI::Widget::getGlobalWidgetCount();
                    auto eventSystem = context->getSystemT<UI::EventSystem>();
                    auto fontSystem = context->getSystemT<Render2D::Font::FontSystem>();
                    const float glyphCachePercentage = fontSystem->getGlyphCachePercentage();
                    auto thumbnailSystem = context->getSystemT<AV::ThumbnailSystem>();
                    const float thumbnailInfoCachePercentage = thumbnailSystem->getInfoCachePercentage();
                    const float thumbnailImageCachePercentage = thumbnailSystem->getImageCachePercentage();
                    auto iconSystem = context->getSystemT<UI::IconSystem>();
                    const float iconCachePercentage = iconSystem->getCachePercentage();

                    _lineGraphs["FPS"]->addSample(fps);
                    _lineGraphs["TotalSystemTime"]->addSample(totalSystemTime.count());
                    _lineGraphs["TopSystemTime"]->addSample(topSystemTimeValue.count());
                    _lineGraphs["ObjectCount"]->addSample(objectCount);
                    _lineGraphs["WidgetCount"]->addSample(widgetCount);
                    _thermometerWidgets["ThumbnailInfoCache"]->setPercentage(thumbnailInfoCachePercentage);
                    _thermometerWidgets["ThumbnailImageCache"]->setPercentage(thumbnailImageCachePercentage);
                    _thermometerWidgets["IconCache"]->setPercentage(iconCachePercentage);
                    _thermometerWidgets["GlyphCache"]->setPercentage(glyphCachePercentage);

                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_fps")) << ":";
                        _labels["FPS"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss.precision(2);
                        ss << std::fixed << fps;
                        _labels["FPSValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_total_system_time")) << ":";
                        _labels["TotalSystemTime"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << std::fixed << totalSystemTime.count();
                        _labels["TotalSystemTimeValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_top_system_time")) << ":";
                        _labels["TopSystemTime"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << std::fixed << topSystemTime << ", " << topSystemTimeValue.count();
                        _labels["TopSystemTimeValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_object_count")) << ":";
                        _labels["ObjectCount"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << objectCount;
                        _labels["ObjectCountValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_widget_count")) << ":";
                        _labels["WidgetCount"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << widgetCount;
                        _labels["WidgetCountValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        auto object = eventSystem->observeHover()->get();
                        ss << _getText(DJV_TEXT("debug_general_hover")) << ": " << (object ? object->getClassName() : _getText(DJV_TEXT("debug_general_hover_none")));
                        _labels["Hover"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        auto object = eventSystem->observeGrab()->get();
                        ss << _getText(DJV_TEXT("debug_general_grab")) << ": " << (object ? object->getClassName() : _getText(DJV_TEXT("debug_general_grab_none")));
                        _labels["Grab"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        auto object = eventSystem->observeKeyGrab()->get();
                        ss << _getText(DJV_TEXT("debug_general_key_grab")) << ": " << (object ? object->getClassName() : _getText(DJV_TEXT("debug_general_key_grab_none")));
                        _labels["KeyGrab"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        std::string name;
                        if (auto object = eventSystem->getTextFocus().lock())
                        {
                            name = object->getClassName();
                        }
                        else
                        {
                            name = _getText(DJV_TEXT("debug_general_text_focus_none"));
                        }
                        ss << _getText(DJV_TEXT("debug_general_text_focus")) << ": " << name;
                        _labels["TextFocus"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_font_system_glyph_cache")) << ":";
                        _labels["GlyphCache"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss.precision(2);
                        ss << std::fixed << glyphCachePercentage << "%";
                        _labels["GlyphCacheValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_thumbnail_system_information_cache")) << ":";
                        _labels["ThumbnailInfoCache"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss.precision(2);
                        ss << std::fixed << thumbnailInfoCachePercentage << "%";
                        _labels["ThumbnailInfoCacheValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_thumbnail_system_image_cache")) << ":";
                        _labels["ThumbnailImageCache"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss.precision(2);
                        ss << std::fixed << thumbnailImageCachePercentage << "%";
                        _labels["ThumbnailImageCacheValue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_general_icon_system_cache")) << ":";
                        _labels["IconCache"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss.precision(2);
                        ss << std::fixed << iconCachePercentage << "%";
                        _labels["IconCacheValue"]->setText(ss.str());
                    }
                }
            }

            class RenderDebugWidget : public IDebugWidget
            {
                DJV_NON_COPYABLE(RenderDebugWidget);

            protected:
                void _init(const std::shared_ptr<System::Context>&);
                RenderDebugWidget();

            public:
                static std::shared_ptr<RenderDebugWidget> create(const std::shared_ptr<System::Context>&);

            protected:
                void _widgetUpdate() override;
            };

            void RenderDebugWidget::_init(const std::shared_ptr<System::Context>& context)
            {
                IDebugWidget::_init(context);

                setClassName("djv::ViewApp::RenderDebugWidget");

                _labels["Primitives"] = UI::Text::Label::create(context);
                _labels["PrimitivesValue"] = UI::Text::Label::create(context);
                _labels["PrimitivesValue"]->setFontFamily(Render2D::Font::familyMono);
                _lineGraphs["Primitives"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["Primitives"]->setPrecision(0);

                _labels["TextureAtlas"] = UI::Text::Label::create(context);
                _labels["TextureAtlasValue"] = UI::Text::Label::create(context);
                _labels["TextureAtlasValue"]->setFontFamily(Render2D::Font::familyMono);
                _thermometerWidgets["TextureAtlas"] = UIComponents::ThermometerWidget::create(context);

                _labels["DynamicTextureCount"] = UI::Text::Label::create(context);
                _labels["DynamicTextureCountValue"] = UI::Text::Label::create(context);
                _labels["DynamicTextureCountValue"]->setFontFamily(Render2D::Font::familyMono);
                _lineGraphs["DynamicTextureCount"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["DynamicTextureCount"]->setPrecision(0);

                _labels["VBOSize"] = UI::Text::Label::create(context);
                _labels["VBOSizeValue"] = UI::Text::Label::create(context);
                _labels["VBOSizeValue"]->setFontFamily(Render2D::Font::familyMono);
                _lineGraphs["VBOSize"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["VBOSize"]->setPrecision(0);

                for (auto& i : _labels)
                {
                    i.second->setTextHAlign(UI::TextHAlign::Left);
                }

                _layout = UI::VerticalLayout::create(context);
                _layout->setMargin(UI::MetricsRole::Margin);
                auto hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["Primitives"]);
                hLayout->addChild(_labels["PrimitivesValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["Primitives"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["TextureAtlas"]);
                hLayout->addChild(_labels["TextureAtlasValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_thermometerWidgets["TextureAtlas"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["DynamicTextureCount"]);
                hLayout->addChild(_labels["DynamicTextureCountValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["DynamicTextureCount"]);
                hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["VBOSize"]);
                hLayout->addChild(_labels["VBOSizeValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_lineGraphs["VBOSize"]);
                addChild(_layout);

                _timer = System::Timer::create(context);
                _timer->setRepeating(true);
                auto weak = std::weak_ptr<RenderDebugWidget>(std::dynamic_pointer_cast<RenderDebugWidget>(shared_from_this()));
                _timer->start(
                    System::getTimerDuration(System::TimerValue::Medium),
                    [weak](const std::chrono::steady_clock::time_point&, const Time::Duration&)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_widgetUpdate();
                    }
                });
            }

            RenderDebugWidget::RenderDebugWidget()
            {}

            std::shared_ptr<RenderDebugWidget> RenderDebugWidget::create(const std::shared_ptr<System::Context>& context)
            {
                auto out = std::shared_ptr<RenderDebugWidget>(new RenderDebugWidget);
                out->_init(context);
                return out;
            }

            void RenderDebugWidget::_widgetUpdate()
            {
                const auto& render = _getRender();
                const size_t primitives = render->getPrimitivesCount();
                const float textureAtlasPercentage = render->getTextureAtlasPercentage();
                const size_t dynamicTextureCount = render->getDynamicTextureCount();
                const size_t vboSize = render->getVBOSize();

                _lineGraphs["Primitives"]->addSample(primitives);
                _thermometerWidgets["TextureAtlas"]->setPercentage(textureAtlasPercentage);
                _lineGraphs["DynamicTextureCount"]->addSample(dynamicTextureCount);
                _lineGraphs["VBOSize"]->addSample(vboSize);

                {
                    std::stringstream ss;
                    ss << _getText(DJV_TEXT("debug_render_primitives")) << ":";
                    _labels["Primitives"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss << primitives;
                    _labels["PrimitivesValue"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss << _getText(DJV_TEXT("debug_render_texture_atlas")) << ":";
                    _labels["TextureAtlas"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss.precision(2);
                    ss << std::fixed << textureAtlasPercentage << "%";
                    _labels["TextureAtlasValue"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss << _getText(DJV_TEXT("debug_render_dynamic_texture_count")) << ":";
                    _labels["DynamicTextureCount"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss << dynamicTextureCount;
                    _labels["DynamicTextureCountValue"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss << _getText(DJV_TEXT("debug_render_vbo_size")) << ":";
                    _labels["VBOSize"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss << vboSize;
                    _labels["VBOSizeValue"]->setText(ss.str());
                }
            }

            class MediaDebugWidget : public UI::Widget
            {
                DJV_NON_COPYABLE(MediaDebugWidget);

            protected:
                void _init(const std::shared_ptr<System::Context>&);
                MediaDebugWidget();

            public:
                static std::shared_ptr<MediaDebugWidget> create(const std::shared_ptr<System::Context>&);

            protected:
                void _preLayoutEvent(System::Event::PreLayout&) override;
                void _layoutEvent(System::Event::Layout&) override;

                void _initEvent(System::Event::Init&) override;

            private:
                void _widgetUpdate();

                Math::Frame::Sequence _sequence;
                Math::Frame::Index _currentFrame = 0;
                size_t _videoQueueMax = 0;
                size_t _videoQueueCount = 0;
                size_t _audioQueueMax = 0;
                size_t _audioQueueCount = 0;
                std::map<std::string, std::shared_ptr<UI::Text::Label> > _labels;
                std::map<std::string, std::shared_ptr<UIComponents::LineGraphWidget> > _lineGraphs;
                std::shared_ptr<UI::VerticalLayout> _layout;
                std::shared_ptr<Observer::Value<std::shared_ptr<Media> > > _currentMediaObserver;
                std::shared_ptr<Observer::Value<Math::Frame::Sequence> > _sequenceObserver;
                std::shared_ptr<Observer::Value<Math::Frame::Index> > _currentFrameObserver;
                std::shared_ptr<Observer::Value<size_t> > _videoQueueMaxObserver;
                std::shared_ptr<Observer::Value<size_t> > _videoQueueCountObserver;
                std::shared_ptr<Observer::Value<size_t> > _audioQueueMaxObserver;
                std::shared_ptr<Observer::Value<size_t> > _audioQueueCountObserver;
            };

            void MediaDebugWidget::_init(const std::shared_ptr<System::Context>& context)
            {
                Widget::_init(context);

                setClassName("djv::ViewApp::MediaDebugWidget");

                _labels["CurrentFrame"] = UI::Text::Label::create(context);
                _labels["CurrentFrameValue"] = UI::Text::Label::create(context);
                _labels["CurrentFrameValue"]->setFontFamily(Render2D::Font::familyMono);
                
                _labels["VideoQueue"] = UI::Text::Label::create(context);
                _lineGraphs["VideoQueue"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["VideoQueue"]->setPrecision(0);

                _labels["AudioQueue"] = UI::Text::Label::create(context);
                _lineGraphs["AudioQueue"] = UIComponents::LineGraphWidget::create(context);
                _lineGraphs["AudioQueue"]->setPrecision(0);

                for (auto& i : _labels)
                {
                    i.second->setTextHAlign(UI::TextHAlign::Left);
                }

                _layout = UI::VerticalLayout::create(context);
                _layout->setMargin(UI::MetricsRole::Margin);
                auto hLayout = UI::HorizontalLayout::create(context);
                hLayout->addChild(_labels["CurrentFrame"]);
                hLayout->addChild(_labels["CurrentFrameValue"]);
                _layout->addChild(hLayout);
                _layout->addChild(_labels["VideoQueue"]);
                _layout->addChild(_lineGraphs["VideoQueue"]);
                _layout->addChild(_labels["AudioQueue"]);
                _layout->addChild(_lineGraphs["AudioQueue"]);
                addChild(_layout);

                auto weak = std::weak_ptr<MediaDebugWidget>(std::dynamic_pointer_cast<MediaDebugWidget>(shared_from_this()));
                if (auto fileSystem = context->getSystemT<FileSystem>())
                {
                    _currentMediaObserver = Observer::Value<std::shared_ptr<Media>>::create(
                        fileSystem->observeCurrentMedia(),
                        [weak](const std::shared_ptr<Media> & value)
                    {
                        if (auto widget = weak.lock())
                        {
                            for (const auto& i : widget->_lineGraphs)
                            {
                                i.second->resetSamples();
                            }

                            if (value)
                            {
                                widget->_sequenceObserver = Observer::Value<Math::Frame::Sequence>::create(
                                    value->observeSequence(),
                                    [weak](const Math::Frame::Sequence& value)
                                {
                                    if (auto widget = weak.lock())
                                    {
                                        widget->_sequence = value;
                                        widget->_widgetUpdate();
                                    }
                                });
                                widget->_currentFrameObserver = Observer::Value<Math::Frame::Index>::create(
                                    value->observeCurrentFrame(),
                                    [weak](Math::Frame::Index value)
                                {
                                    if (auto widget = weak.lock())
                                    {
                                        widget->_currentFrame = value;
                                        widget->_widgetUpdate();
                                    }
                                });
                                widget->_videoQueueMaxObserver = Observer::Value<size_t>::create(
                                    value->observeVideoQueueMax(),
                                    [weak](size_t value)
                                {
                                    if (auto widget = weak.lock())
                                    {
                                        widget->_videoQueueMax = value;
                                        widget->_widgetUpdate();
                                    }
                                });
                                widget->_videoQueueCountObserver = Observer::Value<size_t>::create(
                                    value->observeVideoQueueCount(),
                                    [weak](size_t value)
                                    {
                                        if (auto widget = weak.lock())
                                        {
                                            widget->_videoQueueCount = value;
                                            widget->_lineGraphs["VideoQueue"]->addSample(value);
                                            widget->_widgetUpdate();
                                        }
                                    });
                                widget->_audioQueueMaxObserver = Observer::Value<size_t>::create(
                                    value->observeAudioQueueMax(),
                                    [weak](size_t value)
                                {
                                    if (auto widget = weak.lock())
                                    {
                                        widget->_audioQueueMax = value;
                                        widget->_widgetUpdate();
                                    }
                                });
                                widget->_audioQueueCountObserver = Observer::Value<size_t>::create(
                                    value->observeAudioQueueCount(),
                                    [weak](size_t value)
                                {
                                    if (auto widget = weak.lock())
                                    {
                                        widget->_audioQueueCount = value;
                                        widget->_lineGraphs["AudioQueue"]->addSample(value);
                                        widget->_widgetUpdate();
                                    }
                                });
                            }
                            else
                            {
                                widget->_sequence = Math::Frame::Sequence();
                                widget->_currentFrame = 0;
                                widget->_videoQueueMax = 0;
                                widget->_videoQueueCount = 0;
                                widget->_audioQueueMax = 0;
                                widget->_audioQueueCount = 0;
                                widget->_sequenceObserver.reset();
                                widget->_currentFrameObserver.reset();
                                widget->_videoQueueMaxObserver.reset();
                                widget->_videoQueueCountObserver.reset();
                                widget->_audioQueueMaxObserver.reset();
                                widget->_audioQueueCountObserver.reset();
                                widget->_widgetUpdate();
                            }
                        }
                    });
                }
            }

            MediaDebugWidget::MediaDebugWidget()
            {}

            std::shared_ptr<MediaDebugWidget> MediaDebugWidget::create(const std::shared_ptr<System::Context>& context)
            {
                auto out = std::shared_ptr<MediaDebugWidget>(new MediaDebugWidget);
                out->_init(context);
                return out;
            }

            void MediaDebugWidget::_preLayoutEvent(System::Event::PreLayout&)
            {
                _setMinimumSize(_layout->getMinimumSize());
            }

            void MediaDebugWidget::_layoutEvent(System::Event::Layout&)
            {
                _layout->setGeometry(getGeometry());
            }

            void MediaDebugWidget::_initEvent(System::Event::Init& event)
            {
                if (event.getData().text)
                {
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_media_video_queue")) << ":";
                        _labels["VideoQueue"]->setText(ss.str());
                    }
                    {
                        std::stringstream ss;
                        ss << _getText(DJV_TEXT("debug_media_audio_queue")) << ":";
                        _labels["AudioQueue"]->setText(ss.str());
                    }
                    _widgetUpdate();
                }
            }

            void MediaDebugWidget::_widgetUpdate()
            {
                {
                    std::stringstream ss;
                    ss << _getText(DJV_TEXT("debug_media_current_time")) << ":";
                    _labels["CurrentFrame"]->setText(ss.str());
                }
                {
                    std::stringstream ss;
                    ss << _currentFrame << " / " << _sequence.getFrameCount();
                    _labels["CurrentFrameValue"]->setText(ss.str());
                }
            }

        } // namespace

        struct DebugWidget::Private
        {
            std::map < std::string, std::shared_ptr<UI::Bellows> > bellows;
        };

        void DebugWidget::_init(const std::shared_ptr<System::Context>& context)
        {
            MDIWidget::_init(context);

            DJV_PRIVATE_PTR();
            setClassName("djv::ViewApp::DebugWidget");

            auto layout = UI::VerticalLayout::create(context);
            layout->setSpacing(UI::MetricsRole::None);

            auto generalDebugWidget = GeneralDebugWidget::create(context);
            p.bellows["General"] = UI::Bellows::create(context);
            p.bellows["General"]->addChild(generalDebugWidget);
            p.bellows["General"]->setOpen(true, false);
            layout->addChild(p.bellows["General"]);

            auto renderDebugWidget = RenderDebugWidget::create(context);
            p.bellows["Render"] = UI::Bellows::create(context);
            p.bellows["Render"]->addChild(renderDebugWidget);
            layout->addChild(p.bellows["Render"]);

            auto mediaDebugWidget = MediaDebugWidget::create(context);
            p.bellows["Media"] = UI::Bellows::create(context);
            p.bellows["Media"]->addChild(mediaDebugWidget);
            layout->addChild(p.bellows["Media"]);

            auto scrollWidget = UI::ScrollWidget::create(UI::ScrollType::Vertical, context);
            scrollWidget->setBorder(false);
            scrollWidget->setBackgroundRole(UI::ColorRole::Background);
            scrollWidget->setShadowOverlay({ UI::Side::Top });
            scrollWidget->addChild(layout);
            addChild(scrollWidget);
        }

        DebugWidget::DebugWidget() :
            _p(new Private)
        {}

        DebugWidget::~DebugWidget()
        {}

        std::shared_ptr<DebugWidget> DebugWidget::create(const std::shared_ptr<System::Context>& context)
        {
            auto out = std::shared_ptr<DebugWidget>(new DebugWidget);
            out->_init(context);
            return out;
        }

        std::map<std::string, bool> DebugWidget::getBellowsState() const
        {
            DJV_PRIVATE_PTR();
            std::map<std::string, bool> out;
            for (const auto& i : p.bellows)
            {
                out[i.first] = i.second->isOpen();
            }
            return out;
        }

        void DebugWidget::setBellowsState(const std::map<std::string, bool>& value)
        {
            DJV_PRIVATE_PTR();
            for (const auto& i : value)
            {
                const auto j = p.bellows.find(i.first);
                if (j != p.bellows.end())
                {
                    j->second->setOpen(i.second, false);
                }
            }
        }

        void DebugWidget::_initEvent(System::Event::Init & event)
        {
            MDIWidget::_initEvent(event);
            DJV_PRIVATE_PTR();
            if (event.getData().text)
            {
                setTitle(_getText(DJV_TEXT("debug_title")));
                p.bellows["General"]->setText(_getText(DJV_TEXT("debug_section_general")));
                p.bellows["Render"]->setText(_getText(DJV_TEXT("debug_section_render")));
                p.bellows["Media"]->setText(_getText(DJV_TEXT("debug_section_media")));
            }
        }

    } // namespace ViewApp
} // namespace djv

