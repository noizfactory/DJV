// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#pragma once

#include "IO.h"

struct AVPacket;

class FFmpegIO : public IIO
{
    DJV_NON_COPYABLE(FFmpegIO);

protected:
    void _init(
        const djv::System::File::Info&,
        const std::shared_ptr<djv::System::LogSystem>&);
    FFmpegIO();

public:
    ~FFmpegIO() override;

    static std::shared_ptr<FFmpegIO> create(
        const djv::System::File::Info&,
        const std::shared_ptr<djv::System::LogSystem>&);

    std::future<IOInfo> getInfo() override;

    void seek(Timestamp) override;

private:
    void _init(const djv::System::File::Info&);
    void _work();
    void _cleanup();

    void _read(
        AVPacket*,
        Timestamp seekTimestamp,
        Timestamp& videoTimestamp,
        Timestamp& audioTimestamp,
        std::queue<VideoFrame>&,
        std::queue<AudioFrame>&);
    int _decodeVideo(AVPacket*, Timestamp seek, Timestamp&, std::queue<VideoFrame>&);
    int _decodeAudio(AVPacket*, Timestamp seek, Timestamp&, std::queue<AudioFrame>&);

    DJV_PRIVATE();
};

class FFmpegPlugin : public IIOPlugin
{
    DJV_NON_COPYABLE(FFmpegPlugin);

protected:
    FFmpegPlugin(const std::shared_ptr<djv::System::LogSystem>&);

public:
    ~FFmpegPlugin() override;

    static std::shared_ptr<FFmpegPlugin> create(const std::shared_ptr<djv::System::LogSystem>&);

    bool canRead(const djv::System::File::Info&) override;
    std::shared_ptr<IIO> read(const djv::System::File::Info&) override;
};
