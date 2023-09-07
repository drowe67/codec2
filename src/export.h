#ifndef CODEC2_EXPORT_H
#define CODEC2_EXPORT_H

#if defined(_MSC_VER)
#ifdef CODEC2_LIBRARY_EXPORTS
#define CODEC2_API __declspec(dllexport)
#else
#define CODEC2_API __declspec(dllimport)
#endif
#else
#define CODEC2_API
#endif

#endif  // CODEC2_EXPORT_H
