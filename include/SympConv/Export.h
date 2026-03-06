#ifndef SYMPCONV_EXPORT_H
#define SYMPCONV_EXPORT_H

#ifdef SYMPCONV_EXPORTS
#define SYMPCONV_API __declspec(dllexport)
#else
#define SYMPCONV_API __declspec(dllimport)
#endif

#endif // SYMPCONV_EXPORT_H