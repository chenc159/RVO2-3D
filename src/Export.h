
#ifndef RVO3D_EXPORT_H
#define RVO3D_EXPORT_H

#ifdef RVO3D_STATIC_DEFINE
#  define RVO3D_EXPORT
#  define RVO3D_NO_EXPORT
#else
#  ifndef RVO3D_EXPORT
#    ifdef RVO3D_EXPORTS
        /* We are building this library */
#      define RVO3D_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define RVO3D_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef RVO3D_NO_EXPORT
#    define RVO3D_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef RVO3D_DEPRECATED
#  define RVO3D_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef RVO3D_DEPRECATED_EXPORT
#  define RVO3D_DEPRECATED_EXPORT RVO3D_EXPORT RVO3D_DEPRECATED
#endif

#ifndef RVO3D_DEPRECATED_NO_EXPORT
#  define RVO3D_DEPRECATED_NO_EXPORT RVO3D_NO_EXPORT RVO3D_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef RVO3D_NO_DEPRECATED
#    define RVO3D_NO_DEPRECATED
#  endif
#endif

#endif
