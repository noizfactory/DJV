//------------------------------------------------------------------------------
// Copyright (c) 2004-2018 Darby Johnston
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

#include <djvViewLib/PlaybackActions.h>

#include <djvViewLib/Context.h>
#include <djvViewLib/ShortcutPrefs.h>

#include <djvUI/IconLibrary.h>
#include <djvUI/StylePrefs.h>

#include <QApplication>
#include <QPointer>

namespace djv
{
    namespace ViewLib
    {
        struct PlaybackActions::Private
        {
            Util::PLAYBACK playback = static_cast<Util::PLAYBACK>(0);
        };

        PlaybackActions::PlaybackActions(
            const QPointer<Context> & context,
            QObject * parent) :
            AbstractActions(context, parent),
            _p(new Private)
        {
            // Create the actions.
            for (int i = 0; i < ACTION_COUNT; ++i)
            {
                _actions[i] = new QAction(this);
            }
            _actions[PLAYBACK_TOGGLE]->setText(
                qApp->translate("djv::ViewLib::PlaybackActions", "Toggle Playback"));
            _actions[PLAYBACK_TOGGLE]->setToolTip(
                qApp->translate("djv::ViewLib::PlaybackActions", "Toggle playback"));
            _actions[EVERY_FRAME]->setText(
                qApp->translate("djv::ViewLib::PlaybackActions", "Ever&y Frame"));
            _actions[EVERY_FRAME]->setCheckable(true);
            _actions[EVERY_FRAME]->setToolTip(
                qApp->translate("djv::ViewLib::PlaybackActions", "Set whether every frame is played back"));

            // Create the action groups.
            for (int i = 0; i < GROUP_COUNT; ++i)
            {
                _groups[i] = new QActionGroup(this);
            }

            _groups[PLAYBACK_GROUP] = new QActionGroup(this);
            _groups[PLAYBACK_GROUP]->setExclusive(true);
            for (int i = 0; i < Util::PLAYBACK_COUNT; ++i)
            {
                QAction * action = new QAction(Util::playbackLabels()[i], _groups[PLAYBACK_GROUP]);
                action->setCheckable(true);
                action->setData(i);
            }

            _groups[LOOP_GROUP] = new QActionGroup(this);
            _groups[LOOP_GROUP]->setExclusive(true);
            for (int i = 0; i < Util::LOOP_COUNT; ++i)
            {
                QAction * action = new QAction(Util::loopLabels()[i], _groups[LOOP_GROUP]);
                action->setCheckable(true);
                action->setData(i);
            }

            _groups[FRAME_GROUP] = new QActionGroup(this);
            _groups[FRAME_GROUP]->setExclusive(false);
            for (int i = 0; i < Util::FRAME_COUNT; ++i)
            {
                QAction * action = new QAction(Util::frameLabels()[i], _groups[FRAME_GROUP]);
                action->setData(i);
            }

            _groups[IN_OUT_GROUP] = new QActionGroup(this);
            _groups[IN_OUT_GROUP]->setExclusive(false);
            for (int i = 0; i < Util::IN_OUT_COUNT; ++i)
            {
                QAction * action = new QAction(Util::inOutLabels()[i], _groups[IN_OUT_GROUP]);
                action->setData(i);
                if (Util::IN_OUT_ENABLE == i)
                {
                    action->setCheckable(true);
                }
            }

            _groups[LAYOUT_GROUP] = new QActionGroup(this);
            _groups[LAYOUT_GROUP]->setExclusive(true);
            for (int i = 0; i < Util::LAYOUT_COUNT; ++i)
            {
                QAction * action = new QAction(this);
                action->setText(Util::layoutLabels()[i]);
                action->setCheckable(true);
                action->setData(i);
                _groups[LAYOUT_GROUP]->addAction(action);
            }

            // Initialize.
            update();

            // Setup the callbacks.
            connect(
                context->shortcutPrefs(),
                SIGNAL(shortcutsChanged(const QVector<djv::UI::Shortcut> &)),
                SLOT(update()));
            connect(
                context->stylePrefs(),
                SIGNAL(prefChanged()),
                SLOT(update()));
        }

        PlaybackActions::~PlaybackActions()
        {}

        void PlaybackActions::update()
        {
            const QVector<UI::Shortcut> & shortcuts = context()->shortcutPrefs()->shortcuts();

            QKeySequence key = shortcuts[Util::SHORTCUT_PLAYBACK_TOGGLE].value;
            _actions[PLAYBACK_TOGGLE]->setShortcut(key);
            _actions[PLAYBACK_TOGGLE]->setToolTip(
                qApp->translate("djv::ViewLib::PlaybackActions", "Toggle playback\n\nShortcut: %1").
                arg(key.toString()));
            _actions[EVERY_FRAME]->setIcon(
                context()->iconLibrary()->icon("djv/UI/UnlockIcon", "djv/UI/LockIcon"));
            
            const QVector<QIcon> playbackIcons = QVector<QIcon>() <<
                context()->iconLibrary()->icon("djv/UI/PlayReverseIcon") <<
                context()->iconLibrary()->icon("djv/UI/PlayStopIcon") <<
                context()->iconLibrary()->icon("djv/UI/PlayForwardIcon");
            const QVector<Util::SHORTCUT> playbackShortcuts =
                QVector<Util::SHORTCUT>() <<
                Util::SHORTCUT_PLAYBACK_REVERSE <<
                Util::SHORTCUT_PLAYBACK_STOP <<
                Util::SHORTCUT_PLAYBACK_FORWARD;
            const QStringList playbackToolTips = QStringList() <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Start reverse playback\n\nShortcut: %1") <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Stop playback\n\nShortcut: %1") <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Start forward playback\n\nShortcut: %1");
            for (int i = 0; i < Util::PLAYBACK_COUNT; ++i)
            {
                _groups[PLAYBACK_GROUP]->actions()[i]->setIcon(playbackIcons[i]);
                const QKeySequence key = shortcuts[playbackShortcuts[i]].value;
                _groups[PLAYBACK_GROUP]->actions()[i]->setShortcut(key);
                _groups[PLAYBACK_GROUP]->actions()[i]->setToolTip(playbackToolTips[i].arg(key.toString()));
            }

            const QVector<QIcon> loopIcons = QVector<QIcon>() <<
                context()->iconLibrary()->icon("djv/UI/PlayLoopOnceIcon") <<
                context()->iconLibrary()->icon("djv/UI/PlayLoopRepeatIcon") <<
                context()->iconLibrary()->icon("djv/UI/PlayLoopPingPongIcon");
            for (int i = 0; i < Util::LOOP_COUNT; ++i)
            {
                _groups[LOOP_GROUP]->actions()[i]->setIcon(loopIcons[i]);
            }

            const QVector<QIcon> frameIcons = QVector<QIcon>() <<
                context()->iconLibrary()->icon("djv/UI/FrameStartIcon") <<
                QIcon() <<
                context()->iconLibrary()->icon("djv/UI/FramePrevIcon") <<
                QIcon() <<
                QIcon() <<
                context()->iconLibrary()->icon("djv/UI/FrameNextIcon") <<
                QIcon() <<
                QIcon() <<
                context()->iconLibrary()->icon("djv/UI/FrameEndIcon") <<
                QIcon();
            const QVector<bool> frameAutoRepeat = QVector<bool>() <<
                false <<
                false <<
                true <<
                true <<
                true <<
                true <<
                true <<
                true <<
                false <<
                false;
            const QVector<Util::SHORTCUT> frameShortcuts =
                QVector<Util::SHORTCUT>() <<
                Util::SHORTCUT_PLAYBACK_START <<
                Util::SHORTCUT_PLAYBACK_START_ABS <<
                Util::SHORTCUT_PLAYBACK_PREV <<
                Util::SHORTCUT_PLAYBACK_PREV_10 <<
                Util::SHORTCUT_PLAYBACK_PREV_100 <<
                Util::SHORTCUT_PLAYBACK_NEXT <<
                Util::SHORTCUT_PLAYBACK_NEXT_10 <<
                Util::SHORTCUT_PLAYBACK_NEXT_100 <<
                Util::SHORTCUT_PLAYBACK_END <<
                Util::SHORTCUT_PLAYBACK_END_ABS;
            const QStringList frameToolTips = QStringList() <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Go to the start frame or in point\n\nShortcut: %1") <<
                QString() <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Go to the previous frame\n\nShortcut: %1") <<
                QString() <<
                QString() <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Go to the next frame\n\nShortcut: %1") <<
                QString() <<
                QString() <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Go to the end frame or out point\n\nShortcut: %1") <<
                QString();
            for (int i = 0; i < Util::FRAME_COUNT; ++i)
            {
                _groups[FRAME_GROUP]->actions()[i]->setIcon(frameIcons[i]);
                const QKeySequence key = shortcuts[frameShortcuts[i]].value;
                _groups[FRAME_GROUP]->actions()[i]->setAutoRepeat(frameAutoRepeat[i]);
                _groups[FRAME_GROUP]->actions()[i]->setShortcut(key);
                if (!frameToolTips[i].isEmpty())
                    _groups[FRAME_GROUP]->actions()[i]->setToolTip(frameToolTips[i].arg(key.toString()));
            }

            const QVector<QIcon> inOutIcons = QVector<QIcon>() <<
                context()->iconLibrary()->icon("djv/UI/PlayInOutIcon") <<
                context()->iconLibrary()->icon("djv/UI/InPointMarkIcon") <<
                context()->iconLibrary()->icon("djv/UI/OutPointMarkIcon") <<
                context()->iconLibrary()->icon("djv/UI/InPointResetIcon") <<
                context()->iconLibrary()->icon("djv/UI/OutPointResetIcon") <<
                QIcon();
            const QVector<Util::SHORTCUT> inOutShortcuts =
                QVector<Util::SHORTCUT>() <<
                Util::SHORTCUT_PLAYBACK_IN_OUT <<
                Util::SHORTCUT_PLAYBACK_MARK_IN <<
                Util::SHORTCUT_PLAYBACK_MARK_OUT <<
                Util::SHORTCUT_PLAYBACK_RESET_IN <<
                Util::SHORTCUT_PLAYBACK_RESET_OUT;
            const QStringList inOutToolTips = QStringList() <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Enable in/out points\n\nShortcut: %1") <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Set the current frame as the in point\n\nShortcut: %1") <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Set the current frame as the out point\n\nShortcut: %1") <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Reset the in point\n\nShortcut: %1") <<
                qApp->translate("djv::ViewLib::PlaybackActions", "Reset the out point\n\nShortcut: %1");
            for (int i = 0; i < Util::IN_OUT_COUNT; ++i)
            {
                _groups[IN_OUT_GROUP]->actions()[i]->setIcon(inOutIcons[i]);
                const QKeySequence key = shortcuts[inOutShortcuts[i]].value;
                _groups[IN_OUT_GROUP]->actions()[i]->setShortcut(key);
                _groups[IN_OUT_GROUP]->actions()[i]->setToolTip(inOutToolTips[i].arg(key.toString()));
            }

            Q_EMIT changed();
        }

    } // namespace ViewLib
} // namespace djv