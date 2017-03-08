#include "Plane.hpp"

namespace FLIGHT {
Plane::Plane() : m_pitch{}, m_roll{}, m_thrust{}, m_yVelocity{} {}

void AssertGLStatus(const std::string & context);

void Plane::Display(ShaderProgram & shader) {
    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, m_position);
    modelMatrix = glm::rotate(modelMatrix, m_rotation.y, {0, 1, 0});
    modelMatrix = glm::rotate(modelMatrix, m_rotation.z, {0, 0, 1});
    modelMatrix = glm::rotate(modelMatrix, m_rotation.x, {1, 0, 0});
    for (auto & comp : m_components) {
        comp.Display(modelMatrix, shader);
    }
}

AABB Plane::GetAABB() {
    AABB ret = m_components.front().GetAABB();
    auto it = m_components.begin();
    ++it;
    for (; it != m_components.end(); ++it) {
	ret.Merge(it->GetAABB());
    }
    ret.Rotate(m_rotation.y, {0, 1, 0});
    ret.Rotate(m_rotation.z, {0, 0, 1});
    ret.Rotate(m_rotation.x, {1, 0, 0});
    ret.Translate(m_position);
    return ret;
}

void Plane::SetThrust(const float thrust) { m_thrust = thrust; }

float Plane::GetThrust() const { return m_thrust; }

void Plane::MessageLoop() {
    while (auto msg = m_inbox.Poll()) {
        switch (msg->GetId()) {
        case Message::Id::Collision: {
            Collision * collision = static_cast<Collision *>(msg.get());
            if (dynamic_cast<Plane *>(collision->with.get())) {
                SetDeallocFlag();
            } else if (dynamic_cast<Coin *>(collision->with.get())) {
                SetColor({0.949f, 0.765f, 0.027f, 1.f});
                BeginDecay();
                m_outbox.Push(std::make_unique<PickedUpCoin>());
            } else if (dynamic_cast<TerrainChunk *>(collision->with.get())) {
                throw std::runtime_error("Ran into terrain!");
            }
        } break;

        default:
            throw MessageError(msg->GetId());
        }
    }
}

void Plane::Update(const Time dt) {
    MessageLoop();
    ColorMixDecay::Update(dt);
    const float rateFactor = 0.000035f * dt;
    static const float yCeil = GetElevationLimit();
    static const float yFloor = -3.f;
    const float yDisp = std::sin(m_rotation.x) * rateFactor;
    m_yVelocity = MATH::lerp(yDisp, m_yVelocity, 0.05 * dt * 0.0001f);
    if (m_position.y + m_yVelocity < yCeil &&
        m_position.y + m_yVelocity > yFloor) {
        m_position.y += m_yVelocity;
    }
    m_position.z -=
        std::cos(m_rotation.y) * rateFactor * std::cos(m_rotation.x);
    m_position.x -=
        std::sin(m_rotation.y) * rateFactor * std::cos(m_rotation.x);
    static const float turningRate = 0.000000025f;
    m_rotation.y += turningRate * dt * m_roll;
}

const glm::vec3 & Plane::GetDirection() const { return m_direction; }

void Plane::SetDirection(const glm::vec3 & direction) {
    m_direction = direction;
}

void Plane::SetPitch(const float pitch) {
    m_rotation.x = glm::radians(pitch);
    m_pitch = pitch;
}

float Plane::GetPitch() const { return m_pitch; }

void Plane::SetRoll(const float roll) {
    m_rotation.z = glm::radians(roll);
    m_roll = roll;
}

float Plane::GetRoll() const { return m_roll; }
}
