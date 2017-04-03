#include <FLIGHT/Core/Game.hpp>
#include <FLIGHT/Entity/Coin.hpp>
#include <FLIGHT/Entity/Plane.hpp>

namespace FLIGHT {
Coin::Coin(const glm::vec3 & position) : m_timer(0) {
    m_position = position;
    m_model = GetGame().GetAssetMgr().GetModel("Box.obj");
    m_mbsRadius = MBS(GetAABB()).GetRadius();
}

void Coin::Display(DisplayImpl & gfx) {
    gfx.Dispatch(*this);
}

OBB Coin::GetOBB() { return OBB(GetAABB()); }

void Coin::MessageLoop() {
    while (auto msg = m_inbox.Poll()) {
        switch (msg->GetId()) {
        case Message::Id::Collision: {
            Collision * collision = static_cast<Collision *>(msg.get());
            if (dynamic_cast<Plane *>(collision->with.get())) {
                SetDeallocFlag();
            }
        } break;

        default:
            throw MessageError(msg->GetId());
        }
    }
}

void Coin::Update(const Time dt) {
    MessageLoop();
    m_timer += dt;
    const float offset = 0.0025 * sinf(2 * UTIL::PI * 0.0000005 * m_timer);
    m_position.y = m_position.y + offset;
}

AABB Coin::GetAABB() {
    return {{m_position.x - 0.5, m_position.y - 0.5, m_position.z - 0.5},
            {m_position.x + 1.5, m_position.y + 1.5, m_position.z + 1.5}};
}
}