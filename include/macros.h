#ifndef MACROS_H_
#define MACROS_H_

#define DEBUG(format, ...) \
	printf(format, ##__VA_ARGS__)

#define METHOD(iface, name, ret, this, ...) \
	static ret name(union {iface *_public; this;} \
	__attribute__((transparent_union)), ##__VA_ARGS__); \
	static typeof(name) *_##name = (typeof(name)*)name; \
	static ret name(this, ##__VA_ARGS__)

#define METHOD2(iface1, iface2, name, ret, this, ...) \
	static ret name(union {iface1 *_public1; iface2 *_public2; this;} \
	__attribute__((transparent_union)), ##__VA_ARGS__); \
	static typeof(name) *_##name = (typeof(name)*)name; \
	static ret name(this, ##__VA_ARGS__)

#define countof(x) sizeof(x) / sizeof(x[0])
#define DESTROY_IF(x) if(x != NULL) free(x)
#endif //MACROS_H_