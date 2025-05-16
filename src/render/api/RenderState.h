#pragma once

namespace Engine {

    enum class DepthFunc {
        Never, Less, Equal, Lequal, Greater, NotEqual, Gequal, Always
    };

    enum class CullMode {
        None, Back, Front
    };

    enum class BlendMode {
        None, Alpha, Additive
    };

    struct RenderState {
        bool depthTest = true;
        bool depthWrite = true;
        DepthFunc depthFunc = DepthFunc::Less;

        CullMode cullMode = CullMode::Back;

        BlendMode blend = BlendMode::None;

        // bool operator==(const RenderState& other) const;
    };

} // namespace Engine
