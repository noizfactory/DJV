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

#include <djvUI/ActionGroup.h>

#include <djvUI/Action.h>
#include <djvUI/Shortcut.h>

#include <map>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        struct ActionGroup::Private
        {
            std::vector<std::shared_ptr<Action> > actions;
            ButtonType buttonType = ButtonType::Push;
            std::function<void(int)> clickedCallback;
            std::function<void(int, bool)> checkedCallback;
            std::function<void(int)> radioCallback;
            std::map<std::shared_ptr<Action>, std::shared_ptr<ValueObserver<bool> > > clickedObservers;
            std::map<std::shared_ptr<Action>, std::shared_ptr<ValueObserver<bool> > > checkedObservers;
        };

        ActionGroup::ActionGroup() :
            _p(new Private)
        {}

        ActionGroup::~ActionGroup()
        {
            clearActions();
        }

        std::shared_ptr<ActionGroup> ActionGroup::create(ButtonType buttonType)
        {
            auto out = std::shared_ptr<ActionGroup>(new ActionGroup);
            out->setButtonType(buttonType);
            return out;
        }

        const std::vector<std::shared_ptr<Action> >& ActionGroup::getActions() const
        {
            return _p->actions;
        }

        size_t ActionGroup::getActionCount() const
        {
            return _p->actions.size();
        }

        int ActionGroup::getActionIndex(const std::shared_ptr<Action>& value) const
        {
            DJV_PRIVATE_PTR();
            const auto i = std::find(p.actions.begin(), p.actions.end(), value);
            if (i != p.actions.end())
            {
                return static_cast<int>(i - p.actions.begin());
            }
            return -1;
        }

        void ActionGroup::addAction(const std::shared_ptr<Action>& action)
        {
            DJV_PRIVATE_PTR();
            action->setButtonType(p.buttonType);

            if (ButtonType::Radio == p.buttonType)
            {
                size_t i = 0;
                for (; i < p.actions.size(); ++i)
                {
                    if (p.actions[i]->observeChecked()->get())
                    {
                        break;
                    }
                }
                if (i == p.actions.size())
                {
                    action->setChecked(true);
                }
            }

            const int index = static_cast<int>(p.actions.size());

            auto weak = std::weak_ptr<ActionGroup>(std::dynamic_pointer_cast<ActionGroup>(shared_from_this()));
            p.clickedObservers[action] =
                ValueObserver<bool>::create(
                    action->observeClicked(),
                    [weak, index](bool value)
            {
                if (value)
                {
                    if (auto group = weak.lock())
                    {
                        if (group->_p->clickedCallback)
                        {
                            group->_p->clickedCallback(index);
                        }
                    }
                }
            });

            p.checkedObservers[action] =
                ValueObserver<bool>::create(
                    action->observeChecked(),
                    [this, index](bool value)
            {
                if (value && ButtonType::Radio == _p->buttonType && _p->actions.size() > 0)
                {
                    for (size_t i = 0; i < _p->actions.size(); ++i)
                    {
                        _p->actions[i]->setChecked(i == index);
                    }
                    if (_p->radioCallback)
                    {
                        _p->radioCallback(index);
                    }
                }
                if (_p->checkedCallback)
                {
                    _p->checkedCallback(index, true);
                }
            });

            p.actions.push_back(action);
        }

        void ActionGroup::removeAction(const std::shared_ptr<Action>& action)
        {
            DJV_PRIVATE_PTR();
            const auto i = std::find(p.actions.begin(), p.actions.end(), action);
            if (i != p.actions.end())
            {
                const auto j = p.clickedObservers.find(*i);
                if (j != p.clickedObservers.end())
                {
                    p.clickedObservers.erase(j);
                }
                const auto k = p.checkedObservers.find(*i);
                if (k != p.checkedObservers.end())
                {
                    p.checkedObservers.erase(k);
                }
                p.actions.erase(i);
            }
        }

        void ActionGroup::clearActions()
        {
            DJV_PRIVATE_PTR();
            p.actions.clear();
            p.clickedObservers.clear();
            p.checkedObservers.clear();
        }

        ButtonType ActionGroup::getButtonType() const
        {
            return _p->buttonType;
        }

        void ActionGroup::setButtonType(ButtonType value)
        {
            DJV_PRIVATE_PTR();
            p.buttonType = value;

            for (size_t i = 0; i < p.actions.size(); ++i)
            {
                p.actions[i]->setButtonType(value);
            }

            if (ButtonType::Radio == p.buttonType)
            {
                setChecked(0);
            }
        }

        void ActionGroup::setChecked(int index, bool value)
        {
            DJV_PRIVATE_PTR();
            if (index >= 0 && index < static_cast<int>(p.actions.size()))
            {
                if (p.actions[index]->observeChecked()->get() != value)
                {
                    if (ButtonType::Radio == p.buttonType)
                    {
                        for (size_t i = 0; i < p.actions.size(); ++i)
                        {
                            p.actions[i]->setChecked(i == index);
                        }

                        if (p.radioCallback)
                        {
                            p.radioCallback(index);
                        }
                    }
                    else
                    {
                        p.actions[index]->setChecked(value);
                    }

                    if (p.checkedCallback)
                    {
                        p.checkedCallback(index, value);
                    }
                }
            }
        }

        void ActionGroup::setClickedCallback(const std::function<void(int)>& callback)
        {
            _p->clickedCallback = callback;
        }

        void ActionGroup::setCheckedCallback(const std::function<void(int, bool)>& callback)
        {
            _p->checkedCallback = callback;
        }

        void ActionGroup::setRadioCallback(const std::function<void(int)>& callback)
        {
            _p->radioCallback = callback;
        }

    } // namespace UI
} // namespace djv
