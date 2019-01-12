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

#include <djvUI/FlowLayout.h>

#include <djvCore/Math.h>

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        namespace Layout
        {
            struct Flow::Private
            {
                Spacing spacing = Spacing(Style::MetricsRole::Spacing, Style::MetricsRole::Spacing);
            };

            void Flow::_init(Context * context)
            {
                IContainer::_init(context);
                setClassName("djv::UI::Layout::Flow");
            }

            Flow::Flow() :
                _p(new Private)
            {}

            Flow::~Flow()
            {}

            std::shared_ptr<Flow> Flow::create(Context * context)
            {
                auto out = std::shared_ptr<Flow>(new Flow);
                out->_init(context);
                return out;
            }

            const Spacing& Flow::getSpacing() const
            {
                return _p->spacing;
            }

            void Flow::setSpacing(const Spacing& value)
            {
                DJV_PRIVATE_PTR();
                if (value == p.spacing)
                    return;
                p.spacing = value;
                _resize();
            }

            float Flow::getHeightForWidth(float width) const
            {
                float out = 0.f;
                if (auto style = _getStyle().lock())
                {
                    const glm::vec2 s = _p->spacing.get(style);

                    glm::vec2 pos = glm::vec2(0.f, 0.f);
                    float h = 0.f;
                    const auto children = getChildrenT<Widget>();
                    const int childrenSize = static_cast<int>(children.size());
                    for (int i = 0, j = 0; i < childrenSize; ++i, ++j)
                    {
                        const auto& ms = children[i]->getMinimumSize();
                        if (j && pos.x + ms.x > width - getMargin().getWidth(style))
                        {
                            pos.x = 0.f;
                            pos.y += h + s.y;
                            h = ms.y;
                            j = 0;
                        }
                        else
                        {
                            h = std::max(h, ms.y);
                        }
                        pos.x += ms.x + s.x;
                    }
                    out = pos.y + h + getMargin().getHeight(style);
                }
                return out;
            }

            void Flow::_preLayoutEvent(Event::PreLayout&)
            {
                if (auto style = _getStyle().lock())
                {
                    glm::vec2 minimumSize = glm::vec2(0.f, 0.f);
                    for (const auto& child : getChildrenT<Widget>())
                    {
                        const auto& childMinimumSize = child->getMinimumSize();
                        minimumSize.x = std::max(minimumSize.x, childMinimumSize.x);
                        minimumSize.y += childMinimumSize.y;
                    }
                    _setMinimumSize(minimumSize + getMargin().getSize(style));
                }
            }

            void Flow::_layoutEvent(Event::Layout&)
            {
                if (auto style = _getStyle().lock())
                {
                    const BBox2f& g = getMargin().bbox(getGeometry(), style);
                    const float gx = g.min.x;
                    const float gw = g.w();
                    const glm::vec2 s = _p->spacing.get(style);

                    glm::vec2 pos = g.min;
                    const auto children = getChildrenT<Widget>();
                    const int childrenSize = static_cast<int>(children.size());
                    typedef std::pair<std::shared_ptr<Widget>, BBox2f> Pair;
                    std::map<int, std::vector<Pair> > rows;
                    for (int i = 0, j = 0, k = 0; i < childrenSize; ++i, ++j)
                    {
                        const auto& childMinimumSize = children[i]->getMinimumSize();
                        if (j && pos.x + childMinimumSize.x > gx + gw)
                        {
                            pos.x = gx;
                            j = 0;
                            ++k;
                        }
                        rows[k].push_back(std::make_pair(children[i], BBox2f(pos.x, pos.y, childMinimumSize.x, childMinimumSize.y)));
                        pos.x += childMinimumSize.x + s.x;
                    }

                    for (const auto& i : rows)
                    {
                        float h = 0.f;
                        for (auto j : i.second)
                        {
                            h = std::max(h, j.second.h());
                        }
                        for (auto j : i.second)
                        {
                            BBox2f g = j.second;
                            g.min.y = pos.y;
                            g.max.y = pos.y + h;
                            j.first->setGeometry(Widget::getAlign(g, j.first->getMinimumSize(), j.first->getHAlign(), j.first->getVAlign()));
                        }
                        pos.y += h + s.y;
                    }
                }
            }

        } // namespace Layout
    } // namespace UI
} // namespace djv
