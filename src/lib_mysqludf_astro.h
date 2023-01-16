#define LIBVERSION                  "1.0.0"
#define LIBNAME                     "lib_mysqludf_astro"
#define LIBBUILD                    __DATE__ " " __TIME__

#define MAX_RET_STRLEN              2048    // max string length returned by functions using strings

//#define DEBUG                       // debug output via syslog

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32)
#define DLLEXP __declspec(dllexport)
#else
#define DLLEXP
#endif

#ifdef __WIN__
typedef unsigned __int64 ulonglong;
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif  // __WIN__

// get_json_value() return codes
#define JSON_OK                  0
#define JSON_ERROR_EMPTY_STR    -1
#define JSON_ERROR_INVALID_STR  -2
#define JSON_ERROR_WRONG_TYPE   -3
#define JSON_ERROR_WRONG_VALUE  -4
#define JSON_ERROR_NOT_FOUND    -5


extern "C" {
// your function declaration here...
/**
 * astro_info
 *
 * Returns udf library info as JSON string
 * astro_info()
 *
 */
DLLEXP bool astro_info_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
DLLEXP void astro_info_deinit(UDF_INIT *initid);
DLLEXP char* astro_info(UDF_INIT *initid, UDF_ARGS *args, char* result, unsigned long* length, char *is_null, char *error);
}
