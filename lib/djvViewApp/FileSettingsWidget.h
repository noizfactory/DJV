// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUIComponents/ISettingsWidget.h>

namespace djv
{
    namespace ViewApp
    {
        //! This class provides the sequence settings widget.
        class SequenceSettingsWidget : public UI::ISettingsWidget
        {
            DJV_NON_COPYABLE(SequenceSettingsWidget);

        protected:
            void _init(const std::shared_ptr<System::Context>&);
            SequenceSettingsWidget();

        public:
            static std::shared_ptr<SequenceSettingsWidget> create(const std::shared_ptr<System::Context>&);

            std::string getSettingsName() const override;
            std::string getSettingsGroup() const override;
            std::string getSettingsSortKey() const override;

            void setLabelSizeGroup(const std::weak_ptr<UI::LabelSizeGroup>&) override;

        protected:
            void _initEvent(System::Event::Init&) override;

        private:
            DJV_PRIVATE();
        };

        //! This class provides the recent files settings widget.
        class RecentFilesSettingsWidget : public UI::ISettingsWidget
        {
            DJV_NON_COPYABLE(RecentFilesSettingsWidget);

        protected:
            void _init(const std::shared_ptr<System::Context>&);
            RecentFilesSettingsWidget();

        public:
            static std::shared_ptr<RecentFilesSettingsWidget> create(const std::shared_ptr<System::Context>&);

            std::string getSettingsName() const override;
            std::string getSettingsGroup() const override;
            std::string getSettingsSortKey() const override;

        protected:
            void _initEvent(System::Event::Init&) override;

        private:
            DJV_PRIVATE();
        };

    } // namespace ViewApp
} // namespace djv

