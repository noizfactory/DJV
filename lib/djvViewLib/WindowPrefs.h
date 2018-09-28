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

#pragma once

#include <djvViewLib/AbstractPrefs.h>
#include <djvViewLib/Util.h>

namespace djv
{
    namespace ViewLib
    {
        //! \class WindowPrefs
        //!
        //! This class provides the window group preferences.
        class WindowPrefs : public AbstractPrefs
        {
            Q_OBJECT

        public:
            explicit WindowPrefs(const QPointer<Context> &, QObject * parent = nullptr);
            ~WindowPrefs() override;

            //! Get the default for whether to automatically fit the window to the
            //! image.
            static bool autoFitDefault();

            //! Get whether to automatically fit the window to the image.
            bool hasAutoFit() const;

            //! Get the default maximum view size.
            static Util::VIEW_MAX viewMaxDefault();

            //! Get the maximum view size.
            Util::VIEW_MAX viewMax() const;

            //! Get the default user specified maximum view size.
            static const glm::ivec2 & viewMaxUserDefault();

            //! Get the user specified maximum view size.
            const glm::ivec2 & viewMaxUser() const;

            //! Get the default for whether the controls are visible when going full
            //! screen.
            static bool fullScreenControlsDefault();

            //! Get whether the controls are visible when going full screen.
            bool hasFullScreenControls() const;

            //! Get the default tool bar visiblity.
            static QVector<bool> toolBarDefault();

            //! Get the tool bar visibility.
            const QVector<bool> & toolBar() const;

        public Q_SLOTS:
            //! Set whether to automatically fit the window to the image.
            void setAutoFit(bool);

            //! Set the maximum view size.
            void setViewMax(djv::ViewLib::Util::VIEW_MAX);

            //! Set the user specified maximum view size.
            void setViewMaxUser(const glm::ivec2 &);

            //! Set whether the controls are visible when going full screen.
            void setFullScreenControls(bool);

            //! Set the tool bar visibility.
            void setToolBar(const QVector<bool> &);

        Q_SIGNALS:
            //! This signal is emitted when automatically fitting the window to the
            //! image is changed.
            void autoFitChanged(bool);

            //! This signal is emitted when the maximum view size is changed.
            void viewMaxChanged(djv::ViewLib::Util::VIEW_MAX);

            //! This signal is emitted when the user specified maximum view size is
            //! changed.
            void viewMaxUserChanged(const glm::ivec2 &);

            //! This signal is emitted when the full screen controls option is
            //! changed.
            void fullScreenControlsChanged(bool);

            //! This signal is emitted when the tool bar visibility is changed.
            void toolBarChanged(const QVector<bool> &);

        private:
            bool           _autoFit;
            Util::VIEW_MAX _viewMax;
            glm::ivec2     _viewMaxUser = glm::ivec2(0, 0);
            bool           _fullScreenControls;
            QVector<bool>  _toolBar;
        };

    } // namespace ViewLib
} // namespace djv