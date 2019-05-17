// Copyright (c) 2017-2018 Darby Johnston
// All rights reserved.
//
// See LICENSE.txt for licensing information.

#pragma once

#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        namespace Button
        {
            //! This enum provides the menu button styles.
            enum class MenuStyle
            {
                Flat,
                Round
            };

            //! This class provides a button for menu widgets.
            class Menu : public Widget
            {
                DJV_NON_COPYABLE(Menu);

            protected:
                void _init(Core::Context *);
                Menu();

            public:
                virtual ~Menu();

                static std::shared_ptr<Menu> create(Core::Context *);
                static std::shared_ptr<Menu> create(const std::string &, Core::Context *);
                static std::shared_ptr<Menu> create(const std::string &, const std::string & icon, Core::Context *);

                const std::string& getIcon() const;
                const std::string& getPopupIcon() const;
                void setIcon(const std::string&);
                void setPopupIcon(const std::string&);

                const std::string & getText() const;
                void setText(const std::string &);

                void setFontSizeRole(MetricsRole);

                bool isChecked() const;
                void setChecked(bool);
                void setCheckedCallback(const std::function<void(bool)> &);

                MenuStyle getMenuStyle() const;
                void setMenuStyle(MenuStyle);

            protected:
                void _preLayoutEvent(Core::Event::PreLayout &) override;
                void _layoutEvent(Core::Event::Layout &) override;
                void _clipEvent(Core::Event::Clip&) override;
                void _paintEvent(Core::Event::Paint &) override;
                void _pointerEnterEvent(Core::Event::PointerEnter &) override;
                void _pointerLeaveEvent(Core::Event::PointerLeave &) override;
                void _pointerMoveEvent(Core::Event::PointerMove &) override;
                void _buttonPressEvent(Core::Event::ButtonPress &) override;

            private:
                bool _isHovered() const;
                void _widgetUpdate();

                DJV_PRIVATE();
            };

        } // namespace Button
    } // namespace UI
} // namespace djv