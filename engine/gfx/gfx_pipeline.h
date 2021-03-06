/* Copyright (c) The Grit Game Engine authors 2016
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* TODO:
 * clutter / rclutter
 * shaders
 * materials
 */

#include "../shared_ptr.h"
#include "../vect_util.h"

// Only things that are referenced from Lua AND can be destroyed (before shutdown) get a SharedPtr
class GfxPipeline;

#ifndef GfxPipeline_h
#define GfxPipeline_h

#include "gfx_internal.h"

void gfx_pipeline_init (void);

struct CameraOpts {
    float fovY, nearClip, farClip;
    float frustumOffset;
    float saturationMask;
    Vector3 mask; // colour
    bool decals;
    int debugMode;
    bool bloomAndToneMap;
    bool pointLights;
    bool particles;
    bool tracers;
    bool sky;
    bool sun;
    bool firstPerson;
    bool reflect;
    Vector3 reflectPlaneNormal;
    float reflectPlaneDist;
    Vector3 pos;
    Quaternion dir;
    CameraOpts (void)
      : fovY(55), nearClip(0.3f), farClip(800),
        frustumOffset(0), saturationMask(1), mask(1,1,1), decals(true), debugMode(0),
        bloomAndToneMap(true), pointLights(true), particles(true), tracers(true), sky(true),
        sun(true), firstPerson(true), reflect(false), reflectPlaneNormal(0,0,1),
        reflectPlaneDist(0), pos(0,0,0), dir(0,0,0,1)
    { }
};

/** The pipeline handles complete rendering of the scene for one 'eye' of a stereographic view. */
class GfxPipeline {
    Ogre::Camera *cam;
    GfxLastRenderStats gBufferStats;
    GfxLastRenderStats deferredStats;

    // gbuffer target
    Ogre::TexturePtr gBufferElements[3];
    Ogre::MultiRenderTarget *gBuffer;
    Ogre::RenderQueueInvocationSequence *rqisGbuffer;

    Ogre::TexturePtr hdrFb[3];

    // ultimate target
    Ogre::Viewport *targetViewport;
    Ogre::RenderQueueInvocationSequence *rqisDebug;
    Ogre::RenderQueueInvocationSequence *rqisDeferred;

    CameraOpts opts;

    public:
    GfxPipeline (const std::string &name, Ogre::Viewport *target_viewport);

    ~GfxPipeline (void);

    void render (const CameraOpts &opts, bool additive=false);

    const GfxLastRenderStats &getGBufferStats (void) { return gBufferStats; }
    const GfxLastRenderStats &getDeferredStats (void) { return deferredStats; }

    const CameraOpts &getCameraOpts (void) const { return opts; }
    Ogre::Camera *getCamera (void) const { return cam; }
    const Ogre::TexturePtr &getGBufferTexture (unsigned i) const { return gBufferElements[i]; }
};

#endif 
