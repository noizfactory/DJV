// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvAV/SequenceIO.h>

namespace djv
{
    namespace System
    {
        namespace File
        {
            class IO;

        } // namespace File
    } // namespace System

    namespace AV
    {
        namespace IO
        {
            //! This namespace provides Targa image I/O.
            //!
            //! References:
            //! - James D. Murray, William vanRyper, "Encyclopedia of AV File Formats,
            //!   Second Edition"
            namespace Targa
            {
                static const std::string pluginName = "Targa";
                static const std::set<std::string> fileExtensions = { ".tga" };

                //! This class provides the Targa file reader.
                class Read : public ISequenceRead
                {
                    DJV_NON_COPYABLE(Read);

                protected:
                    Read();

                public:
                    ~Read() override;

                    static std::shared_ptr<Read> create(
                        const System::File::Info&,
                        const ReadOptions&,
                        const std::shared_ptr<System::TextSystem>&,
                        const std::shared_ptr<System::ResourceSystem>&,
                        const std::shared_ptr<System::LogSystem>&);

                protected:
                    Info _readInfo(const std::string& fileName) override;
                    std::shared_ptr<Image::Data> _readImage(const std::string& fileName) override;

                private:
                    Info _open(const std::string&, const std::shared_ptr<System::File::IO>&);

                    bool _bgr = false;
                    bool _compression = false;
                };
                
                //! This class provides the Targa file I/O plugin.
                class Plugin : public ISequencePlugin
                {
                    DJV_NON_COPYABLE(Plugin);

                protected:
                    Plugin();

                public:
                    static std::shared_ptr<Plugin> create(const std::shared_ptr<System::Context>&);

                    std::shared_ptr<IRead> read(const System::File::Info&, const ReadOptions&) const override;
                };

            } // namespace Targa
        } // namespace IO
    } // namespace AV
} // namespace djv
