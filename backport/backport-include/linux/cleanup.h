#ifndef _BACKPORTS_LINUX_CLEANUP_H
#define _BACKPORTS_LINUX_CLEANUP_H
#include <linux/version.h>

#if LINUX_VERSION_IS_GEQ(6,5,0) || \
	LINUX_VERSION_IN_RANGE(6,1,79, 6,2,0) || \
	LINUX_VERSION_IN_RANGE(5,15,195, 5,16,0) || \
	LINUX_VERSION_IN_RANGE(5,10,246, 5,11,0)
#include_next <linux/cleanup.h>
#else
#include <linux/compiler.h>

#ifndef __free
#define DEFINE_FREE(_name, _type, _free) \
	static inline void __free_##_name(void *p) { _type _T = *(_type *)p; _free; }
#define __free(_name)	__cleanup(__free_##_name)
#endif /* __free */

#ifndef no_free_ptr
#define no_free_ptr(p) \
	({ __auto_type __ptr = (p); (p) = NULL; __ptr; })
#endif /* no_free_ptr */

#ifndef return_ptr
#define return_ptr(p)	return no_free_ptr(p)
#endif /* return_ptr */

#define DEFINE_CLASS(_name, _type, _exit, _init, _init_args...)		\
typedef _type class_##_name##_t;					\
static inline void class_##_name##_destructor(_type *p)			\
{ _type _T = *p; _exit; }						\
static inline _type class_##_name##_constructor(_init_args)		\
{ _type t = _init; return t; }

#define EXTEND_CLASS(_name, ext, _init, _init_args...)			\
typedef class_##_name##_t class_##_name##ext##_t;			\
static inline void class_##_name##ext##_destructor(class_##_name##_t *p)\
{ class_##_name##_destructor(p); }					\
static inline class_##_name##_t class_##_name##ext##_constructor(_init_args) \
{ class_##_name##_t t = _init; return t; }

#define CLASS(_name, var)						\
	class_##_name##_t var __cleanup(class_##_name##_destructor) =	\
		class_##_name##_constructor

#define DEFINE_GUARD(_name, _type, _lock, _unlock) \
	DEFINE_CLASS(_name, _type, _unlock, ({ _lock; _T; }), _type _T)

#define guard(_name) \
	CLASS(_name, __UNIQUE_ID(guard))

#define scoped_guard(_name, args...)					\
	for (CLASS(_name, scope)(args),					\
	     *done = NULL; !done; done = (void *)1)

#define __DEFINE_UNLOCK_GUARD(_name, _type, _unlock, ...)		\
typedef struct {							\
	_type *lock;							\
	__VA_ARGS__;							\
} class_##_name##_t;							\
									\
static inline void class_##_name##_destructor(class_##_name##_t *_T)	\
{									\
	if (_T->lock) { _unlock; }					\
}

#define __DEFINE_LOCK_GUARD_1(_name, _type, _lock)			\
static inline class_##_name##_t class_##_name##_constructor(_type *l)	\
{									\
	class_##_name##_t _t = { .lock = l }, *_T = &_t;		\
	_lock;								\
	return _t;							\
}

#define __DEFINE_LOCK_GUARD_0(_name, _lock)				\
static inline class_##_name##_t class_##_name##_constructor(void)	\
{									\
	class_##_name##_t _t = { .lock = (void*)1 },			\
			 *_T __maybe_unused = &_t;			\
	_lock;								\
	return _t;							\
}

#define DEFINE_LOCK_GUARD_1(_name, _type, _lock, _unlock, ...)		\
__DEFINE_UNLOCK_GUARD(_name, _type, _unlock, __VA_ARGS__)		\
__DEFINE_LOCK_GUARD_1(_name, _type, _lock)

#define DEFINE_LOCK_GUARD_0(_name, _lock, _unlock, ...)			\
__DEFINE_UNLOCK_GUARD(_name, void, _unlock, __VA_ARGS__)		\
__DEFINE_LOCK_GUARD_0(_name, _lock)

#endif /* < 6.5 */

#endif /* _BACKPORTS_LINUX_CLEANUP_H */
