// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUI/Bellows.h>
#include <djvUI/Label.h>
#include <djvUI/RowLayout.h>

#include <djvCore/ValueObserver.h>

class ISettingsWidget : public djv::UI::Widget
{
    DJV_NON_COPYABLE(ISettingsWidget);

protected:
    void _init(const std::shared_ptr<djv::System::Context>&);
    ISettingsWidget();

public:
    ~ISettingsWidget() override;

    std::shared_ptr<djv::Core::Observer::IValueSubject<std::string> > observeTitle() const;
    void setTitle(const std::string&);

    virtual void setLabelSizeGroup(const std::weak_ptr<djv::UI::Text::LabelSizeGroup>&) {}

    float getHeightForWidth(float) const override;

    void addChild(const std::shared_ptr<IObject>&) override;
    void removeChild(const std::shared_ptr<IObject>&) override;
    void clearChildren() override;

protected:
    void _preLayoutEvent(djv::System::Event::PreLayout&) override;
    void _layoutEvent(djv::System::Event::Layout&) override;

private:
    std::weak_ptr<djv::UI::Text::LabelSizeGroup> _sizeGroup;
    std::shared_ptr<djv::Core::Observer::ValueSubject<std::string> > _title;
    std::shared_ptr<djv::UI::VerticalLayout> _childLayout;
    std::shared_ptr<djv::UI::Layout::Bellows> _bellows;
};
