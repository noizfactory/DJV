// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019-2020 Darby Johnston
// All rights reserved.

#include <djvScene3D/Light.h>

#include <djvRender3D/Light.h>

using namespace djv::Core;

namespace djv
{
    namespace Scene3D
    {
        ILight::ILight()
        {}

        ILight::~ILight()
        {}

        HemisphereLight::HemisphereLight()
        {}

        std::shared_ptr<HemisphereLight> HemisphereLight::create()
        {
            auto out = std::shared_ptr<HemisphereLight>(new HemisphereLight);
            return out;
        }

        /*void HemisphereLight::render(const glm::mat4x4& xform, const std::shared_ptr<Render3D::Render>& render)
        {
            if (isEnabled())
            {
            }
        }*/

        DirectionalLight::DirectionalLight()
        {}

        std::shared_ptr<DirectionalLight> DirectionalLight::create()
        {
            auto out = std::shared_ptr<DirectionalLight>(new DirectionalLight);
            return out;
        }

        /*void DirectionalLight::render(const glm::mat4x4& xform, const std::shared_ptr<Render3D::Render>& render)
        {
            if (isEnabled())
            {
                auto renderLight = Render3D::DirectionalLight::create();
                glm::vec3 renderDirection = xform * glm::vec4(_direction.x, _direction.y, _direction.z, 1.F);
                glm::vec3 renderPosition = xform * glm::vec4(0.F, 0.F, 0.F, 1.F);
                renderLight->setIntensity(getIntensity());
                renderLight->setDirection(renderDirection - renderPosition);
                render->addLight(renderLight);
            }
        }*/

        PointLight::PointLight()
        {}

        std::shared_ptr<PointLight> PointLight::create()
        {
            auto out = std::shared_ptr<PointLight>(new PointLight);
            return out;
        }

        /*void PointLight::render(const glm::mat4x4& xform, const std::shared_ptr<Render3D::Render>& render)
        {
            if (isEnabled())
            {
                auto renderLight = Render3D::PointLight::create();
                glm::vec3 renderPosition = xform * glm::vec4(0.F, 0.F, 0.F, 1.F);
                renderLight->setIntensity(getIntensity());
                renderLight->setPosition(renderPosition);
                render->addLight(renderLight);
            }
        }*/

        SpotLight::SpotLight()
        {}

        std::shared_ptr<SpotLight> SpotLight::create()
        {
            auto out = std::shared_ptr<SpotLight>(new SpotLight);
            return out;
        }

        /*void SpotLight::render(const glm::mat4x4& xform, const std::shared_ptr<Render3D::Render>& render)
        {
            if (isEnabled())
            {
                auto renderLight = Render3D::SpotLight::create();
                glm::vec3 renderDirection = xform * glm::vec4(_direction.x, _direction.y, _direction.z, 1.F);
                glm::vec3 renderPosition = xform * glm::vec4(0.F, 0.F, 0.F, 1.F);
                renderLight->setIntensity(getIntensity());
                renderLight->setConeAngle(_coneAngle);
                renderLight->setPosition(renderPosition);
                renderLight->setDirection(renderDirection - renderPosition);
                render->addLight(renderLight);
            }
        }*/

    } // namespace Scene3D
} // namespace djv

