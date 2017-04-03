#pragma once

#include <FLIGHT/Graphics/DisplayImpl.hpp>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// FIXME: I'm in the process of refactoring all OpenGL stuff into
// this class. Some stuff like assets, the window, and terrain
// generation aren't completely integrated.

namespace FLIGHT {
    class OpenGLDisplayImpl : public DisplayImpl {
	struct {
	    GLuint chunkIndicesHQ;
	    GLuint chunkIndicesMQ;
	    GLuint chunkIndicesLQ;
	    GLuint chunkIndicesDQ;
	} m_chunkInfo;

	void InitChunkIndexBufs();

    public:
	OpenGLDisplayImpl();
	void Dispatch(Plane & plane) override;
	void Dispatch(Coin & coin) override;
	void Dispatch(TerrainChunk & chunk) override;
	void Dispatch(Bullet & bullet) override;
	void Dispatch(Thruster & thruster) override;
	void Dispatch(TitleScreen & titleScreen) override;
	void Dispatch(CreditsScreen & creditsScreen) override;
	void Dispatch(WorldLoader & worldLoader) override;
	void Dispatch(World & world) override;
	void Dispatch(WorldTransitionIn & wtin) override;
	void Dispatch(SkyManager & sky) override;
    };
}