//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
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

#include <djvViewLib/ImageViewSystem.h>

#include <djvUI/Action.h>
#include <djvUI/Menu.h>

#include <djvCore/Context.h>

#include <GLFW/glfw3.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewLib
    {
        struct ImageViewSystem::Private
        {
            std::map<std::string, std::shared_ptr<UI::Action> > actions;
            std::map<std::string, std::shared_ptr<UI::Menu> > menus;
            std::map<std::string, std::shared_ptr<ValueObserver<bool> > > clickedObservers;
        };

        void ImageViewSystem::_init(Context * context)
        {
            IViewSystem::_init("djv::ViewLib::ImageViewSystem", context);

            DJV_PRIVATE_PTR();
            //! \todo Implement me!
            p.actions["Left"] = UI::Action::create();
            p.actions["Left"]->addShortcut(GLFW_KEY_LEFT, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL);
            p.actions["Left"]->addShortcut(GLFW_KEY_KP_4);
            p.actions["Left"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["Right"] = UI::Action::create();
            p.actions["Right"]->addShortcut(GLFW_KEY_RIGHT, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL);
            p.actions["Right"]->addShortcut(GLFW_KEY_KP_6);
            p.actions["Right"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["Up"] = UI::Action::create();
            p.actions["Up"]->addShortcut(GLFW_KEY_UP, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL);
            p.actions["Up"]->addShortcut(GLFW_KEY_KP_8);
            p.actions["Up"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["Down"] = UI::Action::create();
            p.actions["Down"]->addShortcut(GLFW_KEY_DOWN, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL);
            p.actions["Down"]->addShortcut(GLFW_KEY_KP_2);
            p.actions["Down"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["Center"] = UI::Action::create();
            p.actions["Center"]->setShortcut(GLFW_KEY_KP_5);
            p.actions["Center"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["ZoomIn"] = UI::Action::create();
            p.actions["ZoomIn"]->addShortcut(GLFW_KEY_EQUAL);
            p.actions["ZoomIn"]->addShortcut(GLFW_KEY_KP_ADD);
            p.actions["ZoomIn"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["ZoomOut"] = UI::Action::create();
            p.actions["ZoomOut"]->addShortcut(GLFW_KEY_MINUS);
            p.actions["ZoomOut"]->addShortcut(GLFW_KEY_KP_SUBTRACT);
            p.actions["ZoomOut"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["ResetZoom"] = UI::Action::create();
            p.actions["ResetZoom"]->addShortcut(GLFW_KEY_0);
            p.actions["ResetZoom"]->addShortcut(GLFW_KEY_KP_0);
            p.actions["ResetZoom"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["ResetPosZoom"] = UI::Action::create();
            p.actions["ResetPosZoom"]->addShortcut(GLFW_KEY_DELETE);
            p.actions["ResetPosZoom"]->addShortcut(GLFW_KEY_KP_DECIMAL);
            p.actions["ResetPosZoom"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["Fit"] = UI::Action::create();
            p.actions["Fit"]->addShortcut(GLFW_KEY_BACKSPACE);
            p.actions["Fit"]->addShortcut(GLFW_KEY_KP_ENTER);
            p.actions["Fit"]->setEnabled(false);

            //! \todo Implement me!
            p.actions["HUD"] = UI::Action::create();
            p.actions["HUD"]->setShortcut(GLFW_KEY_H);
            p.actions["HUD"]->setEnabled(false);
        }

        ImageViewSystem::ImageViewSystem() :
            _p(new Private)
        {}

        ImageViewSystem::~ImageViewSystem()
        {}

        std::shared_ptr<ImageViewSystem> ImageViewSystem::create(Context * context)
        {
            auto out = std::shared_ptr<ImageViewSystem>(new ImageViewSystem);
            out->_init(context);
            return out;
        }

        std::map<std::string, std::shared_ptr<UI::Action> > ImageViewSystem::getActions()
        {
            return _p->actions;
        }

        std::string ImageViewSystem::getMenuSortKey() const
        {
            return "2";
        }

        std::shared_ptr<UI::Menu> ImageViewSystem::createMenu()
        {
            DJV_PRIVATE_PTR();
            auto context = getContext();
            p.menus["View"] = UI::Menu::create(_getText(DJV_TEXT("djv::ViewLib", "View")), context);
            p.menus["View"]->addAction(p.actions["Left"]);
            p.menus["View"]->addAction(p.actions["Right"]);
            p.menus["View"]->addAction(p.actions["Up"]);
            p.menus["View"]->addAction(p.actions["Down"]);
            p.menus["View"]->addAction(p.actions["Center"]);
            p.menus["View"]->addAction(p.actions["ZoomIn"]);
            p.menus["View"]->addAction(p.actions["ZoomOut"]);
            p.menus["View"]->addAction(p.actions["ResetZoom"]);
            p.menus["View"]->addAction(p.actions["ResetPosZoom"]);
            p.menus["View"]->addAction(p.actions["Fit"]);
            p.menus["View"]->addSeparator();
            //! \todo Implement me!
            p.menus["Grid"] = UI::Menu::create(_getText(DJV_TEXT("djv::ViewLib", "Grid")), context);
            p.menus["View"]->addMenu(p.menus["Grid"]);
            p.menus["View"]->addAction(p.actions["HUD"]);
            return p.menus["View"];
        }

        void ImageViewSystem::_localeChangedEvent(Event::LocaleChanged &)
        {
            DJV_PRIVATE_PTR();
            p.actions["Left"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Left")));
            p.actions["Right"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Right")));
            p.actions["Up"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Up")));
            p.actions["Down"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Down")));
            p.actions["Center"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Center")));
            p.actions["ZoomIn"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Zoom In")));
            p.actions["ZoomOut"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Zoom Out")));
            p.actions["ResetZoom"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Reset Zoom")));
            p.actions["ResetPosZoom"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Reset Position and Zoom")));
            p.actions["Fit"]->setText(_getText(DJV_TEXT("djv::ViewLib", "Fit")));
            p.actions["HUD"]->setText(_getText(DJV_TEXT("djv::ViewLib", "HUD")));

            p.menus["View"]->setMenuName(_getText(DJV_TEXT("djv::ViewLib", "View")));
            p.menus["Grid"]->setMenuName(_getText(DJV_TEXT("djv::ViewLib", "Grid")));
        }

    } // namespace ViewLib
} // namespace djv
