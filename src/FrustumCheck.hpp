#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstring>

bool IntersectsFrustum(const glm::vec3 & objectPos,
		       const glm::vec3 & eyePos,
		       const glm::vec3 & viewDir);