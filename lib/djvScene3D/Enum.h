// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019-2020 Darby Johnston
// All rights reserved.

#pragma once

namespace djv
{
    namespace Scene3D
    {
        //! This enumeration provides the scene orientation.
        enum class SceneOrient
        {
            YUp,
            ZUp
        };

        //! This enumeration provides how colors are assigned.
        enum class ColorAssignment
        {
            Layer,
            Parent,
            Primitive,

            Count,
            First = Layer
        };

        //! This enumeration provides how materials are assigned.
        enum class MaterialAssignment
        {
            Layer,
            Parent,
            Primitive,

            Count,
            First = Layer
        };

    } // namespace Scene3D
} // namespace djv

