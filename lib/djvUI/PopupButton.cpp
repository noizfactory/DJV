// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvUI/PopupButton.h>

#include <djvUI/Action.h>
#include <djvUI/Border.h>
#include <djvUI/LayoutUtil.h>
#include <djvUI/MenuButton.h>
#include <djvUI/Overlay.h>
#include <djvUI/PopupLayout.h>
#include <djvUI/PopupWidget.h>
#include <djvUI/StackLayout.h>
#include <djvUI/Style.h>
#include <djvUI/Window.h>

#include <djvRender2D/Render.h>

#include <djvSystem/Context.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        namespace Button
        {
            struct Popup::Private
            {
                MenuButtonStyle buttonStyle = MenuButtonStyle::Flat;
                UI::Popup popupDefault = UI::Popup::BelowRight;
                bool capturePointer = true;
                bool captureKeyboard = true;

                std::shared_ptr<Widget> widget;
                std::shared_ptr<Button::Menu> button;
                std::shared_ptr<UI::Window> window;

                std::function<std::shared_ptr<Widget>(void)> openCallback;
                std::function<void(const std::shared_ptr<Widget>&)> closeCallback;
            };

            void Popup::_init(MenuButtonStyle buttonStyle, const std::shared_ptr<System::Context>& context)
            {
                Widget::_init(context);
                DJV_PRIVATE_PTR();

                setClassName("djv::UI::Button::Popup");

                p.buttonStyle = buttonStyle;

                p.button = Button::Menu::create(buttonStyle, context);
                if (MenuButtonStyle::ComboBox == p.buttonStyle)
                {
                    p.button->setTextFocusEnabled(true);
                    p.button->setBackgroundRole(ColorRole::Button);
                }
                Widget::addChild(p.button);

                auto weak = std::weak_ptr<Popup>(std::dynamic_pointer_cast<Popup>(shared_from_this()));
                p.button->setOpenCallback(
                    [weak](bool value)
                    {
                        if (auto widget = weak.lock())
                        {
                            if (value)
                            {
                                widget->open();
                            }
                            else
                            {
                                widget->close();
                            }
                        }
                    });
            }

            Popup::Popup() :
                _p(new Private)
            {}

            Popup::~Popup()
            {
                DJV_PRIVATE_PTR();
                if (p.window)
                {
                    p.window->close();
                }
            }

            std::shared_ptr<Popup> Popup::create(MenuButtonStyle buttonStyle, const std::shared_ptr<System::Context>& context)
            {
                auto out = std::shared_ptr<Popup>(new Popup);
                out->_init(buttonStyle, context);
                return out;
            }

            void Popup::open()
            {
                DJV_PRIVATE_PTR();
                if (auto context = getContext().lock())
                {
                    if (p.openCallback)
                    {
                        p.widget = p.openCallback();

                        auto popupWidget = PopupWidget::create(context);
                        popupWidget->addChild(p.widget);

                        auto popupLayout = Layout::Popup::create(context);
                        popupLayout->setPopupDefault(p.popupDefault);
                        popupLayout->addChild(popupWidget);
                        popupLayout->setButton(p.button);

                        auto overlay = Layout::Overlay::create(context);
                        overlay->setCapturePointer(p.capturePointer);
                        overlay->setCaptureKeyboard(p.captureKeyboard);
                        overlay->setFadeIn(false);
                        overlay->setBackgroundRole(ColorRole::None);
                        overlay->addChild(popupLayout);
                        
                        p.window = Window::create(context);
                        p.window->setBackgroundRole(ColorRole::None);
                        p.window->addChild(overlay);
                        p.window->show();
                        p.button->setOpen(true);

                        auto weak = std::weak_ptr<Popup>(std::dynamic_pointer_cast<Popup>(shared_from_this()));
                        overlay->setCloseCallback(
                            [weak]
                            {
                                if (auto widget = weak.lock())
                                {
                                    widget->close();
                                }
                            });
                    }
                }
            }

            void Popup::close()
            {
                DJV_PRIVATE_PTR();
                if (p.window)
                {
                    p.window->close();
                    p.window.reset();
                }
                p.button->setOpen(false);
                if (MenuButtonStyle::ComboBox == p.buttonStyle)
                {
                    p.button->takeTextFocus();
                }
                if (p.closeCallback)
                {
                    p.closeCallback(p.widget);
                }
                p.widget.reset();
            }

            void Popup::setPopupDefault(UI::Popup value)
            {
                _p->popupDefault = value;
            }

            const std::string& Popup::getIcon() const
            {
                return _p->button->getIcon();
            }

            const std::string& Popup::getPopupIcon() const
            {
                return _p->button->getPopupIcon();
            }

            void Popup::setIcon(const std::string& value)
            {
                _p->button->setIcon(value);
            }

            void Popup::setPopupIcon(const std::string& value)
            {
                _p->button->setPopupIcon(value);
            }

            const std::string& Popup::getText() const
            {
                return _p->button->getText();
            }

            void Popup::setText(const std::string& value)
            {
                _p->button->setText(value);
            }

            const std::string& Popup::getFontFamily() const
            {
                return _p->button->getFontFamily();
            }

            const std::string& Popup::getFontFace() const
            {
                return _p->button->getFontFace();
            }

            MetricsRole Popup::getFontSizeRole() const
            {
                return _p->button->getFontSizeRole();
            }

            void Popup::setFontFamily(const std::string& value)
            {
                _p->button->setFontFamily(value);
            }

            void Popup::setFontFace(const std::string& value)
            {
                _p->button->setFontFace(value);
            }

            void Popup::setFontSizeRole(MetricsRole value)
            {
                _p->button->setFontSizeRole(value);
            }

            size_t Popup::getTextElide() const
            {
                return _p->button->getTextElide();
            }

            MetricsRole Popup::getInsideMargin() const
            {
                return _p->button->getInsideMargin();
            }

            bool Popup::hasCapturePointer() const
            {
                return _p->capturePointer;
            }

            bool Popup::hasCaptureKeyboard() const
            {
                return _p->captureKeyboard;
            }

            void Popup::setTextElide(size_t value)
            {
                _p->button->setTextElide(value);
            }

            void Popup::setInsideMargin(MetricsRole value)
            {
                _p->button->setInsideMargin(value);
            }

            void Popup::setCapturePointer(bool value)
            {
                _p->capturePointer = value;
            }

            void Popup::setCaptureKeyboard(bool value)
            {
                _p->captureKeyboard = value;
            }

            void Popup::setOpenCallback(const std::function<std::shared_ptr<Widget>(void)>& value)
            {
                _p->openCallback = value;
            }

            void Popup::setCloseCallback(const std::function<void(const std::shared_ptr<Widget>&)>& value)
            {
                _p->closeCallback = value;
            }

            void Popup::_preLayoutEvent(System::Event::PreLayout& event)
            {
                _setMinimumSize(_p->button->getMinimumSize());
            }

            void Popup::_layoutEvent(System::Event::Layout& event)
            {
                _p->button->setGeometry(getGeometry());
            }

        } // namespace Button
    } // namespace UI
} // namespace djv
