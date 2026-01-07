#ifdef SERVER_EXPORT
#define SERVER_API __declspec(dllexport)
#else
#define SERVER_API __declspec(dllimport)
#endif // SERVER_EXPORT
