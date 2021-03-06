// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvViewApp/MDIWidget.h>

namespace djv
{
    namespace ViewApp
    {
        //! This class provides a widget for displaying messages.
        class MessagesWidget : public MDIWidget
        {
            DJV_NON_COPYABLE(MessagesWidget);

        protected:
            void _init(const std::shared_ptr<System::Context>&);
            MessagesWidget();

        public:
            ~MessagesWidget() override;

            static std::shared_ptr<MessagesWidget> create(const std::shared_ptr<System::Context>&);

            //! \name Text
            ///@{
            
            void setText(const std::string&);

            ///@}

            //! \name Options
            ///@{

            void setPopup(bool);

            ///@}

            //! \name Callbacks
            ///@{

            void setPopupCallback(const std::function<void(bool)>&);
            void setCopyCallback(const std::function<void(void)>&);
            void setClearCallback(const std::function<void(void)>&);

            ///@}

        protected:
            void _initEvent(System::Event::Init &) override;

        private:
            DJV_PRIVATE();
        };

    } // namespace ViewApp
} // namespace djv

