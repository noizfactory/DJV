// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#include <djvUIComponents/FileBrowserPrivate.h>

#include <djvUIComponents/FileBrowserSettings.h>

#include <djvUI/Bellows.h>
#include <djvUI/RowLayout.h>
#include <djvUI/SettingsSystem.h>

#include <djvSystem/Context.h>
#include <djvSystem/DirectoryModel.h>

using namespace djv::Core;

namespace djv
{
    namespace UIComponents
    {
        namespace FileBrowser
        {
            namespace
            {
                //! \todo Should this be configurable?
                const size_t elide = 20;

            } // namespace

            struct DrawerWidget::Private
            {
                std::shared_ptr<ShortcutsWidget> shortcutsWidget;
                std::shared_ptr<RecentPathsWidget> recentPathsWidget;
                std::shared_ptr<DrivesWidget> drivesWidget;
                std::map<std::string, std::shared_ptr<UI::Bellows> > bellows;
                std::shared_ptr<UI::VerticalLayout> layout;

                std::shared_ptr<Observer::Value<System::File::Path> > pathObserver;
                std::shared_ptr<Observer::Map<std::string, bool> > pathsBellowsStateObserver;
            };

            void DrawerWidget::_init(
                const std::shared_ptr<System::File::DirectoryModel>& directoryModel,
                const std::shared_ptr<ShortcutsModel>& shortcutsModel,
                const std::shared_ptr<System::File::RecentFilesModel>& recentFilesModel,
                const std::shared_ptr<System::File::DrivesModel>& drivesModel,
                const std::shared_ptr<System::Context>& context)
            {
                Widget::_init(context);
                DJV_PRIVATE_PTR();

                setClassName("djv::UIComponents::FileBrowser::DrawerWidget");

                p.shortcutsWidget = ShortcutsWidget::create(shortcutsModel, elide, context);
                p.bellows["Shortcuts"] = UI::Bellows::create(context);
                p.bellows["Shortcuts"]->addChild(p.shortcutsWidget);

                auto recentPathsWidget = RecentPathsWidget::create(recentFilesModel, elide, context);
                p.bellows["Recent"] = UI::Bellows::create(context);
                p.bellows["Recent"]->addChild(recentPathsWidget);

                auto drivesWidget = DrivesWidget::create(drivesModel, elide, context);
                p.bellows["Drives"] = UI::Bellows::create(context);
                p.bellows["Drives"]->addChild(drivesWidget);

                p.layout = UI::VerticalLayout::create(context);
                p.layout->setSpacing(UI::MetricsRole::None);
                p.layout->addChild(p.bellows["Shortcuts"]);
                p.layout->addChild(p.bellows["Recent"]);
                p.layout->addChild(p.bellows["Drives"]);
                addChild(p.layout);

                auto weak = std::weak_ptr<DrawerWidget>(std::dynamic_pointer_cast<DrawerWidget>(shared_from_this()));
                p.shortcutsWidget->setCallback(
                    [directoryModel](const System::File::Path& value)
                    {
                        directoryModel->setPath(value);
                    });

                recentPathsWidget->setCallback(
                    [directoryModel](const System::File::Path& value)
                    {
                        directoryModel->setPath(value);
                    });

                drivesWidget->setCallback(
                    [directoryModel](const System::File::Path& value)
                    {
                        directoryModel->setPath(value);
                    });

                p.pathObserver = Observer::Value<System::File::Path>::create(
                    directoryModel->observePath(),
                    [weak](const System::File::Path& value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->shortcutsWidget->setPath(value);
                        }
                    });

                auto settingsSystem = context->getSystemT<UI::Settings::SettingsSystem>();
                auto fileBrowserSettings = settingsSystem->getSettingsT<Settings::FileBrowser>();
                p.pathsBellowsStateObserver = Observer::Map<std::string, bool>::create(
                    fileBrowserSettings->observePathsBellowsState(),
                    [weak](const std::map<std::string, bool>& value)
                    {
                        if (auto widget = weak.lock())
                        {
                            for (const auto& i : value)
                            {
                                const auto j = widget->_p->bellows.find(i.first);
                                if (j != widget->_p->bellows.end())
                                {
                                    j->second->setOpen(i.second, false);
                                }
                            }
                        }
                    });

                for (const auto& i : p.bellows)
                {
                    i.second->setOpenCallback(
                        [weak](bool value)
                        {
                            if (auto widget = weak.lock())
                            {
                                widget->_saveBellowsState();
                            }
                        });
                }
            }

            DrawerWidget::DrawerWidget() :
                _p(new Private)
            {}

            DrawerWidget::~DrawerWidget()
            {}

            std::shared_ptr<DrawerWidget> DrawerWidget::create(
                const std::shared_ptr<System::File::DirectoryModel>& directoryModel,
                const std::shared_ptr<ShortcutsModel>& shortcutsModel,
                const std::shared_ptr<System::File::RecentFilesModel>& recentFilesModel,
                const std::shared_ptr<System::File::DrivesModel>& drivesModel,
                const std::shared_ptr<System::Context>& context)
            {
                auto out = std::shared_ptr<DrawerWidget>(new DrawerWidget);
                out->_init(directoryModel, shortcutsModel, recentFilesModel, drivesModel, context);
                return out;
            }

            void DrawerWidget::_preLayoutEvent(System::Event::PreLayout& event)
            {
                _setMinimumSize(_p->layout->getMinimumSize());
            }

            void DrawerWidget::_layoutEvent(System::Event::Layout& event)
            {
                _p->layout->setGeometry(getGeometry());
            }

            void DrawerWidget::_initEvent(System::Event::Init& event)
            {
                DJV_PRIVATE_PTR();
                if (event.getData().text)
                {
                    p.bellows["Shortcuts"]->setText(_getText(DJV_TEXT("file_browser_shortcuts")));
                    p.bellows["Recent"]->setText(_getText(DJV_TEXT("file_browser_recent_paths")));
                    p.bellows["Drives"]->setText(_getText(DJV_TEXT("file_browser_drives")));
                }
            }

            void DrawerWidget::_saveBellowsState()
            {
                DJV_PRIVATE_PTR();
                if (auto context = getContext().lock())
                {
                    if (!p.bellows.empty())
                    {
                        std::map<std::string, bool> state;
                        for (const auto& i : p.bellows)
                        {
                            state[i.first] = i.second->isOpen();
                        }
                        auto settingsSystem = context->getSystemT<UI::Settings::SettingsSystem>();
                        auto fileBrowserSettings = settingsSystem->getSettingsT<Settings::FileBrowser>();
                        fileBrowserSettings->setPathsBellowsState(state);
                    }
                }
            }

        } // namespace FileBrowser
    } // namespace UIComponents
} // namespace djv
