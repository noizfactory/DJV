// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019-2020 Darby Johnston
// All rights reserved.

#include <djvViewApp/MagnifySystem.h>

#include <djvViewApp/MagnifyWidget.h>
#include <djvViewApp/MagnifySettings.h>

#include <djvUI/Action.h>
#include <djvUI/ShortcutDataFunc.h>

#include <djvSystem/Context.h>
#include <djvSystem/TextSystem.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewApp
    {
        struct MagnifySystem::Private
        {
            glm::vec2 magnifyPos = glm::vec2(0.F, 0.F);
            std::shared_ptr<MagnifySettings> settings;
            std::map<std::string, std::shared_ptr<UI::Action> > actions;
            std::weak_ptr<MagnifyWidget> widget;
        };

        void MagnifySystem::_init(const std::shared_ptr<System::Context>& context)
        {
            IToolSystem::_init("djv::ViewApp::MagnifySystem", context);
            DJV_PRIVATE_PTR();

            p.settings = MagnifySettings::create(context);
            _setWidgetGeom(p.settings->getWidgetGeom());

            p.actions["Magnify"] = UI::Action::create();
            p.actions["Magnify"]->setIcon("djvIconMagnify");

            _addShortcut("shortcut_magnify", GLFW_KEY_Y, UI::getSystemModifier());

            _textUpdate();
            _shortcutsUpdate();
        }

        MagnifySystem::MagnifySystem() :
            _p(new Private)
        {}

        MagnifySystem::~MagnifySystem()
        {
            DJV_PRIVATE_PTR();
            _closeWidget("Magnify");
            p.settings->setWidgetGeom(_getWidgetGeom());
        }

        std::shared_ptr<MagnifySystem> MagnifySystem::create(const std::shared_ptr<System::Context>& context)
        {
            auto out = context->getSystemT<MagnifySystem>();
            if (!out)
            {
                out = std::shared_ptr<MagnifySystem>(new MagnifySystem);
                out->_init(context);
            }
            return out;
        }

        ToolActionData MagnifySystem::getToolAction() const
        {
            return
            {
                _p->actions["Magnify"],
                "C"
            };
        }

        void MagnifySystem::setCurrentTool(bool value, int index)
        {
            DJV_PRIVATE_PTR();
            if (value)
            {
                if (p.widget.expired())
                {
                    if (auto context = getContext().lock())
                    {
                        auto widget = MagnifyWidget::create(context);
                        widget->setMagnifyPos(p.magnifyPos);
                        p.widget = widget;
                        _openWidget("Magnify", widget);
                    }
                }
            }
            else if (-1 == index)
            {
                _closeWidget("Magnify");
            }
            if (auto widget = p.widget.lock())
            {
                widget->setCurrentTool(value);
            }
        }

        std::map<std::string, std::shared_ptr<UI::Action> > MagnifySystem::getActions() const
        {
            return _p->actions;
        }

        void MagnifySystem::_closeWidget(const std::string& value)
        {
            DJV_PRIVATE_PTR();
            const auto i = p.actions.find(value);
            if (i != p.actions.end())
            {
                i->second->setChecked(false);
            }
            if (auto widget = p.widget.lock())
            {
                p.magnifyPos = widget->getMagnifyPos();
                p.widget.reset();
            }
            IToolSystem::_closeWidget(value);
        }

        void MagnifySystem::_textUpdate()
        {
            DJV_PRIVATE_PTR();
            if (p.actions.size())
            {
                p.actions["Magnify"]->setText(_getText(DJV_TEXT("menu_tools_magnify")));
                p.actions["Magnify"]->setTooltip(_getText(DJV_TEXT("menu_magnify_tooltip")));
            }
        }

        void MagnifySystem::_shortcutsUpdate()
        {
            DJV_PRIVATE_PTR();
            if (p.actions.size())
            {
                p.actions["Magnify"]->setShortcuts(_getShortcuts("shortcut_magnify"));
            }
        }

    } // namespace ViewApp
} // namespace djv

