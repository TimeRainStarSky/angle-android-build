#include <stdio.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

int main(void) {
    // 1. 获取 EGLDisplay，优先尝试 Surfaceless 平台
    EGLDisplay display = EGL_NO_DISPLAY;
    PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplay =
        (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");

    if (getPlatformDisplay) {
        display = getPlatformDisplay(EGL_PLATFORM_SURFACELESS_MESA, EGL_DEFAULT_DISPLAY, NULL);
    }
    // 如果 Surfaceless 失败，回退到默认显示
    if (display == EGL_NO_DISPLAY) {
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    }
    if (display == EGL_NO_DISPLAY) {
        fprintf(stderr, "eglGetDisplay failed\n");
        return 1;
    }

    // 2. 初始化 EGL
    EGLint major, minor;
    if (!eglInitialize(display, &major, &minor)) {
        fprintf(stderr, "eglInitialize failed (error 0x%x)\n", eglGetError());
        return 1;
    }

    // 3. 选择配置
    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0) {
        fprintf(stderr, "eglChooseConfig failed\n");
        eglTerminate(display);
        return 1;
    }

    // 4. 创建上下文 (OpenGL ES 2.0，兼容性最好；驱动通常会自动升级到 3.x)
    EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT) {
        fprintf(stderr, "eglCreateContext failed (error 0x%x)\n", eglGetError());
        eglTerminate(display);
        return 1;
    }

    // 5. 绑定上下文（Surfaceless：读写表面都传 EGL_NO_SURFACE）
    if (!eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context)) {
        fprintf(stderr, "eglMakeCurrent failed (error 0x%x)\n", eglGetError());
        eglDestroyContext(display, context);
        eglTerminate(display);
        return 1;
    }

    // 6. 获取并输出 GL 版本与渲染器信息
    const char *version  = (const char *)glGetString(GL_VERSION);
    const char *renderer = (const char *)glGetString(GL_RENDERER);

    if (version)  printf("%s ", version);
    if (renderer) printf("%s", renderer);
    printf("\n");

    // 7. 清理
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(display, context);
    eglTerminate(display);
    return 0;
}
