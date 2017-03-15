#include "GLRenderer.h"
#include "GLBuffer.h"
#include "GLBufferContainer.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLFrameBuffer.h"

namespace FreeWorldEngine {

namespace Renderer {

GLRenderer *pGLRenderer = 0;

GLRenderer::GLRenderer() :
	m_pMainFrameBuffer(new GLFrameBuffer(0))
{
	for (uint32 i = 0; i < TEXTURE_UNITS_COUNT; ++i)
		m_cachedTextures[i] = 0;

	m_cachedBufferConatiner = 0;

	for (uint32 i = 0; i < BUFFER_UNITS_COUNT; ++i)
		m_cachedBuffers[i] = 0;

    m_cachedProgram = 0;

	m_cachedRenderBuffer = 0;
	m_cachedFrameBuffer = 0;

	m_cachedDepthTest = false;
	m_cachedDepthTestFunc = DepthTestFunc_Less;

	for (uint32 i = 0; i < BLEND_UNITS_COUNT; ++i)
		m_cachedBlend[i] = false;
	m_cachedBlendRGBSrc = m_cachedBlendASrc = BlendFunc_SrcAlpha;
	m_cachedBlendASrc = m_cachedBlendADst = BlendFunc_InvSrcAlpha;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_cachedBlendRGBEquat = m_cachedBlendAEquat = BlendEquation_Add;
}

GLRenderer::~GLRenderer()
{
	delete m_pMainFrameBuffer;
}

IGPUBuffer *GLRenderer::createBuffer(uint64 size, IGPUBuffer::IGPUBufferUsage usage, void *pData)
{
	GLuint id;
	glGenBuffers(1, &id);

	GLBuffer *pBuffer = new GLBuffer(id);
	
	if (size) {
		bindBuffer(pBuffer, GL_ARRAY_BUFFER);
		glBufferData(GL_ARRAY_BUFFER, size, pData, GLBuffer::toGLUsage(usage));
	}

	return pBuffer;
}

void GLRenderer::destroyBuffer(IGPUBuffer *pBuffer)
{
	GLuint id = static_cast<GLBuffer*>(pBuffer)->GLid();
	glDeleteBuffers(1, &id);
	delete pBuffer;
}

void GLRenderer::setUniformBuffer(const IGPUBuffer * pBuffer, const uint32 bindingPoint) const
{
	bindBuffer(static_cast<const GLBuffer*>(pBuffer), GL_UNIFORM_BUFFER, bindingPoint);
}

IGPUBufferContainer *GLRenderer::createBufferContainer()
{
	GLuint id;
	glGenVertexArrays(1, &id);
	GLBufferContainer *pBufferConatiner = new GLBufferContainer(id);
	return pBufferConatiner;
}

void GLRenderer::destroyBufferContainer(IGPUBufferContainer *pBufferContainer)
{
	GLuint id = static_cast<GLBufferContainer*>(pBufferContainer)->id();
	glDeleteVertexArrays(1, &id);
	delete pBufferContainer;
}

IGPUTexture *GLRenderer::createTexture(IGPUTexture::IGPUTextureType type, const uint32 *size, const TextureFormat& internalFormat)
{
    GLenum GLtarget = GLTexture::GLtarget(type);
    GLenum GLinternalFormat = GLTexture::GLinternalFormat(internalFormat);
	GLenum GLformat = (internalFormat.pixelFormat == TextureFormat::PixelFormat_SpecialDepth) ? GL_DEPTH_COMPONENT : GL_RGBA;

    if (!GLtarget || !GLinternalFormat)
        return 0;

    GLuint id;
    glGenTextures(1, &id);

    GLTexture *pTexture = new GLTexture(type, id);
	bindTexture(pTexture, 0);

	glTexParameteri(GLtarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GLtarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    switch (type) {

	case IGPUTexture::IGPUTextureType_2DArray:
	case IGPUTexture::IGPUTextureType_3D: {
		glTexImage3D(GLtarget, 0, GLinternalFormat, size[0], size[1], size[2], 0, GLformat, GL_UNSIGNED_BYTE, 0);
		break;
	}
	
	case IGPUTexture::IGPUTextureType_1DArray:
	case IGPUTexture::IGPUTextureType_2D:
	case IGPUTexture::IGPUTextureType_Rectangle: {
		glTexImage2D(GLtarget, 0, GLinternalFormat, size[0], size[1], 0, GLformat, GL_UNSIGNED_BYTE, 0);
		break;
	}

	case IGPUTexture::IGPUTextureType_CubeMap: {
		for (GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X; target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; ++target)
			glTexImage2D(target, 0, GLinternalFormat, size[0], size[1], 0, GLformat, GL_UNSIGNED_BYTE, 0);
		break;
	}

    case IGPUTexture::IGPUTextureType_1D: {
        glTexImage1D(GLtarget, 0, GLinternalFormat, size[0], 0, GLformat, GL_UNSIGNED_BYTE, 0);
		break;
    }
	
    }

    return pTexture;
}

void GLRenderer::destroyTexture(IGPUTexture *pTexture)
{
	GLTexture *pGLTexture = static_cast<GLTexture*>(pTexture);
	GLuint id = pGLTexture->GLid();
	glDeleteTextures(1, &id);
	delete pTexture;
}

void GLRenderer::setTexture(const IGPUTexture *pTexture, const uint32 slot) const
{
	bindTexture(static_cast<const GLTexture*>(pTexture), slot);
}

IGPUShader *GLRenderer::createShader(IGPUShader::IGPUShaderType type)
{
	GLenum GLtype = GLShader::GLtype(type);
	if (!GLtype)
		return 0;

	GLuint id = glCreateShader(GLtype);
	if (!id)
		return 0;

	return new GLShader(id);
}

void GLRenderer::destroyShader(IGPUShader *pShader)
{
	GLShader *pGLShader = static_cast<GLShader*>(pShader);
	glDeleteShader(pGLShader->GLid());
	delete pShader;
}

IGPUProgram *GLRenderer::createProgram()
{
	GLuint id = glCreateProgram();
	if (!id)
		return 0;

	return new GLProgram(id);
}

void GLRenderer::destroyProgram(IGPUProgram *pProgram)
{
	GLProgram *pGLProgram = static_cast<GLProgram*>(pProgram);
	glDeleteProgram(pGLProgram->GLid());
	delete pProgram;
}

IGPURenderBuffer *GLRenderer::createRenderBuffer(const TextureFormat& format, const uint32 width, const uint32 height)
{
	GLuint id;
	glGenRenderbuffers(1, &id);
	if (!id)
		return 0;

	GLRenderBuffer *pGLRenderBuffer = new GLRenderBuffer(id);

	bindRenderBuffer(pGLRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GLTexture::GLinternalFormat(format), width, height);

	return pGLRenderBuffer;
}

void GLRenderer::destroyRenderBuffer(IGPURenderBuffer *pRenderBuffer)
{
	GLRenderBuffer *pGLRenderBuffer = static_cast<GLRenderBuffer*>(pRenderBuffer);
	GLuint id = pGLRenderBuffer->GLid();
	glDeleteRenderbuffers(1, &id);
	delete pRenderBuffer;
}

IGPUFrameBuffer *GLRenderer::createFrameBuffer()
{
	GLuint id;
	glGenFramebuffers(1, &id);
	if (!id)
		return 0;

	return new GLFrameBuffer(id);
}

void GLRenderer::destroyFrameBuffer(IGPUFrameBuffer *pFrameBuffer)
{
	GLFrameBuffer *pGLFrameBuffer = static_cast<GLFrameBuffer*>(pFrameBuffer);
	GLuint id = pGLFrameBuffer->GLid();
	glDeleteFramebuffers(1, &id);
	delete pFrameBuffer;
}

void GLRenderer::setFrameBuffer(const IGPUFrameBuffer *pFrameBuffer, const uint32 numDrawBuffers) const
{
	bindFrameBuffer(static_cast<const GLFrameBuffer*>(pFrameBuffer));
	if (pFrameBuffer) {
		GLenum buffers[16];
		for (uint32 i = 0; i < numDrawBuffers; ++i)
			buffers[i] = GL_COLOR_ATTACHMENT0 + i;
		glDrawBuffers(numDrawBuffers, buffers);
	}
}

IGPUFrameBuffer *GLRenderer::mainFrameBuffer() const
{
	return m_pMainFrameBuffer;
}

void GLRenderer::renderGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const uint32 firstVertex, const uint32 numVertices) const
{
	GLenum mode = GLPrimitiveFormat(primitiveFormat);
	if (mode) {
		bindProgram(static_cast<const GLProgram*>(pProgram));
		bindBufferContainer(static_cast<const GLBufferContainer*>(pBufferContainer));
		glDrawArrays(mode, firstVertex, numVertices);
	}
}

void GLRenderer::renderIndexedGeometry(const IGPUProgram *pProgram, const IGPUBufferContainer *pBufferContainer, const PrimitiveFormat primitiveFormat, const Type indicesType, const uint32 numIndices, const uint32 offset) const
{
	GLenum mode = GLPrimitiveFormat(primitiveFormat);
	GLenum type = GLType(indicesType);
	if (mode && type) {
		bindProgram(static_cast<const GLProgram*>(pProgram));
		bindBufferContainer(static_cast<const GLBufferContainer*>(pBufferContainer));
		glDrawElements(mode, numIndices, type, (const void*)offset);
	}
}

void GLRenderer::enableDepthTest(const DepthTestFunc func)
{
	if (m_cachedDepthTestFunc != func) {
		m_cachedDepthTestFunc = func;
		glDepthFunc(GLDepthTestFunc(func));
	}

	if (!m_cachedDepthTest) {
		m_cachedDepthTest = true;
		glEnable(GL_DEPTH_TEST);
	}
}

void GLRenderer::disableDepthTest()
{
	if (m_cachedDepthTest) {
		m_cachedDepthTest = false;
		glDisable(GL_DEPTH_TEST);
	}
}

void GLRenderer::setColorWriteMask(bool red, bool green, bool blue, bool alpha)
{
	glColorMask(red, green, blue, alpha);
}

void GLRenderer::setDepthWriteMask(bool depth)
{
	glDepthMask(depth);
}

void GLRenderer::enableBlend(const int32 slot)
{
	if (slot < 0) {
		for (uint32 i = 0; i < BLEND_UNITS_COUNT; ++i)
			m_cachedBlend[slot] = true;
		glEnable(GL_BLEND);
	}
	else if (!m_cachedBlend[slot]) {
		m_cachedBlend[slot] = true;
		glEnablei(GL_BLEND, GL_DRAW_BUFFER0 + slot);
	}
}

void GLRenderer::disableBlend(const int32 slot)
{
	if (slot < 0) {
		for (uint32 i = 0; i < BLEND_UNITS_COUNT; ++i)
			m_cachedBlend[slot] = false;
		glDisable(GL_BLEND);
	}
	else if (m_cachedBlend[slot]) {
		m_cachedBlend[slot] = false;
		glDisablei(GL_BLEND, GL_DRAW_BUFFER0 + slot);
	}
}

void GLRenderer::setBlendEquation(const BlendEquation funcRGB, const BlendEquation funcA)
{
	if (m_cachedBlendRGBEquat != funcRGB && m_cachedBlendAEquat != funcA) {
		m_cachedBlendRGBEquat = funcRGB;
		m_cachedBlendAEquat = funcA;
		glBlendEquationSeparate(GLBlendEquation(funcRGB), GLBlendEquation(funcA));
	}
}

void GLRenderer::setBlendFunc(const BlendFunc funcSrcRGB, const BlendFunc funcDstRGB, const BlendFunc funcSrcA, const BlendFunc funcDstA)
{
	if (m_cachedBlendRGBSrc != funcSrcRGB &&
		m_cachedBlendRGBDst != funcDstRGB &&
		m_cachedBlendASrc != funcSrcA &&
		m_cachedBlendADst != funcDstA) {
		m_cachedBlendRGBSrc = funcSrcRGB;
		m_cachedBlendRGBDst = funcDstRGB;
		m_cachedBlendASrc = funcSrcA;
		m_cachedBlendADst = funcDstA;
		glBlendFuncSeparate(GLBlendFunc(funcSrcRGB), GLBlendFunc(funcDstRGB), GLBlendFunc(funcSrcA), GLBlendFunc(funcDstA));
	}
}

void GLRenderer::setViewport(const uint32 x, const uint32 y, const uint32 width, const uint32 height)
{
	glViewport(x, y, width, height);
}

void GLRenderer::viewport(uint32& x, uint32& y, uint32& width, uint32& height) const
{
	GLint vp[4];
	glGetIntegerv(GL_VIEWPORT, vp);
	x = vp[0];
	y = vp[1];
	width = vp[2];
	height = vp[3];
}

void GLRenderer::setDepthRange(const float near, const float far)
{
	glDepthRange(near, far);
}

void GLRenderer::depthRange(float& near, float& far) const
{
	float dr[2];
	glGetFloatv(GL_DEPTH_RANGE, dr);
	near = dr[0];
	far = dr[1];
}

void GLRenderer::bindBuffer(const GLBuffer *pBuffer, GLenum GLTarget, const uint32 bindingPoint) const
{
	int32 cacheIdx = -1;
	switch (GLTarget) {
	case GL_ARRAY_BUFFER : { cacheIdx = 0; break; }
	case GL_COPY_READ_BUFFER : { cacheIdx = 1; break; }
	case GL_COPY_WRITE_BUFFER : { cacheIdx = 2; break; }
	case GL_ELEMENT_ARRAY_BUFFER: { cacheIdx = 3; break; }
	case GL_PIXEL_PACK_BUFFER: { cacheIdx = 4; break; }
	case GL_PIXEL_UNPACK_BUFFER: { cacheIdx = 5; break; }
	case GL_TEXTURE_BUFFER: { cacheIdx = 6; break; }
	case GL_TRANSFORM_FEEDBACK_BUFFER: { cacheIdx = 7; break; }
	case GL_UNIFORM_BUFFER: { cacheIdx = 8 + bindingPoint; break; }
	default: break;
	}

	if (cacheIdx >= 0) {
		if (m_cachedBuffers[cacheIdx] == pBuffer)
			return;
		m_cachedBuffers[cacheIdx] = pBuffer;
	}

	GLuint id = pBuffer ? pBuffer->GLid() : 0;
	switch (GLTarget) {
	case GL_UNIFORM_BUFFER: { glBindBufferBase(GLTarget, bindingPoint, id); break; }
	default: { glBindBuffer(GLTarget, id); break; }
	}
}

void GLRenderer::bindBufferContainer(const GLBufferContainer *pBufferContainer) const
{
	if (m_cachedBufferConatiner == pBufferContainer)
		return;
	m_cachedBufferConatiner = pBufferContainer;
	glBindVertexArray(pBufferContainer ? pBufferContainer->id() : 0);
}

void GLRenderer::bindTexture(const GLTexture *pTexture, uint32 unit) const
{
	if (m_cachedTextures[unit] == pTexture)
		return;
	m_cachedTextures[unit] = pTexture;
	glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GLTexture::GLtarget(pTexture->type()), pTexture->GLid());
}

void GLRenderer::bindProgram(const GLProgram *pProgram) const
{
    if (m_cachedProgram == pProgram)
        return;
    m_cachedProgram = pProgram;
    glUseProgram(pProgram ? pProgram->GLid() : 0);
}

void GLRenderer::bindRenderBuffer(const GLRenderBuffer *pRenderBuffer) const
{
	if (m_cachedRenderBuffer == pRenderBuffer)
		return;
	m_cachedRenderBuffer = pRenderBuffer;
	glBindRenderbuffer(GL_RENDERBUFFER, pRenderBuffer ? pRenderBuffer->GLid() : 0);
}

void GLRenderer::bindFrameBuffer(const GLFrameBuffer *pFrameBuffer) const
{
	if (m_cachedFrameBuffer == pFrameBuffer)
		return;
	m_cachedFrameBuffer = pFrameBuffer;
	glBindFramebuffer(GL_FRAMEBUFFER, pFrameBuffer ? pFrameBuffer->GLid() : 0);
}

GLenum GLRenderer::GLPrimitiveFormat(PrimitiveFormat primitiveFormat)
{
	switch (primitiveFormat) {
	case PrimitiveFormat_Points: return GL_POINTS;
	case PrimitiveFormat_Lines: return GL_LINES;
	case PrimitiveFormat_LineStrip: return GL_LINE_STRIP;
	case PrimitiveFormat_LineLoop: return GL_LINE_LOOP;
	case PrimitiveFormat_Triangles: return GL_TRIANGLES;
	case PrimitiveFormat_TriangleStrip: return GL_TRIANGLE_STRIP;
	case PrimitiveFormat_TrangleFan: return GL_TRIANGLE_FAN;
	}
	return 0;
}

GLenum GLRenderer::GLType(Type type)
{
	switch (type) {
	case TYPE_SIGNED_INT_8: return GL_BYTE;
	case TYPE_SIGNED_INT_16: return GL_SHORT;
	case TYPE_SIGNED_INT_32: return GL_INT;
	case TYPE_UNSIGNED_INT_8: return GL_UNSIGNED_BYTE;
	case TYPE_UNSIGNED_INT_16: return GL_UNSIGNED_SHORT;
	case TYPE_UNSIGNED_INT_32: return GL_UNSIGNED_INT;
	case TYPE_FLOAT: return GL_FLOAT;
	}
	return 0;
}

GLenum GLRenderer::GLDepthTestFunc(DepthTestFunc func)
{
	switch (func) {
	case DepthTestFunc_Never: return GL_NEVER;
	case DepthTestFunc_Always: return GL_ALWAYS;
	case DepthTestFunc_Less: return GL_LESS;
	case DepthTestFunc_LessEqual: return GL_LEQUAL;
	case DepthTestFunc_Equal: return GL_EQUAL;
	case DepthTestFunc_Greater: return GL_GREATER;
	case DepthTestFunc_GreaterEqual: return GL_GEQUAL;
	case DepthTestFunc_NotEqual: return GL_NOTEQUAL;
	}
	return 0;
}

GLenum GLRenderer::GLBlendFunc(BlendFunc func)
{
	switch (func) {
	case BlendFunc_Zero: return GL_ZERO;
	case BlendFunc_One: return GL_ONE;
	case BlendFunc_SrcColor: return GL_SRC_COLOR;
	case BlendFunc_InvSrcColor: return GL_ONE_MINUS_SRC_COLOR;
	case BlendFunc_DstColor: return GL_DST_COLOR;
	case BlendFunc_InvDstColor: return GL_ONE_MINUS_DST_COLOR;
	case BlendFunc_SrcAlpha: return GL_SRC_ALPHA;
	case BlendFunc_InvSrcAlpha: return GL_ONE_MINUS_SRC_ALPHA;
	case BlendFunc_DstAlpha: return GL_DST_ALPHA;
	case BlendFunc_InvDstAlpha: return GL_ONE_MINUS_DST_ALPHA;
	case BlendFunc_ConstColor: return GL_CONSTANT_COLOR;
	case BlendFunc_InvConstColor: return GL_ONE_MINUS_CONSTANT_COLOR;
	case BlendFunc_ConstAlpha: return GL_CONSTANT_ALPHA;
	case BlendFunc_InvConstAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
	}
	return 0;
}

GLenum GLRenderer::GLBlendEquation(BlendEquation func)
{
	switch (func) {
	case BlendEquation_Add: return GL_FUNC_ADD;
	case BlendEquation_Sub: return GL_FUNC_SUBTRACT;
	case BlendEquation_ReverseSub: return GL_FUNC_REVERSE_SUBTRACT;
	case BlendEquation_Min: return GL_MIN;
	case BlendEquation_Max: return GL_MAX;
	}
	return 0;
}

} // namespace

} // namespace