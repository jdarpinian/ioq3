// CORE

/*** Start of inlined file: humblenet_core.cpp ***/

/*** Start of inlined file: humblenet.h ***/
#ifndef HUMBLENET_H
#define HUMBLENET_H

#include <stddef.h>
#include <stdint.h>

#ifdef HUMBLENET_STATIC
	#define HUMBLENET_DLL_EXPORT
#else
	#ifdef WIN32
		#ifdef HUMBLENET_DLL_BUILD
			#define HUMBLENET_DLL_EXPORT __declspec(dllexport)
		#else
			#define HUMBLENET_DLL_EXPORT __declspec(dllimport)
		#endif
	#else // GCC
		#if defined(__GNUC__) && __GNUC__ >= 4
		# define HUMBLENET_DLL_EXPORT __attribute__ ((visibility("default")))
		#else
		# define HUMBLENET_DLL_EXPORT
		#endif
	#endif
#endif

#if defined(WIN32)
	#define HUMBLENET_CALL __cdecl
#elif defined(__GNUC__)
	#if defined(__LP64__)
		#define HUMBLENET_CALL
	#else
		#define HUMBLENET_CALL __attribute__((cdecl))
	#endif
#else
	#define HUMBLENET_CALL
#endif

#define HUMBLENET_API HUMBLENET_DLL_EXPORT

#define HUMBLENET_MAJOR_VERSION 1
#define HUMBLENET_MINOR_VERSION 0
#define HUMBLENET_PATCHLEVEL 0

/**
 * Create a comparable version number value
 *
 * e.g.  (1,2,3) -> (0x010203)
 */
#define HUMBLENET_VERSIONNUM(X, Y, Z) \
	(((X) << 16) | ((Y) << 8) | (Z))

/**
 * this is the version that we are currently compiling against
 */
#define HUMBLENET_COMPILEDVERSION \
	HUMBLENET_VERSIONNUM(HUMBLENET_MAJOR_VERSION, HUMBLENET_MINOR_VERSION, HUMBLENET_PATCHLEVEL)

/**
 * check if compiled with HumbleNet version of at least X.Y.Z
 */
#define HUMBLENET_VERSION_ATLEAST(X, Y, Z) \
	(HUMBLENET_COMPILEDVERSION >= HUMBLENET_VERSIONNUM(X, Y, Z))

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t ha_bool;
typedef uint32_t PeerId;

/*
 * Get the version of the humblenet library we are linked to
 */
HUMBLENET_API uint32_t HUMBLENET_CALL humblenet_version();

/*
 * Initialize the core humblenet library
 */
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_init();

/*
 * Shutdown the entire humblenet library
 */
HUMBLENET_API void HUMBLENET_CALL humblenet_shutdown();

/*
 * Allow network polling to occur. This is already thread safe and must NOT be within a lock/unlock block
 */
HUMBLENET_API void HUMBLENET_CALL humblenet_poll(int ms);

/*
 * Get error string
 * Return value will stay valid until next call to humblenet_set_error
 * or humblenet_clear_error() is called
 */
HUMBLENET_API const char * HUMBLENET_CALL humblenet_get_error();

/*
 * Set error string
 * Parameter is copied to private storage
 * Must not be NULL
 */
HUMBLENET_API void HUMBLENET_CALL humblenet_set_error(const char *error);

/*
 * Clear error string
 */
HUMBLENET_API void HUMBLENET_CALL humblenet_clear_error();

/*
 * Set the value of a hint
 */
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_set_hint(const char* name, const char* value);

/*
 * Get the value of a hint
 */
HUMBLENET_API const char* HUMBLENET_CALL humblenet_get_hint(const char* name);

/*
 * If using the loader this will set the loader path.
 * If using the loading returns 1 if the library loads or 0 if it fails to load.
 *    Use humblenet_get_error() to see why loading failed.
 * If not using the loader, simply returns 1
 */
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_loader_init(const char* libpath);

#ifdef __cplusplus
}
#endif

#endif /*HUMBLENET_H */

/*** End of inlined file: humblenet.h ***/


/*** Start of inlined file: humblenet_p2p.h ***/
#ifndef HUMBLENET_P2P_H
#define HUMBLENET_P2P_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum SendMode {
	// Send the message reliably.
	SEND_RELIABLE = 0,

	// As above but buffers the data for more efficient packets transmission
	SEND_RELIABLE_BUFFERED = 1
} SendMode;

/*
* Is the peer-to-peer network supported on this platform.
*/
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_supported();

/*
* Initialize the peer-to-peer library.
*/
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_init(const char* server, const char* client_token, const char* client_secret, const char* auth_token);

/*
* Is the peer-to-peer network supported on this platform.
*/
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_is_initialized();

/*
* Get the current peer ID of this client
* returns 0 if not yet connected
*/
HUMBLENET_API PeerId HUMBLENET_CALL humblenet_p2p_get_my_peer_id();

/*
* Register an alias for this peer so it can be found by name
*/
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_register_alias(const char* name);

/*
 * Unregister an alias for this peer. Passing NULL will unpublish all aliases for the peer.
 */
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_unregister_alias(const char* name);

/*
 * Create a virtual peer for an alias on the server
 */
HUMBLENET_API PeerId HUMBLENET_CALL humblenet_p2p_virtual_peer_for_alias(const char* name);

/*
* Send a message to a peer.
*/
HUMBLENET_API int HUMBLENET_CALL humblenet_p2p_sendto(const void* message, uint32_t length, PeerId topeer, SendMode mode, uint8_t nChannel);

/*
* Test if a message is available on the specified channel.
*/
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_peek(uint32_t* length, uint8_t nChannel);

/*
* Receive a message sent from a peer
*/
HUMBLENET_API int HUMBLENET_CALL humblenet_p2p_recvfrom(void* message, uint32_t length, PeerId* frompeer, uint8_t nChannel);

/*
* Disconnect a peer
*/
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_disconnect(PeerId peer);

/*
* Wait for an IO event to occur
*/
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_p2p_wait(int ms);

#ifndef __EMSCRIPTEN__
/*
* POSIX compatible select use to wait on IO from either humblenet or use supplied fds
*/
#if defined(WIN32)
	typedef struct fd_set fd_set;
#else
	#include <sys/select.h>
#endif

HUMBLENET_API int HUMBLENET_CALL humblenet_p2p_select(int nfds, fd_set *readfds, fd_set *writefds,
													  fd_set *exceptfds, struct timeval *timeout);
#endif

#ifdef __cplusplus
}
#endif

#endif /* HUMBLENET_P2P_H */

/*** End of inlined file: humblenet_p2p.h ***/


/*** Start of inlined file: humblenet_p2p_internal.h ***/
#ifndef HUMBLENET_P2P_INTERNAL
#define HUMBLENET_P2P_INTERNAL


/*** Start of inlined file: libsocket.h ***/
//
//  libsocket.h
//  humblenet
//
//  Created by Chris Rudd on 3/11/16.
//
//

#ifndef LIBSOCKET_H
#define LIBSOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

struct internal_socket_t;
struct internal_context_t;

struct internal_callbacks_t {
	// called to return the local sdp offer
	int (*on_sdp)( internal_socket_t*, const char* offer, void* user_data );
	// called when a local candidate is discovered
	int (*on_ice_candidate)( internal_socket_t*, const char* candidate, void* user_data);

	// called for incoming connections to indicate the connection process is completed.
	int (*on_accept) (internal_socket_t*, void* user_data);
	// called for outgoing connections to indicate the connection process is completed.
	int (*on_connect) (internal_socket_t*, void* user_data);
	// called when an incoming datachannel is established
	int (*on_accept_channel)(internal_socket_t*, const char* name, void* user_data);
	// called when an outgoing datachannel is established
	int (*on_connect_channel)(internal_socket_t*, const char* name, void* user_data);
	// called each time data is received.
	int (*on_data)( internal_socket_t* s, const void* data, int len, void* user_data );
	// called to indicate the connection is wriable.
	int (*on_writable) (internal_socket_t*, void* user_data);
	// called when the conenction is terminated (from either side)
	int (*on_disconnect)( internal_socket_t* s, void* user_data );
	// called when socket object will be destroyed.
	int (*on_destroy)( internal_socket_t* s, void* user_data );
};

internal_context_t* internal_init(internal_callbacks_t*);
void internal_deinit(internal_context_t*);

bool internal_supports_webRTC( internal_context_t *);
void internal_register_protocol( internal_context_t*, const char* protocol, internal_callbacks_t* callbacks );

internal_socket_t* internal_connect_websocket( const char* addr, const char* protocol );
void internal_set_stun_servers( internal_context_t*, const char** servers, int count);
void internal_add_turn_server( internal_context_t*, const char* server, const char* username, const char* password);
internal_socket_t* internal_create_webrtc(internal_context_t *);
int internal_create_offer( internal_socket_t* socket );
int internal_set_offer( internal_socket_t* socket, const char* offer );
int internal_set_answer( internal_socket_t* socket, const char* offer );
int internal_create_channel( internal_socket_t* socket, const char* name );
int internal_add_ice_candidate( internal_socket_t*, const char* candidate );

void internal_set_data( internal_socket_t*, void* user_data);
void internal_set_callbacks(internal_socket_t* socket, internal_callbacks_t* callbacks );
int internal_write_socket( internal_socket_t*, const void* buf, int len );
void internal_close_socket( internal_socket_t* );

#ifdef __cplusplus
}
#endif

#endif /* LIBSOCKET_H */

/*** End of inlined file: libsocket.h ***/


/*** Start of inlined file: humblenet_p2p_signaling.h ***/
#ifndef HUMBLENET_SIGNALING
#define HUMBLENET_SIGNALING


/*** Start of inlined file: humblepeer.h ***/
#ifndef HUMBLEPEER_H
#define HUMBLEPEER_H

// HumbleNet internal, do not include

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <map>


/*** Start of inlined file: humblepeer_generated.h ***/
// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_HUMBLEPEER_HUMBLENET_HUMBLEPEER_H_
#define FLATBUFFERS_GENERATED_HUMBLEPEER_HUMBLENET_HUMBLEPEER_H_


/*** Start of inlined file: flatbuffers.h ***/
#ifndef FLATBUFFERS_H_
#define FLATBUFFERS_H_

#include <algorithm>

// TODO: These includes are for mitigating the pains of users editing their
// source because they relied on flatbuffers.h to include everything for them.

/*** Start of inlined file: array.h ***/
#ifndef FLATBUFFERS_ARRAY_H_
#define FLATBUFFERS_ARRAY_H_

#include <cstdint>
#include <memory>


/*** Start of inlined file: base.h ***/
#ifndef FLATBUFFERS_BASE_H_
#define FLATBUFFERS_BASE_H_

// clang-format off

// If activate should be declared and included first.
#if defined(FLATBUFFERS_MEMORY_LEAK_TRACKING) && \
	defined(_MSC_VER) && defined(_DEBUG)
  // The _CRTDBG_MAP_ALLOC inside <crtdbg.h> will replace
  // calloc/free (etc) to its debug version using #define directives.
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
  // Replace operator new by trace-enabled version.
  #define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
  #define new DEBUG_NEW
#endif

#if !defined(FLATBUFFERS_ASSERT)
#include <assert.h>
#define FLATBUFFERS_ASSERT assert
#elif defined(FLATBUFFERS_ASSERT_INCLUDE)
// Include file with forward declaration
#include FLATBUFFERS_ASSERT_INCLUDE
#endif

#ifndef ARDUINO
#include <cstdint>
#endif

#include <cstddef>
#include <cstdlib>
#include <cstring>

#if defined(ARDUINO) && !defined(ARDUINOSTL_M_H) && defined(__AVR__)
  #include <utility.h>
#else
  #include <utility>
#endif

#include <string>
#include <type_traits>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
#include <iterator>
#include <memory>

#if defined(__unix__) && !defined(FLATBUFFERS_LOCALE_INDEPENDENT)
  #include <unistd.h>
#endif

#ifdef __ANDROID__
  #include <android/api-level.h>
#endif

#if defined(__ICCARM__)
#include <intrinsics.h>
#endif

// Note the __clang__ check is needed, because clang presents itself
// as an older GNUC compiler (4.2).
// Clang 3.3 and later implement all of the ISO C++ 2011 standard.
// Clang 3.4 and later implement all of the ISO C++ 2014 standard.
// http://clang.llvm.org/cxx_status.html

// Note the MSVC value '__cplusplus' may be incorrect:
// The '__cplusplus' predefined macro in the MSVC stuck at the value 199711L,
// indicating (erroneously!) that the compiler conformed to the C++98 Standard.
// This value should be correct starting from MSVC2017-15.7-Preview-3.
// The '__cplusplus' will be valid only if MSVC2017-15.7-P3 and the `/Zc:__cplusplus` switch is set.
// Workaround (for details see MSDN):
// Use the _MSC_VER and _MSVC_LANG definition instead of the __cplusplus  for compatibility.
// The _MSVC_LANG macro reports the Standard version regardless of the '/Zc:__cplusplus' switch.

#if defined(__GNUC__) && !defined(__clang__)
  #define FLATBUFFERS_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
  #define FLATBUFFERS_GCC 0
#endif

#if defined(__clang__)
  #define FLATBUFFERS_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#else
  #define FLATBUFFERS_CLANG 0
#endif

/// @cond FLATBUFFERS_INTERNAL
#if __cplusplus <= 199711L && \
	(!defined(_MSC_VER) || _MSC_VER < 1600) && \
	(!defined(__GNUC__) || \
	  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ < 40400))
  #error A C++11 compatible compiler with support for the auto typing is \
		 required for FlatBuffers.
  #error __cplusplus _MSC_VER __GNUC__  __GNUC_MINOR__  __GNUC_PATCHLEVEL__
#endif

#if !defined(__clang__) && \
	defined(__GNUC__) && \
	(__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__ < 40600)
  // Backwards compatibility for g++ 4.4, and 4.5 which don't have the nullptr
  // and constexpr keywords. Note the __clang__ check is needed, because clang
  // presents itself as an older GNUC compiler.
  #ifndef nullptr_t
	const class nullptr_t {
	public:
	  template<class T> inline operator T*() const { return 0; }
	private:
	  void operator&() const;
	} nullptr = {};
  #endif
  #ifndef constexpr
	#define constexpr const
  #endif
#endif

// The wire format uses a little endian encoding (since that's efficient for
// the common platforms).
#if defined(__s390x__)
  #define FLATBUFFERS_LITTLEENDIAN 0
#endif // __s390x__
#if !defined(FLATBUFFERS_LITTLEENDIAN)
  #if defined(__GNUC__) || defined(__clang__) || defined(__ICCARM__)
	#if (defined(__BIG_ENDIAN__) || \
		 (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__))
	  #define FLATBUFFERS_LITTLEENDIAN 0
	#else
	  #define FLATBUFFERS_LITTLEENDIAN 1
	#endif // __BIG_ENDIAN__
  #elif defined(_MSC_VER)
	#if defined(_M_PPC)
	  #define FLATBUFFERS_LITTLEENDIAN 0
	#else
	  #define FLATBUFFERS_LITTLEENDIAN 1
	#endif
  #else
	#error Unable to determine endianness, define FLATBUFFERS_LITTLEENDIAN.
  #endif
#endif // !defined(FLATBUFFERS_LITTLEENDIAN)

#define FLATBUFFERS_VERSION_MAJOR 24
#define FLATBUFFERS_VERSION_MINOR 3
#define FLATBUFFERS_VERSION_REVISION 25
#define FLATBUFFERS_STRING_EXPAND(X) #X
#define FLATBUFFERS_STRING(X) FLATBUFFERS_STRING_EXPAND(X)
namespace flatbuffers {
  // Returns version as string  "MAJOR.MINOR.REVISION".
  const char* FLATBUFFERS_VERSION();
}

#if (!defined(_MSC_VER) || _MSC_VER > 1600) && \
	(!defined(__GNUC__) || (__GNUC__ * 100 + __GNUC_MINOR__ >= 407)) || \
	defined(__clang__)
  #define FLATBUFFERS_FINAL_CLASS final
  #define FLATBUFFERS_OVERRIDE override
  #define FLATBUFFERS_EXPLICIT_CPP11 explicit
  #define FLATBUFFERS_VTABLE_UNDERLYING_TYPE : ::flatbuffers::voffset_t
#else
  #define FLATBUFFERS_FINAL_CLASS
  #define FLATBUFFERS_OVERRIDE
  #define FLATBUFFERS_EXPLICIT_CPP11
  #define FLATBUFFERS_VTABLE_UNDERLYING_TYPE
#endif

#if (!defined(_MSC_VER) || _MSC_VER >= 1900) && \
	(!defined(__GNUC__) || (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)) || \
	(defined(__cpp_constexpr) && __cpp_constexpr >= 200704)
  #define FLATBUFFERS_CONSTEXPR constexpr
  #define FLATBUFFERS_CONSTEXPR_CPP11 constexpr
  #define FLATBUFFERS_CONSTEXPR_DEFINED
#else
  #define FLATBUFFERS_CONSTEXPR const
  #define FLATBUFFERS_CONSTEXPR_CPP11
#endif

#if (defined(__cplusplus) && __cplusplus >= 201402L) || \
	(defined(__cpp_constexpr) && __cpp_constexpr >= 201304)
  #define FLATBUFFERS_CONSTEXPR_CPP14 FLATBUFFERS_CONSTEXPR_CPP11
#else
  #define FLATBUFFERS_CONSTEXPR_CPP14
#endif

#if (defined(__GXX_EXPERIMENTAL_CXX0X__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)) || \
	(defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 190023026)) || \
	defined(__clang__)
  #define FLATBUFFERS_NOEXCEPT noexcept
#else
  #define FLATBUFFERS_NOEXCEPT
#endif

// NOTE: the FLATBUFFERS_DELETE_FUNC macro may change the access mode to
// private, so be sure to put it at the end or reset access mode explicitly.
#if (!defined(_MSC_VER) || _MSC_FULL_VER >= 180020827) && \
	(!defined(__GNUC__) || (__GNUC__ * 100 + __GNUC_MINOR__ >= 404)) || \
	defined(__clang__)
  #define FLATBUFFERS_DELETE_FUNC(func) func = delete
#else
  #define FLATBUFFERS_DELETE_FUNC(func) private: func
#endif

#if (!defined(_MSC_VER) || _MSC_VER >= 1900) && \
	(!defined(__GNUC__) || (__GNUC__ * 100 + __GNUC_MINOR__ >= 409)) || \
	defined(__clang__)
  #define FLATBUFFERS_DEFAULT_DECLARATION
#endif

// Check if we can use template aliases
// Not possible if Microsoft Compiler before 2012
// Possible is the language feature __cpp_alias_templates is defined well
// Or possible if the C++ std is C+11 or newer
#if (defined(_MSC_VER) && _MSC_VER > 1700 /* MSVC2012 */) \
	|| (defined(__cpp_alias_templates) && __cpp_alias_templates >= 200704) \
	|| (defined(__cplusplus) && __cplusplus >= 201103L)
  #define FLATBUFFERS_TEMPLATES_ALIASES
#endif

#ifndef FLATBUFFERS_HAS_STRING_VIEW
  // Only provide flatbuffers::string_view if __has_include can be used
  // to detect a header that provides an implementation
  #if defined(__has_include)
	// Check for std::string_view (in c++17)
	#if __has_include(<string_view>) && (__cplusplus >= 201606 || (defined(_HAS_CXX17) && _HAS_CXX17))
	  #include <string_view>
	  namespace flatbuffers {
		typedef std::string_view string_view;
	  }
	  #define FLATBUFFERS_HAS_STRING_VIEW 1
	// Check for std::experimental::string_view (in c++14, compiler-dependent)
	#elif __has_include(<experimental/string_view>) && (__cplusplus >= 201411)
	  #include <experimental/string_view>
	  namespace flatbuffers {
		typedef std::experimental::string_view string_view;
	  }
	  #define FLATBUFFERS_HAS_STRING_VIEW 1
	// Check for absl::string_view
	#elif __has_include("absl/strings/string_view.h") && \
		  __has_include("absl/base/config.h") && \
		  (__cplusplus >= 201411)
	  #include "absl/base/config.h"
	  #if !defined(ABSL_USES_STD_STRING_VIEW)
		#include "absl/strings/string_view.h"
		namespace flatbuffers {
		  typedef absl::string_view string_view;
		}
		#define FLATBUFFERS_HAS_STRING_VIEW 1
	  #endif
	#endif
  #endif // __has_include
#endif // !FLATBUFFERS_HAS_STRING_VIEW

#ifndef FLATBUFFERS_GENERAL_HEAP_ALLOC_OK
  // Allow heap allocations to be used
  #define FLATBUFFERS_GENERAL_HEAP_ALLOC_OK 1
#endif // !FLATBUFFERS_GENERAL_HEAP_ALLOC_OK

#ifndef FLATBUFFERS_HAS_NEW_STRTOD
  // Modern (C++11) strtod and strtof functions are available for use.
  // 1) nan/inf strings as argument of strtod;
  // 2) hex-float  as argument of  strtod/strtof.
  #if (defined(_MSC_VER) && _MSC_VER >= 1900) || \
	  (defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 409)) || \
	  (defined(__clang__))
	#define FLATBUFFERS_HAS_NEW_STRTOD 1
  #endif
#endif // !FLATBUFFERS_HAS_NEW_STRTOD

#ifndef FLATBUFFERS_LOCALE_INDEPENDENT
  // Enable locale independent functions {strtof_l, strtod_l,strtoll_l,
  // strtoull_l}.
  #if (defined(_MSC_VER) && _MSC_VER >= 1800) || \
	  (defined(__ANDROID_API__) && __ANDROID_API__>= 21) || \
	  (defined(_XOPEN_VERSION) && (_XOPEN_VERSION >= 700)) && \
		(!defined(__Fuchsia__) && !defined(__ANDROID_API__))
	#define FLATBUFFERS_LOCALE_INDEPENDENT 1
  #else
	#define FLATBUFFERS_LOCALE_INDEPENDENT 0
  #endif
#endif  // !FLATBUFFERS_LOCALE_INDEPENDENT

// Suppress Undefined Behavior Sanitizer (recoverable only). Usage:
// - FLATBUFFERS_SUPPRESS_UBSAN("undefined")
// - FLATBUFFERS_SUPPRESS_UBSAN("signed-integer-overflow")
#if defined(__clang__) && (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >=7))
  #define FLATBUFFERS_SUPPRESS_UBSAN(type) __attribute__((no_sanitize(type)))
#elif defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 409)
  #define FLATBUFFERS_SUPPRESS_UBSAN(type) __attribute__((no_sanitize_undefined))
#else
  #define FLATBUFFERS_SUPPRESS_UBSAN(type)
#endif

namespace flatbuffers {
  // This is constexpr function used for checking compile-time constants.
  // Avoid `#pragma warning(disable: 4127) // C4127: expression is constant`.
  template<typename T> FLATBUFFERS_CONSTEXPR inline bool IsConstTrue(T t) {
	return !!t;
  }
}

// Enable C++ attribute [[]] if std:c++17 or higher.
#if ((__cplusplus >= 201703L) \
	|| (defined(_MSVC_LANG) &&  (_MSVC_LANG >= 201703L)))
  // All attributes unknown to an implementation are ignored without causing an error.
  #define FLATBUFFERS_ATTRIBUTE(attr) attr

  #define FLATBUFFERS_FALLTHROUGH() [[fallthrough]]
#else
  #define FLATBUFFERS_ATTRIBUTE(attr)

  #if FLATBUFFERS_CLANG >= 30800
	#define FLATBUFFERS_FALLTHROUGH() [[clang::fallthrough]]
  #elif FLATBUFFERS_GCC >= 70300
	#define FLATBUFFERS_FALLTHROUGH() [[gnu::fallthrough]]
  #else
	#define FLATBUFFERS_FALLTHROUGH()
  #endif
#endif

/// @endcond

/// @file
namespace flatbuffers {

/// @cond FLATBUFFERS_INTERNAL
// Our default offset / size type, 32bit on purpose on 64bit systems.
// Also, using a consistent offset type maintains compatibility of serialized
// offset values between 32bit and 64bit systems.
typedef uint32_t uoffset_t;
typedef uint64_t uoffset64_t;

// Signed offsets for references that can go in both directions.
typedef int32_t soffset_t;
typedef int64_t soffset64_t;

// Offset/index used in v-tables, can be changed to uint8_t in
// format forks to save a bit of space if desired.
typedef uint16_t voffset_t;

typedef uintmax_t largest_scalar_t;

// In 32bits, this evaluates to 2GB - 1
#define FLATBUFFERS_MAX_BUFFER_SIZE std::numeric_limits<::flatbuffers::soffset_t>::max()
#define FLATBUFFERS_MAX_64_BUFFER_SIZE std::numeric_limits<::flatbuffers::soffset64_t>::max()

// The minimum size buffer that can be a valid flatbuffer.
// Includes the offset to the root table (uoffset_t), the offset to the vtable
// of the root table (soffset_t), the size of the vtable (uint16_t), and the
// size of the referring table (uint16_t).
#define FLATBUFFERS_MIN_BUFFER_SIZE sizeof(::flatbuffers::uoffset_t) + \
  sizeof(::flatbuffers::soffset_t) + sizeof(uint16_t) + sizeof(uint16_t)

// We support aligning the contents of buffers up to this size.
#ifndef FLATBUFFERS_MAX_ALIGNMENT
  #define FLATBUFFERS_MAX_ALIGNMENT 32
#endif

/// @brief The length of a FlatBuffer file header.
static const size_t kFileIdentifierLength = 4;

inline bool VerifyAlignmentRequirements(size_t align, size_t min_align = 1) {
  return (min_align <= align) && (align <= (FLATBUFFERS_MAX_ALIGNMENT)) &&
		 (align & (align - 1)) == 0;  // must be power of 2
}

#if defined(_MSC_VER)
  #pragma warning(push)
  #pragma warning(disable: 4127) // C4127: conditional expression is constant
#endif

template<typename T> T EndianSwap(T t) {
  #if defined(_MSC_VER)
	#define FLATBUFFERS_BYTESWAP16 _byteswap_ushort
	#define FLATBUFFERS_BYTESWAP32 _byteswap_ulong
	#define FLATBUFFERS_BYTESWAP64 _byteswap_uint64
  #elif defined(__ICCARM__)
	#define FLATBUFFERS_BYTESWAP16 __REV16
	#define FLATBUFFERS_BYTESWAP32 __REV
	#define FLATBUFFERS_BYTESWAP64(x) \
	   ((__REV(static_cast<uint32_t>(x >> 32U))) | (static_cast<uint64_t>(__REV(static_cast<uint32_t>(x)))) << 32U)
  #else
	#if defined(__GNUC__) && __GNUC__ * 100 + __GNUC_MINOR__ < 408 && !defined(__clang__)
	  // __builtin_bswap16 was missing prior to GCC 4.8.
	  #define FLATBUFFERS_BYTESWAP16(x) \
		static_cast<uint16_t>(__builtin_bswap32(static_cast<uint32_t>(x) << 16))
	#else
	  #define FLATBUFFERS_BYTESWAP16 __builtin_bswap16
	#endif
	#define FLATBUFFERS_BYTESWAP32 __builtin_bswap32
	#define FLATBUFFERS_BYTESWAP64 __builtin_bswap64
  #endif
  if (sizeof(T) == 1) {   // Compile-time if-then's.
	return t;
  } else if (sizeof(T) == 2) {
	union { T t; uint16_t i; } u = { t };
	u.i = FLATBUFFERS_BYTESWAP16(u.i);
	return u.t;
  } else if (sizeof(T) == 4) {
	union { T t; uint32_t i; } u = { t };
	u.i = FLATBUFFERS_BYTESWAP32(u.i);
	return u.t;
  } else if (sizeof(T) == 8) {
	union { T t; uint64_t i; } u = { t };
	u.i = FLATBUFFERS_BYTESWAP64(u.i);
	return u.t;
  } else {
	FLATBUFFERS_ASSERT(0);
	return t;
  }
}

#if defined(_MSC_VER)
  #pragma warning(pop)
#endif

template<typename T> T EndianScalar(T t) {
  #if FLATBUFFERS_LITTLEENDIAN
	return t;
  #else
	return EndianSwap(t);
  #endif
}

template<typename T>
// UBSAN: C++ aliasing type rules, see std::bit_cast<> for details.
FLATBUFFERS_SUPPRESS_UBSAN("alignment")
T ReadScalar(const void *p) {
  return EndianScalar(*reinterpret_cast<const T *>(p));
}

// See https://github.com/google/flatbuffers/issues/5950

#if (FLATBUFFERS_GCC >= 100000) && (FLATBUFFERS_GCC < 110000)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif

template<typename T>
// UBSAN: C++ aliasing type rules, see std::bit_cast<> for details.
FLATBUFFERS_SUPPRESS_UBSAN("alignment")
void WriteScalar(void *p, T t) {
  *reinterpret_cast<T *>(p) = EndianScalar(t);
}

template<typename T> struct Offset;
template<typename T> FLATBUFFERS_SUPPRESS_UBSAN("alignment") void WriteScalar(void *p, Offset<T> t) {
  *reinterpret_cast<uoffset_t *>(p) = EndianScalar(t.o);
}

#if (FLATBUFFERS_GCC >= 100000) && (FLATBUFFERS_GCC < 110000)
  #pragma GCC diagnostic pop
#endif

// Computes how many bytes you'd have to pad to be able to write an
// "scalar_size" scalar if the buffer had grown to "buf_size" (downwards in
// memory).
FLATBUFFERS_SUPPRESS_UBSAN("unsigned-integer-overflow")
inline size_t PaddingBytes(size_t buf_size, size_t scalar_size) {
  return ((~buf_size) + 1) & (scalar_size - 1);
}

#if !defined(_MSC_VER)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
// Generic 'operator==' with conditional specialisations.
// T e - new value of a scalar field.
// T def - default of scalar (is known at compile-time).
template<typename T> inline bool IsTheSameAs(T e, T def) { return e == def; }
#if !defined(_MSC_VER)
  #pragma GCC diagnostic pop
#endif

#if defined(FLATBUFFERS_NAN_DEFAULTS) && \
	defined(FLATBUFFERS_HAS_NEW_STRTOD) && (FLATBUFFERS_HAS_NEW_STRTOD > 0)
// Like `operator==(e, def)` with weak NaN if T=(float|double).
template<typename T> inline bool IsFloatTheSameAs(T e, T def) {
  return (e == def) || ((def != def) && (e != e));
}
template<> inline bool IsTheSameAs<float>(float e, float def) {
  return IsFloatTheSameAs(e, def);
}
template<> inline bool IsTheSameAs<double>(double e, double def) {
  return IsFloatTheSameAs(e, def);
}
#endif

// Check 'v' is out of closed range [low; high].
// Workaround for GCC warning [-Werror=type-limits]:
// comparison is always true due to limited range of data type.
template<typename T>
inline bool IsOutRange(const T &v, const T &low, const T &high) {
  return (v < low) || (high < v);
}

// Check 'v' is in closed range [low; high].
template<typename T>
inline bool IsInRange(const T &v, const T &low, const T &high) {
  return !IsOutRange(v, low, high);
}

}  // namespace flatbuffers
#endif  // FLATBUFFERS_BASE_H_

/*** End of inlined file: base.h ***/


/*** Start of inlined file: stl_emulation.h ***/
#ifndef FLATBUFFERS_STL_EMULATION_H_
#define FLATBUFFERS_STL_EMULATION_H_

// clang-format off

#include <string>
#include <type_traits>
#include <vector>
#include <memory>
#include <limits>

#ifndef FLATBUFFERS_USE_STD_OPTIONAL
  // Detect C++17 compatible compiler.
  // __cplusplus >= 201703L - a compiler has support of 'static inline' variables.
  #if (defined(__cplusplus) && __cplusplus >= 201703L) \
	  || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
	#define FLATBUFFERS_USE_STD_OPTIONAL 1
  #else
	#define FLATBUFFERS_USE_STD_OPTIONAL 0
  #endif // (defined(__cplusplus) && __cplusplus >= 201703L) ...
#endif // FLATBUFFERS_USE_STD_OPTIONAL

#if FLATBUFFERS_USE_STD_OPTIONAL
  #include <optional>
#endif

#ifndef FLATBUFFERS_USE_STD_SPAN
  // Testing __cpp_lib_span requires including either <version> or <span>,
  // both of which were added in C++20.
  // See: https://en.cppreference.com/w/cpp/utility/feature_test
  #if defined(__cplusplus) && __cplusplus >= 202002L \
	  || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
	#define FLATBUFFERS_USE_STD_SPAN 1
  #endif
#endif // FLATBUFFERS_USE_STD_SPAN

#if defined(FLATBUFFERS_USE_STD_SPAN)
  #include <array>
  #include <span>
#else
  // Disable non-trivial ctors if FLATBUFFERS_SPAN_MINIMAL defined.
  #if !defined(FLATBUFFERS_TEMPLATES_ALIASES)
	#define FLATBUFFERS_SPAN_MINIMAL
  #else
	// Enable implicit construction of a span<T,N> from a std::array<T,N>.
	#include <array>
  #endif
#endif // defined(FLATBUFFERS_USE_STD_SPAN)

// This header provides backwards compatibility for older versions of the STL.
namespace flatbuffers {

#if defined(FLATBUFFERS_TEMPLATES_ALIASES)
  template <typename T>
  using numeric_limits = std::numeric_limits<T>;
#else
  template <typename T> class numeric_limits :
	public std::numeric_limits<T> {};
#endif  // defined(FLATBUFFERS_TEMPLATES_ALIASES)

#if defined(FLATBUFFERS_TEMPLATES_ALIASES)
  template <typename T> using is_scalar = std::is_scalar<T>;
  template <typename T, typename U> using is_same = std::is_same<T,U>;
  template <typename T> using is_floating_point = std::is_floating_point<T>;
  template <typename T> using is_unsigned = std::is_unsigned<T>;
  template <typename T> using is_enum = std::is_enum<T>;
  template <typename T> using make_unsigned = std::make_unsigned<T>;
  template<bool B, class T, class F>
  using conditional = std::conditional<B, T, F>;
  template<class T, T v>
  using integral_constant = std::integral_constant<T, v>;
  template <bool B>
  using bool_constant = integral_constant<bool, B>;
  using true_type  = std::true_type;
  using false_type = std::false_type;
#else
  // MSVC 2010 doesn't support C++11 aliases.
  template <typename T> struct is_scalar : public std::is_scalar<T> {};
  template <typename T, typename U> struct is_same : public std::is_same<T,U> {};
  template <typename T> struct is_floating_point :
		public std::is_floating_point<T> {};
  template <typename T> struct is_unsigned : public std::is_unsigned<T> {};
  template <typename T> struct is_enum : public std::is_enum<T> {};
  template <typename T> struct make_unsigned : public std::make_unsigned<T> {};
  template<bool B, class T, class F>
  struct conditional : public std::conditional<B, T, F> {};
  template<class T, T v>
  struct integral_constant : public std::integral_constant<T, v> {};
  template <bool B>
  struct bool_constant : public integral_constant<bool, B> {};
  typedef bool_constant<true>  true_type;
  typedef bool_constant<false> false_type;
#endif  // defined(FLATBUFFERS_TEMPLATES_ALIASES)

#if defined(FLATBUFFERS_TEMPLATES_ALIASES)
  template <class T> using unique_ptr = std::unique_ptr<T>;
#else
  // MSVC 2010 doesn't support C++11 aliases.
  // We're manually "aliasing" the class here as we want to bring unique_ptr
  // into the flatbuffers namespace.  We have unique_ptr in the flatbuffers
  // namespace we have a completely independent implementation (see below)
  // for C++98 STL implementations.
  template <class T> class unique_ptr : public std::unique_ptr<T> {
	public:
	unique_ptr() {}
	explicit unique_ptr(T* p) : std::unique_ptr<T>(p) {}
	unique_ptr(std::unique_ptr<T>&& u) { *this = std::move(u); }
	unique_ptr(unique_ptr&& u) { *this = std::move(u); }
	unique_ptr& operator=(std::unique_ptr<T>&& u) {
	  std::unique_ptr<T>::reset(u.release());
	  return *this;
	}
	unique_ptr& operator=(unique_ptr&& u) {
	  std::unique_ptr<T>::reset(u.release());
	  return *this;
	}
	unique_ptr& operator=(T* p) {
	  return std::unique_ptr<T>::operator=(p);
	}
  };
#endif  // defined(FLATBUFFERS_TEMPLATES_ALIASES)

#if FLATBUFFERS_USE_STD_OPTIONAL
template<class T>
using Optional = std::optional<T>;
using nullopt_t = std::nullopt_t;
inline constexpr nullopt_t nullopt = std::nullopt;

#else
// Limited implementation of Optional<T> type for a scalar T.
// This implementation limited by trivial types compatible with
// std::is_arithmetic<T> or std::is_enum<T> type traits.

// A tag to indicate an empty flatbuffers::optional<T>.
struct nullopt_t {
  explicit FLATBUFFERS_CONSTEXPR_CPP11 nullopt_t(int) {}
};

#if defined(FLATBUFFERS_CONSTEXPR_DEFINED)
  namespace internal {
	template <class> struct nullopt_holder {
	  static constexpr nullopt_t instance_ = nullopt_t(0);
	};
	template<class Dummy>
	constexpr nullopt_t nullopt_holder<Dummy>::instance_;
  }
  static constexpr const nullopt_t &nullopt = internal::nullopt_holder<void>::instance_;

#else
  namespace internal {
	template <class> struct nullopt_holder {
	  static const nullopt_t instance_;
	};
	template<class Dummy>
	const nullopt_t nullopt_holder<Dummy>::instance_  = nullopt_t(0);
  }
  static const nullopt_t &nullopt = internal::nullopt_holder<void>::instance_;

#endif

template<class T>
class Optional FLATBUFFERS_FINAL_CLASS {
  // Non-scalar 'T' would extremely complicated Optional<T>.
  // Use is_scalar<T> checking because flatbuffers flatbuffers::is_arithmetic<T>
  // isn't implemented.
  static_assert(flatbuffers::is_scalar<T>::value, "unexpected type T");

 public:
  ~Optional() {}

  FLATBUFFERS_CONSTEXPR_CPP11 Optional() FLATBUFFERS_NOEXCEPT
	: value_(), has_value_(false) {}

  FLATBUFFERS_CONSTEXPR_CPP11 Optional(nullopt_t) FLATBUFFERS_NOEXCEPT
	: value_(), has_value_(false) {}

  FLATBUFFERS_CONSTEXPR_CPP11 Optional(T val) FLATBUFFERS_NOEXCEPT
	: value_(val), has_value_(true) {}

  FLATBUFFERS_CONSTEXPR_CPP11 Optional(const Optional &other) FLATBUFFERS_NOEXCEPT
	: value_(other.value_), has_value_(other.has_value_) {}

  FLATBUFFERS_CONSTEXPR_CPP14 Optional &operator=(const Optional &other) FLATBUFFERS_NOEXCEPT {
	value_ = other.value_;
	has_value_ = other.has_value_;
	return *this;
  }

  FLATBUFFERS_CONSTEXPR_CPP14 Optional &operator=(nullopt_t) FLATBUFFERS_NOEXCEPT {
	value_ = T();
	has_value_ = false;
	return *this;
  }

  FLATBUFFERS_CONSTEXPR_CPP14 Optional &operator=(T val) FLATBUFFERS_NOEXCEPT {
	value_ = val;
	has_value_ = true;
	return *this;
  }

  void reset() FLATBUFFERS_NOEXCEPT {
	*this = nullopt;
  }

  void swap(Optional &other) FLATBUFFERS_NOEXCEPT {
	std::swap(value_, other.value_);
	std::swap(has_value_, other.has_value_);
  }

  FLATBUFFERS_CONSTEXPR_CPP11 FLATBUFFERS_EXPLICIT_CPP11 operator bool() const FLATBUFFERS_NOEXCEPT {
	return has_value_;
  }

  FLATBUFFERS_CONSTEXPR_CPP11 bool has_value() const FLATBUFFERS_NOEXCEPT {
	return has_value_;
  }

  FLATBUFFERS_CONSTEXPR_CPP11 const T& operator*() const FLATBUFFERS_NOEXCEPT {
	return value_;
  }

  const T& value() const {
	FLATBUFFERS_ASSERT(has_value());
	return value_;
  }

  T value_or(T default_value) const FLATBUFFERS_NOEXCEPT {
	return has_value() ? value_ : default_value;
  }

 private:
  T value_;
  bool has_value_;
};

template<class T>
FLATBUFFERS_CONSTEXPR_CPP11 bool operator==(const Optional<T>& opt, nullopt_t) FLATBUFFERS_NOEXCEPT {
  return !opt;
}
template<class T>
FLATBUFFERS_CONSTEXPR_CPP11 bool operator==(nullopt_t, const Optional<T>& opt) FLATBUFFERS_NOEXCEPT {
  return !opt;
}

template<class T, class U>
FLATBUFFERS_CONSTEXPR_CPP11 bool operator==(const Optional<T>& lhs, const U& rhs) FLATBUFFERS_NOEXCEPT {
  return static_cast<bool>(lhs) && (*lhs == rhs);
}

template<class T, class U>
FLATBUFFERS_CONSTEXPR_CPP11 bool operator==(const T& lhs, const Optional<U>& rhs) FLATBUFFERS_NOEXCEPT {
  return static_cast<bool>(rhs) && (lhs == *rhs);
}

template<class T, class U>
FLATBUFFERS_CONSTEXPR_CPP11 bool operator==(const Optional<T>& lhs, const Optional<U>& rhs) FLATBUFFERS_NOEXCEPT {
  return static_cast<bool>(lhs) != static_cast<bool>(rhs)
			  ? false
			  : !static_cast<bool>(lhs) ? true : (*lhs == *rhs);
}
#endif // FLATBUFFERS_USE_STD_OPTIONAL

// Very limited and naive partial implementation of C++20 std::span<T,Extent>.
#if defined(FLATBUFFERS_USE_STD_SPAN)
  inline constexpr std::size_t dynamic_extent = std::dynamic_extent;
  template<class T, std::size_t Extent = std::dynamic_extent>
  using span = std::span<T, Extent>;

#else // !defined(FLATBUFFERS_USE_STD_SPAN)
FLATBUFFERS_CONSTEXPR std::size_t dynamic_extent = static_cast<std::size_t>(-1);

// Exclude this code if MSVC2010 or non-STL Android is active.
// The non-STL Android doesn't have `std::is_convertible` required for SFINAE.
#if !defined(FLATBUFFERS_SPAN_MINIMAL)
namespace internal {
  // This is SFINAE helper class for checking of a common condition:
  // > This overload only participates in overload resolution
  // > Check whether a pointer to an array of From can be converted
  // > to a pointer to an array of To.
  // This helper is used for checking of 'From -> const From'.
  template<class To, std::size_t Extent, class From, std::size_t N>
  struct is_span_convertible {
	using type =
	  typename std::conditional<std::is_convertible<From (*)[], To (*)[]>::value
								&& (Extent == dynamic_extent || N == Extent),
								int, void>::type;
  };

  template<typename T>
  struct SpanIterator {
	// TODO: upgrade to std::random_access_iterator_tag.
	using iterator_category = std::forward_iterator_tag;
	using difference_type  = std::ptrdiff_t;
	using value_type = typename std::remove_cv<T>::type;
	using reference = T&;
	using pointer   = T*;

	// Convince MSVC compiler that this iterator is trusted (it is verified).
	#ifdef _MSC_VER
	  using _Unchecked_type = pointer;
	#endif // _MSC_VER

	SpanIterator(pointer ptr) : ptr_(ptr) {}
	reference operator*() const { return *ptr_; }
	pointer operator->() { return ptr_; }
	SpanIterator& operator++() { ptr_++; return *this; }
	SpanIterator  operator++(int) { auto tmp = *this; ++(*this); return tmp; }

	friend bool operator== (const SpanIterator& lhs, const SpanIterator& rhs) { return lhs.ptr_ == rhs.ptr_; }
	friend bool operator!= (const SpanIterator& lhs, const SpanIterator& rhs) { return lhs.ptr_ != rhs.ptr_; }

   private:
	pointer ptr_;
  };
}  // namespace internal
#endif  // !defined(FLATBUFFERS_SPAN_MINIMAL)

// T - element type; must be a complete type that is not an abstract
// class type.
// Extent - the number of elements in the sequence, or dynamic.
template<class T, std::size_t Extent = dynamic_extent>
class span FLATBUFFERS_FINAL_CLASS {
 public:
  typedef T element_type;
  typedef T& reference;
  typedef const T& const_reference;
  typedef T* pointer;
  typedef const T* const_pointer;
  typedef std::size_t size_type;

  static FLATBUFFERS_CONSTEXPR size_type extent = Extent;

  // Returns the number of elements in the span.
  FLATBUFFERS_CONSTEXPR_CPP11 size_type size() const FLATBUFFERS_NOEXCEPT {
	return count_;
  }

  // Returns the size of the sequence in bytes.
  FLATBUFFERS_CONSTEXPR_CPP11
  size_type size_bytes() const FLATBUFFERS_NOEXCEPT {
	return size() * sizeof(element_type);
  }

  // Checks if the span is empty.
  FLATBUFFERS_CONSTEXPR_CPP11 bool empty() const FLATBUFFERS_NOEXCEPT {
	return size() == 0;
  }

  // Returns a pointer to the beginning of the sequence.
  FLATBUFFERS_CONSTEXPR_CPP11 pointer data() const FLATBUFFERS_NOEXCEPT {
	return data_;
  }

  #if !defined(FLATBUFFERS_SPAN_MINIMAL)
	using Iterator = internal::SpanIterator<T>;

	Iterator begin() const { return Iterator(data()); }
	Iterator end() const   { return Iterator(data() + size()); }
  #endif

  // Returns a reference to the idx-th element of the sequence.
  // The behavior is undefined if the idx is greater than or equal to size().
  FLATBUFFERS_CONSTEXPR_CPP11 reference operator[](size_type idx) const {
	return data()[idx];
  }

  FLATBUFFERS_CONSTEXPR_CPP11 span(const span &other) FLATBUFFERS_NOEXCEPT
	  : data_(other.data_), count_(other.count_) {}

  FLATBUFFERS_CONSTEXPR_CPP14 span &operator=(const span &other)
	  FLATBUFFERS_NOEXCEPT {
	data_ = other.data_;
	count_ = other.count_;
  }

  // Limited implementation of
  // `template <class It> constexpr std::span(It first, size_type count);`.
  //
  // Constructs a span that is a view over the range [first, first + count);
  // the resulting span has: data() == first and size() == count.
  // The behavior is undefined if [first, first + count) is not a valid range,
  // or if (extent != flatbuffers::dynamic_extent && count != extent).
  FLATBUFFERS_CONSTEXPR_CPP11
  explicit span(pointer first, size_type count) FLATBUFFERS_NOEXCEPT
	: data_ (Extent == dynamic_extent ? first : (Extent == count ? first : nullptr)),
	  count_(Extent == dynamic_extent ? count : (Extent == count ? Extent : 0)) {
	  // Make span empty if the count argument is incompatible with span<T,N>.
  }

  // Exclude this code if MSVC2010 is active. The MSVC2010 isn't C++11
  // compliant, it doesn't support default template arguments for functions.
  #if defined(FLATBUFFERS_SPAN_MINIMAL)
  FLATBUFFERS_CONSTEXPR_CPP11 span() FLATBUFFERS_NOEXCEPT : data_(nullptr),
															count_(0) {
	static_assert(extent == 0 || extent == dynamic_extent, "invalid span");
  }

  #else
  // Constructs an empty span whose data() == nullptr and size() == 0.
  // This overload only participates in overload resolution if
  // extent == 0 || extent == flatbuffers::dynamic_extent.
  // A dummy template argument N is need dependency for SFINAE.
  template<std::size_t N = 0,
	typename internal::is_span_convertible<element_type, Extent, element_type, (N - N)>::type = 0>
  FLATBUFFERS_CONSTEXPR_CPP11 span() FLATBUFFERS_NOEXCEPT : data_(nullptr),
															count_(0) {
	static_assert(extent == 0 || extent == dynamic_extent, "invalid span");
  }

  // Constructs a span that is a view over the array arr; the resulting span
  // has size() == N and data() == std::data(arr). These overloads only
  // participate in overload resolution if
  // extent == std::dynamic_extent || N == extent is true and
  // std::remove_pointer_t<decltype(std::data(arr))>(*)[]
  // is convertible to element_type (*)[].
  template<std::size_t N,
	typename internal::is_span_convertible<element_type, Extent, element_type, N>::type = 0>
  FLATBUFFERS_CONSTEXPR_CPP11 span(element_type (&arr)[N]) FLATBUFFERS_NOEXCEPT
	  : data_(arr), count_(N) {}

  template<class U, std::size_t N,
	typename internal::is_span_convertible<element_type, Extent, U, N>::type = 0>
  FLATBUFFERS_CONSTEXPR_CPP11 span(std::array<U, N> &arr) FLATBUFFERS_NOEXCEPT
	 : data_(arr.data()), count_(N) {}

  //template<class U, std::size_t N,
  //  int = 0>
  //FLATBUFFERS_CONSTEXPR_CPP11 span(std::array<U, N> &arr) FLATBUFFERS_NOEXCEPT
  //   : data_(arr.data()), count_(N) {}

  template<class U, std::size_t N,
	typename internal::is_span_convertible<element_type, Extent, U, N>::type = 0>
  FLATBUFFERS_CONSTEXPR_CPP11 span(const std::array<U, N> &arr) FLATBUFFERS_NOEXCEPT
	: data_(arr.data()), count_(N) {}

  // Converting constructor from another span s;
  // the resulting span has size() == s.size() and data() == s.data().
  // This overload only participates in overload resolution
  // if extent == std::dynamic_extent || N == extent is true and U (*)[]
  // is convertible to element_type (*)[].
  template<class U, std::size_t N,
	typename internal::is_span_convertible<element_type, Extent, U, N>::type = 0>
  FLATBUFFERS_CONSTEXPR_CPP11 span(const flatbuffers::span<U, N> &s) FLATBUFFERS_NOEXCEPT
	  : span(s.data(), s.size()) {
  }

  #endif  // !defined(FLATBUFFERS_SPAN_MINIMAL)

 private:
  // This is a naive implementation with 'count_' member even if (Extent != dynamic_extent).
  pointer const data_;
  size_type count_;
};
#endif  // defined(FLATBUFFERS_USE_STD_SPAN)

#if !defined(FLATBUFFERS_SPAN_MINIMAL)
template<class ElementType, std::size_t Extent>
FLATBUFFERS_CONSTEXPR_CPP11
flatbuffers::span<ElementType, Extent> make_span(ElementType(&arr)[Extent]) FLATBUFFERS_NOEXCEPT {
  return span<ElementType, Extent>(arr);
}

template<class ElementType, std::size_t Extent>
FLATBUFFERS_CONSTEXPR_CPP11
flatbuffers::span<const ElementType, Extent> make_span(const ElementType(&arr)[Extent]) FLATBUFFERS_NOEXCEPT {
  return span<const ElementType, Extent>(arr);
}

template<class ElementType, std::size_t Extent>
FLATBUFFERS_CONSTEXPR_CPP11
flatbuffers::span<ElementType, Extent> make_span(std::array<ElementType, Extent> &arr) FLATBUFFERS_NOEXCEPT {
  return span<ElementType, Extent>(arr);
}

template<class ElementType, std::size_t Extent>
FLATBUFFERS_CONSTEXPR_CPP11
flatbuffers::span<const ElementType, Extent> make_span(const std::array<ElementType, Extent> &arr) FLATBUFFERS_NOEXCEPT {
  return span<const ElementType, Extent>(arr);
}

template<class ElementType, std::size_t Extent>
FLATBUFFERS_CONSTEXPR_CPP11
flatbuffers::span<ElementType, dynamic_extent> make_span(ElementType *first, std::size_t count) FLATBUFFERS_NOEXCEPT {
  return span<ElementType, dynamic_extent>(first, count);
}

template<class ElementType, std::size_t Extent>
FLATBUFFERS_CONSTEXPR_CPP11
flatbuffers::span<const ElementType, dynamic_extent> make_span(const ElementType *first, std::size_t count) FLATBUFFERS_NOEXCEPT {
  return span<const ElementType, dynamic_extent>(first, count);
}
#endif // !defined(FLATBUFFERS_SPAN_MINIMAL)

}  // namespace flatbuffers

#endif  // FLATBUFFERS_STL_EMULATION_H_

/*** End of inlined file: stl_emulation.h ***/


/*** Start of inlined file: vector.h ***/
#ifndef FLATBUFFERS_VECTOR_H_
#define FLATBUFFERS_VECTOR_H_


/*** Start of inlined file: buffer.h ***/
#ifndef FLATBUFFERS_BUFFER_H_
#define FLATBUFFERS_BUFFER_H_

#include <algorithm>

namespace flatbuffers {

// Wrapper for uoffset_t to allow safe template specialization.
// Value is allowed to be 0 to indicate a null object (see e.g. AddOffset).
template<typename T = void> struct Offset {
  // The type of offset to use.
  typedef uoffset_t offset_type;

  offset_type o;
  Offset() : o(0) {}
  Offset(const offset_type _o) : o(_o) {}
  Offset<> Union() const { return o; }
  bool IsNull() const { return !o; }
};

// Wrapper for uoffset64_t Offsets.
template<typename T = void> struct Offset64 {
  // The type of offset to use.
  typedef uoffset64_t offset_type;

  offset_type o;
  Offset64() : o(0) {}
  Offset64(const offset_type offset) : o(offset) {}
  Offset64<> Union() const { return o; }
  bool IsNull() const { return !o; }
};

// Litmus check for ensuring the Offsets are the expected size.
static_assert(sizeof(Offset<>) == 4, "Offset has wrong size");
static_assert(sizeof(Offset64<>) == 8, "Offset64 has wrong size");

inline void EndianCheck() {
  int endiantest = 1;
  // If this fails, see FLATBUFFERS_LITTLEENDIAN above.
  FLATBUFFERS_ASSERT(*reinterpret_cast<char *>(&endiantest) ==
					 FLATBUFFERS_LITTLEENDIAN);
  (void)endiantest;
}

template<typename T> FLATBUFFERS_CONSTEXPR size_t AlignOf() {
  // clang-format off
  #ifdef _MSC_VER
	return __alignof(T);
  #else
	#ifndef alignof
	  return __alignof__(T);
	#else
	  return alignof(T);
	#endif
  #endif
  // clang-format on
}

// Lexicographically compare two strings (possibly containing nulls), and
// return true if the first is less than the second.
static inline bool StringLessThan(const char *a_data, uoffset_t a_size,
								  const char *b_data, uoffset_t b_size) {
  const auto cmp = memcmp(a_data, b_data, (std::min)(a_size, b_size));
  return cmp == 0 ? a_size < b_size : cmp < 0;
}

// When we read serialized data from memory, in the case of most scalars,
// we want to just read T, but in the case of Offset, we want to actually
// perform the indirection and return a pointer.
// The template specialization below does just that.
// It is wrapped in a struct since function templates can't overload on the
// return type like this.
// The typedef is for the convenience of callers of this function
// (avoiding the need for a trailing return decltype)
template<typename T> struct IndirectHelper {
  typedef T return_type;
  typedef T mutable_return_type;
  static const size_t element_stride = sizeof(T);

  static return_type Read(const uint8_t *p, const size_t i) {
	return EndianScalar((reinterpret_cast<const T *>(p))[i]);
  }
  static mutable_return_type Read(uint8_t *p, const size_t i) {
	return reinterpret_cast<mutable_return_type>(
		Read(const_cast<const uint8_t *>(p), i));
  }
};

// For vector of Offsets.
template<typename T, template<typename> class OffsetT>
struct IndirectHelper<OffsetT<T>> {
  typedef const T *return_type;
  typedef T *mutable_return_type;
  typedef typename OffsetT<T>::offset_type offset_type;
  static const offset_type element_stride = sizeof(offset_type);

  static return_type Read(const uint8_t *const p, const offset_type i) {
	// Offsets are relative to themselves, so first update the pointer to
	// point to the offset location.
	const uint8_t *const offset_location = p + i * element_stride;

	// Then read the scalar value of the offset (which may be 32 or 64-bits) and
	// then determine the relative location from the offset location.
	return reinterpret_cast<return_type>(
		offset_location + ReadScalar<offset_type>(offset_location));
  }
  static mutable_return_type Read(uint8_t *const p, const offset_type i) {
	// Offsets are relative to themselves, so first update the pointer to
	// point to the offset location.
	uint8_t *const offset_location = p + i * element_stride;

	// Then read the scalar value of the offset (which may be 32 or 64-bits) and
	// then determine the relative location from the offset location.
	return reinterpret_cast<mutable_return_type>(
		offset_location + ReadScalar<offset_type>(offset_location));
  }
};

// For vector of structs.
template<typename T> struct IndirectHelper<const T *> {
  typedef const T *return_type;
  typedef T *mutable_return_type;
  static const size_t element_stride = sizeof(T);

  static return_type Read(const uint8_t *const p, const size_t i) {
	// Structs are stored inline, relative to the first struct pointer.
	return reinterpret_cast<return_type>(p + i * element_stride);
  }
  static mutable_return_type Read(uint8_t *const p, const size_t i) {
	// Structs are stored inline, relative to the first struct pointer.
	return reinterpret_cast<mutable_return_type>(p + i * element_stride);
  }
};

/// @brief Get a pointer to the file_identifier section of the buffer.
/// @return Returns a const char pointer to the start of the file_identifier
/// characters in the buffer.  The returned char * has length
/// 'flatbuffers::FlatBufferBuilder::kFileIdentifierLength'.
/// This function is UNDEFINED for FlatBuffers whose schema does not include
/// a file_identifier (likely points at padding or the start of a the root
/// vtable).
inline const char *GetBufferIdentifier(const void *buf,
									   bool size_prefixed = false) {
  return reinterpret_cast<const char *>(buf) +
		 ((size_prefixed) ? 2 * sizeof(uoffset_t) : sizeof(uoffset_t));
}

// Helper to see if the identifier in a buffer has the expected value.
inline bool BufferHasIdentifier(const void *buf, const char *identifier,
								bool size_prefixed = false) {
  return strncmp(GetBufferIdentifier(buf, size_prefixed), identifier,
				 flatbuffers::kFileIdentifierLength) == 0;
}

/// @cond FLATBUFFERS_INTERNAL
// Helpers to get a typed pointer to the root object contained in the buffer.
template<typename T> T *GetMutableRoot(void *buf) {
  if (!buf) return nullptr;
  EndianCheck();
  return reinterpret_cast<T *>(
	  reinterpret_cast<uint8_t *>(buf) +
	  EndianScalar(*reinterpret_cast<uoffset_t *>(buf)));
}

template<typename T, typename SizeT = uoffset_t>
T *GetMutableSizePrefixedRoot(void *buf) {
  return GetMutableRoot<T>(reinterpret_cast<uint8_t *>(buf) + sizeof(SizeT));
}

template<typename T> const T *GetRoot(const void *buf) {
  return GetMutableRoot<T>(const_cast<void *>(buf));
}

template<typename T, typename SizeT = uoffset_t>
const T *GetSizePrefixedRoot(const void *buf) {
  return GetRoot<T>(reinterpret_cast<const uint8_t *>(buf) + sizeof(SizeT));
}

}  // namespace flatbuffers

#endif  // FLATBUFFERS_BUFFER_H_

/*** End of inlined file: buffer.h ***/

namespace flatbuffers {

struct String;

// An STL compatible iterator implementation for Vector below, effectively
// calling Get() for every element.
template<typename T, typename IT, typename Data = uint8_t *,
		 typename SizeT = uoffset_t>
struct VectorIterator {
  typedef std::random_access_iterator_tag iterator_category;
  typedef IT value_type;
  typedef ptrdiff_t difference_type;
  typedef IT *pointer;
  typedef IT &reference;

  static const SizeT element_stride = IndirectHelper<T>::element_stride;

  VectorIterator(Data data, SizeT i) : data_(data + element_stride * i) {}
  VectorIterator(const VectorIterator &other) : data_(other.data_) {}
  VectorIterator() : data_(nullptr) {}

  VectorIterator &operator=(const VectorIterator &other) {
	data_ = other.data_;
	return *this;
  }

  VectorIterator &operator=(VectorIterator &&other) {
	data_ = other.data_;
	return *this;
  }

  bool operator==(const VectorIterator &other) const {
	return data_ == other.data_;
  }

  bool operator!=(const VectorIterator &other) const {
	return data_ != other.data_;
  }

  bool operator<(const VectorIterator &other) const {
	return data_ < other.data_;
  }

  bool operator>(const VectorIterator &other) const {
	return data_ > other.data_;
  }

  bool operator<=(const VectorIterator &other) const {
	return !(data_ > other.data_);
  }

  bool operator>=(const VectorIterator &other) const {
	return !(data_ < other.data_);
  }

  difference_type operator-(const VectorIterator &other) const {
	return (data_ - other.data_) / element_stride;
  }

  // Note: return type is incompatible with the standard
  // `reference operator*()`.
  IT operator*() const { return IndirectHelper<T>::Read(data_, 0); }

  // Note: return type is incompatible with the standard
  // `pointer operator->()`.
  IT operator->() const { return IndirectHelper<T>::Read(data_, 0); }

  VectorIterator &operator++() {
	data_ += element_stride;
	return *this;
  }

  VectorIterator operator++(int) {
	VectorIterator temp(data_, 0);
	data_ += element_stride;
	return temp;
  }

  VectorIterator operator+(const SizeT &offset) const {
	return VectorIterator(data_ + offset * element_stride, 0);
  }

  VectorIterator &operator+=(const SizeT &offset) {
	data_ += offset * element_stride;
	return *this;
  }

  VectorIterator &operator--() {
	data_ -= element_stride;
	return *this;
  }

  VectorIterator operator--(int) {
	VectorIterator temp(data_, 0);
	data_ -= element_stride;
	return temp;
  }

  VectorIterator operator-(const SizeT &offset) const {
	return VectorIterator(data_ - offset * element_stride, 0);
  }

  VectorIterator &operator-=(const SizeT &offset) {
	data_ -= offset * element_stride;
	return *this;
  }

 private:
  Data data_;
};

template<typename T, typename IT, typename SizeT = uoffset_t>
using VectorConstIterator = VectorIterator<T, IT, const uint8_t *, SizeT>;

template<typename Iterator>
struct VectorReverseIterator : public std::reverse_iterator<Iterator> {
  explicit VectorReverseIterator(Iterator iter)
	  : std::reverse_iterator<Iterator>(iter) {}

  // Note: return type is incompatible with the standard
  // `reference operator*()`.
  typename Iterator::value_type operator*() const {
	auto tmp = std::reverse_iterator<Iterator>::current;
	return *--tmp;
  }

  // Note: return type is incompatible with the standard
  // `pointer operator->()`.
  typename Iterator::value_type operator->() const {
	auto tmp = std::reverse_iterator<Iterator>::current;
	return *--tmp;
  }
};

// This is used as a helper type for accessing vectors.
// Vector::data() assumes the vector elements start after the length field.
template<typename T, typename SizeT = uoffset_t> class Vector {
 public:
  typedef VectorIterator<T, typename IndirectHelper<T>::mutable_return_type,
						 uint8_t *, SizeT>
	  iterator;
  typedef VectorConstIterator<T, typename IndirectHelper<T>::return_type, SizeT>
	  const_iterator;
  typedef VectorReverseIterator<iterator> reverse_iterator;
  typedef VectorReverseIterator<const_iterator> const_reverse_iterator;

  typedef typename flatbuffers::bool_constant<flatbuffers::is_scalar<T>::value>
	  scalar_tag;

  static FLATBUFFERS_CONSTEXPR bool is_span_observable =
	  scalar_tag::value && (FLATBUFFERS_LITTLEENDIAN || sizeof(T) == 1);

  SizeT size() const { return EndianScalar(length_); }

  // Deprecated: use size(). Here for backwards compatibility.
  FLATBUFFERS_ATTRIBUTE([[deprecated("use size() instead")]])
  SizeT Length() const { return size(); }

  typedef SizeT size_type;
  typedef typename IndirectHelper<T>::return_type return_type;
  typedef typename IndirectHelper<T>::mutable_return_type mutable_return_type;
  typedef return_type value_type;

  return_type Get(SizeT i) const {
	FLATBUFFERS_ASSERT(i < size());
	return IndirectHelper<T>::Read(Data(), i);
  }

  return_type operator[](SizeT i) const { return Get(i); }

  // If this is a Vector of enums, T will be its storage type, not the enum
  // type. This function makes it convenient to retrieve value with enum
  // type E.
  template<typename E> E GetEnum(SizeT i) const {
	return static_cast<E>(Get(i));
  }

  // If this a vector of unions, this does the cast for you. There's no check
  // to make sure this is the right type!
  template<typename U> const U *GetAs(SizeT i) const {
	return reinterpret_cast<const U *>(Get(i));
  }

  // If this a vector of unions, this does the cast for you. There's no check
  // to make sure this is actually a string!
  const String *GetAsString(SizeT i) const {
	return reinterpret_cast<const String *>(Get(i));
  }

  const void *GetStructFromOffset(size_t o) const {
	return reinterpret_cast<const void *>(Data() + o);
  }

  iterator begin() { return iterator(Data(), 0); }
  const_iterator begin() const { return const_iterator(Data(), 0); }

  iterator end() { return iterator(Data(), size()); }
  const_iterator end() const { return const_iterator(Data(), size()); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
	return const_reverse_iterator(end());
  }

  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
	return const_reverse_iterator(begin());
  }

  const_iterator cbegin() const { return begin(); }

  const_iterator cend() const { return end(); }

  const_reverse_iterator crbegin() const { return rbegin(); }

  const_reverse_iterator crend() const { return rend(); }

  // Change elements if you have a non-const pointer to this object.
  // Scalars only. See reflection.h, and the documentation.
  void Mutate(SizeT i, const T &val) {
	FLATBUFFERS_ASSERT(i < size());
	WriteScalar(data() + i, val);
  }

  // Change an element of a vector of tables (or strings).
  // "val" points to the new table/string, as you can obtain from
  // e.g. reflection::AddFlatBuffer().
  void MutateOffset(SizeT i, const uint8_t *val) {
	FLATBUFFERS_ASSERT(i < size());
	static_assert(sizeof(T) == sizeof(SizeT), "Unrelated types");
	WriteScalar(data() + i,
				static_cast<SizeT>(val - (Data() + i * sizeof(SizeT))));
  }

  // Get a mutable pointer to tables/strings inside this vector.
  mutable_return_type GetMutableObject(SizeT i) const {
	FLATBUFFERS_ASSERT(i < size());
	return const_cast<mutable_return_type>(IndirectHelper<T>::Read(Data(), i));
  }

  // The raw data in little endian format. Use with care.
  const uint8_t *Data() const {
	return reinterpret_cast<const uint8_t *>(&length_ + 1);
  }

  uint8_t *Data() { return reinterpret_cast<uint8_t *>(&length_ + 1); }

  // Similarly, but typed, much like std::vector::data
  const T *data() const { return reinterpret_cast<const T *>(Data()); }
  T *data() { return reinterpret_cast<T *>(Data()); }

  template<typename K> return_type LookupByKey(K key) const {
	void *search_result = std::bsearch(
		&key, Data(), size(), IndirectHelper<T>::element_stride, KeyCompare<K>);

	if (!search_result) {
	  return nullptr;  // Key not found.
	}

	const uint8_t *element = reinterpret_cast<const uint8_t *>(search_result);

	return IndirectHelper<T>::Read(element, 0);
  }

  template<typename K> mutable_return_type MutableLookupByKey(K key) {
	return const_cast<mutable_return_type>(LookupByKey(key));
  }

 protected:
  // This class is only used to access pre-existing data. Don't ever
  // try to construct these manually.
  Vector();

  SizeT length_;

 private:
  // This class is a pointer. Copying will therefore create an invalid object.
  // Private and unimplemented copy constructor.
  Vector(const Vector &);
  Vector &operator=(const Vector &);

  template<typename K> static int KeyCompare(const void *ap, const void *bp) {
	const K *key = reinterpret_cast<const K *>(ap);
	const uint8_t *data = reinterpret_cast<const uint8_t *>(bp);
	auto table = IndirectHelper<T>::Read(data, 0);

	// std::bsearch compares with the operands transposed, so we negate the
	// result here.
	return -table->KeyCompareWithValue(*key);
  }
};

template<typename T> using Vector64 = Vector<T, uoffset64_t>;

template<class U>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<U> make_span(Vector<U> &vec)
	FLATBUFFERS_NOEXCEPT {
  static_assert(Vector<U>::is_span_observable,
				"wrong type U, only LE-scalar, or byte types are allowed");
  return span<U>(vec.data(), vec.size());
}

template<class U>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<const U> make_span(
	const Vector<U> &vec) FLATBUFFERS_NOEXCEPT {
  static_assert(Vector<U>::is_span_observable,
				"wrong type U, only LE-scalar, or byte types are allowed");
  return span<const U>(vec.data(), vec.size());
}

template<class U>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<uint8_t> make_bytes_span(
	Vector<U> &vec) FLATBUFFERS_NOEXCEPT {
  static_assert(Vector<U>::scalar_tag::value,
				"wrong type U, only LE-scalar, or byte types are allowed");
  return span<uint8_t>(vec.Data(), vec.size() * sizeof(U));
}

template<class U>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<const uint8_t> make_bytes_span(
	const Vector<U> &vec) FLATBUFFERS_NOEXCEPT {
  static_assert(Vector<U>::scalar_tag::value,
				"wrong type U, only LE-scalar, or byte types are allowed");
  return span<const uint8_t>(vec.Data(), vec.size() * sizeof(U));
}

// Convenient helper functions to get a span of any vector, regardless
// of whether it is null or not (the field is not set).
template<class U>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<U> make_span(Vector<U> *ptr)
	FLATBUFFERS_NOEXCEPT {
  static_assert(Vector<U>::is_span_observable,
				"wrong type U, only LE-scalar, or byte types are allowed");
  return ptr ? make_span(*ptr) : span<U>();
}

template<class U>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<const U> make_span(
	const Vector<U> *ptr) FLATBUFFERS_NOEXCEPT {
  static_assert(Vector<U>::is_span_observable,
				"wrong type U, only LE-scalar, or byte types are allowed");
  return ptr ? make_span(*ptr) : span<const U>();
}

// Represent a vector much like the template above, but in this case we
// don't know what the element types are (used with reflection.h).
class VectorOfAny {
 public:
  uoffset_t size() const { return EndianScalar(length_); }

  const uint8_t *Data() const {
	return reinterpret_cast<const uint8_t *>(&length_ + 1);
  }
  uint8_t *Data() { return reinterpret_cast<uint8_t *>(&length_ + 1); }

 protected:
  VectorOfAny();

  uoffset_t length_;

 private:
  VectorOfAny(const VectorOfAny &);
  VectorOfAny &operator=(const VectorOfAny &);
};

template<typename T, typename U>
Vector<Offset<T>> *VectorCast(Vector<Offset<U>> *ptr) {
  static_assert(std::is_base_of<T, U>::value, "Unrelated types");
  return reinterpret_cast<Vector<Offset<T>> *>(ptr);
}

template<typename T, typename U>
const Vector<Offset<T>> *VectorCast(const Vector<Offset<U>> *ptr) {
  static_assert(std::is_base_of<T, U>::value, "Unrelated types");
  return reinterpret_cast<const Vector<Offset<T>> *>(ptr);
}

// Convenient helper function to get the length of any vector, regardless
// of whether it is null or not (the field is not set).
template<typename T> static inline size_t VectorLength(const Vector<T> *v) {
  return v ? v->size() : 0;
}

}  // namespace flatbuffers

#endif  // FLATBUFFERS_VERIFIER_H_

/*** End of inlined file: vector.h ***/

namespace flatbuffers {

// This is used as a helper type for accessing arrays.
template<typename T, uint16_t length> class Array {
  // Array<T> can carry only POD data types (scalars or structs).
  typedef typename flatbuffers::bool_constant<flatbuffers::is_scalar<T>::value>
	  scalar_tag;
  typedef
	  typename flatbuffers::conditional<scalar_tag::value, T, const T *>::type
		  IndirectHelperType;

 public:
  typedef uint16_t size_type;
  typedef typename IndirectHelper<IndirectHelperType>::return_type return_type;
  typedef VectorConstIterator<T, return_type, uoffset_t> const_iterator;
  typedef VectorReverseIterator<const_iterator> const_reverse_iterator;

  // If T is a LE-scalar or a struct (!scalar_tag::value).
  static FLATBUFFERS_CONSTEXPR bool is_span_observable =
	  (scalar_tag::value && (FLATBUFFERS_LITTLEENDIAN || sizeof(T) == 1)) ||
	  !scalar_tag::value;

  FLATBUFFERS_CONSTEXPR uint16_t size() const { return length; }

  return_type Get(uoffset_t i) const {
	FLATBUFFERS_ASSERT(i < size());
	return IndirectHelper<IndirectHelperType>::Read(Data(), i);
  }

  return_type operator[](uoffset_t i) const { return Get(i); }

  // If this is a Vector of enums, T will be its storage type, not the enum
  // type. This function makes it convenient to retrieve value with enum
  // type E.
  template<typename E> E GetEnum(uoffset_t i) const {
	return static_cast<E>(Get(i));
  }

  const_iterator begin() const { return const_iterator(Data(), 0); }
  const_iterator end() const { return const_iterator(Data(), size()); }

  const_reverse_iterator rbegin() const {
	return const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const {
	return const_reverse_iterator(begin());
  }

  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  const_reverse_iterator crbegin() const { return rbegin(); }
  const_reverse_iterator crend() const { return rend(); }

  // Get a mutable pointer to elements inside this array.
  // This method used to mutate arrays of structs followed by a @p Mutate
  // operation. For primitive types use @p Mutate directly.
  // @warning Assignments and reads to/from the dereferenced pointer are not
  //  automatically converted to the correct endianness.
  typename flatbuffers::conditional<scalar_tag::value, void, T *>::type
  GetMutablePointer(uoffset_t i) const {
	FLATBUFFERS_ASSERT(i < size());
	return const_cast<T *>(&data()[i]);
  }

  // Change elements if you have a non-const pointer to this object.
  void Mutate(uoffset_t i, const T &val) { MutateImpl(scalar_tag(), i, val); }

  // The raw data in little endian format. Use with care.
  const uint8_t *Data() const { return data_; }

  uint8_t *Data() { return data_; }

  // Similarly, but typed, much like std::vector::data
  const T *data() const { return reinterpret_cast<const T *>(Data()); }
  T *data() { return reinterpret_cast<T *>(Data()); }

  // Copy data from a span with endian conversion.
  // If this Array and the span overlap, the behavior is undefined.
  void CopyFromSpan(flatbuffers::span<const T, length> src) {
	const auto p1 = reinterpret_cast<const uint8_t *>(src.data());
	const auto p2 = Data();
	FLATBUFFERS_ASSERT(!(p1 >= p2 && p1 < (p2 + length)) &&
					   !(p2 >= p1 && p2 < (p1 + length)));
	(void)p1;
	(void)p2;
	CopyFromSpanImpl(flatbuffers::bool_constant<is_span_observable>(), src);
  }

 protected:
  void MutateImpl(flatbuffers::true_type, uoffset_t i, const T &val) {
	FLATBUFFERS_ASSERT(i < size());
	WriteScalar(data() + i, val);
  }

  void MutateImpl(flatbuffers::false_type, uoffset_t i, const T &val) {
	*(GetMutablePointer(i)) = val;
  }

  void CopyFromSpanImpl(flatbuffers::true_type,
						flatbuffers::span<const T, length> src) {
	// Use std::memcpy() instead of std::copy() to avoid performance degradation
	// due to aliasing if T is char or unsigned char.
	// The size is known at compile time, so memcpy would be inlined.
	std::memcpy(data(), src.data(), length * sizeof(T));
  }

  // Copy data from flatbuffers::span with endian conversion.
  void CopyFromSpanImpl(flatbuffers::false_type,
						flatbuffers::span<const T, length> src) {
	for (size_type k = 0; k < length; k++) { Mutate(k, src[k]); }
  }

  // This class is only used to access pre-existing data. Don't ever
  // try to construct these manually.
  // 'constexpr' allows us to use 'size()' at compile time.
  // @note Must not use 'FLATBUFFERS_CONSTEXPR' here, as const is not allowed on
  //  a constructor.
#if defined(__cpp_constexpr)
  constexpr Array();
#else
  Array();
#endif

  uint8_t data_[length * sizeof(T)];

 private:
  // This class is a pointer. Copying will therefore create an invalid object.
  // Private and unimplemented copy constructor.
  Array(const Array &);
  Array &operator=(const Array &);
};

// Specialization for Array[struct] with access using Offset<void> pointer.
// This specialization used by idl_gen_text.cpp.
template<typename T, uint16_t length, template<typename> class OffsetT>
class Array<OffsetT<T>, length> {
  static_assert(flatbuffers::is_same<T, void>::value, "unexpected type T");

 public:
  typedef const void *return_type;
  typedef uint16_t size_type;

  const uint8_t *Data() const { return data_; }

  // Make idl_gen_text.cpp::PrintContainer happy.
  return_type operator[](uoffset_t) const {
	FLATBUFFERS_ASSERT(false);
	return nullptr;
  }

 private:
  // This class is only used to access pre-existing data.
  Array();
  Array(const Array &);
  Array &operator=(const Array &);

  uint8_t data_[1];
};

template<class U, uint16_t N>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<U, N> make_span(Array<U, N> &arr)
	FLATBUFFERS_NOEXCEPT {
  static_assert(
	  Array<U, N>::is_span_observable,
	  "wrong type U, only plain struct, LE-scalar, or byte types are allowed");
  return span<U, N>(arr.data(), N);
}

template<class U, uint16_t N>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<const U, N> make_span(
	const Array<U, N> &arr) FLATBUFFERS_NOEXCEPT {
  static_assert(
	  Array<U, N>::is_span_observable,
	  "wrong type U, only plain struct, LE-scalar, or byte types are allowed");
  return span<const U, N>(arr.data(), N);
}

template<class U, uint16_t N>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<uint8_t, sizeof(U) * N>
make_bytes_span(Array<U, N> &arr) FLATBUFFERS_NOEXCEPT {
  static_assert(Array<U, N>::is_span_observable,
				"internal error, Array<T> might hold only scalars or structs");
  return span<uint8_t, sizeof(U) * N>(arr.Data(), sizeof(U) * N);
}

template<class U, uint16_t N>
FLATBUFFERS_CONSTEXPR_CPP11 flatbuffers::span<const uint8_t, sizeof(U) * N>
make_bytes_span(const Array<U, N> &arr) FLATBUFFERS_NOEXCEPT {
  static_assert(Array<U, N>::is_span_observable,
				"internal error, Array<T> might hold only scalars or structs");
  return span<const uint8_t, sizeof(U) * N>(arr.Data(), sizeof(U) * N);
}

// Cast a raw T[length] to a raw flatbuffers::Array<T, length>
// without endian conversion. Use with care.
// TODO: move these Cast-methods to `internal` namespace.
template<typename T, uint16_t length>
Array<T, length> &CastToArray(T (&arr)[length]) {
  return *reinterpret_cast<Array<T, length> *>(arr);
}

template<typename T, uint16_t length>
const Array<T, length> &CastToArray(const T (&arr)[length]) {
  return *reinterpret_cast<const Array<T, length> *>(arr);
}

template<typename E, typename T, uint16_t length>
Array<E, length> &CastToArrayOfEnum(T (&arr)[length]) {
  static_assert(sizeof(E) == sizeof(T), "invalid enum type E");
  return *reinterpret_cast<Array<E, length> *>(arr);
}

template<typename E, typename T, uint16_t length>
const Array<E, length> &CastToArrayOfEnum(const T (&arr)[length]) {
  static_assert(sizeof(E) == sizeof(T), "invalid enum type E");
  return *reinterpret_cast<const Array<E, length> *>(arr);
}

template<typename T, uint16_t length>
bool operator==(const Array<T, length> &lhs,
				const Array<T, length> &rhs) noexcept {
  return std::addressof(lhs) == std::addressof(rhs) ||
		 (lhs.size() == rhs.size() &&
		  std::memcmp(lhs.Data(), rhs.Data(), rhs.size() * sizeof(T)) == 0);
}

}  // namespace flatbuffers

#endif  // FLATBUFFERS_ARRAY_H_

/*** End of inlined file: array.h ***/



/*** Start of inlined file: buffer_ref.h ***/
#ifndef FLATBUFFERS_BUFFER_REF_H_
#define FLATBUFFERS_BUFFER_REF_H_


/*** Start of inlined file: verifier.h ***/
#ifndef FLATBUFFERS_VERIFIER_H_
#define FLATBUFFERS_VERIFIER_H_

namespace flatbuffers {

// Helper class to verify the integrity of a FlatBuffer
template <bool TrackVerifierBufferSize>
class VerifierTemplate FLATBUFFERS_FINAL_CLASS {
 public:
  struct Options {
	// The maximum nesting of tables and vectors before we call it invalid.
	uoffset_t max_depth = 64;
	// The maximum number of tables we will verify before we call it invalid.
	uoffset_t max_tables = 1000000;
	// If true, verify all data is aligned.
	bool check_alignment = true;
	// If true, run verifier on nested flatbuffers
	bool check_nested_flatbuffers = true;
	// The maximum size of a buffer.
	size_t max_size = FLATBUFFERS_MAX_BUFFER_SIZE;
	// Use assertions to check for errors.
	bool assert = false;
  };

  explicit VerifierTemplate(const uint8_t *const buf, const size_t buf_len,
							const Options &opts)
	  : buf_(buf), size_(buf_len), opts_(opts) {
	FLATBUFFERS_ASSERT(size_ < opts.max_size);
  }

  // Deprecated API, please construct with VerifierTemplate::Options.
  VerifierTemplate(const uint8_t *const buf, const size_t buf_len,
				   const uoffset_t max_depth = 64,
				   const uoffset_t max_tables = 1000000,
				   const bool check_alignment = true)
	  : VerifierTemplate(buf, buf_len, [&] {
		  Options opts;
		  opts.max_depth = max_depth;
		  opts.max_tables = max_tables;
		  opts.check_alignment = check_alignment;
		  return opts;
		}()) {}

  // Central location where any verification failures register.
  bool Check(const bool ok) const {
	// clang-format off
	#ifdef FLATBUFFERS_DEBUG_VERIFICATION_FAILURE
	  if (opts_.assert) { FLATBUFFERS_ASSERT(ok); }
	#endif
	// clang-format on
	if (TrackVerifierBufferSize) {
	  if (!ok) {
		upper_bound_ = 0;
	  }
	}
	return ok;
  }

  // Verify any range within the buffer.
  bool Verify(const size_t elem, const size_t elem_len) const {
	if (TrackVerifierBufferSize) {
	  auto upper_bound = elem + elem_len;
	  if (upper_bound_ < upper_bound) {
		upper_bound_ =  upper_bound;
	  }
	}
	return Check(elem_len < size_ && elem <= size_ - elem_len);
  }

  bool VerifyAlignment(const size_t elem, const size_t align) const {
	return Check((elem & (align - 1)) == 0 || !opts_.check_alignment);
  }

  // Verify a range indicated by sizeof(T).
  template<typename T> bool Verify(const size_t elem) const {
	return VerifyAlignment(elem, sizeof(T)) && Verify(elem, sizeof(T));
  }

  bool VerifyFromPointer(const uint8_t *const p, const size_t len) {
	return Verify(static_cast<size_t>(p - buf_), len);
  }

  // Verify relative to a known-good base pointer.
  bool VerifyFieldStruct(const uint8_t *const base, const voffset_t elem_off,
						 const size_t elem_len, const size_t align) const {
	const auto f = static_cast<size_t>(base - buf_) + elem_off;
	return VerifyAlignment(f, align) && Verify(f, elem_len);
  }

  template<typename T>
  bool VerifyField(const uint8_t *const base, const voffset_t elem_off,
				   const size_t align) const {
	const auto f = static_cast<size_t>(base - buf_) + elem_off;
	return VerifyAlignment(f, align) && Verify(f, sizeof(T));
  }

  // Verify a pointer (may be NULL) of a table type.
  template<typename T> bool VerifyTable(const T *const table) {
	return !table || table->Verify(*this);
  }

  // Verify a pointer (may be NULL) of any vector type.
  template<int &..., typename T, typename LenT>
  bool VerifyVector(const Vector<T, LenT> *const vec) const {
	return !vec || VerifyVectorOrString<LenT>(
					   reinterpret_cast<const uint8_t *>(vec), sizeof(T));
  }

  // Verify a pointer (may be NULL) of a vector to struct.
  template<int &..., typename T, typename LenT>
  bool VerifyVector(const Vector<const T *, LenT> *const vec) const {
	return VerifyVector(reinterpret_cast<const Vector<T, LenT> *>(vec));
  }

  // Verify a pointer (may be NULL) to string.
  bool VerifyString(const String *const str) const {
	size_t end;
	return !str || (VerifyVectorOrString<uoffset_t>(
						reinterpret_cast<const uint8_t *>(str), 1, &end) &&
					Verify(end, 1) &&           // Must have terminator
					Check(buf_[end] == '\0'));  // Terminating byte must be 0.
  }

  // Common code between vectors and strings.
  template<typename LenT = uoffset_t>
  bool VerifyVectorOrString(const uint8_t *const vec, const size_t elem_size,
							size_t *const end = nullptr) const {
	const auto vec_offset = static_cast<size_t>(vec - buf_);
	// Check we can read the size field.
	if (!Verify<LenT>(vec_offset)) return false;
	// Check the whole array. If this is a string, the byte past the array must
	// be 0.
	const LenT size = ReadScalar<LenT>(vec);
	const auto max_elems = opts_.max_size / elem_size;
	if (!Check(size < max_elems))
	  return false;  // Protect against byte_size overflowing.
	const auto byte_size = sizeof(LenT) + elem_size * size;
	if (end) *end = vec_offset + byte_size;
	return Verify(vec_offset, byte_size);
  }

  // Special case for string contents, after the above has been called.
  bool VerifyVectorOfStrings(const Vector<Offset<String>> *const vec) const {
	if (vec) {
	  for (uoffset_t i = 0; i < vec->size(); i++) {
		if (!VerifyString(vec->Get(i))) return false;
	  }
	}
	return true;
  }

  // Special case for table contents, after the above has been called.
  template<typename T>
  bool VerifyVectorOfTables(const Vector<Offset<T>> *const vec) {
	if (vec) {
	  for (uoffset_t i = 0; i < vec->size(); i++) {
		if (!vec->Get(i)->Verify(*this)) return false;
	  }
	}
	return true;
  }

  FLATBUFFERS_SUPPRESS_UBSAN("unsigned-integer-overflow")
  bool VerifyTableStart(const uint8_t *const table) {
	// Check the vtable offset.
	const auto tableo = static_cast<size_t>(table - buf_);
	if (!Verify<soffset_t>(tableo)) return false;
	// This offset may be signed, but doing the subtraction unsigned always
	// gives the result we want.
	const auto vtableo =
		tableo - static_cast<size_t>(ReadScalar<soffset_t>(table));
	// Check the vtable size field, then check vtable fits in its entirety.
	if (!(VerifyComplexity() && Verify<voffset_t>(vtableo) &&
		  VerifyAlignment(ReadScalar<voffset_t>(buf_ + vtableo),
						  sizeof(voffset_t))))
	  return false;
	const auto vsize = ReadScalar<voffset_t>(buf_ + vtableo);
	return Check((vsize & 1) == 0) && Verify(vtableo, vsize);
  }

  template<typename T>
  bool VerifyBufferFromStart(const char *const identifier, const size_t start) {
	// Buffers have to be of some size to be valid. The reason it is a runtime
	// check instead of static_assert, is that nested flatbuffers go through
	// this call and their size is determined at runtime.
	if (!Check(size_ >= FLATBUFFERS_MIN_BUFFER_SIZE)) return false;

	// If an identifier is provided, check that we have a buffer
	if (identifier && !Check((size_ >= 2 * sizeof(flatbuffers::uoffset_t) &&
							  BufferHasIdentifier(buf_ + start, identifier)))) {
	  return false;
	}

	// Call T::Verify, which must be in the generated code for this type.
	const auto o = VerifyOffset<uoffset_t>(start);
	if (!Check(o != 0)) return false;
	if (!(reinterpret_cast<const T *>(buf_ + start + o)->Verify(*this))) {
	  return false;
	}
	if (TrackVerifierBufferSize) {
	  if (GetComputedSize() == 0) return false;
	}
	return true;
  }

  template<typename T, int &..., typename SizeT>
  bool VerifyNestedFlatBuffer(const Vector<uint8_t, SizeT> *const buf,
							  const char *const identifier) {
	// Caller opted out of this.
	if (!opts_.check_nested_flatbuffers) return true;

	// An empty buffer is OK as it indicates not present.
	if (!buf) return true;

	// If there is a nested buffer, it must be greater than the min size.
	if (!Check(buf->size() >= FLATBUFFERS_MIN_BUFFER_SIZE)) return false;

	VerifierTemplate<TrackVerifierBufferSize> nested_verifier(
		buf->data(), buf->size(), opts_);
	return nested_verifier.VerifyBuffer<T>(identifier);
  }

  // Verify this whole buffer, starting with root type T.
  template<typename T> bool VerifyBuffer() { return VerifyBuffer<T>(nullptr); }

  template<typename T> bool VerifyBuffer(const char *const identifier) {
	return VerifyBufferFromStart<T>(identifier, 0);
  }

  template<typename T, typename SizeT = uoffset_t>
  bool VerifySizePrefixedBuffer(const char *const identifier) {
	return Verify<SizeT>(0U) &&
		   // Ensure the prefixed size is within the bounds of the provided
		   // length.
		   Check(ReadScalar<SizeT>(buf_) + sizeof(SizeT) <= size_) &&
		   VerifyBufferFromStart<T>(identifier, sizeof(SizeT));
  }

  template<typename OffsetT = uoffset_t, typename SOffsetT = soffset_t>
  size_t VerifyOffset(const size_t start) const {
	if (!Verify<OffsetT>(start)) return 0;
	const auto o = ReadScalar<OffsetT>(buf_ + start);
	// May not point to itself.
	if (!Check(o != 0)) return 0;
	// Can't wrap around larger than the max size.
	if (!Check(static_cast<SOffsetT>(o) >= 0)) return 0;
	// Must be inside the buffer to create a pointer from it (pointer outside
	// buffer is UB).
	if (!Verify(start + o, 1)) return 0;
	return o;
  }

  template<typename OffsetT = uoffset_t>
  size_t VerifyOffset(const uint8_t *const base, const voffset_t start) const {
	return VerifyOffset<OffsetT>(static_cast<size_t>(base - buf_) + start);
  }

  // Called at the start of a table to increase counters measuring data
  // structure depth and amount, and possibly bails out with false if limits set
  // by the constructor have been hit. Needs to be balanced with EndTable().
  bool VerifyComplexity() {
	depth_++;
	num_tables_++;
	return Check(depth_ <= opts_.max_depth && num_tables_ <= opts_.max_tables);
  }

  // Called at the end of a table to pop the depth count.
  bool EndTable() {
	depth_--;
	return true;
  }

  // Returns the message size in bytes.
  //
  // This should only be called after first calling VerifyBuffer or
  // VerifySizePrefixedBuffer.
  //
  // This method should only be called for VerifierTemplate instances
  // where the TrackVerifierBufferSize template parameter is true,
  // i.e. for SizeVerifier.  For instances where TrackVerifierBufferSize
  // is false, this fails at runtime or returns zero.
  size_t GetComputedSize() const {
	if (TrackVerifierBufferSize) {
	  uintptr_t size = upper_bound_;
	  // Align the size to uoffset_t
	  size = (size - 1 + sizeof(uoffset_t)) & ~(sizeof(uoffset_t) - 1);
	  return (size > size_) ?  0 : size;
	}
	// Must use SizeVerifier, or (deprecated) turn on
	// FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE, for this to work.
	(void)upper_bound_;
	FLATBUFFERS_ASSERT(false);
	return 0;
  }

  std::vector<uint8_t> *GetFlexReuseTracker() { return flex_reuse_tracker_; }

  void SetFlexReuseTracker(std::vector<uint8_t> *const rt) {
	flex_reuse_tracker_ = rt;
  }

 private:
  const uint8_t *buf_;
  const size_t size_;
  const Options opts_;

  mutable size_t upper_bound_ = 0;

  uoffset_t depth_ = 0;
  uoffset_t num_tables_ = 0;
  std::vector<uint8_t> *flex_reuse_tracker_ = nullptr;
};

// Specialization for 64-bit offsets.
template<>
template<>
inline size_t VerifierTemplate<false>::VerifyOffset<uoffset64_t>(
	const size_t start) const {
  return VerifyOffset<uoffset64_t, soffset64_t>(start);
}
template<>
template<>
inline size_t VerifierTemplate<true>::VerifyOffset<uoffset64_t>(
	const size_t start) const {
  return VerifyOffset<uoffset64_t, soffset64_t>(start);
}

// Instance of VerifierTemplate that supports GetComputedSize().
using SizeVerifier = VerifierTemplate</*TrackVerifierBufferSize = */ true>;

// The FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE build configuration macro is
// deprecated, and should not be defined, since it is easy to misuse in ways
// that result in ODR violations. Rather than using Verifier and defining
// FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE, please use SizeVerifier instead.
#ifdef FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE  // Deprecated, see above.
using Verifier = SizeVerifier;
#else
// Instance of VerifierTemplate that is slightly faster, but does not
// support GetComputedSize().
using Verifier = VerifierTemplate</*TrackVerifierBufferSize = */ false>;
#endif

}  // namespace flatbuffers

#endif  // FLATBUFFERS_VERIFIER_H_

/*** End of inlined file: verifier.h ***/

namespace flatbuffers {

// Convenient way to bundle a buffer and its length, to pass it around
// typed by its root.
// A BufferRef does not own its buffer.
struct BufferRefBase {};  // for std::is_base_of

template<typename T> struct BufferRef : BufferRefBase {
  BufferRef() : buf(nullptr), len(0), must_free(false) {}
  BufferRef(uint8_t *_buf, uoffset_t _len)
	  : buf(_buf), len(_len), must_free(false) {}

  ~BufferRef() {
	if (must_free) free(buf);
  }

  const T *GetRoot() const { return flatbuffers::GetRoot<T>(buf); }

  bool Verify() {
	Verifier verifier(buf, len);
	return verifier.VerifyBuffer<T>(nullptr);
  }

  uint8_t *buf;
  uoffset_t len;
  bool must_free;
};

}  // namespace flatbuffers

#endif  // FLATBUFFERS_BUFFER_REF_H_

/*** End of inlined file: buffer_ref.h ***/


/*** Start of inlined file: detached_buffer.h ***/
#ifndef FLATBUFFERS_DETACHED_BUFFER_H_
#define FLATBUFFERS_DETACHED_BUFFER_H_


/*** Start of inlined file: allocator.h ***/
#ifndef FLATBUFFERS_ALLOCATOR_H_
#define FLATBUFFERS_ALLOCATOR_H_

namespace flatbuffers {

// Allocator interface. This is flatbuffers-specific and meant only for
// `vector_downward` usage.
class Allocator {
 public:
  virtual ~Allocator() {}

  // Allocate `size` bytes of memory.
  virtual uint8_t *allocate(size_t size) = 0;

  // Deallocate `size` bytes of memory at `p` allocated by this allocator.
  virtual void deallocate(uint8_t *p, size_t size) = 0;

  // Reallocate `new_size` bytes of memory, replacing the old region of size
  // `old_size` at `p`. In contrast to a normal realloc, this grows downwards,
  // and is intended specifcally for `vector_downward` use.
  // `in_use_back` and `in_use_front` indicate how much of `old_size` is
  // actually in use at each end, and needs to be copied.
  virtual uint8_t *reallocate_downward(uint8_t *old_p, size_t old_size,
									   size_t new_size, size_t in_use_back,
									   size_t in_use_front) {
	FLATBUFFERS_ASSERT(new_size > old_size);  // vector_downward only grows
	uint8_t *new_p = allocate(new_size);
	memcpy_downward(old_p, old_size, new_p, new_size, in_use_back,
					in_use_front);
	deallocate(old_p, old_size);
	return new_p;
  }

 protected:
  // Called by `reallocate_downward` to copy memory from `old_p` of `old_size`
  // to `new_p` of `new_size`. Only memory of size `in_use_front` and
  // `in_use_back` will be copied from the front and back of the old memory
  // allocation.
  void memcpy_downward(uint8_t *old_p, size_t old_size, uint8_t *new_p,
					   size_t new_size, size_t in_use_back,
					   size_t in_use_front) {
	memcpy(new_p + new_size - in_use_back, old_p + old_size - in_use_back,
		   in_use_back);
	memcpy(new_p, old_p, in_use_front);
  }
};

}  // namespace flatbuffers

#endif  // FLATBUFFERS_ALLOCATOR_H_

/*** End of inlined file: allocator.h ***/


/*** Start of inlined file: default_allocator.h ***/
#ifndef FLATBUFFERS_DEFAULT_ALLOCATOR_H_
#define FLATBUFFERS_DEFAULT_ALLOCATOR_H_

namespace flatbuffers {

// DefaultAllocator uses new/delete to allocate memory regions
class DefaultAllocator : public Allocator {
 public:
  uint8_t *allocate(size_t size) FLATBUFFERS_OVERRIDE {
	return new uint8_t[size];
  }

  void deallocate(uint8_t *p, size_t) FLATBUFFERS_OVERRIDE { delete[] p; }

  static void dealloc(void *p, size_t) { delete[] static_cast<uint8_t *>(p); }
};

// These functions allow for a null allocator to mean use the default allocator,
// as used by DetachedBuffer and vector_downward below.
// This is to avoid having a statically or dynamically allocated default
// allocator, or having to move it between the classes that may own it.
inline uint8_t *Allocate(Allocator *allocator, size_t size) {
  return allocator ? allocator->allocate(size)
				   : DefaultAllocator().allocate(size);
}

inline void Deallocate(Allocator *allocator, uint8_t *p, size_t size) {
  if (allocator)
	allocator->deallocate(p, size);
  else
	DefaultAllocator().deallocate(p, size);
}

inline uint8_t *ReallocateDownward(Allocator *allocator, uint8_t *old_p,
								   size_t old_size, size_t new_size,
								   size_t in_use_back, size_t in_use_front) {
  return allocator ? allocator->reallocate_downward(old_p, old_size, new_size,
													in_use_back, in_use_front)
				   : DefaultAllocator().reallocate_downward(
						 old_p, old_size, new_size, in_use_back, in_use_front);
}

}  // namespace flatbuffers

#endif  // FLATBUFFERS_DEFAULT_ALLOCATOR_H_

/*** End of inlined file: default_allocator.h ***/

namespace flatbuffers {

// DetachedBuffer is a finished flatbuffer memory region, detached from its
// builder. The original memory region and allocator are also stored so that
// the DetachedBuffer can manage the memory lifetime.
class DetachedBuffer {
 public:
  DetachedBuffer()
	  : allocator_(nullptr),
		own_allocator_(false),
		buf_(nullptr),
		reserved_(0),
		cur_(nullptr),
		size_(0) {}

  DetachedBuffer(Allocator *allocator, bool own_allocator, uint8_t *buf,
				 size_t reserved, uint8_t *cur, size_t sz)
	  : allocator_(allocator),
		own_allocator_(own_allocator),
		buf_(buf),
		reserved_(reserved),
		cur_(cur),
		size_(sz) {}

  DetachedBuffer(DetachedBuffer &&other) noexcept
	  : allocator_(other.allocator_),
		own_allocator_(other.own_allocator_),
		buf_(other.buf_),
		reserved_(other.reserved_),
		cur_(other.cur_),
		size_(other.size_) {
	other.reset();
  }

  DetachedBuffer &operator=(DetachedBuffer &&other) noexcept {
	if (this == &other) return *this;

	destroy();

	allocator_ = other.allocator_;
	own_allocator_ = other.own_allocator_;
	buf_ = other.buf_;
	reserved_ = other.reserved_;
	cur_ = other.cur_;
	size_ = other.size_;

	other.reset();

	return *this;
  }

  ~DetachedBuffer() { destroy(); }

  const uint8_t *data() const { return cur_; }

  uint8_t *data() { return cur_; }

  size_t size() const { return size_; }

  // These may change access mode, leave these at end of public section
  FLATBUFFERS_DELETE_FUNC(DetachedBuffer(const DetachedBuffer &other));
  FLATBUFFERS_DELETE_FUNC(
	  DetachedBuffer &operator=(const DetachedBuffer &other));

 protected:
  Allocator *allocator_;
  bool own_allocator_;
  uint8_t *buf_;
  size_t reserved_;
  uint8_t *cur_;
  size_t size_;

  inline void destroy() {
	if (buf_) Deallocate(allocator_, buf_, reserved_);
	if (own_allocator_ && allocator_) { delete allocator_; }
	reset();
  }

  inline void reset() {
	allocator_ = nullptr;
	own_allocator_ = false;
	buf_ = nullptr;
	reserved_ = 0;
	cur_ = nullptr;
	size_ = 0;
  }
};

}  // namespace flatbuffers

#endif  // FLATBUFFERS_DETACHED_BUFFER_H_

/*** End of inlined file: detached_buffer.h ***/


/*** Start of inlined file: flatbuffer_builder.h ***/
#ifndef FLATBUFFERS_FLATBUFFER_BUILDER_H_
#define FLATBUFFERS_FLATBUFFER_BUILDER_H_

#include <algorithm>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <type_traits>


/*** Start of inlined file: string.h ***/
#ifndef FLATBUFFERS_STRING_H_
#define FLATBUFFERS_STRING_H_

namespace flatbuffers {

struct String : public Vector<char> {
  const char *c_str() const { return reinterpret_cast<const char *>(Data()); }
  std::string str() const { return std::string(c_str(), size()); }

  // clang-format off
  #ifdef FLATBUFFERS_HAS_STRING_VIEW
  flatbuffers::string_view string_view() const {
	return flatbuffers::string_view(c_str(), size());
  }

  /* implicit */
  operator flatbuffers::string_view() const {
	return flatbuffers::string_view(c_str(), size());
  }
  #endif // FLATBUFFERS_HAS_STRING_VIEW
  // clang-format on

  bool operator<(const String &o) const {
	return StringLessThan(this->data(), this->size(), o.data(), o.size());
  }
};

// Convenience function to get std::string from a String returning an empty
// string on null pointer.
static inline std::string GetString(const String *str) {
  return str ? str->str() : "";
}

// Convenience function to get char* from a String returning an empty string on
// null pointer.
static inline const char *GetCstring(const String *str) {
  return str ? str->c_str() : "";
}

#ifdef FLATBUFFERS_HAS_STRING_VIEW
// Convenience function to get string_view from a String returning an empty
// string_view on null pointer.
static inline flatbuffers::string_view GetStringView(const String *str) {
  return str ? str->string_view() : flatbuffers::string_view();
}
#endif  // FLATBUFFERS_HAS_STRING_VIEW

}  // namespace flatbuffers

#endif  // FLATBUFFERS_STRING_H_

/*** End of inlined file: string.h ***/


/*** Start of inlined file: struct.h ***/
#ifndef FLATBUFFERS_STRUCT_H_
#define FLATBUFFERS_STRUCT_H_

namespace flatbuffers {

// "structs" are flat structures that do not have an offset table, thus
// always have all members present and do not support forwards/backwards
// compatible extensions.

class Struct FLATBUFFERS_FINAL_CLASS {
 public:
  template<typename T> T GetField(uoffset_t o) const {
	return ReadScalar<T>(&data_[o]);
  }

  template<typename T> T GetStruct(uoffset_t o) const {
	return reinterpret_cast<T>(&data_[o]);
  }

  const uint8_t *GetAddressOf(uoffset_t o) const { return &data_[o]; }
  uint8_t *GetAddressOf(uoffset_t o) { return &data_[o]; }

 private:
  // private constructor & copy constructor: you obtain instances of this
  // class by pointing to existing data only
  Struct();
  Struct(const Struct &);
  Struct &operator=(const Struct &);

  uint8_t data_[1];
};

}  // namespace flatbuffers

#endif  // FLATBUFFERS_STRUCT_H_

/*** End of inlined file: struct.h ***/


/*** Start of inlined file: table.h ***/
#ifndef FLATBUFFERS_TABLE_H_
#define FLATBUFFERS_TABLE_H_

namespace flatbuffers {

// "tables" use an offset table (possibly shared) that allows fields to be
// omitted and added at will, but uses an extra indirection to read.
class Table {
 public:
  const uint8_t *GetVTable() const {
	return data_ - ReadScalar<soffset_t>(data_);
  }

  // This gets the field offset for any of the functions below it, or 0
  // if the field was not present.
  voffset_t GetOptionalFieldOffset(voffset_t field) const {
	// The vtable offset is always at the start.
	auto vtable = GetVTable();
	// The first element is the size of the vtable (fields + type id + itself).
	auto vtsize = ReadScalar<voffset_t>(vtable);
	// If the field we're accessing is outside the vtable, we're reading older
	// data, so it's the same as if the offset was 0 (not present).
	return field < vtsize ? ReadScalar<voffset_t>(vtable + field) : 0;
  }

  template<typename T> T GetField(voffset_t field, T defaultval) const {
	auto field_offset = GetOptionalFieldOffset(field);
	return field_offset ? ReadScalar<T>(data_ + field_offset) : defaultval;
  }

  template<typename P, typename OffsetSize = uoffset_t>
  P GetPointer(voffset_t field) {
	auto field_offset = GetOptionalFieldOffset(field);
	auto p = data_ + field_offset;
	return field_offset ? reinterpret_cast<P>(p + ReadScalar<OffsetSize>(p))
						: nullptr;
  }
  template<typename P, typename OffsetSize = uoffset_t>
  P GetPointer(voffset_t field) const {
	return const_cast<Table *>(this)->GetPointer<P, OffsetSize>(field);
  }

  template<typename P> P GetPointer64(voffset_t field) {
	return GetPointer<P, uoffset64_t>(field);
  }

  template<typename P> P GetPointer64(voffset_t field) const {
	return GetPointer<P, uoffset64_t>(field);
  }

  template<typename P> P GetStruct(voffset_t field) const {
	auto field_offset = GetOptionalFieldOffset(field);
	auto p = const_cast<uint8_t *>(data_ + field_offset);
	return field_offset ? reinterpret_cast<P>(p) : nullptr;
  }

  template<typename Raw, typename Face>
  flatbuffers::Optional<Face> GetOptional(voffset_t field) const {
	auto field_offset = GetOptionalFieldOffset(field);
	auto p = data_ + field_offset;
	return field_offset ? Optional<Face>(static_cast<Face>(ReadScalar<Raw>(p)))
						: Optional<Face>();
  }

  template<typename T> bool SetField(voffset_t field, T val, T def) {
	auto field_offset = GetOptionalFieldOffset(field);
	if (!field_offset) return IsTheSameAs(val, def);
	WriteScalar(data_ + field_offset, val);
	return true;
  }
  template<typename T> bool SetField(voffset_t field, T val) {
	auto field_offset = GetOptionalFieldOffset(field);
	if (!field_offset) return false;
	WriteScalar(data_ + field_offset, val);
	return true;
  }

  bool SetPointer(voffset_t field, const uint8_t *val) {
	auto field_offset = GetOptionalFieldOffset(field);
	if (!field_offset) return false;
	WriteScalar(data_ + field_offset,
				static_cast<uoffset_t>(val - (data_ + field_offset)));
	return true;
  }

  uint8_t *GetAddressOf(voffset_t field) {
	auto field_offset = GetOptionalFieldOffset(field);
	return field_offset ? data_ + field_offset : nullptr;
  }
  const uint8_t *GetAddressOf(voffset_t field) const {
	return const_cast<Table *>(this)->GetAddressOf(field);
  }

  bool CheckField(voffset_t field) const {
	return GetOptionalFieldOffset(field) != 0;
  }

  // Verify the vtable of this table.
  // Call this once per table, followed by VerifyField once per field.
  bool VerifyTableStart(Verifier &verifier) const {
	return verifier.VerifyTableStart(data_);
  }

  // Verify a particular field.
  template<typename T>
  bool VerifyField(const Verifier &verifier, voffset_t field,
				   size_t align) const {
	// Calling GetOptionalFieldOffset should be safe now thanks to
	// VerifyTable().
	auto field_offset = GetOptionalFieldOffset(field);
	// Check the actual field.
	return !field_offset || verifier.VerifyField<T>(data_, field_offset, align);
  }

  // VerifyField for required fields.
  template<typename T>
  bool VerifyFieldRequired(const Verifier &verifier, voffset_t field,
						   size_t align) const {
	auto field_offset = GetOptionalFieldOffset(field);
	return verifier.Check(field_offset != 0) &&
		   verifier.VerifyField<T>(data_, field_offset, align);
  }

  // Versions for offsets.
  template<typename OffsetT = uoffset_t>
  bool VerifyOffset(const Verifier &verifier, voffset_t field) const {
	auto field_offset = GetOptionalFieldOffset(field);
	return !field_offset || verifier.VerifyOffset<OffsetT>(data_, field_offset);
  }

  template<typename OffsetT = uoffset_t>
  bool VerifyOffsetRequired(const Verifier &verifier, voffset_t field) const {
	auto field_offset = GetOptionalFieldOffset(field);
	return verifier.Check(field_offset != 0) &&
		   verifier.VerifyOffset<OffsetT>(data_, field_offset);
  }

  bool VerifyOffset64(const Verifier &verifier, voffset_t field) const {
	return VerifyOffset<uoffset64_t>(verifier, field);
  }

  bool VerifyOffset64Required(const Verifier &verifier, voffset_t field) const {
	return VerifyOffsetRequired<uoffset64_t>(verifier, field);
  }

 private:
  // private constructor & copy constructor: you obtain instances of this
  // class by pointing to existing data only
  Table();
  Table(const Table &other);
  Table &operator=(const Table &);

  uint8_t data_[1];
};

// This specialization allows avoiding warnings like:
// MSVC C4800: type: forcing value to bool 'true' or 'false'.
template<>
inline flatbuffers::Optional<bool> Table::GetOptional<uint8_t, bool>(
	voffset_t field) const {
  auto field_offset = GetOptionalFieldOffset(field);
  auto p = data_ + field_offset;
  return field_offset ? Optional<bool>(ReadScalar<uint8_t>(p) != 0)
					  : Optional<bool>();
}

}  // namespace flatbuffers

#endif  // FLATBUFFERS_TABLE_H_

/*** End of inlined file: table.h ***/


/*** Start of inlined file: vector_downward.h ***/
#ifndef FLATBUFFERS_VECTOR_DOWNWARD_H_
#define FLATBUFFERS_VECTOR_DOWNWARD_H_

#include <algorithm>
#include <cstdint>

namespace flatbuffers {

// This is a minimal replication of std::vector<uint8_t> functionality,
// except growing from higher to lower addresses. i.e. push_back() inserts data
// in the lowest address in the vector.
// Since this vector leaves the lower part unused, we support a "scratch-pad"
// that can be stored there for temporary data, to share the allocated space.
// Essentially, this supports 2 std::vectors in a single buffer.
template<typename SizeT = uoffset_t> class vector_downward {
 public:
  explicit vector_downward(size_t initial_size, Allocator *allocator,
						   bool own_allocator, size_t buffer_minalign,
						   const SizeT max_size = FLATBUFFERS_MAX_BUFFER_SIZE)
	  : allocator_(allocator),
		own_allocator_(own_allocator),
		initial_size_(initial_size),
		max_size_(max_size),
		buffer_minalign_(buffer_minalign),
		reserved_(0),
		size_(0),
		buf_(nullptr),
		cur_(nullptr),
		scratch_(nullptr) {}

  vector_downward(vector_downward &&other) noexcept
	  // clang-format on
	  : allocator_(other.allocator_),
		own_allocator_(other.own_allocator_),
		initial_size_(other.initial_size_),
		max_size_(other.max_size_),
		buffer_minalign_(other.buffer_minalign_),
		reserved_(other.reserved_),
		size_(other.size_),
		buf_(other.buf_),
		cur_(other.cur_),
		scratch_(other.scratch_) {
	// No change in other.allocator_
	// No change in other.initial_size_
	// No change in other.buffer_minalign_
	other.own_allocator_ = false;
	other.reserved_ = 0;
	other.buf_ = nullptr;
	other.cur_ = nullptr;
	other.scratch_ = nullptr;
  }

  vector_downward &operator=(vector_downward &&other) noexcept {
	// Move construct a temporary and swap idiom
	vector_downward temp(std::move(other));
	swap(temp);
	return *this;
  }

  ~vector_downward() {
	clear_buffer();
	clear_allocator();
  }

  void reset() {
	clear_buffer();
	clear();
  }

  void clear() {
	if (buf_) {
	  cur_ = buf_ + reserved_;
	} else {
	  reserved_ = 0;
	  cur_ = nullptr;
	}
	size_ = 0;
	clear_scratch();
  }

  void clear_scratch() { scratch_ = buf_; }

  void clear_allocator() {
	if (own_allocator_ && allocator_) { delete allocator_; }
	allocator_ = nullptr;
	own_allocator_ = false;
  }

  void clear_buffer() {
	if (buf_) Deallocate(allocator_, buf_, reserved_);
	buf_ = nullptr;
  }

  // Relinquish the pointer to the caller.
  uint8_t *release_raw(size_t &allocated_bytes, size_t &offset) {
	auto *buf = buf_;
	allocated_bytes = reserved_;
	offset = vector_downward::offset();

	// release_raw only relinquishes the buffer ownership.
	// Does not deallocate or reset the allocator. Destructor will do that.
	buf_ = nullptr;
	clear();
	return buf;
  }

  // Relinquish the pointer to the caller.
  DetachedBuffer release() {
	// allocator ownership (if any) is transferred to DetachedBuffer.
	DetachedBuffer fb(allocator_, own_allocator_, buf_, reserved_, cur_,
					  size());
	if (own_allocator_) {
	  allocator_ = nullptr;
	  own_allocator_ = false;
	}
	buf_ = nullptr;
	clear();
	return fb;
  }

  size_t ensure_space(size_t len) {
	FLATBUFFERS_ASSERT(cur_ >= scratch_ && scratch_ >= buf_);
	// If the length is larger than the unused part of the buffer, we need to
	// grow.
	if (len > unused_buffer_size()) { reallocate(len); }
	FLATBUFFERS_ASSERT(size() < max_size_);
	return len;
  }

  inline uint8_t *make_space(size_t len) {
	if (len) {
	  ensure_space(len);
	  cur_ -= len;
	  size_ += static_cast<SizeT>(len);
	}
	return cur_;
  }

  // Returns nullptr if using the DefaultAllocator.
  Allocator *get_custom_allocator() { return allocator_; }

  // The current offset into the buffer.
  size_t offset() const { return cur_ - buf_; }

  // The total size of the vector (both the buffer and scratch parts).
  inline SizeT size() const { return size_; }

  // The size of the buffer part of the vector that is currently unused.
  SizeT unused_buffer_size() const {
	return static_cast<SizeT>(cur_ - scratch_);
  }

  // The size of the scratch part of the vector.
  SizeT scratch_size() const { return static_cast<SizeT>(scratch_ - buf_); }

  size_t capacity() const { return reserved_; }

  uint8_t *data() const {
	FLATBUFFERS_ASSERT(cur_);
	return cur_;
  }

  uint8_t *scratch_data() const {
	FLATBUFFERS_ASSERT(buf_);
	return buf_;
  }

  uint8_t *scratch_end() const {
	FLATBUFFERS_ASSERT(scratch_);
	return scratch_;
  }

  uint8_t *data_at(size_t offset) const { return buf_ + reserved_ - offset; }

  void push(const uint8_t *bytes, size_t num) {
	if (num > 0) { memcpy(make_space(num), bytes, num); }
  }

  // Specialized version of push() that avoids memcpy call for small data.
  template<typename T> void push_small(const T &little_endian_t) {
	make_space(sizeof(T));
	*reinterpret_cast<T *>(cur_) = little_endian_t;
  }

  template<typename T> void scratch_push_small(const T &t) {
	ensure_space(sizeof(T));
	*reinterpret_cast<T *>(scratch_) = t;
	scratch_ += sizeof(T);
  }

  // fill() is most frequently called with small byte counts (<= 4),
  // which is why we're using loops rather than calling memset.
  void fill(size_t zero_pad_bytes) {
	make_space(zero_pad_bytes);
	for (size_t i = 0; i < zero_pad_bytes; i++) cur_[i] = 0;
  }

  // Version for when we know the size is larger.
  // Precondition: zero_pad_bytes > 0
  void fill_big(size_t zero_pad_bytes) {
	memset(make_space(zero_pad_bytes), 0, zero_pad_bytes);
  }

  void pop(size_t bytes_to_remove) {
	cur_ += bytes_to_remove;
	size_ -= static_cast<SizeT>(bytes_to_remove);
  }

  void scratch_pop(size_t bytes_to_remove) { scratch_ -= bytes_to_remove; }

  void swap(vector_downward &other) {
	using std::swap;
	swap(allocator_, other.allocator_);
	swap(own_allocator_, other.own_allocator_);
	swap(initial_size_, other.initial_size_);
	swap(buffer_minalign_, other.buffer_minalign_);
	swap(reserved_, other.reserved_);
	swap(size_, other.size_);
	swap(max_size_, other.max_size_);
	swap(buf_, other.buf_);
	swap(cur_, other.cur_);
	swap(scratch_, other.scratch_);
  }

  void swap_allocator(vector_downward &other) {
	using std::swap;
	swap(allocator_, other.allocator_);
	swap(own_allocator_, other.own_allocator_);
  }

 private:
  // You shouldn't really be copying instances of this class.
  FLATBUFFERS_DELETE_FUNC(vector_downward(const vector_downward &));
  FLATBUFFERS_DELETE_FUNC(vector_downward &operator=(const vector_downward &));

  Allocator *allocator_;
  bool own_allocator_;
  size_t initial_size_;

  // The maximum size the vector can be.
  SizeT max_size_;
  size_t buffer_minalign_;
  size_t reserved_;
  SizeT size_;
  uint8_t *buf_;
  uint8_t *cur_;  // Points at location between empty (below) and used (above).
  uint8_t *scratch_;  // Points to the end of the scratchpad in use.

  void reallocate(size_t len) {
	auto old_reserved = reserved_;
	auto old_size = size();
	auto old_scratch_size = scratch_size();
	reserved_ +=
		(std::max)(len, old_reserved ? old_reserved / 2 : initial_size_);
	reserved_ = (reserved_ + buffer_minalign_ - 1) & ~(buffer_minalign_ - 1);
	if (buf_) {
	  buf_ = ReallocateDownward(allocator_, buf_, old_reserved, reserved_,
								old_size, old_scratch_size);
	} else {
	  buf_ = Allocate(allocator_, reserved_);
	}
	cur_ = buf_ + reserved_ - old_size;
	scratch_ = buf_ + old_scratch_size;
  }
};

}  // namespace flatbuffers

#endif  // FLATBUFFERS_VECTOR_DOWNWARD_H_

/*** End of inlined file: vector_downward.h ***/

namespace flatbuffers {

// Converts a Field ID to a virtual table offset.
inline voffset_t FieldIndexToOffset(voffset_t field_id) {
  // Should correspond to what EndTable() below builds up.
  const voffset_t fixed_fields =
	  2 * sizeof(voffset_t);  // Vtable size and Object Size.
  size_t offset = fixed_fields + field_id * sizeof(voffset_t);
  FLATBUFFERS_ASSERT(offset < std::numeric_limits<voffset_t>::max());
  return static_cast<voffset_t>(offset);
}

template<typename T, typename Alloc = std::allocator<T>>
const T *data(const std::vector<T, Alloc> &v) {
  // Eventually the returned pointer gets passed down to memcpy, so
  // we need it to be non-null to avoid undefined behavior.
  static uint8_t t;
  return v.empty() ? reinterpret_cast<const T *>(&t) : &v.front();
}
template<typename T, typename Alloc = std::allocator<T>>
T *data(std::vector<T, Alloc> &v) {
  // Eventually the returned pointer gets passed down to memcpy, so
  // we need it to be non-null to avoid undefined behavior.
  static uint8_t t;
  return v.empty() ? reinterpret_cast<T *>(&t) : &v.front();
}

/// @addtogroup flatbuffers_cpp_api
/// @{
/// @class FlatBufferBuilder
/// @brief Helper class to hold data needed in creation of a FlatBuffer.
/// To serialize data, you typically call one of the `Create*()` functions in
/// the generated code, which in turn call a sequence of `StartTable`/
/// `PushElement`/`AddElement`/`EndTable`, or the builtin `CreateString`/
/// `CreateVector` functions. Do this is depth-first order to build up a tree to
/// the root. `Finish()` wraps up the buffer ready for transport.
template<bool Is64Aware = false> class FlatBufferBuilderImpl {
 public:
  // This switches the size type of the builder, based on if its 64-bit aware
  // (uoffset64_t) or not (uoffset_t).
  typedef
	  typename std::conditional<Is64Aware, uoffset64_t, uoffset_t>::type SizeT;

  /// @brief Default constructor for FlatBufferBuilder.
  /// @param[in] initial_size The initial size of the buffer, in bytes. Defaults
  /// to `1024`.
  /// @param[in] allocator An `Allocator` to use. If null will use
  /// `DefaultAllocator`.
  /// @param[in] own_allocator Whether the builder/vector should own the
  /// allocator. Defaults to / `false`.
  /// @param[in] buffer_minalign Force the buffer to be aligned to the given
  /// minimum alignment upon reallocation. Only needed if you intend to store
  /// types with custom alignment AND you wish to read the buffer in-place
  /// directly after creation.
  explicit FlatBufferBuilderImpl(
	  size_t initial_size = 1024, Allocator *allocator = nullptr,
	  bool own_allocator = false,
	  size_t buffer_minalign = AlignOf<largest_scalar_t>())
	  : buf_(initial_size, allocator, own_allocator, buffer_minalign,
			 static_cast<SizeT>(Is64Aware ? FLATBUFFERS_MAX_64_BUFFER_SIZE
										  : FLATBUFFERS_MAX_BUFFER_SIZE)),
		num_field_loc(0),
		max_voffset_(0),
		length_of_64_bit_region_(0),
		nested(false),
		finished(false),
		minalign_(1),
		force_defaults_(false),
		dedup_vtables_(true),
		string_pool(nullptr) {
	EndianCheck();
  }

  /// @brief Move constructor for FlatBufferBuilder.
  FlatBufferBuilderImpl(FlatBufferBuilderImpl &&other) noexcept
	  : buf_(1024, nullptr, false, AlignOf<largest_scalar_t>(),
			 static_cast<SizeT>(Is64Aware ? FLATBUFFERS_MAX_64_BUFFER_SIZE
										  : FLATBUFFERS_MAX_BUFFER_SIZE)),
		num_field_loc(0),
		max_voffset_(0),
		length_of_64_bit_region_(0),
		nested(false),
		finished(false),
		minalign_(1),
		force_defaults_(false),
		dedup_vtables_(true),
		string_pool(nullptr) {
	EndianCheck();
	// Default construct and swap idiom.
	// Lack of delegating constructors in vs2010 makes it more verbose than
	// needed.
	Swap(other);
  }

  /// @brief Move assignment operator for FlatBufferBuilder.
  FlatBufferBuilderImpl &operator=(FlatBufferBuilderImpl &&other) noexcept {
	// Move construct a temporary and swap idiom
	FlatBufferBuilderImpl temp(std::move(other));
	Swap(temp);
	return *this;
  }

  void Swap(FlatBufferBuilderImpl &other) {
	using std::swap;
	buf_.swap(other.buf_);
	swap(num_field_loc, other.num_field_loc);
	swap(max_voffset_, other.max_voffset_);
	swap(length_of_64_bit_region_, other.length_of_64_bit_region_);
	swap(nested, other.nested);
	swap(finished, other.finished);
	swap(minalign_, other.minalign_);
	swap(force_defaults_, other.force_defaults_);
	swap(dedup_vtables_, other.dedup_vtables_);
	swap(string_pool, other.string_pool);
  }

  ~FlatBufferBuilderImpl() {
	if (string_pool) delete string_pool;
  }

  void Reset() {
	Clear();       // clear builder state
	buf_.reset();  // deallocate buffer
  }

  /// @brief Reset all the state in this FlatBufferBuilder so it can be reused
  /// to construct another buffer.
  void Clear() {
	ClearOffsets();
	buf_.clear();
	nested = false;
	finished = false;
	minalign_ = 1;
	length_of_64_bit_region_ = 0;
	if (string_pool) string_pool->clear();
  }

  /// @brief The current size of the serialized buffer, counting from the end.
  /// @return Returns an `SizeT` with the current size of the buffer.
  SizeT GetSize() const { return buf_.size(); }

  /// @brief The current size of the serialized buffer relative to the end of
  /// the 32-bit region.
  /// @return Returns an `uoffset_t` with the current size of the buffer.
  template<bool is_64 = Is64Aware>
  // Only enable this method for the 64-bit builder, as only that builder is
  // concerned with the 32/64-bit boundary, and should be the one to bare any
  // run time costs.
  typename std::enable_if<is_64, uoffset_t>::type GetSizeRelative32BitRegion()
	  const {
	//[32-bit region][64-bit region]
	//         [XXXXXXXXXXXXXXXXXXX] GetSize()
	//               [YYYYYYYYYYYYY] length_of_64_bit_region_
	//         [ZZZZ]                return size
	return static_cast<uoffset_t>(GetSize() - length_of_64_bit_region_);
  }

  template<bool is_64 = Is64Aware>
  // Only enable this method for the 32-bit builder.
  typename std::enable_if<!is_64, uoffset_t>::type GetSizeRelative32BitRegion()
	  const {
	return static_cast<uoffset_t>(GetSize());
  }

  /// @brief Get the serialized buffer (after you call `Finish()`).
  /// @return Returns an `uint8_t` pointer to the FlatBuffer data inside the
  /// buffer.
  uint8_t *GetBufferPointer() const {
	Finished();
	return buf_.data();
  }

  /// @brief Get the serialized buffer (after you call `Finish()`) as a span.
  /// @return Returns a constructed flatbuffers::span that is a view over the
  /// FlatBuffer data inside the buffer.
  flatbuffers::span<uint8_t> GetBufferSpan() const {
	Finished();
	return flatbuffers::span<uint8_t>(buf_.data(), buf_.size());
  }

  /// @brief Get a pointer to an unfinished buffer.
  /// @return Returns a `uint8_t` pointer to the unfinished buffer.
  uint8_t *GetCurrentBufferPointer() const { return buf_.data(); }

  /// @brief Get the released DetachedBuffer.
  /// @return A `DetachedBuffer` that owns the buffer and its allocator.
  DetachedBuffer Release() {
	Finished();
	DetachedBuffer buffer = buf_.release();
	Clear();
	return buffer;
  }

  /// @brief Get the released pointer to the serialized buffer.
  /// @param size The size of the memory block containing
  /// the serialized `FlatBuffer`.
  /// @param offset The offset from the released pointer where the finished
  /// `FlatBuffer` starts.
  /// @return A raw pointer to the start of the memory block containing
  /// the serialized `FlatBuffer`.
  /// @remark If the allocator is owned, it gets deleted when the destructor is
  /// called.
  uint8_t *ReleaseRaw(size_t &size, size_t &offset) {
	Finished();
	uint8_t *raw = buf_.release_raw(size, offset);
	Clear();
	return raw;
  }

  /// @brief get the minimum alignment this buffer needs to be accessed
  /// properly. This is only known once all elements have been written (after
  /// you call Finish()). You can use this information if you need to embed
  /// a FlatBuffer in some other buffer, such that you can later read it
  /// without first having to copy it into its own buffer.
  size_t GetBufferMinAlignment() const {
	Finished();
	return minalign_;
  }

  /// @cond FLATBUFFERS_INTERNAL
  void Finished() const {
	// If you get this assert, you're attempting to get access a buffer
	// which hasn't been finished yet. Be sure to call
	// FlatBufferBuilder::Finish with your root table.
	// If you really need to access an unfinished buffer, call
	// GetCurrentBufferPointer instead.
	FLATBUFFERS_ASSERT(finished);
  }
  /// @endcond

  /// @brief In order to save space, fields that are set to their default value
  /// don't get serialized into the buffer.
  /// @param[in] fd When set to `true`, always serializes default values that
  /// are set. Optional fields which are not set explicitly, will still not be
  /// serialized.
  void ForceDefaults(bool fd) { force_defaults_ = fd; }

  /// @brief By default vtables are deduped in order to save space.
  /// @param[in] dedup When set to `true`, dedup vtables.
  void DedupVtables(bool dedup) { dedup_vtables_ = dedup; }

  /// @cond FLATBUFFERS_INTERNAL
  void Pad(size_t num_bytes) { buf_.fill(num_bytes); }

  void TrackMinAlign(size_t elem_size) {
	if (elem_size > minalign_) minalign_ = elem_size;
  }

  void Align(size_t elem_size) {
	TrackMinAlign(elem_size);
	buf_.fill(PaddingBytes(buf_.size(), elem_size));
  }

  void PushFlatBuffer(const uint8_t *bytes, size_t size) {
	PushBytes(bytes, size);
	finished = true;
  }

  void PushBytes(const uint8_t *bytes, size_t size) { buf_.push(bytes, size); }

  void PopBytes(size_t amount) { buf_.pop(amount); }

  template<typename T> void AssertScalarT() {
	// The code assumes power of 2 sizes and endian-swap-ability.
	static_assert(flatbuffers::is_scalar<T>::value, "T must be a scalar type");
  }

  // Write a single aligned scalar to the buffer
  template<typename T, typename ReturnT = uoffset_t>
  ReturnT PushElement(T element) {
	AssertScalarT<T>();
	Align(sizeof(T));
	buf_.push_small(EndianScalar(element));
	return CalculateOffset<ReturnT>();
  }

  template<typename T, template<typename> class OffsetT = Offset>
  uoffset_t PushElement(OffsetT<T> off) {
	// Special case for offsets: see ReferTo below.
	return PushElement(ReferTo(off.o));
  }

  // When writing fields, we track where they are, so we can create correct
  // vtables later.
  void TrackField(voffset_t field, uoffset_t off) {
	FieldLoc fl = { off, field };
	buf_.scratch_push_small(fl);
	num_field_loc++;
	if (field > max_voffset_) { max_voffset_ = field; }
  }

  // Like PushElement, but additionally tracks the field this represents.
  template<typename T> void AddElement(voffset_t field, T e, T def) {
	// We don't serialize values equal to the default.
	if (IsTheSameAs(e, def) && !force_defaults_) return;
	TrackField(field, PushElement(e));
  }

  template<typename T> void AddElement(voffset_t field, T e) {
	TrackField(field, PushElement(e));
  }

  template<typename T> void AddOffset(voffset_t field, Offset<T> off) {
	if (off.IsNull()) return;  // Don't store.
	AddElement(field, ReferTo(off.o), static_cast<uoffset_t>(0));
  }

  template<typename T> void AddOffset(voffset_t field, Offset64<T> off) {
	if (off.IsNull()) return;  // Don't store.
	AddElement(field, ReferTo(off.o), static_cast<uoffset64_t>(0));
  }

  template<typename T> void AddStruct(voffset_t field, const T *structptr) {
	if (!structptr) return;  // Default, don't store.
	Align(AlignOf<T>());
	buf_.push_small(*structptr);
	TrackField(field, CalculateOffset<uoffset_t>());
  }

  void AddStructOffset(voffset_t field, uoffset_t off) {
	TrackField(field, off);
  }

  // Offsets initially are relative to the end of the buffer (downwards).
  // This function converts them to be relative to the current location
  // in the buffer (when stored here), pointing upwards.
  uoffset_t ReferTo(uoffset_t off) {
	// Align to ensure GetSizeRelative32BitRegion() below is correct.
	Align(sizeof(uoffset_t));
	// 32-bit offsets are relative to the tail of the 32-bit region of the
	// buffer. For most cases (without 64-bit entities) this is equivalent to
	// size of the whole buffer (e.g. GetSize())
	return ReferTo(off, GetSizeRelative32BitRegion());
  }

  uoffset64_t ReferTo(uoffset64_t off) {
	// Align to ensure GetSize() below is correct.
	Align(sizeof(uoffset64_t));
	// 64-bit offsets are relative to tail of the whole buffer
	return ReferTo(off, GetSize());
  }

  template<typename T, typename T2> T ReferTo(const T off, const T2 size) {
	FLATBUFFERS_ASSERT(off && off <= size);
	return size - off + static_cast<T>(sizeof(T));
  }

  template<typename T> T ReferTo(const T off, const T size) {
	FLATBUFFERS_ASSERT(off && off <= size);
	return size - off + static_cast<T>(sizeof(T));
  }

  void NotNested() {
	// If you hit this, you're trying to construct a Table/Vector/String
	// during the construction of its parent table (between the MyTableBuilder
	// and table.Finish().
	// Move the creation of these sub-objects to above the MyTableBuilder to
	// not get this assert.
	// Ignoring this assert may appear to work in simple cases, but the reason
	// it is here is that storing objects in-line may cause vtable offsets
	// to not fit anymore. It also leads to vtable duplication.
	FLATBUFFERS_ASSERT(!nested);
	// If you hit this, fields were added outside the scope of a table.
	FLATBUFFERS_ASSERT(!num_field_loc);
  }

  // From generated code (or from the parser), we call StartTable/EndTable
  // with a sequence of AddElement calls in between.
  uoffset_t StartTable() {
	NotNested();
	nested = true;
	return GetSizeRelative32BitRegion();
  }

  // This finishes one serialized object by generating the vtable if it's a
  // table, comparing it against existing vtables, and writing the
  // resulting vtable offset.
  uoffset_t EndTable(uoffset_t start) {
	// If you get this assert, a corresponding StartTable wasn't called.
	FLATBUFFERS_ASSERT(nested);
	// Write the vtable offset, which is the start of any Table.
	// We fill its value later.
	// This is relative to the end of the 32-bit region.
	const uoffset_t vtable_offset_loc =
		static_cast<uoffset_t>(PushElement<soffset_t>(0));
	// Write a vtable, which consists entirely of voffset_t elements.
	// It starts with the number of offsets, followed by a type id, followed
	// by the offsets themselves. In reverse:
	// Include space for the last offset and ensure empty tables have a
	// minimum size.
	max_voffset_ =
		(std::max)(static_cast<voffset_t>(max_voffset_ + sizeof(voffset_t)),
				   FieldIndexToOffset(0));
	buf_.fill_big(max_voffset_);
	const uoffset_t table_object_size = vtable_offset_loc - start;
	// Vtable use 16bit offsets.
	FLATBUFFERS_ASSERT(table_object_size < 0x10000);
	WriteScalar<voffset_t>(buf_.data() + sizeof(voffset_t),
						   static_cast<voffset_t>(table_object_size));
	WriteScalar<voffset_t>(buf_.data(), max_voffset_);
	// Write the offsets into the table
	for (auto it = buf_.scratch_end() - num_field_loc * sizeof(FieldLoc);
		 it < buf_.scratch_end(); it += sizeof(FieldLoc)) {
	  auto field_location = reinterpret_cast<FieldLoc *>(it);
	  const voffset_t pos =
		  static_cast<voffset_t>(vtable_offset_loc - field_location->off);
	  // If this asserts, it means you've set a field twice.
	  FLATBUFFERS_ASSERT(
		  !ReadScalar<voffset_t>(buf_.data() + field_location->id));
	  WriteScalar<voffset_t>(buf_.data() + field_location->id, pos);
	}
	ClearOffsets();
	auto vt1 = reinterpret_cast<voffset_t *>(buf_.data());
	auto vt1_size = ReadScalar<voffset_t>(vt1);
	auto vt_use = GetSizeRelative32BitRegion();
	// See if we already have generated a vtable with this exact same
	// layout before. If so, make it point to the old one, remove this one.
	if (dedup_vtables_) {
	  for (auto it = buf_.scratch_data(); it < buf_.scratch_end();
		   it += sizeof(uoffset_t)) {
		auto vt_offset_ptr = reinterpret_cast<uoffset_t *>(it);
		auto vt2 = reinterpret_cast<voffset_t *>(buf_.data_at(*vt_offset_ptr));
		auto vt2_size = ReadScalar<voffset_t>(vt2);
		if (vt1_size != vt2_size || 0 != memcmp(vt2, vt1, vt1_size)) continue;
		vt_use = *vt_offset_ptr;
		buf_.pop(GetSizeRelative32BitRegion() - vtable_offset_loc);
		break;
	  }
	}
	// If this is a new vtable, remember it.
	if (vt_use == GetSizeRelative32BitRegion()) {
	  buf_.scratch_push_small(vt_use);
	}
	// Fill the vtable offset we created above.
	// The offset points from the beginning of the object to where the vtable is
	// stored.
	// Offsets default direction is downward in memory for future format
	// flexibility (storing all vtables at the start of the file).
	WriteScalar(buf_.data_at(vtable_offset_loc + length_of_64_bit_region_),
				static_cast<soffset_t>(vt_use) -
					static_cast<soffset_t>(vtable_offset_loc));
	nested = false;
	return vtable_offset_loc;
  }

  FLATBUFFERS_ATTRIBUTE([[deprecated("call the version above instead")]])
  uoffset_t EndTable(uoffset_t start, voffset_t /*numfields*/) {
	return EndTable(start);
  }

  // This checks a required field has been set in a given table that has
  // just been constructed.
  template<typename T> void Required(Offset<T> table, voffset_t field) {
	auto table_ptr = reinterpret_cast<const Table *>(buf_.data_at(table.o));
	bool ok = table_ptr->GetOptionalFieldOffset(field) != 0;
	// If this fails, the caller will show what field needs to be set.
	FLATBUFFERS_ASSERT(ok);
	(void)ok;
  }

  uoffset_t StartStruct(size_t alignment) {
	Align(alignment);
	return GetSizeRelative32BitRegion();
  }

  uoffset_t EndStruct() { return GetSizeRelative32BitRegion(); }

  void ClearOffsets() {
	buf_.scratch_pop(num_field_loc * sizeof(FieldLoc));
	num_field_loc = 0;
	max_voffset_ = 0;
  }

  // Aligns such that when "len" bytes are written, an object can be written
  // after it (forward in the buffer) with "alignment" without padding.
  void PreAlign(size_t len, size_t alignment) {
	if (len == 0) return;
	TrackMinAlign(alignment);
	buf_.fill(PaddingBytes(GetSize() + len, alignment));
  }

  // Aligns such than when "len" bytes are written, an object of type `AlignT`
  // can be written after it (forward in the buffer) without padding.
  template<typename AlignT> void PreAlign(size_t len) {
	AssertScalarT<AlignT>();
	PreAlign(len, AlignOf<AlignT>());
  }
  /// @endcond

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// @param[in] str A const char pointer to the data to be stored as a string.
  /// @param[in] len The number of bytes that should be stored from `str`.
  /// @return Returns the offset in the buffer where the string starts.
  template<template<typename> class OffsetT = Offset>
  OffsetT<String> CreateString(const char *str, size_t len) {
	CreateStringImpl(str, len);
	return OffsetT<String>(
		CalculateOffset<typename OffsetT<String>::offset_type>());
  }

  /// @brief Store a string in the buffer, which is null-terminated.
  /// @param[in] str A const char pointer to a C-string to add to the buffer.
  /// @return Returns the offset in the buffer where the string starts.
  template<template<typename> class OffsetT = Offset>
  OffsetT<String> CreateString(const char *str) {
	return CreateString<OffsetT>(str, strlen(str));
  }

  /// @brief Store a string in the buffer, which is null-terminated.
  /// @param[in] str A char pointer to a C-string to add to the buffer.
  /// @return Returns the offset in the buffer where the string starts.
  template<template<typename> class OffsetT = Offset>
  OffsetT<String> CreateString(char *str) {
	return CreateString<OffsetT>(str, strlen(str));
  }

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// @param[in] str A const reference to a std::string to store in the buffer.
  /// @return Returns the offset in the buffer where the string starts.
  template<template<typename> class OffsetT = Offset>
  OffsetT<String> CreateString(const std::string &str) {
	return CreateString<OffsetT>(str.c_str(), str.length());
  }

  // clang-format off
  #ifdef FLATBUFFERS_HAS_STRING_VIEW
  /// @brief Store a string in the buffer, which can contain any binary data.
  /// @param[in] str A const string_view to copy in to the buffer.
  /// @return Returns the offset in the buffer where the string starts.
  template<template <typename> class OffsetT = Offset>
  OffsetT<String>CreateString(flatbuffers::string_view str) {
	return CreateString<OffsetT>(str.data(), str.size());
  }
  #endif // FLATBUFFERS_HAS_STRING_VIEW
  // clang-format on

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// @param[in] str A const pointer to a `String` struct to add to the buffer.
  /// @return Returns the offset in the buffer where the string starts
  template<template<typename> class OffsetT = Offset>
  OffsetT<String> CreateString(const String *str) {
	return str ? CreateString<OffsetT>(str->c_str(), str->size()) : 0;
  }

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// @param[in] str A const reference to a std::string like type with support
  /// of T::data() and T::length() to store in the buffer.
  /// @return Returns the offset in the buffer where the string starts.
  template<template<typename> class OffsetT = Offset,
		   // No need to explicitly declare the T type, let the compiler deduce
		   // it.
		   int &...ExplicitArgumentBarrier, typename T>
  OffsetT<String> CreateString(const T &str) {
	return CreateString<OffsetT>(str.data(), str.length());
  }

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// If a string with this exact contents has already been serialized before,
  /// instead simply returns the offset of the existing string. This uses a map
  /// stored on the heap, but only stores the numerical offsets.
  /// @param[in] str A const char pointer to the data to be stored as a string.
  /// @param[in] len The number of bytes that should be stored from `str`.
  /// @return Returns the offset in the buffer where the string starts.
  Offset<String> CreateSharedString(const char *str, size_t len) {
	FLATBUFFERS_ASSERT(FLATBUFFERS_GENERAL_HEAP_ALLOC_OK);
	if (!string_pool) {
	  string_pool = new StringOffsetMap(StringOffsetCompare(buf_));
	}

	const size_t size_before_string = buf_.size();
	// Must first serialize the string, since the set is all offsets into
	// buffer.
	const Offset<String> off = CreateString<Offset>(str, len);
	auto it = string_pool->find(off);
	// If it exists we reuse existing serialized data!
	if (it != string_pool->end()) {
	  // We can remove the string we serialized.
	  buf_.pop(buf_.size() - size_before_string);
	  return *it;
	}
	// Record this string for future use.
	string_pool->insert(off);
	return off;
  }

#ifdef FLATBUFFERS_HAS_STRING_VIEW
  /// @brief Store a string in the buffer, which can contain any binary data.
  /// If a string with this exact contents has already been serialized before,
  /// instead simply returns the offset of the existing string. This uses a map
  /// stored on the heap, but only stores the numerical offsets.
  /// @param[in] str A const std::string_view to store in the buffer.
  /// @return Returns the offset in the buffer where the string starts
  Offset<String> CreateSharedString(const flatbuffers::string_view str) {
	return CreateSharedString(str.data(), str.size());
  }
#else
  /// @brief Store a string in the buffer, which null-terminated.
  /// If a string with this exact contents has already been serialized before,
  /// instead simply returns the offset of the existing string. This uses a map
  /// stored on the heap, but only stores the numerical offsets.
  /// @param[in] str A const char pointer to a C-string to add to the buffer.
  /// @return Returns the offset in the buffer where the string starts.
  Offset<String> CreateSharedString(const char *str) {
	return CreateSharedString(str, strlen(str));
  }

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// If a string with this exact contents has already been serialized before,
  /// instead simply returns the offset of the existing string. This uses a map
  /// stored on the heap, but only stores the numerical offsets.
  /// @param[in] str A const reference to a std::string to store in the buffer.
  /// @return Returns the offset in the buffer where the string starts.
  Offset<String> CreateSharedString(const std::string &str) {
	return CreateSharedString(str.c_str(), str.length());
  }
#endif

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// If a string with this exact contents has already been serialized before,
  /// instead simply returns the offset of the existing string. This uses a map
  /// stored on the heap, but only stores the numerical offsets.
  /// @param[in] str A const pointer to a `String` struct to add to the buffer.
  /// @return Returns the offset in the buffer where the string starts
  Offset<String> CreateSharedString(const String *str) {
	return str ? CreateSharedString(str->c_str(), str->size()) : 0;
  }

  /// @cond FLATBUFFERS_INTERNAL
  template<typename LenT = uoffset_t, typename ReturnT = uoffset_t>
  ReturnT EndVector(size_t len) {
	FLATBUFFERS_ASSERT(nested);  // Hit if no corresponding StartVector.
	nested = false;
	return PushElement<LenT, ReturnT>(static_cast<LenT>(len));
  }

  template<template<typename> class OffsetT = Offset, typename LenT = uint32_t>
  void StartVector(size_t len, size_t elemsize, size_t alignment) {
	NotNested();
	nested = true;
	// Align to the Length type of the vector (either 32-bit or 64-bit), so
	// that the length of the buffer can be added without padding.
	PreAlign<LenT>(len * elemsize);
	PreAlign(len * elemsize, alignment);  // Just in case elemsize > uoffset_t.
  }

  template<typename T, template<typename> class OffsetT = Offset,
		   typename LenT = uint32_t>
  void StartVector(size_t len) {
	return StartVector<OffsetT, LenT>(len, sizeof(T), AlignOf<T>());
  }

  // Call this right before StartVector/CreateVector if you want to force the
  // alignment to be something different than what the element size would
  // normally dictate.
  // This is useful when storing a nested_flatbuffer in a vector of bytes,
  // or when storing SIMD floats, etc.
  void ForceVectorAlignment(const size_t len, const size_t elemsize,
							const size_t alignment) {
	if (len == 0) return;
	FLATBUFFERS_ASSERT(VerifyAlignmentRequirements(alignment));
	PreAlign(len * elemsize, alignment);
  }

  template<bool is_64 = Is64Aware>
  typename std::enable_if<is_64, void>::type ForceVectorAlignment64(
	  const size_t len, const size_t elemsize, const size_t alignment) {
	// If you hit this assertion, you are trying to force alignment on a
	// vector with offset64 after serializing a 32-bit offset.
	FLATBUFFERS_ASSERT(GetSize() == length_of_64_bit_region_);

	// Call through.
	ForceVectorAlignment(len, elemsize, alignment);

	// Update the 64 bit region.
	length_of_64_bit_region_ = GetSize();
  }

  // Similar to ForceVectorAlignment but for String fields.
  void ForceStringAlignment(size_t len, size_t alignment) {
	if (len == 0) return;
	FLATBUFFERS_ASSERT(VerifyAlignmentRequirements(alignment));
	PreAlign((len + 1) * sizeof(char), alignment);
  }

  /// @endcond

  /// @brief Serialize an array into a FlatBuffer `vector`.
  /// @tparam T The data type of the array elements.
  /// @tparam OffsetT the type of offset to return
  /// @tparam VectorT the type of vector to cast to.
  /// @param[in] v A pointer to the array of type `T` to serialize into the
  /// buffer as a `vector`.
  /// @param[in] len The number of elements to serialize.
  /// @return Returns a typed `TOffset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, template<typename...> class OffsetT = Offset,
		   template<typename...> class VectorT = Vector>
  OffsetT<VectorT<T>> CreateVector(const T *v, size_t len) {
	// The type of the length field in the vector.
	typedef typename VectorT<T>::size_type LenT;
	typedef typename OffsetT<VectorT<T>>::offset_type offset_type;
	// If this assert hits, you're specifying a template argument that is
	// causing the wrong overload to be selected, remove it.
	AssertScalarT<T>();
	StartVector<T, OffsetT, LenT>(len);
	if (len > 0) {
	  // clang-format off
	  #if FLATBUFFERS_LITTLEENDIAN
		PushBytes(reinterpret_cast<const uint8_t *>(v), len * sizeof(T));
	  #else
		if (sizeof(T) == 1) {
		  PushBytes(reinterpret_cast<const uint8_t *>(v), len);
		} else {
		  for (auto i = len; i > 0; ) {
			PushElement(v[--i]);
		  }
		}
	  #endif
	  // clang-format on
	}
	return OffsetT<VectorT<T>>(EndVector<LenT, offset_type>(len));
  }

  /// @brief Serialize an array like object into a FlatBuffer `vector`.
  /// @tparam T The data type of the array elements.
  /// @tparam C The type of the array.
  /// @param[in] array A reference to an array like object of type `T` to
  /// serialize into the buffer as a `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, class C> Offset<Vector<T>> CreateVector(const C &array) {
	return CreateVector(array.data(), array.size());
  }

  /// @brief Serialize an initializer list into a FlatBuffer `vector`.
  /// @tparam T The data type of the initializer list elements.
  /// @param[in] v The value of the initializer list.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T>
  Offset<Vector<T>> CreateVector(std::initializer_list<T> v) {
	return CreateVector(v.begin(), v.size());
  }

  template<typename T>
  Offset<Vector<Offset<T>>> CreateVector(const Offset<T> *v, size_t len) {
	StartVector<Offset<T>>(len);
	for (auto i = len; i > 0;) { PushElement(v[--i]); }
	return Offset<Vector<Offset<T>>>(EndVector(len));
  }

  /// @brief Serialize a `std::vector` into a FlatBuffer `vector`.
  /// @tparam T The data type of the `std::vector` elements.
  /// @param v A const reference to the `std::vector` to serialize into the
  /// buffer as a `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename Alloc = std::allocator<T>>
  Offset<Vector<T>> CreateVector(const std::vector<T, Alloc> &v) {
	return CreateVector(data(v), v.size());
  }

  template<template<typename...> class VectorT = Vector64,
		   int &...ExplicitArgumentBarrier, typename T>
  Offset64<VectorT<T>> CreateVector64(const std::vector<T> &v) {
	return CreateVector<T, Offset64, VectorT>(data(v), v.size());
  }

  // vector<bool> may be implemented using a bit-set, so we can't access it as
  // an array. Instead, read elements manually.
  // Background: https://isocpp.org/blog/2012/11/on-vectorbool
  Offset<Vector<uint8_t>> CreateVector(const std::vector<bool> &v) {
	StartVector<uint8_t>(v.size());
	for (auto i = v.size(); i > 0;) {
	  PushElement(static_cast<uint8_t>(v[--i]));
	}
	return Offset<Vector<uint8_t>>(EndVector(v.size()));
  }

  /// @brief Serialize values returned by a function into a FlatBuffer `vector`.
  /// This is a convenience function that takes care of iteration for you.
  /// @tparam T The data type of the `std::vector` elements.
  /// @param f A function that takes the current iteration 0..vector_size-1 and
  /// returns any type that you can construct a FlatBuffers vector out of.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T>
  Offset<Vector<T>> CreateVector(size_t vector_size,
								 const std::function<T(size_t i)> &f) {
	FLATBUFFERS_ASSERT(FLATBUFFERS_GENERAL_HEAP_ALLOC_OK);
	std::vector<T> elems(vector_size);
	for (size_t i = 0; i < vector_size; i++) elems[i] = f(i);
	return CreateVector(elems);
  }

  /// @brief Serialize values returned by a function into a FlatBuffer `vector`.
  /// This is a convenience function that takes care of iteration for you. This
  /// uses a vector stored on the heap to store the intermediate results of the
  /// iteration.
  /// @tparam T The data type of the `std::vector` elements.
  /// @param f A function that takes the current iteration 0..vector_size-1,
  /// and the state parameter returning any type that you can construct a
  /// FlatBuffers vector out of.
  /// @param state State passed to f.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename F, typename S>
  Offset<Vector<T>> CreateVector(size_t vector_size, F f, S *state) {
	FLATBUFFERS_ASSERT(FLATBUFFERS_GENERAL_HEAP_ALLOC_OK);
	std::vector<T> elems(vector_size);
	for (size_t i = 0; i < vector_size; i++) elems[i] = f(i, state);
	return CreateVector(elems);
  }

  /// @brief Serialize a `std::vector<StringType>` into a FlatBuffer `vector`.
  /// whereas StringType is any type that is accepted by the CreateString()
  /// overloads.
  /// This is a convenience function for a common case.
  /// @param v A const reference to the `std::vector` to serialize into the
  /// buffer as a `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename StringType = std::string,
		   typename Alloc = std::allocator<StringType>>
  Offset<Vector<Offset<String>>> CreateVectorOfStrings(
	  const std::vector<StringType, Alloc> &v) {
	return CreateVectorOfStrings(v.cbegin(), v.cend());
  }

  /// @brief Serialize a collection of Strings into a FlatBuffer `vector`.
  /// This is a convenience function for a common case.
  /// @param begin The beginning iterator of the collection
  /// @param end The ending iterator of the collection
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<class It>
  Offset<Vector<Offset<String>>> CreateVectorOfStrings(It begin, It end) {
	auto distance = std::distance(begin, end);
	FLATBUFFERS_ASSERT(distance >= 0);
	auto size = static_cast<size_t>(distance);
	auto scratch_buffer_usage = size * sizeof(Offset<String>);
	// If there is not enough space to store the offsets, there definitely won't
	// be enough space to store all the strings. So ensuring space for the
	// scratch region is OK, for if it fails, it would have failed later.
	buf_.ensure_space(scratch_buffer_usage);
	for (auto it = begin; it != end; ++it) {
	  buf_.scratch_push_small(CreateString(*it));
	}
	StartVector<Offset<String>>(size);
	for (size_t i = 1; i <= size; i++) {
	  // Note we re-evaluate the buf location each iteration to account for any
	  // underlying buffer resizing that may occur.
	  PushElement(*reinterpret_cast<Offset<String> *>(
		  buf_.scratch_end() - i * sizeof(Offset<String>)));
	}
	buf_.scratch_pop(scratch_buffer_usage);
	return Offset<Vector<Offset<String>>>(EndVector(size));
  }

  /// @brief Serialize an array of structs into a FlatBuffer `vector`.
  /// @tparam T The data type of the struct array elements.
  /// @param[in] v A pointer to the array of type `T` to serialize into the
  /// buffer as a `vector`.
  /// @param[in] len The number of elements to serialize.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, template<typename...> class OffsetT = Offset,
		   template<typename...> class VectorT = Vector>
  OffsetT<VectorT<const T *>> CreateVectorOfStructs(const T *v, size_t len) {
	// The type of the length field in the vector.
	typedef typename VectorT<T>::size_type LenT;
	typedef typename OffsetT<VectorT<const T *>>::offset_type offset_type;

	StartVector<OffsetT, LenT>(len, sizeof(T), AlignOf<T>());
	if (len > 0) {
	  PushBytes(reinterpret_cast<const uint8_t *>(v), sizeof(T) * len);
	}
	return OffsetT<VectorT<const T *>>(EndVector<LenT, offset_type>(len));
  }

  /// @brief Serialize an array of structs into a FlatBuffer `vector`.
  /// @tparam T The data type of the struct array elements.
  /// @param[in] filler A function that takes the current iteration
  /// 0..vector_size-1 and a pointer to the struct that must be filled.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  /// This is mostly useful when flatbuffers are generated with mutation
  /// accessors.
  template<typename T>
  Offset<Vector<const T *>> CreateVectorOfStructs(
	  size_t vector_size, const std::function<void(size_t i, T *)> &filler) {
	T *structs = StartVectorOfStructs<T>(vector_size);
	for (size_t i = 0; i < vector_size; i++) {
	  filler(i, structs);
	  structs++;
	}
	return EndVectorOfStructs<T>(vector_size);
  }

  /// @brief Serialize an array of structs into a FlatBuffer `vector`.
  /// @tparam T The data type of the struct array elements.
  /// @param[in] f A function that takes the current iteration 0..vector_size-1,
  /// a pointer to the struct that must be filled and the state argument.
  /// @param[in] state Arbitrary state to pass to f.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  /// This is mostly useful when flatbuffers are generated with mutation
  /// accessors.
  template<typename T, typename F, typename S>
  Offset<Vector<const T *>> CreateVectorOfStructs(size_t vector_size, F f,
												  S *state) {
	T *structs = StartVectorOfStructs<T>(vector_size);
	for (size_t i = 0; i < vector_size; i++) {
	  f(i, structs, state);
	  structs++;
	}
	return EndVectorOfStructs<T>(vector_size);
  }

  /// @brief Serialize a `std::vector` of structs into a FlatBuffer `vector`.
  /// @tparam T The data type of the `std::vector` struct elements.
  /// @param[in] v A const reference to the `std::vector` of structs to
  /// serialize into the buffer as a `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, template<typename...> class OffsetT = Offset,
		   template<typename...> class VectorT = Vector,
		   typename Alloc = std::allocator<T>>
  OffsetT<VectorT<const T *>> CreateVectorOfStructs(
	  const std::vector<T, Alloc> &v) {
	return CreateVectorOfStructs<T, OffsetT, VectorT>(data(v), v.size());
  }

  template<template<typename...> class VectorT = Vector64, int &..., typename T>
  Offset64<VectorT<const T *>> CreateVectorOfStructs64(
	  const std::vector<T> &v) {
	return CreateVectorOfStructs<T, Offset64, VectorT>(data(v), v.size());
  }

  /// @brief Serialize an array of native structs into a FlatBuffer `vector`.
  /// @tparam T The data type of the struct array elements.
  /// @tparam S The data type of the native struct array elements.
  /// @param[in] v A pointer to the array of type `S` to serialize into the
  /// buffer as a `vector`.
  /// @param[in] len The number of elements to serialize.
  /// @param[in] pack_func Pointer to a function to convert the native struct
  /// to the FlatBuffer struct.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename S>
  Offset<Vector<const T *>> CreateVectorOfNativeStructs(
	  const S *v, size_t len, T (*const pack_func)(const S &)) {
	FLATBUFFERS_ASSERT(pack_func);
	auto structs = StartVectorOfStructs<T>(len);
	for (size_t i = 0; i < len; i++) { structs[i] = pack_func(v[i]); }
	return EndVectorOfStructs<T>(len);
  }

  /// @brief Serialize an array of native structs into a FlatBuffer `vector`.
  /// @tparam T The data type of the struct array elements.
  /// @tparam S The data type of the native struct array elements.
  /// @param[in] v A pointer to the array of type `S` to serialize into the
  /// buffer as a `vector`.
  /// @param[in] len The number of elements to serialize.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename S>
  Offset<Vector<const T *>> CreateVectorOfNativeStructs(const S *v,
														size_t len) {
	extern T Pack(const S &);
	return CreateVectorOfNativeStructs(v, len, Pack);
  }

  /// @brief Serialize a `std::vector` of native structs into a FlatBuffer
  /// `vector`.
  /// @tparam T The data type of the `std::vector` struct elements.
  /// @tparam S The data type of the `std::vector` native struct elements.
  /// @param[in] v A const reference to the `std::vector` of structs to
  /// serialize into the buffer as a `vector`.
  /// @param[in] pack_func Pointer to a function to convert the native struct
  /// to the FlatBuffer struct.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename S, typename Alloc = std::allocator<T>>
  Offset<Vector<const T *>> CreateVectorOfNativeStructs(
	  const std::vector<S, Alloc> &v, T (*const pack_func)(const S &)) {
	return CreateVectorOfNativeStructs<T, S>(data(v), v.size(), pack_func);
  }

  /// @brief Serialize a `std::vector` of native structs into a FlatBuffer
  /// `vector`.
  /// @tparam T The data type of the `std::vector` struct elements.
  /// @tparam S The data type of the `std::vector` native struct elements.
  /// @param[in] v A const reference to the `std::vector` of structs to
  /// serialize into the buffer as a `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename S, typename Alloc = std::allocator<S>>
  Offset<Vector<const T *>> CreateVectorOfNativeStructs(
	  const std::vector<S, Alloc> &v) {
	return CreateVectorOfNativeStructs<T, S>(data(v), v.size());
  }

  /// @cond FLATBUFFERS_INTERNAL
  template<typename T> struct StructKeyComparator {
	bool operator()(const T &a, const T &b) const {
	  return a.KeyCompareLessThan(&b);
	}
  };
  /// @endcond

  /// @brief Serialize a `std::vector` of structs into a FlatBuffer `vector`
  /// in sorted order.
  /// @tparam T The data type of the `std::vector` struct elements.
  /// @param[in] v A const reference to the `std::vector` of structs to
  /// serialize into the buffer as a `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename Alloc = std::allocator<T>>
  Offset<Vector<const T *>> CreateVectorOfSortedStructs(
	  std::vector<T, Alloc> *v) {
	return CreateVectorOfSortedStructs(data(*v), v->size());
  }

  /// @brief Serialize a `std::vector` of native structs into a FlatBuffer
  /// `vector` in sorted order.
  /// @tparam T The data type of the `std::vector` struct elements.
  /// @tparam S The data type of the `std::vector` native struct elements.
  /// @param[in] v A const reference to the `std::vector` of structs to
  /// serialize into the buffer as a `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename S, typename Alloc = std::allocator<T>>
  Offset<Vector<const T *>> CreateVectorOfSortedNativeStructs(
	  std::vector<S, Alloc> *v) {
	return CreateVectorOfSortedNativeStructs<T, S>(data(*v), v->size());
  }

  /// @brief Serialize an array of structs into a FlatBuffer `vector` in sorted
  /// order.
  /// @tparam T The data type of the struct array elements.
  /// @param[in] v A pointer to the array of type `T` to serialize into the
  /// buffer as a `vector`.
  /// @param[in] len The number of elements to serialize.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T>
  Offset<Vector<const T *>> CreateVectorOfSortedStructs(T *v, size_t len) {
	std::stable_sort(v, v + len, StructKeyComparator<T>());
	return CreateVectorOfStructs(v, len);
  }

  /// @brief Serialize an array of native structs into a FlatBuffer `vector` in
  /// sorted order.
  /// @tparam T The data type of the struct array elements.
  /// @tparam S The data type of the native struct array elements.
  /// @param[in] v A pointer to the array of type `S` to serialize into the
  /// buffer as a `vector`.
  /// @param[in] len The number of elements to serialize.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename S>
  Offset<Vector<const T *>> CreateVectorOfSortedNativeStructs(S *v,
															  size_t len) {
	extern T Pack(const S &);
	auto structs = StartVectorOfStructs<T>(len);
	for (size_t i = 0; i < len; i++) { structs[i] = Pack(v[i]); }
	std::stable_sort(structs, structs + len, StructKeyComparator<T>());
	return EndVectorOfStructs<T>(len);
  }

  /// @cond FLATBUFFERS_INTERNAL
  template<typename T> struct TableKeyComparator {
	explicit TableKeyComparator(vector_downward<SizeT> &buf) : buf_(buf) {}
	TableKeyComparator(const TableKeyComparator &other) : buf_(other.buf_) {}
	bool operator()(const Offset<T> &a, const Offset<T> &b) const {
	  auto table_a = reinterpret_cast<T *>(buf_.data_at(a.o));
	  auto table_b = reinterpret_cast<T *>(buf_.data_at(b.o));
	  return table_a->KeyCompareLessThan(table_b);
	}
	vector_downward<SizeT> &buf_;

   private:
	FLATBUFFERS_DELETE_FUNC(
		TableKeyComparator &operator=(const TableKeyComparator &other));
  };
  /// @endcond

  /// @brief Serialize an array of `table` offsets as a `vector` in the buffer
  /// in sorted order.
  /// @tparam T The data type that the offset refers to.
  /// @param[in] v An array of type `Offset<T>` that contains the `table`
  /// offsets to store in the buffer in sorted order.
  /// @param[in] len The number of elements to store in the `vector`.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T>
  Offset<Vector<Offset<T>>> CreateVectorOfSortedTables(Offset<T> *v,
													   size_t len) {
	std::stable_sort(v, v + len, TableKeyComparator<T>(buf_));
	return CreateVector(v, len);
  }

  /// @brief Serialize an array of `table` offsets as a `vector` in the buffer
  /// in sorted order.
  /// @tparam T The data type that the offset refers to.
  /// @param[in] v An array of type `Offset<T>` that contains the `table`
  /// offsets to store in the buffer in sorted order.
  /// @return Returns a typed `Offset` into the serialized data indicating
  /// where the vector is stored.
  template<typename T, typename Alloc = std::allocator<T>>
  Offset<Vector<Offset<T>>> CreateVectorOfSortedTables(
	  std::vector<Offset<T>, Alloc> *v) {
	return CreateVectorOfSortedTables(data(*v), v->size());
  }

  /// @brief Specialized version of `CreateVector` for non-copying use cases.
  /// Write the data any time later to the returned buffer pointer `buf`.
  /// @param[in] len The number of elements to store in the `vector`.
  /// @param[in] elemsize The size of each element in the `vector`.
  /// @param[out] buf A pointer to a `uint8_t` pointer that can be
  /// written to at a later time to serialize the data into a `vector`
  /// in the buffer.
  uoffset_t CreateUninitializedVector(size_t len, size_t elemsize,
									  size_t alignment, uint8_t **buf) {
	NotNested();
	StartVector(len, elemsize, alignment);
	buf_.make_space(len * elemsize);
	const uoffset_t vec_start = GetSizeRelative32BitRegion();
	auto vec_end = EndVector(len);
	*buf = buf_.data_at(vec_start);
	return vec_end;
  }

  FLATBUFFERS_ATTRIBUTE([[deprecated("call the version above instead")]])
  uoffset_t CreateUninitializedVector(size_t len, size_t elemsize,
									  uint8_t **buf) {
	return CreateUninitializedVector(len, elemsize, elemsize, buf);
  }

  /// @brief Specialized version of `CreateVector` for non-copying use cases.
  /// Write the data any time later to the returned buffer pointer `buf`.
  /// @tparam T The data type of the data that will be stored in the buffer
  /// as a `vector`.
  /// @param[in] len The number of elements to store in the `vector`.
  /// @param[out] buf A pointer to a pointer of type `T` that can be
  /// written to at a later time to serialize the data into a `vector`
  /// in the buffer.
  template<typename T>
  Offset<Vector<T>> CreateUninitializedVector(size_t len, T **buf) {
	AssertScalarT<T>();
	return CreateUninitializedVector(len, sizeof(T), AlignOf<T>(),
									 reinterpret_cast<uint8_t **>(buf));
  }

  template<typename T>
  Offset<Vector<const T *>> CreateUninitializedVectorOfStructs(size_t len,
															   T **buf) {
	return CreateUninitializedVector(len, sizeof(T), AlignOf<T>(),
									 reinterpret_cast<uint8_t **>(buf));
  }

  // @brief Create a vector of scalar type T given as input a vector of scalar
  // type U, useful with e.g. pre "enum class" enums, or any existing scalar
  // data of the wrong type.
  template<typename T, typename U>
  Offset<Vector<T>> CreateVectorScalarCast(const U *v, size_t len) {
	AssertScalarT<T>();
	AssertScalarT<U>();
	StartVector<T>(len);
	for (auto i = len; i > 0;) { PushElement(static_cast<T>(v[--i])); }
	return Offset<Vector<T>>(EndVector(len));
  }

  /// @brief Write a struct by itself, typically to be part of a union.
  template<typename T> Offset<const T *> CreateStruct(const T &structobj) {
	NotNested();
	Align(AlignOf<T>());
	buf_.push_small(structobj);
	return Offset<const T *>(
		CalculateOffset<typename Offset<const T *>::offset_type>());
  }

  /// @brief Finish serializing a buffer by writing the root offset.
  /// @param[in] file_identifier If a `file_identifier` is given, the buffer
  /// will be prefixed with a standard FlatBuffers file header.
  template<typename T>
  void Finish(Offset<T> root, const char *file_identifier = nullptr) {
	Finish(root.o, file_identifier, false);
  }

  /// @brief Finish a buffer with a 32 bit size field pre-fixed (size of the
  /// buffer following the size field). These buffers are NOT compatible
  /// with standard buffers created by Finish, i.e. you can't call GetRoot
  /// on them, you have to use GetSizePrefixedRoot instead.
  /// All >32 bit quantities in this buffer will be aligned when the whole
  /// size pre-fixed buffer is aligned.
  /// These kinds of buffers are useful for creating a stream of FlatBuffers.
  template<typename T>
  void FinishSizePrefixed(Offset<T> root,
						  const char *file_identifier = nullptr) {
	Finish(root.o, file_identifier, true);
  }

  void SwapBufAllocator(FlatBufferBuilderImpl &other) {
	buf_.swap_allocator(other.buf_);
  }

  /// @brief The length of a FlatBuffer file header.
  static const size_t kFileIdentifierLength =
	  ::flatbuffers::kFileIdentifierLength;

 protected:
  // You shouldn't really be copying instances of this class.
  FlatBufferBuilderImpl(const FlatBufferBuilderImpl &);
  FlatBufferBuilderImpl &operator=(const FlatBufferBuilderImpl &);

  void Finish(uoffset_t root, const char *file_identifier, bool size_prefix) {
	// A buffer can only be finished once. To reuse a builder use `clear()`.
	FLATBUFFERS_ASSERT(!finished);

	NotNested();
	buf_.clear_scratch();

	const size_t prefix_size = size_prefix ? sizeof(SizeT) : 0;
	// Make sure we track the alignment of the size prefix.
	TrackMinAlign(prefix_size);

	const size_t root_offset_size = sizeof(uoffset_t);
	const size_t file_id_size = file_identifier ? kFileIdentifierLength : 0;

	// This will cause the whole buffer to be aligned.
	PreAlign(prefix_size + root_offset_size + file_id_size, minalign_);

	if (file_identifier) {
	  FLATBUFFERS_ASSERT(strlen(file_identifier) == kFileIdentifierLength);
	  PushBytes(reinterpret_cast<const uint8_t *>(file_identifier),
				kFileIdentifierLength);
	}
	PushElement(ReferTo(root));  // Location of root.
	if (size_prefix) { PushElement(GetSize()); }
	finished = true;
  }

  struct FieldLoc {
	uoffset_t off;
	voffset_t id;
  };

  vector_downward<SizeT> buf_;

  // Accumulating offsets of table members while it is being built.
  // We store these in the scratch pad of buf_, after the vtable offsets.
  uoffset_t num_field_loc;
  // Track how much of the vtable is in use, so we can output the most compact
  // possible vtable.
  voffset_t max_voffset_;

  // This is the length of the 64-bit region of the buffer. The buffer supports
  // 64-bit offsets by forcing serialization of those elements in the "tail"
  // region of the buffer (i.e. "64-bit region"). To properly keep track of
  // offsets that are referenced from the tail of the buffer to not overflow
  // their size (e.g. Offset is a uint32_t type), the boundary of the 32-/64-bit
  // regions must be tracked.
  //
  // [    Complete FlatBuffer     ]
  // [32-bit region][64-bit region]
  //               ^              ^
  //               |              Tail of the buffer.
  //               |
  //               Tail of the 32-bit region of the buffer.
  //
  // This keeps track of the size of the 64-bit region so that the tail of the
  // 32-bit region can be calculated as `GetSize() - length_of_64_bit_region_`.
  //
  // This will remain 0 if no 64-bit offset types are added to the buffer.
  size_t length_of_64_bit_region_;

  // Ensure objects are not nested.
  bool nested;

  // Ensure the buffer is finished before it is being accessed.
  bool finished;

  size_t minalign_;

  bool force_defaults_;  // Serialize values equal to their defaults anyway.

  bool dedup_vtables_;

  struct StringOffsetCompare {
	explicit StringOffsetCompare(const vector_downward<SizeT> &buf)
		: buf_(&buf) {}
	bool operator()(const Offset<String> &a, const Offset<String> &b) const {
	  auto stra = reinterpret_cast<const String *>(buf_->data_at(a.o));
	  auto strb = reinterpret_cast<const String *>(buf_->data_at(b.o));
	  return StringLessThan(stra->data(), stra->size(), strb->data(),
							strb->size());
	}
	const vector_downward<SizeT> *buf_;
  };

  // For use with CreateSharedString. Instantiated on first use only.
  typedef std::set<Offset<String>, StringOffsetCompare> StringOffsetMap;
  StringOffsetMap *string_pool;

 private:
  void CanAddOffset64() {
	// If you hit this assertion, you are attempting to add a 64-bit offset to
	// a 32-bit only builder. This is because the builder has overloads that
	// differ only on the offset size returned: e.g.:
	//
	//   FlatBufferBuilder builder;
	//   Offset64<String> string_offset = builder.CreateString<Offset64>();
	//
	// Either use a 64-bit aware builder, or don't try to create an Offset64
	// return type.
	//
	// TODO(derekbailey): we can probably do more enable_if to avoid this
	// looking like its possible to the user.
	static_assert(Is64Aware, "cannot add 64-bit offset to a 32-bit builder");

	// If you hit this assertion, you are attempting to add an 64-bit offset
	// item after already serializing a 32-bit item. All 64-bit offsets have to
	// added to the tail of the buffer before any 32-bit items can be added.
	// Otherwise some items might not be addressable due to the maximum range of
	// the 32-bit offset.
	FLATBUFFERS_ASSERT(GetSize() == length_of_64_bit_region_);
  }

  /// @brief Store a string in the buffer, which can contain any binary data.
  /// @param[in] str A const char pointer to the data to be stored as a string.
  /// @param[in] len The number of bytes that should be stored from `str`.
  /// @return Returns the offset in the buffer where the string starts.
  void CreateStringImpl(const char *str, size_t len) {
	NotNested();
	PreAlign<uoffset_t>(len + 1);  // Always 0-terminated.
	buf_.fill(1);
	PushBytes(reinterpret_cast<const uint8_t *>(str), len);
	PushElement(static_cast<uoffset_t>(len));
  }

  // Allocates space for a vector of structures.
  // Must be completed with EndVectorOfStructs().
  template<typename T, template<typename> class OffsetT = Offset>
  T *StartVectorOfStructs(size_t vector_size) {
	StartVector<OffsetT>(vector_size, sizeof(T), AlignOf<T>());
	return reinterpret_cast<T *>(buf_.make_space(vector_size * sizeof(T)));
  }

  // End the vector of structures in the flatbuffers.
  // Vector should have previously be started with StartVectorOfStructs().
  template<typename T, template<typename> class OffsetT = Offset>
  OffsetT<Vector<const T *>> EndVectorOfStructs(size_t vector_size) {
	return OffsetT<Vector<const T *>>(
		EndVector<typename Vector<const T *>::size_type,
				  typename OffsetT<Vector<const T *>>::offset_type>(
			vector_size));
  }

  template<typename T>
  typename std::enable_if<std::is_same<T, uoffset_t>::value, T>::type
  CalculateOffset() {
	// Default to the end of the 32-bit region. This may or may not be the end
	// of the buffer, depending on if any 64-bit offsets have been added.
	return GetSizeRelative32BitRegion();
  }

  // Specializations to handle the 64-bit CalculateOffset, which is relative to
  // end of the buffer.
  template<typename T>
  typename std::enable_if<std::is_same<T, uoffset64_t>::value, T>::type
  CalculateOffset() {
	// This should never be compiled in when not using a 64-bit builder.
	static_assert(Is64Aware, "invalid 64-bit offset in 32-bit builder");

	// Store how big the 64-bit region of the buffer is, so we can determine
	// where the 32/64 bit boundary is.
	length_of_64_bit_region_ = GetSize();

	return length_of_64_bit_region_;
  }
};
/// @}

// Hack to `FlatBufferBuilder` mean `FlatBufferBuilder<false>` or
// `FlatBufferBuilder<>`, where the template < > syntax is required.
using FlatBufferBuilder = FlatBufferBuilderImpl<false>;
using FlatBufferBuilder64 = FlatBufferBuilderImpl<true>;

// These are external due to GCC not allowing them in the class.
// See: https://stackoverflow.com/q/8061456/868247
template<>
template<>
inline Offset64<String> FlatBufferBuilder64::CreateString(const char *str,
														  size_t len) {
  CanAddOffset64();
  CreateStringImpl(str, len);
  return Offset64<String>(
	  CalculateOffset<typename Offset64<String>::offset_type>());
}

// Used to distinguish from real Offsets.
template<typename T = void> struct EmptyOffset {};

// TODO(derekbailey): it would be nice to combine these two methods.
template<>
template<>
inline void FlatBufferBuilder64::StartVector<Offset64, uint32_t>(
	size_t len, size_t elemsize, size_t alignment) {
  CanAddOffset64();
  StartVector<EmptyOffset, uint32_t>(len, elemsize, alignment);
}

template<>
template<>
inline void FlatBufferBuilder64::StartVector<Offset64, uint64_t>(
	size_t len, size_t elemsize, size_t alignment) {
  CanAddOffset64();
  StartVector<EmptyOffset, uint64_t>(len, elemsize, alignment);
}

/// Helpers to get a typed pointer to objects that are currently being built.
/// @warning Creating new objects will lead to reallocations and invalidates
/// the pointer!
template<typename T>
T *GetMutableTemporaryPointer(FlatBufferBuilder &fbb, Offset<T> offset) {
  return reinterpret_cast<T *>(fbb.GetCurrentBufferPointer() + fbb.GetSize() -
							   offset.o);
}

template<typename T>
const T *GetTemporaryPointer(const FlatBufferBuilder &fbb, Offset<T> offset) {
  return reinterpret_cast<const T *>(fbb.GetCurrentBufferPointer() +
									 fbb.GetSize() - offset.o);
}

}  // namespace flatbuffers

#endif  // FLATBUFFERS_FLATBUFFER_BUILDER_H_

/*** End of inlined file: flatbuffer_builder.h ***/

namespace flatbuffers {

/// @brief This can compute the start of a FlatBuffer from a root pointer, i.e.
/// it is the opposite transformation of GetRoot().
/// This may be useful if you want to pass on a root and have the recipient
/// delete the buffer afterwards.
inline const uint8_t *GetBufferStartFromRootPointer(const void *root) {
  auto table = reinterpret_cast<const Table *>(root);
  auto vtable = table->GetVTable();
  // Either the vtable is before the root or after the root.
  auto start = (std::min)(vtable, reinterpret_cast<const uint8_t *>(root));
  // Align to at least sizeof(uoffset_t).
  start = reinterpret_cast<const uint8_t *>(reinterpret_cast<uintptr_t>(start) &
											~(sizeof(uoffset_t) - 1));
  // Additionally, there may be a file_identifier in the buffer, and the root
  // offset. The buffer may have been aligned to any size between
  // sizeof(uoffset_t) and FLATBUFFERS_MAX_ALIGNMENT (see "force_align").
  // Sadly, the exact alignment is only known when constructing the buffer,
  // since it depends on the presence of values with said alignment properties.
  // So instead, we simply look at the next uoffset_t values (root,
  // file_identifier, and alignment padding) to see which points to the root.
  // None of the other values can "impersonate" the root since they will either
  // be 0 or four ASCII characters.
  static_assert(flatbuffers::kFileIdentifierLength == sizeof(uoffset_t),
				"file_identifier is assumed to be the same size as uoffset_t");
  for (auto possible_roots = FLATBUFFERS_MAX_ALIGNMENT / sizeof(uoffset_t) + 1;
	   possible_roots; possible_roots--) {
	start -= sizeof(uoffset_t);
	if (ReadScalar<uoffset_t>(start) + start ==
		reinterpret_cast<const uint8_t *>(root))
	  return start;
  }
  // We didn't find the root, either the "root" passed isn't really a root,
  // or the buffer is corrupt.
  // Assert, because calling this function with bad data may cause reads
  // outside of buffer boundaries.
  FLATBUFFERS_ASSERT(false);
  return nullptr;
}

/// @brief This return the prefixed size of a FlatBuffer.
template<typename SizeT = uoffset_t>
inline SizeT GetPrefixedSize(const uint8_t *buf) {
  return ReadScalar<SizeT>(buf);
}

// Gets the total length of the buffer given a sized prefixed FlatBuffer.
//
// This includes the size of the prefix as well as the buffer:
//
//  [size prefix][flatbuffer]
//  |---------length--------|
template<typename SizeT = uoffset_t>
inline SizeT GetSizePrefixedBufferLength(const uint8_t *const buf) {
  return ReadScalar<SizeT>(buf) + sizeof(SizeT);
}

// Base class for native objects (FlatBuffer data de-serialized into native
// C++ data structures).
// Contains no functionality, purely documentative.
struct NativeTable {};

/// @brief Function types to be used with resolving hashes into objects and
/// back again. The resolver gets a pointer to a field inside an object API
/// object that is of the type specified in the schema using the attribute
/// `cpp_type` (it is thus important whatever you write to this address
/// matches that type). The value of this field is initially null, so you
/// may choose to implement a delayed binding lookup using this function
/// if you wish. The resolver does the opposite lookup, for when the object
/// is being serialized again.
typedef uint64_t hash_value_t;
typedef std::function<void(void **pointer_adr, hash_value_t hash)>
	resolver_function_t;
typedef std::function<hash_value_t(void *pointer)> rehasher_function_t;

// Helper function to test if a field is present, using any of the field
// enums in the generated code.
// `table` must be a generated table type. Since this is a template parameter,
// this is not typechecked to be a subclass of Table, so beware!
// Note: this function will return false for fields equal to the default
// value, since they're not stored in the buffer (unless force_defaults was
// used).
template<typename T>
bool IsFieldPresent(const T *table, typename T::FlatBuffersVTableOffset field) {
  // Cast, since Table is a private baseclass of any table types.
  return reinterpret_cast<const Table *>(table)->CheckField(
	  static_cast<voffset_t>(field));
}

// Utility function for reverse lookups on the EnumNames*() functions
// (in the generated C++ code)
// names must be NULL terminated.
inline int LookupEnum(const char **names, const char *name) {
  for (const char **p = names; *p; p++)
	if (!strcmp(*p, name)) return static_cast<int>(p - names);
  return -1;
}

// These macros allow us to layout a struct with a guarantee that they'll end
// up looking the same on different compilers and platforms.
// It does this by disallowing the compiler to do any padding, and then
// does padding itself by inserting extra padding fields that make every
// element aligned to its own size.
// Additionally, it manually sets the alignment of the struct as a whole,
// which is typically its largest element, or a custom size set in the schema
// by the force_align attribute.
// These are used in the generated code only.

// clang-format off
#if defined(_MSC_VER)
  #define FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(alignment) \
	__pragma(pack(1)) \
	struct __declspec(align(alignment))
  #define FLATBUFFERS_STRUCT_END(name, size) \
	__pragma(pack()) \
	static_assert(sizeof(name) == size, "compiler breaks packing rules")
#elif defined(__GNUC__) || defined(__clang__) || defined(__ICCARM__)
  #define FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(alignment) \
	_Pragma("pack(1)") \
	struct __attribute__((aligned(alignment)))
  #define FLATBUFFERS_STRUCT_END(name, size) \
	_Pragma("pack()") \
	static_assert(sizeof(name) == size, "compiler breaks packing rules")
#else
  #error Unknown compiler, please define structure alignment macros
#endif
// clang-format on

// Minimal reflection via code generation.
// Besides full-fat reflection (see reflection.h) and parsing/printing by
// loading schemas (see idl.h), we can also have code generation for minimal
// reflection data which allows pretty-printing and other uses without needing
// a schema or a parser.
// Generate code with --reflect-types (types only) or --reflect-names (names
// also) to enable.
// See minireflect.h for utilities using this functionality.

// These types are organized slightly differently as the ones in idl.h.
enum SequenceType { ST_TABLE, ST_STRUCT, ST_UNION, ST_ENUM };

// Scalars have the same order as in idl.h
// clang-format off
#define FLATBUFFERS_GEN_ELEMENTARY_TYPES(ET) \
  ET(ET_UTYPE) \
  ET(ET_BOOL) \
  ET(ET_CHAR) \
  ET(ET_UCHAR) \
  ET(ET_SHORT) \
  ET(ET_USHORT) \
  ET(ET_INT) \
  ET(ET_UINT) \
  ET(ET_LONG) \
  ET(ET_ULONG) \
  ET(ET_FLOAT) \
  ET(ET_DOUBLE) \
  ET(ET_STRING) \
  ET(ET_SEQUENCE)  // See SequenceType.

enum ElementaryType {
  #define FLATBUFFERS_ET(E) E,
	FLATBUFFERS_GEN_ELEMENTARY_TYPES(FLATBUFFERS_ET)
  #undef FLATBUFFERS_ET
};

inline const char * const *ElementaryTypeNames() {
  static const char * const names[] = {
	#define FLATBUFFERS_ET(E) #E,
	  FLATBUFFERS_GEN_ELEMENTARY_TYPES(FLATBUFFERS_ET)
	#undef FLATBUFFERS_ET
  };
  return names;
}
// clang-format on

// Basic type info cost just 16bits per field!
// We're explicitly defining the signedness since the signedness of integer
// bitfields is otherwise implementation-defined and causes warnings on older
// GCC compilers.
struct TypeCode {
  // ElementaryType
  unsigned short base_type : 4;
  // Either vector (in table) or array (in struct)
  unsigned short is_repeating : 1;
  // Index into type_refs below, or -1 for none.
  signed short sequence_ref : 11;
};

static_assert(sizeof(TypeCode) == 2, "TypeCode");

struct TypeTable;

// Signature of the static method present in each type.
typedef const TypeTable *(*TypeFunction)();

struct TypeTable {
  SequenceType st;
  size_t num_elems;  // of type_codes, values, names (but not type_refs).
  const TypeCode *type_codes;     // num_elems count
  const TypeFunction *type_refs;  // less than num_elems entries (see TypeCode).
  const int16_t *array_sizes;     // less than num_elems entries (see TypeCode).
  const int64_t *values;  // Only set for non-consecutive enum/union or structs.
  const char *const *names;  // Only set if compiled with --reflect-names.
};

// String which identifies the current version of FlatBuffers.
inline const char *flatbuffers_version_string() {
  return "FlatBuffers " FLATBUFFERS_STRING(FLATBUFFERS_VERSION_MAJOR) "."
	  FLATBUFFERS_STRING(FLATBUFFERS_VERSION_MINOR) "."
	  FLATBUFFERS_STRING(FLATBUFFERS_VERSION_REVISION);
}

// clang-format off
#define FLATBUFFERS_DEFINE_BITMASK_OPERATORS(E, T)\
	inline FLATBUFFERS_CONSTEXPR_CPP11 E operator | (E lhs, E rhs){\
		return E(T(lhs) | T(rhs));\
	}\
	inline FLATBUFFERS_CONSTEXPR_CPP11 E operator & (E lhs, E rhs){\
		return E(T(lhs) & T(rhs));\
	}\
	inline FLATBUFFERS_CONSTEXPR_CPP11 E operator ^ (E lhs, E rhs){\
		return E(T(lhs) ^ T(rhs));\
	}\
	inline FLATBUFFERS_CONSTEXPR_CPP11 E operator ~ (E lhs){\
		return E(~T(lhs));\
	}\
	inline FLATBUFFERS_CONSTEXPR_CPP11 E operator |= (E &lhs, E rhs){\
		lhs = lhs | rhs;\
		return lhs;\
	}\
	inline FLATBUFFERS_CONSTEXPR_CPP11 E operator &= (E &lhs, E rhs){\
		lhs = lhs & rhs;\
		return lhs;\
	}\
	inline FLATBUFFERS_CONSTEXPR_CPP11 E operator ^= (E &lhs, E rhs){\
		lhs = lhs ^ rhs;\
		return lhs;\
	}\
	inline FLATBUFFERS_CONSTEXPR_CPP11 bool operator !(E rhs) \
	{\
		return !bool(T(rhs)); \
	}
/// @endcond
}  // namespace flatbuffers

// clang-format on

#endif  // FLATBUFFERS_H_

/*** End of inlined file: flatbuffers.h ***/

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
			  FLATBUFFERS_VERSION_MINOR == 3 &&
			  FLATBUFFERS_VERSION_REVISION == 25,
			 "Non-compatible flatbuffers version included");

namespace humblenet {
namespace HumblePeer {

struct Attribute;
struct AttributeBuilder;

struct ICEServer;
struct ICEServerBuilder;

struct HelloServer;
struct HelloServerBuilder;

struct HelloClient;
struct HelloClientBuilder;

struct P2POffer;
struct P2POfferBuilder;

struct P2PAnswer;
struct P2PAnswerBuilder;

struct P2PConnected;
struct P2PConnectedBuilder;

struct P2PDisconnect;
struct P2PDisconnectBuilder;

struct P2PReject;
struct P2PRejectBuilder;

struct ICECandidate;
struct ICECandidateBuilder;

struct P2PRelayData;
struct P2PRelayDataBuilder;

struct AliasRegister;
struct AliasRegisterBuilder;

struct AliasUnregister;
struct AliasUnregisterBuilder;

struct AliasLookup;
struct AliasLookupBuilder;

struct AliasResolved;
struct AliasResolvedBuilder;

struct Message;
struct MessageBuilder;

enum class ICEServerType : uint8_t {
  STUNServer = 1,
  TURNServer = 2,
  MIN = STUNServer,
  MAX = TURNServer
};

inline const ICEServerType (&EnumValuesICEServerType())[2] {
  static const ICEServerType values[] = {
	ICEServerType::STUNServer,
	ICEServerType::TURNServer
  };
  return values;
}

inline const char * const *EnumNamesICEServerType() {
  static const char * const names[3] = {
	"STUNServer",
	"TURNServer",
	nullptr
  };
  return names;
}

inline const char *EnumNameICEServerType(ICEServerType e) {
  if (::flatbuffers::IsOutRange(e, ICEServerType::STUNServer, ICEServerType::TURNServer)) return "";
  const size_t index = static_cast<size_t>(e) - static_cast<size_t>(ICEServerType::STUNServer);
  return EnumNamesICEServerType()[index];
}

enum class P2PRejectReason : uint8_t {
  NotFound = 1,
  PeerRefused = 2,
  MIN = NotFound,
  MAX = PeerRefused
};

inline const P2PRejectReason (&EnumValuesP2PRejectReason())[2] {
  static const P2PRejectReason values[] = {
	P2PRejectReason::NotFound,
	P2PRejectReason::PeerRefused
  };
  return values;
}

inline const char * const *EnumNamesP2PRejectReason() {
  static const char * const names[3] = {
	"NotFound",
	"PeerRefused",
	nullptr
  };
  return names;
}

inline const char *EnumNameP2PRejectReason(P2PRejectReason e) {
  if (::flatbuffers::IsOutRange(e, P2PRejectReason::NotFound, P2PRejectReason::PeerRefused)) return "";
  const size_t index = static_cast<size_t>(e) - static_cast<size_t>(P2PRejectReason::NotFound);
  return EnumNamesP2PRejectReason()[index];
}

enum class MessageType : uint8_t {
  NONE = 0,
  HelloServer = 1,
  HelloClient = 2,
  P2PConnected = 10,
  P2PDisconnect = 11,
  P2POffer = 12,
  P2PAnswer = 13,
  P2PReject = 14,
  ICECandidate = 15,
  P2PRelayData = 16,
  AliasRegister = 20,
  AliasUnregister = 21,
  AliasLookup = 22,
  AliasResolved = 23,
  MIN = NONE,
  MAX = AliasResolved
};

inline const MessageType (&EnumValuesMessageType())[14] {
  static const MessageType values[] = {
	MessageType::NONE,
	MessageType::HelloServer,
	MessageType::HelloClient,
	MessageType::P2PConnected,
	MessageType::P2PDisconnect,
	MessageType::P2POffer,
	MessageType::P2PAnswer,
	MessageType::P2PReject,
	MessageType::ICECandidate,
	MessageType::P2PRelayData,
	MessageType::AliasRegister,
	MessageType::AliasUnregister,
	MessageType::AliasLookup,
	MessageType::AliasResolved
  };
  return values;
}

inline const char * const *EnumNamesMessageType() {
  static const char * const names[25] = {
	"NONE",
	"HelloServer",
	"HelloClient",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"P2PConnected",
	"P2PDisconnect",
	"P2POffer",
	"P2PAnswer",
	"P2PReject",
	"ICECandidate",
	"P2PRelayData",
	"",
	"",
	"",
	"AliasRegister",
	"AliasUnregister",
	"AliasLookup",
	"AliasResolved",
	nullptr
  };
  return names;
}

inline const char *EnumNameMessageType(MessageType e) {
  if (::flatbuffers::IsOutRange(e, MessageType::NONE, MessageType::AliasResolved)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesMessageType()[index];
}

template<typename T> struct MessageTypeTraits {
  static const MessageType enum_value = MessageType::NONE;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::HelloServer> {
  static const MessageType enum_value = MessageType::HelloServer;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::HelloClient> {
  static const MessageType enum_value = MessageType::HelloClient;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::P2PConnected> {
  static const MessageType enum_value = MessageType::P2PConnected;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::P2PDisconnect> {
  static const MessageType enum_value = MessageType::P2PDisconnect;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::P2POffer> {
  static const MessageType enum_value = MessageType::P2POffer;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::P2PAnswer> {
  static const MessageType enum_value = MessageType::P2PAnswer;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::P2PReject> {
  static const MessageType enum_value = MessageType::P2PReject;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::ICECandidate> {
  static const MessageType enum_value = MessageType::ICECandidate;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::P2PRelayData> {
  static const MessageType enum_value = MessageType::P2PRelayData;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::AliasRegister> {
  static const MessageType enum_value = MessageType::AliasRegister;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::AliasUnregister> {
  static const MessageType enum_value = MessageType::AliasUnregister;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::AliasLookup> {
  static const MessageType enum_value = MessageType::AliasLookup;
};

template<> struct MessageTypeTraits<humblenet::HumblePeer::AliasResolved> {
  static const MessageType enum_value = MessageType::AliasResolved;
};

bool VerifyMessageType(::flatbuffers::Verifier &verifier, const void *obj, MessageType type);
bool VerifyMessageTypeVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<MessageType> *types);

struct Attribute FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef AttributeBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_KEY = 4,
	VT_VALUE = 6
  };
  const ::flatbuffers::String *key() const {
	return GetPointer<const ::flatbuffers::String *>(VT_KEY);
  }
  bool KeyCompareLessThan(const Attribute * const o) const {
	return *key() < *o->key();
  }
  int KeyCompareWithValue(const char *_key) const {
	return strcmp(key()->c_str(), _key);
  }
  template<typename StringType>
  int KeyCompareWithValue(const StringType& _key) const {
	if (key()->c_str() < _key) return -1;
	if (_key < key()->c_str()) return 1;
	return 0;
  }
  const ::flatbuffers::String *value() const {
	return GetPointer<const ::flatbuffers::String *>(VT_VALUE);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyOffsetRequired(verifier, VT_KEY) &&
		   verifier.VerifyString(key()) &&
		   VerifyOffsetRequired(verifier, VT_VALUE) &&
		   verifier.VerifyString(value()) &&
		   verifier.EndTable();
  }
};

struct AttributeBuilder {
  typedef Attribute Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_key(::flatbuffers::Offset<::flatbuffers::String> key) {
	fbb_.AddOffset(Attribute::VT_KEY, key);
  }
  void add_value(::flatbuffers::Offset<::flatbuffers::String> value) {
	fbb_.AddOffset(Attribute::VT_VALUE, value);
  }
  explicit AttributeBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Attribute> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<Attribute>(end);
	fbb_.Required(o, Attribute::VT_KEY);
	fbb_.Required(o, Attribute::VT_VALUE);
	return o;
  }
};

inline ::flatbuffers::Offset<Attribute> CreateAttribute(
	::flatbuffers::FlatBufferBuilder &_fbb,
	::flatbuffers::Offset<::flatbuffers::String> key = 0,
	::flatbuffers::Offset<::flatbuffers::String> value = 0) {
  AttributeBuilder builder_(_fbb);
  builder_.add_value(value);
  builder_.add_key(key);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Attribute> CreateAttributeDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	const char *key = nullptr,
	const char *value = nullptr) {
  auto key__ = key ? _fbb.CreateString(key) : 0;
  auto value__ = value ? _fbb.CreateString(value) : 0;
  return humblenet::HumblePeer::CreateAttribute(
	  _fbb,
	  key__,
	  value__);
}

struct ICEServer FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ICEServerBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_TYPE = 4,
	VT_SERVER = 6,
	VT_USERNAME = 8,
	VT_PASSWORD = 10
  };
  humblenet::HumblePeer::ICEServerType type() const {
	return static_cast<humblenet::HumblePeer::ICEServerType>(GetField<uint8_t>(VT_TYPE, 1));
  }
  const ::flatbuffers::String *server() const {
	return GetPointer<const ::flatbuffers::String *>(VT_SERVER);
  }
  const ::flatbuffers::String *username() const {
	return GetPointer<const ::flatbuffers::String *>(VT_USERNAME);
  }
  const ::flatbuffers::String *password() const {
	return GetPointer<const ::flatbuffers::String *>(VT_PASSWORD);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint8_t>(verifier, VT_TYPE, 1) &&
		   VerifyOffsetRequired(verifier, VT_SERVER) &&
		   verifier.VerifyString(server()) &&
		   VerifyOffset(verifier, VT_USERNAME) &&
		   verifier.VerifyString(username()) &&
		   VerifyOffset(verifier, VT_PASSWORD) &&
		   verifier.VerifyString(password()) &&
		   verifier.EndTable();
  }
};

struct ICEServerBuilder {
  typedef ICEServer Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_type(humblenet::HumblePeer::ICEServerType type) {
	fbb_.AddElement<uint8_t>(ICEServer::VT_TYPE, static_cast<uint8_t>(type), 1);
  }
  void add_server(::flatbuffers::Offset<::flatbuffers::String> server) {
	fbb_.AddOffset(ICEServer::VT_SERVER, server);
  }
  void add_username(::flatbuffers::Offset<::flatbuffers::String> username) {
	fbb_.AddOffset(ICEServer::VT_USERNAME, username);
  }
  void add_password(::flatbuffers::Offset<::flatbuffers::String> password) {
	fbb_.AddOffset(ICEServer::VT_PASSWORD, password);
  }
  explicit ICEServerBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ICEServer> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<ICEServer>(end);
	fbb_.Required(o, ICEServer::VT_SERVER);
	return o;
  }
};

inline ::flatbuffers::Offset<ICEServer> CreateICEServer(
	::flatbuffers::FlatBufferBuilder &_fbb,
	humblenet::HumblePeer::ICEServerType type = humblenet::HumblePeer::ICEServerType::STUNServer,
	::flatbuffers::Offset<::flatbuffers::String> server = 0,
	::flatbuffers::Offset<::flatbuffers::String> username = 0,
	::flatbuffers::Offset<::flatbuffers::String> password = 0) {
  ICEServerBuilder builder_(_fbb);
  builder_.add_password(password);
  builder_.add_username(username);
  builder_.add_server(server);
  builder_.add_type(type);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<ICEServer> CreateICEServerDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	humblenet::HumblePeer::ICEServerType type = humblenet::HumblePeer::ICEServerType::STUNServer,
	const char *server = nullptr,
	const char *username = nullptr,
	const char *password = nullptr) {
  auto server__ = server ? _fbb.CreateString(server) : 0;
  auto username__ = username ? _fbb.CreateString(username) : 0;
  auto password__ = password ? _fbb.CreateString(password) : 0;
  return humblenet::HumblePeer::CreateICEServer(
	  _fbb,
	  type,
	  server__,
	  username__,
	  password__);
}

struct HelloServer FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef HelloServerBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_VERSION = 4,
	VT_FLAGS = 6,
	VT_GAMETOKEN = 8,
	VT_GAMESIGNATURE = 10,
	VT_AUTHTOKEN = 12,
	VT_RECONNECTTOKEN = 14,
	VT_ATTRIBUTES = 16
  };
  uint32_t version() const {
	return GetField<uint32_t>(VT_VERSION, 0);
  }
  uint8_t flags() const {
	return GetField<uint8_t>(VT_FLAGS, 0);
  }
  const ::flatbuffers::String *gameToken() const {
	return GetPointer<const ::flatbuffers::String *>(VT_GAMETOKEN);
  }
  const ::flatbuffers::String *gameSignature() const {
	return GetPointer<const ::flatbuffers::String *>(VT_GAMESIGNATURE);
  }
  const ::flatbuffers::String *authToken() const {
	return GetPointer<const ::flatbuffers::String *>(VT_AUTHTOKEN);
  }
  const ::flatbuffers::String *reconnectToken() const {
	return GetPointer<const ::flatbuffers::String *>(VT_RECONNECTTOKEN);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::Attribute>> *attributes() const {
	return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::Attribute>> *>(VT_ATTRIBUTES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_VERSION, 4) &&
		   VerifyField<uint8_t>(verifier, VT_FLAGS, 1) &&
		   VerifyOffsetRequired(verifier, VT_GAMETOKEN) &&
		   verifier.VerifyString(gameToken()) &&
		   VerifyOffsetRequired(verifier, VT_GAMESIGNATURE) &&
		   verifier.VerifyString(gameSignature()) &&
		   VerifyOffset(verifier, VT_AUTHTOKEN) &&
		   verifier.VerifyString(authToken()) &&
		   VerifyOffset(verifier, VT_RECONNECTTOKEN) &&
		   verifier.VerifyString(reconnectToken()) &&
		   VerifyOffset(verifier, VT_ATTRIBUTES) &&
		   verifier.VerifyVector(attributes()) &&
		   verifier.VerifyVectorOfTables(attributes()) &&
		   verifier.EndTable();
  }
};

struct HelloServerBuilder {
  typedef HelloServer Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_version(uint32_t version) {
	fbb_.AddElement<uint32_t>(HelloServer::VT_VERSION, version, 0);
  }
  void add_flags(uint8_t flags) {
	fbb_.AddElement<uint8_t>(HelloServer::VT_FLAGS, flags, 0);
  }
  void add_gameToken(::flatbuffers::Offset<::flatbuffers::String> gameToken) {
	fbb_.AddOffset(HelloServer::VT_GAMETOKEN, gameToken);
  }
  void add_gameSignature(::flatbuffers::Offset<::flatbuffers::String> gameSignature) {
	fbb_.AddOffset(HelloServer::VT_GAMESIGNATURE, gameSignature);
  }
  void add_authToken(::flatbuffers::Offset<::flatbuffers::String> authToken) {
	fbb_.AddOffset(HelloServer::VT_AUTHTOKEN, authToken);
  }
  void add_reconnectToken(::flatbuffers::Offset<::flatbuffers::String> reconnectToken) {
	fbb_.AddOffset(HelloServer::VT_RECONNECTTOKEN, reconnectToken);
  }
  void add_attributes(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::Attribute>>> attributes) {
	fbb_.AddOffset(HelloServer::VT_ATTRIBUTES, attributes);
  }
  explicit HelloServerBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<HelloServer> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<HelloServer>(end);
	fbb_.Required(o, HelloServer::VT_GAMETOKEN);
	fbb_.Required(o, HelloServer::VT_GAMESIGNATURE);
	return o;
  }
};

inline ::flatbuffers::Offset<HelloServer> CreateHelloServer(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t version = 0,
	uint8_t flags = 0,
	::flatbuffers::Offset<::flatbuffers::String> gameToken = 0,
	::flatbuffers::Offset<::flatbuffers::String> gameSignature = 0,
	::flatbuffers::Offset<::flatbuffers::String> authToken = 0,
	::flatbuffers::Offset<::flatbuffers::String> reconnectToken = 0,
	::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::Attribute>>> attributes = 0) {
  HelloServerBuilder builder_(_fbb);
  builder_.add_attributes(attributes);
  builder_.add_reconnectToken(reconnectToken);
  builder_.add_authToken(authToken);
  builder_.add_gameSignature(gameSignature);
  builder_.add_gameToken(gameToken);
  builder_.add_version(version);
  builder_.add_flags(flags);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<HelloServer> CreateHelloServerDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t version = 0,
	uint8_t flags = 0,
	const char *gameToken = nullptr,
	const char *gameSignature = nullptr,
	const char *authToken = nullptr,
	const char *reconnectToken = nullptr,
	std::vector<::flatbuffers::Offset<humblenet::HumblePeer::Attribute>> *attributes = nullptr) {
  auto gameToken__ = gameToken ? _fbb.CreateString(gameToken) : 0;
  auto gameSignature__ = gameSignature ? _fbb.CreateString(gameSignature) : 0;
  auto authToken__ = authToken ? _fbb.CreateString(authToken) : 0;
  auto reconnectToken__ = reconnectToken ? _fbb.CreateString(reconnectToken) : 0;
  auto attributes__ = attributes ? _fbb.CreateVectorOfSortedTables<humblenet::HumblePeer::Attribute>(attributes) : 0;
  return humblenet::HumblePeer::CreateHelloServer(
	  _fbb,
	  version,
	  flags,
	  gameToken__,
	  gameSignature__,
	  authToken__,
	  reconnectToken__,
	  attributes__);
}

struct HelloClient FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef HelloClientBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4,
	VT_RECONNECTTOKEN = 6,
	VT_ICESERVERS = 8
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  const ::flatbuffers::String *reconnectToken() const {
	return GetPointer<const ::flatbuffers::String *>(VT_RECONNECTTOKEN);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::ICEServer>> *iceServers() const {
	return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::ICEServer>> *>(VT_ICESERVERS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   VerifyOffset(verifier, VT_RECONNECTTOKEN) &&
		   verifier.VerifyString(reconnectToken()) &&
		   VerifyOffset(verifier, VT_ICESERVERS) &&
		   verifier.VerifyVector(iceServers()) &&
		   verifier.VerifyVectorOfTables(iceServers()) &&
		   verifier.EndTable();
  }
};

struct HelloClientBuilder {
  typedef HelloClient Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(HelloClient::VT_PEERID, peerId, 0);
  }
  void add_reconnectToken(::flatbuffers::Offset<::flatbuffers::String> reconnectToken) {
	fbb_.AddOffset(HelloClient::VT_RECONNECTTOKEN, reconnectToken);
  }
  void add_iceServers(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::ICEServer>>> iceServers) {
	fbb_.AddOffset(HelloClient::VT_ICESERVERS, iceServers);
  }
  explicit HelloClientBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<HelloClient> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<HelloClient>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<HelloClient> CreateHelloClient(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	::flatbuffers::Offset<::flatbuffers::String> reconnectToken = 0,
	::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<humblenet::HumblePeer::ICEServer>>> iceServers = 0) {
  HelloClientBuilder builder_(_fbb);
  builder_.add_iceServers(iceServers);
  builder_.add_reconnectToken(reconnectToken);
  builder_.add_peerId(peerId);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<HelloClient> CreateHelloClientDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	const char *reconnectToken = nullptr,
	const std::vector<::flatbuffers::Offset<humblenet::HumblePeer::ICEServer>> *iceServers = nullptr) {
  auto reconnectToken__ = reconnectToken ? _fbb.CreateString(reconnectToken) : 0;
  auto iceServers__ = iceServers ? _fbb.CreateVector<::flatbuffers::Offset<humblenet::HumblePeer::ICEServer>>(*iceServers) : 0;
  return humblenet::HumblePeer::CreateHelloClient(
	  _fbb,
	  peerId,
	  reconnectToken__,
	  iceServers__);
}

struct P2POffer FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef P2POfferBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4,
	VT_FLAGS = 6,
	VT_OFFER = 8
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  uint8_t flags() const {
	return GetField<uint8_t>(VT_FLAGS, 0);
  }
  const ::flatbuffers::String *offer() const {
	return GetPointer<const ::flatbuffers::String *>(VT_OFFER);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   VerifyField<uint8_t>(verifier, VT_FLAGS, 1) &&
		   VerifyOffset(verifier, VT_OFFER) &&
		   verifier.VerifyString(offer()) &&
		   verifier.EndTable();
  }
};

struct P2POfferBuilder {
  typedef P2POffer Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(P2POffer::VT_PEERID, peerId, 0);
  }
  void add_flags(uint8_t flags) {
	fbb_.AddElement<uint8_t>(P2POffer::VT_FLAGS, flags, 0);
  }
  void add_offer(::flatbuffers::Offset<::flatbuffers::String> offer) {
	fbb_.AddOffset(P2POffer::VT_OFFER, offer);
  }
  explicit P2POfferBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<P2POffer> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<P2POffer>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<P2POffer> CreateP2POffer(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	uint8_t flags = 0,
	::flatbuffers::Offset<::flatbuffers::String> offer = 0) {
  P2POfferBuilder builder_(_fbb);
  builder_.add_offer(offer);
  builder_.add_peerId(peerId);
  builder_.add_flags(flags);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<P2POffer> CreateP2POfferDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	uint8_t flags = 0,
	const char *offer = nullptr) {
  auto offer__ = offer ? _fbb.CreateString(offer) : 0;
  return humblenet::HumblePeer::CreateP2POffer(
	  _fbb,
	  peerId,
	  flags,
	  offer__);
}

struct P2PAnswer FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef P2PAnswerBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4,
	VT_OFFER = 6
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  const ::flatbuffers::String *offer() const {
	return GetPointer<const ::flatbuffers::String *>(VT_OFFER);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   VerifyOffset(verifier, VT_OFFER) &&
		   verifier.VerifyString(offer()) &&
		   verifier.EndTable();
  }
};

struct P2PAnswerBuilder {
  typedef P2PAnswer Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(P2PAnswer::VT_PEERID, peerId, 0);
  }
  void add_offer(::flatbuffers::Offset<::flatbuffers::String> offer) {
	fbb_.AddOffset(P2PAnswer::VT_OFFER, offer);
  }
  explicit P2PAnswerBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<P2PAnswer> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<P2PAnswer>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<P2PAnswer> CreateP2PAnswer(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	::flatbuffers::Offset<::flatbuffers::String> offer = 0) {
  P2PAnswerBuilder builder_(_fbb);
  builder_.add_offer(offer);
  builder_.add_peerId(peerId);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<P2PAnswer> CreateP2PAnswerDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	const char *offer = nullptr) {
  auto offer__ = offer ? _fbb.CreateString(offer) : 0;
  return humblenet::HumblePeer::CreateP2PAnswer(
	  _fbb,
	  peerId,
	  offer__);
}

struct P2PConnected FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef P2PConnectedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   verifier.EndTable();
  }
};

struct P2PConnectedBuilder {
  typedef P2PConnected Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(P2PConnected::VT_PEERID, peerId, 0);
  }
  explicit P2PConnectedBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<P2PConnected> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<P2PConnected>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<P2PConnected> CreateP2PConnected(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0) {
  P2PConnectedBuilder builder_(_fbb);
  builder_.add_peerId(peerId);
  return builder_.Finish();
}

struct P2PDisconnect FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef P2PDisconnectBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   verifier.EndTable();
  }
};

struct P2PDisconnectBuilder {
  typedef P2PDisconnect Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(P2PDisconnect::VT_PEERID, peerId, 0);
  }
  explicit P2PDisconnectBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<P2PDisconnect> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<P2PDisconnect>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<P2PDisconnect> CreateP2PDisconnect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0) {
  P2PDisconnectBuilder builder_(_fbb);
  builder_.add_peerId(peerId);
  return builder_.Finish();
}

struct P2PReject FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef P2PRejectBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4,
	VT_REASON = 6
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  humblenet::HumblePeer::P2PRejectReason reason() const {
	return static_cast<humblenet::HumblePeer::P2PRejectReason>(GetField<uint8_t>(VT_REASON, 1));
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   VerifyField<uint8_t>(verifier, VT_REASON, 1) &&
		   verifier.EndTable();
  }
};

struct P2PRejectBuilder {
  typedef P2PReject Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(P2PReject::VT_PEERID, peerId, 0);
  }
  void add_reason(humblenet::HumblePeer::P2PRejectReason reason) {
	fbb_.AddElement<uint8_t>(P2PReject::VT_REASON, static_cast<uint8_t>(reason), 1);
  }
  explicit P2PRejectBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<P2PReject> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<P2PReject>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<P2PReject> CreateP2PReject(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	humblenet::HumblePeer::P2PRejectReason reason = humblenet::HumblePeer::P2PRejectReason::NotFound) {
  P2PRejectBuilder builder_(_fbb);
  builder_.add_peerId(peerId);
  builder_.add_reason(reason);
  return builder_.Finish();
}

struct ICECandidate FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ICECandidateBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4,
	VT_OFFER = 6
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  const ::flatbuffers::String *offer() const {
	return GetPointer<const ::flatbuffers::String *>(VT_OFFER);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   VerifyOffset(verifier, VT_OFFER) &&
		   verifier.VerifyString(offer()) &&
		   verifier.EndTable();
  }
};

struct ICECandidateBuilder {
  typedef ICECandidate Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(ICECandidate::VT_PEERID, peerId, 0);
  }
  void add_offer(::flatbuffers::Offset<::flatbuffers::String> offer) {
	fbb_.AddOffset(ICECandidate::VT_OFFER, offer);
  }
  explicit ICECandidateBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<ICECandidate> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<ICECandidate>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<ICECandidate> CreateICECandidate(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	::flatbuffers::Offset<::flatbuffers::String> offer = 0) {
  ICECandidateBuilder builder_(_fbb);
  builder_.add_offer(offer);
  builder_.add_peerId(peerId);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<ICECandidate> CreateICECandidateDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	const char *offer = nullptr) {
  auto offer__ = offer ? _fbb.CreateString(offer) : 0;
  return humblenet::HumblePeer::CreateICECandidate(
	  _fbb,
	  peerId,
	  offer__);
}

struct P2PRelayData FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef P2PRelayDataBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_PEERID = 4,
	VT_DATA = 6
  };
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  const ::flatbuffers::Vector<int8_t> *data() const {
	return GetPointer<const ::flatbuffers::Vector<int8_t> *>(VT_DATA);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   VerifyOffset(verifier, VT_DATA) &&
		   verifier.VerifyVector(data()) &&
		   verifier.EndTable();
  }
};

struct P2PRelayDataBuilder {
  typedef P2PRelayData Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(P2PRelayData::VT_PEERID, peerId, 0);
  }
  void add_data(::flatbuffers::Offset<::flatbuffers::Vector<int8_t>> data) {
	fbb_.AddOffset(P2PRelayData::VT_DATA, data);
  }
  explicit P2PRelayDataBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<P2PRelayData> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<P2PRelayData>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<P2PRelayData> CreateP2PRelayData(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	::flatbuffers::Offset<::flatbuffers::Vector<int8_t>> data = 0) {
  P2PRelayDataBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_peerId(peerId);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<P2PRelayData> CreateP2PRelayDataDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	uint32_t peerId = 0,
	const std::vector<int8_t> *data = nullptr) {
  auto data__ = data ? _fbb.CreateVector<int8_t>(*data) : 0;
  return humblenet::HumblePeer::CreateP2PRelayData(
	  _fbb,
	  peerId,
	  data__);
}

struct AliasRegister FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef AliasRegisterBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_ALIAS = 4
  };
  const ::flatbuffers::String *alias() const {
	return GetPointer<const ::flatbuffers::String *>(VT_ALIAS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyOffsetRequired(verifier, VT_ALIAS) &&
		   verifier.VerifyString(alias()) &&
		   verifier.EndTable();
  }
};

struct AliasRegisterBuilder {
  typedef AliasRegister Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_alias(::flatbuffers::Offset<::flatbuffers::String> alias) {
	fbb_.AddOffset(AliasRegister::VT_ALIAS, alias);
  }
  explicit AliasRegisterBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<AliasRegister> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<AliasRegister>(end);
	fbb_.Required(o, AliasRegister::VT_ALIAS);
	return o;
  }
};

inline ::flatbuffers::Offset<AliasRegister> CreateAliasRegister(
	::flatbuffers::FlatBufferBuilder &_fbb,
	::flatbuffers::Offset<::flatbuffers::String> alias = 0) {
  AliasRegisterBuilder builder_(_fbb);
  builder_.add_alias(alias);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<AliasRegister> CreateAliasRegisterDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	const char *alias = nullptr) {
  auto alias__ = alias ? _fbb.CreateString(alias) : 0;
  return humblenet::HumblePeer::CreateAliasRegister(
	  _fbb,
	  alias__);
}

struct AliasUnregister FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef AliasUnregisterBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_ALIAS = 4
  };
  const ::flatbuffers::String *alias() const {
	return GetPointer<const ::flatbuffers::String *>(VT_ALIAS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyOffset(verifier, VT_ALIAS) &&
		   verifier.VerifyString(alias()) &&
		   verifier.EndTable();
  }
};

struct AliasUnregisterBuilder {
  typedef AliasUnregister Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_alias(::flatbuffers::Offset<::flatbuffers::String> alias) {
	fbb_.AddOffset(AliasUnregister::VT_ALIAS, alias);
  }
  explicit AliasUnregisterBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<AliasUnregister> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<AliasUnregister>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<AliasUnregister> CreateAliasUnregister(
	::flatbuffers::FlatBufferBuilder &_fbb,
	::flatbuffers::Offset<::flatbuffers::String> alias = 0) {
  AliasUnregisterBuilder builder_(_fbb);
  builder_.add_alias(alias);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<AliasUnregister> CreateAliasUnregisterDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	const char *alias = nullptr) {
  auto alias__ = alias ? _fbb.CreateString(alias) : 0;
  return humblenet::HumblePeer::CreateAliasUnregister(
	  _fbb,
	  alias__);
}

struct AliasLookup FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef AliasLookupBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_ALIAS = 4
  };
  const ::flatbuffers::String *alias() const {
	return GetPointer<const ::flatbuffers::String *>(VT_ALIAS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyOffsetRequired(verifier, VT_ALIAS) &&
		   verifier.VerifyString(alias()) &&
		   verifier.EndTable();
  }
};

struct AliasLookupBuilder {
  typedef AliasLookup Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_alias(::flatbuffers::Offset<::flatbuffers::String> alias) {
	fbb_.AddOffset(AliasLookup::VT_ALIAS, alias);
  }
  explicit AliasLookupBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<AliasLookup> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<AliasLookup>(end);
	fbb_.Required(o, AliasLookup::VT_ALIAS);
	return o;
  }
};

inline ::flatbuffers::Offset<AliasLookup> CreateAliasLookup(
	::flatbuffers::FlatBufferBuilder &_fbb,
	::flatbuffers::Offset<::flatbuffers::String> alias = 0) {
  AliasLookupBuilder builder_(_fbb);
  builder_.add_alias(alias);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<AliasLookup> CreateAliasLookupDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	const char *alias = nullptr) {
  auto alias__ = alias ? _fbb.CreateString(alias) : 0;
  return humblenet::HumblePeer::CreateAliasLookup(
	  _fbb,
	  alias__);
}

struct AliasResolved FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef AliasResolvedBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_ALIAS = 4,
	VT_PEERID = 6
  };
  const ::flatbuffers::String *alias() const {
	return GetPointer<const ::flatbuffers::String *>(VT_ALIAS);
  }
  uint32_t peerId() const {
	return GetField<uint32_t>(VT_PEERID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyOffsetRequired(verifier, VT_ALIAS) &&
		   verifier.VerifyString(alias()) &&
		   VerifyField<uint32_t>(verifier, VT_PEERID, 4) &&
		   verifier.EndTable();
  }
};

struct AliasResolvedBuilder {
  typedef AliasResolved Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_alias(::flatbuffers::Offset<::flatbuffers::String> alias) {
	fbb_.AddOffset(AliasResolved::VT_ALIAS, alias);
  }
  void add_peerId(uint32_t peerId) {
	fbb_.AddElement<uint32_t>(AliasResolved::VT_PEERID, peerId, 0);
  }
  explicit AliasResolvedBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<AliasResolved> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<AliasResolved>(end);
	fbb_.Required(o, AliasResolved::VT_ALIAS);
	return o;
  }
};

inline ::flatbuffers::Offset<AliasResolved> CreateAliasResolved(
	::flatbuffers::FlatBufferBuilder &_fbb,
	::flatbuffers::Offset<::flatbuffers::String> alias = 0,
	uint32_t peerId = 0) {
  AliasResolvedBuilder builder_(_fbb);
  builder_.add_peerId(peerId);
  builder_.add_alias(alias);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<AliasResolved> CreateAliasResolvedDirect(
	::flatbuffers::FlatBufferBuilder &_fbb,
	const char *alias = nullptr,
	uint32_t peerId = 0) {
  auto alias__ = alias ? _fbb.CreateString(alias) : 0;
  return humblenet::HumblePeer::CreateAliasResolved(
	  _fbb,
	  alias__,
	  peerId);
}

struct Message FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef MessageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
	VT_MESSAGE_TYPE = 4,
	VT_MESSAGE = 6
  };
  humblenet::HumblePeer::MessageType message_type() const {
	return static_cast<humblenet::HumblePeer::MessageType>(GetField<uint8_t>(VT_MESSAGE_TYPE, 0));
  }
  const void *message() const {
	return GetPointer<const void *>(VT_MESSAGE);
  }
  template<typename T> const T *message_as() const;
  const humblenet::HumblePeer::HelloServer *message_as_HelloServer() const {
	return message_type() == humblenet::HumblePeer::MessageType::HelloServer ? static_cast<const humblenet::HumblePeer::HelloServer *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::HelloClient *message_as_HelloClient() const {
	return message_type() == humblenet::HumblePeer::MessageType::HelloClient ? static_cast<const humblenet::HumblePeer::HelloClient *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::P2PConnected *message_as_P2PConnected() const {
	return message_type() == humblenet::HumblePeer::MessageType::P2PConnected ? static_cast<const humblenet::HumblePeer::P2PConnected *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::P2PDisconnect *message_as_P2PDisconnect() const {
	return message_type() == humblenet::HumblePeer::MessageType::P2PDisconnect ? static_cast<const humblenet::HumblePeer::P2PDisconnect *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::P2POffer *message_as_P2POffer() const {
	return message_type() == humblenet::HumblePeer::MessageType::P2POffer ? static_cast<const humblenet::HumblePeer::P2POffer *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::P2PAnswer *message_as_P2PAnswer() const {
	return message_type() == humblenet::HumblePeer::MessageType::P2PAnswer ? static_cast<const humblenet::HumblePeer::P2PAnswer *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::P2PReject *message_as_P2PReject() const {
	return message_type() == humblenet::HumblePeer::MessageType::P2PReject ? static_cast<const humblenet::HumblePeer::P2PReject *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::ICECandidate *message_as_ICECandidate() const {
	return message_type() == humblenet::HumblePeer::MessageType::ICECandidate ? static_cast<const humblenet::HumblePeer::ICECandidate *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::P2PRelayData *message_as_P2PRelayData() const {
	return message_type() == humblenet::HumblePeer::MessageType::P2PRelayData ? static_cast<const humblenet::HumblePeer::P2PRelayData *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::AliasRegister *message_as_AliasRegister() const {
	return message_type() == humblenet::HumblePeer::MessageType::AliasRegister ? static_cast<const humblenet::HumblePeer::AliasRegister *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::AliasUnregister *message_as_AliasUnregister() const {
	return message_type() == humblenet::HumblePeer::MessageType::AliasUnregister ? static_cast<const humblenet::HumblePeer::AliasUnregister *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::AliasLookup *message_as_AliasLookup() const {
	return message_type() == humblenet::HumblePeer::MessageType::AliasLookup ? static_cast<const humblenet::HumblePeer::AliasLookup *>(message()) : nullptr;
  }
  const humblenet::HumblePeer::AliasResolved *message_as_AliasResolved() const {
	return message_type() == humblenet::HumblePeer::MessageType::AliasResolved ? static_cast<const humblenet::HumblePeer::AliasResolved *>(message()) : nullptr;
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
	return VerifyTableStart(verifier) &&
		   VerifyField<uint8_t>(verifier, VT_MESSAGE_TYPE, 1) &&
		   VerifyOffset(verifier, VT_MESSAGE) &&
		   VerifyMessageType(verifier, message(), message_type()) &&
		   verifier.EndTable();
  }
};

template<> inline const humblenet::HumblePeer::HelloServer *Message::message_as<humblenet::HumblePeer::HelloServer>() const {
  return message_as_HelloServer();
}

template<> inline const humblenet::HumblePeer::HelloClient *Message::message_as<humblenet::HumblePeer::HelloClient>() const {
  return message_as_HelloClient();
}

template<> inline const humblenet::HumblePeer::P2PConnected *Message::message_as<humblenet::HumblePeer::P2PConnected>() const {
  return message_as_P2PConnected();
}

template<> inline const humblenet::HumblePeer::P2PDisconnect *Message::message_as<humblenet::HumblePeer::P2PDisconnect>() const {
  return message_as_P2PDisconnect();
}

template<> inline const humblenet::HumblePeer::P2POffer *Message::message_as<humblenet::HumblePeer::P2POffer>() const {
  return message_as_P2POffer();
}

template<> inline const humblenet::HumblePeer::P2PAnswer *Message::message_as<humblenet::HumblePeer::P2PAnswer>() const {
  return message_as_P2PAnswer();
}

template<> inline const humblenet::HumblePeer::P2PReject *Message::message_as<humblenet::HumblePeer::P2PReject>() const {
  return message_as_P2PReject();
}

template<> inline const humblenet::HumblePeer::ICECandidate *Message::message_as<humblenet::HumblePeer::ICECandidate>() const {
  return message_as_ICECandidate();
}

template<> inline const humblenet::HumblePeer::P2PRelayData *Message::message_as<humblenet::HumblePeer::P2PRelayData>() const {
  return message_as_P2PRelayData();
}

template<> inline const humblenet::HumblePeer::AliasRegister *Message::message_as<humblenet::HumblePeer::AliasRegister>() const {
  return message_as_AliasRegister();
}

template<> inline const humblenet::HumblePeer::AliasUnregister *Message::message_as<humblenet::HumblePeer::AliasUnregister>() const {
  return message_as_AliasUnregister();
}

template<> inline const humblenet::HumblePeer::AliasLookup *Message::message_as<humblenet::HumblePeer::AliasLookup>() const {
  return message_as_AliasLookup();
}

template<> inline const humblenet::HumblePeer::AliasResolved *Message::message_as<humblenet::HumblePeer::AliasResolved>() const {
  return message_as_AliasResolved();
}

struct MessageBuilder {
  typedef Message Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_message_type(humblenet::HumblePeer::MessageType message_type) {
	fbb_.AddElement<uint8_t>(Message::VT_MESSAGE_TYPE, static_cast<uint8_t>(message_type), 0);
  }
  void add_message(::flatbuffers::Offset<void> message) {
	fbb_.AddOffset(Message::VT_MESSAGE, message);
  }
  explicit MessageBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
		: fbb_(_fbb) {
	start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Message> Finish() {
	const auto end = fbb_.EndTable(start_);
	auto o = ::flatbuffers::Offset<Message>(end);
	return o;
  }
};

inline ::flatbuffers::Offset<Message> CreateMessage(
	::flatbuffers::FlatBufferBuilder &_fbb,
	humblenet::HumblePeer::MessageType message_type = humblenet::HumblePeer::MessageType::NONE,
	::flatbuffers::Offset<void> message = 0) {
  MessageBuilder builder_(_fbb);
  builder_.add_message(message);
  builder_.add_message_type(message_type);
  return builder_.Finish();
}

inline bool VerifyMessageType(::flatbuffers::Verifier &verifier, const void *obj, MessageType type) {
  switch (type) {
	case MessageType::NONE: {
	  return true;
	}
	case MessageType::HelloServer: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::HelloServer *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::HelloClient: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::HelloClient *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::P2PConnected: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::P2PConnected *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::P2PDisconnect: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::P2PDisconnect *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::P2POffer: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::P2POffer *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::P2PAnswer: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::P2PAnswer *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::P2PReject: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::P2PReject *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::ICECandidate: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::ICECandidate *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::P2PRelayData: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::P2PRelayData *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::AliasRegister: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::AliasRegister *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::AliasUnregister: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::AliasUnregister *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::AliasLookup: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::AliasLookup *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	case MessageType::AliasResolved: {
	  auto ptr = reinterpret_cast<const humblenet::HumblePeer::AliasResolved *>(obj);
	  return verifier.VerifyTable(ptr);
	}
	default: return true;
  }
}

inline bool VerifyMessageTypeVector(::flatbuffers::Verifier &verifier, const ::flatbuffers::Vector<::flatbuffers::Offset<void>> *values, const ::flatbuffers::Vector<MessageType> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (::flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
	if (!VerifyMessageType(
		verifier,  values->Get(i), types->GetEnum<MessageType>(i))) {
	  return false;
	}
  }
  return true;
}

inline const humblenet::HumblePeer::Message *GetMessage(const void *buf) {
  return ::flatbuffers::GetRoot<humblenet::HumblePeer::Message>(buf);
}

inline const humblenet::HumblePeer::Message *GetSizePrefixedMessage(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<humblenet::HumblePeer::Message>(buf);
}

inline bool VerifyMessageBuffer(
	::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<humblenet::HumblePeer::Message>(nullptr);
}

inline bool VerifySizePrefixedMessageBuffer(
	::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<humblenet::HumblePeer::Message>(nullptr);
}

inline void FinishMessageBuffer(
	::flatbuffers::FlatBufferBuilder &fbb,
	::flatbuffers::Offset<humblenet::HumblePeer::Message> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedMessageBuffer(
	::flatbuffers::FlatBufferBuilder &fbb,
	::flatbuffers::Offset<humblenet::HumblePeer::Message> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace HumblePeer
}  // namespace humblenet

#endif  // FLATBUFFERS_GENERATED_HUMBLEPEER_HUMBLENET_HUMBLEPEER_H_

/*** End of inlined file: humblepeer_generated.h ***/

#ifdef __GNUC__
#define WARN_UNUSED_RESULT __attribute__ ((warn_unused_result))
#else
#define WARN_UNUSED_RESULT
#endif

namespace humblenet {

	// Ice Server definition
	struct ICEServer {
		HumblePeer::ICEServerType type;
		std::string server;
		std::string username;
		std::string password;

		ICEServer(const std::string& _server)
		: type(HumblePeer::ICEServerType::STUNServer), server(_server) {}
		ICEServer(const std::string& _server, const std::string& _username, const std::string& _password)
		: type(HumblePeer::ICEServerType::TURNServer), server(_server), username(_username), password(_password) {}
	};

	// abstract type for P2P signaling connection
	// we could use C++ classes and inheritance
	// but since there's only ever one type of connection in any binary
	// we opt for static dispatch instead
	struct P2PSignalConnection;
	ha_bool sendP2PMessage(P2PSignalConnection *conn, const uint8_t *buff, size_t length) WARN_UNUSED_RESULT;

	/*
	  P2POffer contains
		PeerID
		  client->server who want to connect to
		  server->client who wants to connect to us
		connectionOffer generated by the browser (stringified JSON)

	   P2PAnswer contains
		PeerID
		connectionOffer generated by the other browser (also str JSON)

	   HelloServer
		auth token which allows server to look up client details in database

	   HelloClient
		PeerID  this is who you are
		IP address?

	  https://developer.mozilla.org/en-US/docs/Web/API/WebRTC_API/Architecture/Connectivity

	  peer1                        server                        peer2

			 P2POffer ->
											   P2POffer ->

											  <- P2PAnswer
			<- P2PAnswer
			 ICECandidate ->
												ICECandidate ->
											 <- ICECandidate
			<- ICECandidate
	*/

	typedef ha_bool(*ProcessMsgFunc) (const HumblePeer::Message* msg, void *data);

	/*
	 returns true if all went wel
			 false if something is wrong, sync is lost and channel should be closed
			 TODO: should this be a tristate (process, noprocess, error)?
			 does anyone care if progress happened or not?
	 */
	ha_bool parseMessage(std::vector<uint8_t> &recvBuf, ProcessMsgFunc processFunc, void *user_data);

	// Peer server connection
	ha_bool sendHelloServer(humblenet::P2PSignalConnection *conn, uint8_t flags,
							const std::string& gametoken, const std::string& gamesecret,
							const std::string& authToken, const std::string& reconnectToken,
							const std::map<std::string, std::string>& attributes);
	ha_bool sendHelloClient(humblenet::P2PSignalConnection *conn, PeerId peerId,
							const std::string& reconnectToken,
							const std::vector<ICEServer>& iceServers);

	// P2P Handling
	ha_bool sendNoSuchPeer(humblenet::P2PSignalConnection *conn, PeerId peerId);
	ha_bool sendPeerRefused(humblenet::P2PSignalConnection *conn, PeerId peerId);
	ha_bool sendP2PConnect(P2PSignalConnection *conn, PeerId peerId, uint8_t flags, const char* offer);
	ha_bool sendP2PResponse(P2PSignalConnection *conn, PeerId peerId, const char* offer);
	ha_bool sendICECandidate(humblenet::P2PSignalConnection *conn, PeerId peerId, const char* offer);
	ha_bool sendP2PDisconnect(humblenet::P2PSignalConnection *conn, PeerId peer);
	ha_bool sendP2PRelayData(humblenet::P2PSignalConnection *conn, PeerId peer, const void* data, uint16_t length);

	// Name Alias
	ha_bool sendAliasRegister(P2PSignalConnection *conn, const std::string& alias);
	ha_bool sendAliasUnregister(P2PSignalConnection *conn, const std::string& alias);
	ha_bool sendAliasLookup(P2PSignalConnection *conn, const std::string& alias);
	ha_bool sendAliasResolved(P2PSignalConnection *conn, const std::string& alias, PeerId peer);

}  // namespace humblenet

#endif  // HUMBLEPEER_H

/*** End of inlined file: humblepeer.h ***/

#include <vector>

namespace humblenet {
	struct P2PSignalConnection {
		internal_socket_t *wsi;
		std::vector<uint8_t> recvBuf;
		std::vector<char> sendBuf;

		P2PSignalConnection()
		: wsi(NULL)
		{
		}

		void disconnect() {
			if( wsi )
				internal_close_socket(wsi);
			wsi = NULL;
		}
	};

	void register_protocol( internal_context_t* contet );
}

ha_bool humblenet_signaling_connect();

#endif // HUMBLENET_SIGNALING

/*** End of inlined file: humblenet_p2p_signaling.h ***/

#include <memory>
#include <deque>

// TODO: should have a way to disable this on release builds
#define LOG printf
#define TRACE( ... )

enum InOrOut
{
	Incoming
	, Outgoing
};

// Connection status
typedef enum ConnectionStatus {
	HUMBLENET_CONNECTION_CLOSED,
	HUMBLENET_CONNECTION_CONNECTING,
	HUMBLENET_CONNECTION_CONNECTED,
} ConnectionStatus;

struct Connection {
	InOrOut inOrOut;
	ConnectionStatus status;

	// peerId of the other peer
	// 0 if not a p2p connection
	PeerId otherPeer;

	std::deque<std::vector<char>> recvBuffer;

	ha_bool writable;

	struct internal_socket_t* socket;

	Connection( InOrOut inOrOut_, struct internal_socket_t *s = NULL)
	: inOrOut(inOrOut_)
	, status(HUMBLENET_CONNECTION_CONNECTING)
	, otherPeer(0)
	, writable(true)
	, socket(NULL)
	{
	}
};

typedef struct HumbleNetState {
	// established connections indexed by socket id
	// pointer not owned
	std::unordered_map<internal_socket_t *, Connection *> connections;

	// new incoming connections waiting accept
	std::unordered_set<Connection *> pendingNewConnections;

	// connections with pending data in buffer
	// pointer not owned
	std::unordered_set<Connection *> pendingDataConnections;

	// connections which have been closed by remote and need user notification
	std::unordered_set<Connection *> remoteClosedConnections;

	// pending outgoing (not established) p2p connections
	// pointer not owned (owned by whoever called humblenet_connect_peer)
	std::unordered_map<PeerId, Connection *> pendingPeerConnectionsOut;

	// pending incoming P2P connections
	// these ARE owned (until humblenet_connection_accept which removes them)
	std::unordered_map<PeerId, Connection *> pendingPeerConnectionsIn;

	// outgoing pending (not established) alias connections
	// pointer not owned
	std::unordered_map<std::string, Connection *> pendingAliasConnectionsOut;

	// map of peers that are blacklisted, value is when they they were blacklisted
	// incoming peers are added to this list when they are disconnected.
	// this is to prevent anemic connection attempts.
	std::unordered_map<PeerId, uint64_t> peerBlacklist;

	PeerId myPeerId;

	std::unique_ptr<humblenet::P2PSignalConnection> p2pConn;

	std::string signalingServerAddr;
	std::string gameToken;
	std::string gameSecret;
	std::string authToken;
	std::string reconnectToken;
	std::vector<humblenet::ICEServer> iceServers;

	ha_bool webRTCSupported;

	internal_context_t *context;

	HumbleNetState()
	:  myPeerId(0)
	, webRTCSupported(false)
	, context(NULL)
	{
	}
} HumbleNetState;

extern HumbleNetState humbleNetState;

#define INIT_GUARD( msg, check, ... ) \
if( ! (check) ) { humblenet_set_error( msg ); return __VA_ARGS__ ; }

// Initialize P2P system with websocket layer
ha_bool internal_p2p_register_protocol();

// internal...
void internal_poll_io();
void humblenet_connection_set_closed( Connection* conn );
bool is_peer_blacklisted( PeerId peer );
void blacklist_peer( PeerId peer );
void signal();

typedef void(*timer_callback_t)(void* data);
void humblenet_timer( timer_callback_t callback, int timeout, void* data);

void humblenet_lock();
void humblenet_unlock();

struct Guard {
	Guard() {
		humblenet_lock();
	}

	~Guard() {
		humblenet_unlock();
	}
};

struct UnGuard {
	UnGuard() {
		humblenet_unlock();
	}

	~UnGuard() {
		humblenet_lock();
	}
};

#ifdef __EMSCRIPTEN__
	#define HUMBLENET_GUARD()
	#define HUMBLENET_UNGUARD()
#else
	#define HUMBLENET_GUARD() Guard lock
#define HUMBLENET_UNGUARD() UnGuard unlock
#endif

/*
 * Shut down the P2P networking library
 */
HUMBLENET_API void HUMBLENET_CALL humblenet_p2p_shutdown();

/*
 * Get connection status
 */
ConnectionStatus humblenet_connection_status(Connection *connection);

/*
 * Connect to server server_addr
 */
Connection *humblenet_connect_websocket(const char *server_addr);

/*
 * Connect to peer peer_id
 */
Connection *humblenet_connect_peer(PeerId peer_id);

/*
 * Close the Connection
 */
void humblenet_connection_close(Connection *connection);

/*
 * Returns true if connection has data to read, false otherwise
 */
ha_bool humblenet_connection_is_readable(Connection *connection);

/*
 * Returns true if connection can be written into, false otherwise
 */
ha_bool humblenet_connection_is_writable(Connection *connection);

/*
 * Send data through Connection
 * Returns the number of bytes sent, -1 on error
 */
int humblenet_connection_write(Connection *connection, const void *buf, uint32_t bufsize);

/*
 * Receive data through Connection
 * Returns the number of bytes received, -1 on error
 */
int humblenet_connection_read(Connection *connection, void *buf, uint32_t bufsize);

/*
 * Poll all connections
 * timeout_ms Timeout in milliseconds
 * 0 = return immediately if no pending connections
 * negative = wait indefinitely
 * returns a connection with readable data, NULL if no such connections
 */
Connection *humblenet_poll_all(int timeout_ms);

/*
 * Poll a single connection
 * connection to poll
 * timeout_ms Timeout in milliseconds
 * 0 = return immediately if no pending connections
 * negative = wait indefinitely
 * returns true if readable
 */
ha_bool humblenet_connection_poll(Connection *connection, int timeout_ms);

/*
 * Accepts new connections
 * returns a new Connection (which you now own) or NULL if no new connections
 * Humblenet will include this connection in future humblenet_connection_poll calls
 */
Connection *humblenet_connection_accept();

/*
 * Get the PeerId of this connection
 * returns PeerId or 0 if not a p2p connection
 */
PeerId humblenet_connection_get_peer_id(Connection *connection);

uint64_t sys_milliseconds();

#endif // HUMBLENET_P2P_INTERNAL

/*** End of inlined file: humblenet_p2p_internal.h ***/


/*** Start of inlined file: humblenet_alias.h ***/
#ifndef HUMBLENET_ALIAS_H
#define HUMBLENET_ALIAS_H

#ifndef HUMBLENET_P2P_INTERNAL
#error Cannot use this header outside humblenet
#endif

/**
 * Register an alias for this peer.
 *
 *
 * NOTE: The peer server can reject the alias, but that will notify via the events.
 */
ha_bool internal_alias_register(const char* alias );

/**
 * Unregister an alias for this peer.
 *
 *
 * Passing NULL means unregister ALL aliases
 */
ha_bool internal_alias_unregister(const char* alias );

/**
 * Return a virtual PeerId that represents the alias.
 *
 */
PeerId internal_alias_lookup(const char* alias );

/**
 * Is this peer id a vritual peer id
 */
ha_bool internal_alias_is_virtual_peer( PeerId peer );

/**
 * Get the virtual peer associated with this conenction (if there is one)
 *
 */
PeerId internal_alias_get_virtual_peer( Connection* conn );

/**
 * Find the connection associated with the virtual peer
 *
 */
Connection* internal_alias_find_connection( PeerId );

/**
 * Setup a connection to the virtual peer
 *
 */
Connection* internal_alias_create_connection( PeerId );

/**
 * Remove the connection from the alias system
 *
 */
void internal_alias_remove_connection( Connection* conn );

/**
 * Called when the peer server resolves the peerid for a a name
 *
 */
void internal_alias_resolved_to( const std::string& alias, PeerId peer );

#endif // HUMBLENET_ALIAS_H

/*** End of inlined file: humblenet_alias.h ***/

#define NOMINMAX

#include <cassert>

#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#if defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#else
	#include <sys/time.h>
#endif

#if defined(__EMSCRIPTEN__)
	#include <emscripten/emscripten.h>
#endif


/*** Start of inlined file: humblenet_utils.h ***/
#ifndef HUMBLENET_UTILS_H
#define HUMBLENET_UTILS_H

template <typename A, typename B>
class BidirectionalMap {
public:
	typedef A key_type;
	typedef B mapped_type;
	typedef std::map<A, B> AtoB;
	typedef std::map<B, A> BtoA;
private:
	AtoB list_a;
	BtoA list_b;
public:
	void insert(const A& a, const B& b) {
		list_a.emplace(a, b);
		list_b.emplace(b, a);
	}

	void erase(const A& a) {
		auto it = list_a.find(a);
		if (it != list_a.end()) {
			list_b.erase(it->second);
			list_a.erase(it);
		}
	}

	void erase(const B& a) {
		auto it = list_b.find(a);
		if (it != list_b.end()) {
			list_a.erase(it->second);
			list_b.erase(it);
		}
	}

	void erase(typename AtoB::iterator& it) {
		list_b.erase(it->second);
		list_a.erase(it);
	}

	void erase(typename BtoA::iterator& it) {
		list_a.erase(it->second);
		list_b.erase(it);
	}

	typename AtoB::iterator find(const A& a) { return list_a.find(a); }

	typename BtoA::iterator find(const B& b) { return list_b.find(b); }

	bool is_end(typename AtoB::iterator& it) {
		return list_a.end() == it;
	}

	bool is_end(typename BtoA::iterator& it) {
		return list_b.end() == it;
	}
};

template< class MapType >
void erase_value( MapType& map, const typename MapType::mapped_type& value ) {
	for( auto it = map.begin(); it != map.end(); ){
		if( it->second == value )
			map.erase( it++ );
		else
			++it;
	}
}

#endif // HUMBLENET_UTILS_H

/*** End of inlined file: humblenet_utils.h ***/

#define USE_STUN

HumbleNetState humbleNetState;

#ifdef WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
uint64_t sys_milliseconds(void)
{
	FILETIME ft;
	ULARGE_INTEGER temp;

	GetSystemTimeAsFileTime(&ft);
	memcpy(&temp, &ft, sizeof(temp));

	return (temp.QuadPart - DELTA_EPOCH_IN_MICROSECS) / 100000;
}

#else
uint64_t sys_milliseconds (void)
{
	struct timeval tp;
	struct timezone tzp;
	static uint64_t	secbase = 0;

	gettimeofday(&tp, &tzp);

	if (!secbase)
	{
		secbase = tp.tv_sec;
		return tp.tv_usec/1000;
	}

	return (tp.tv_sec - secbase)*1000 + tp.tv_usec/1000;
}
#endif

void signal();

void blacklist_peer( PeerId peer ) {
	humbleNetState.peerBlacklist.insert( std::make_pair( peer, sys_milliseconds() + 5000 ) );
}

/*
 * Deterimine if the peer is currently blacklisted.
 *
 * Side effect: will clear expired blacklist entries.
 */
bool is_peer_blacklisted( PeerId peer_id ) {
	auto it = humbleNetState.peerBlacklist.find(peer_id);
	if( it == humbleNetState.peerBlacklist.end() )
		return false;

	if( it->second <= sys_milliseconds() ) {
		humbleNetState.peerBlacklist.erase(it);
		return true;
	}

	return true;
}

/*
 * See if we can try a connection to this peer
 *
 * Side effect: sets error to reason on failure
 */
bool can_try_peer( PeerId peer_id ) {
	if (!humbleNetState.p2pConn) {
		humblenet_set_error("Signaling connection not established");
		// no signaling connection
		return false;
	}
	if (humbleNetState.myPeerId == 0) {
		humblenet_set_error("No peer ID from server");
		return false;
	}
	if (humbleNetState.pendingPeerConnectionsOut.find(peer_id)
		!= humbleNetState.pendingPeerConnectionsOut.end()) {
		// error, already a pending outgoing connection to this peer
		humblenet_set_error("already a pending connection to peer");
		LOG("humblenet_connect_peer: already a pending connection to peer %u\n", peer_id);
		return false;
	}
	if( is_peer_blacklisted(peer_id) ) {
		// peer is currently black listed
		humblenet_set_error("peer blacklisted");
		LOG("humblenet_connect_peer: peer blacklisted %u\n", peer_id);
		return false;
	}

	return true;
}

// BEGIN CONNECTION HANDLING

void humblenet_connection_set_closed( Connection* conn ) {
	if( conn->socket ) {
		// clear our state first so callbacks dont work on us.
		humbleNetState.connections.erase(conn->socket);
		internal_set_data(conn->socket, NULL );
		{
			HUMBLENET_UNGUARD();
			internal_close_socket(conn->socket);
		}
		conn->socket = NULL;
	}

	if( conn->inOrOut == Incoming ) {
		blacklist_peer( conn->otherPeer );
	}

	conn->status = HUMBLENET_CONNECTION_CLOSED;

	LOG("Marking connections closed: %u\n", conn->otherPeer );

	// make sure were not in any lists...
	erase_value( humbleNetState.connections, conn );
	humbleNetState.pendingNewConnections.erase( conn );
	humbleNetState.pendingDataConnections.erase( conn );
//	humbleNetState.remoteClosedConnections.erase( conn );
	erase_value( humbleNetState.pendingPeerConnectionsOut, conn );
	erase_value( humbleNetState.pendingPeerConnectionsIn, conn );
	erase_value( humbleNetState.pendingAliasConnectionsOut, conn );

	// mark it as being close pending.
	humbleNetState.remoteClosedConnections.insert( conn );

	signal();
}

ha_bool humblenet_connection_is_readable(Connection *connection) {
	return !connection->recvBuffer.empty();
}

ha_bool humblenet_connection_is_writable(Connection *connection) {
	assert(connection != NULL);
	if (connection->status == HUMBLENET_CONNECTION_CONNECTED) {
		return connection->writable;
	}
	return false;
}

int humblenet_connection_write(Connection *connection, const void *buf, uint32_t bufsize) {
	assert(connection != NULL);

	switch (connection->status) {
		case HUMBLENET_CONNECTION_CLOSED:
			// connection has been closed
			assert(connection->socket == NULL);
			return -1;

		case HUMBLENET_CONNECTION_CONNECTING:
			assert(connection->socket != NULL);
			return 0;

		case HUMBLENET_CONNECTION_CONNECTED:
			assert(connection->socket != NULL);

			const char* use_relay = humblenet_get_hint("p2p_use_relay");
			if( use_relay && *use_relay == '1' ) {
				if( ! sendP2PRelayData( humbleNetState.p2pConn.get(), connection->otherPeer, buf, bufsize ) ) {
					return -1;
				}
				return bufsize;
			}
		{
			HUMBLENET_UNGUARD();
			return internal_write_socket( connection->socket, buf, bufsize );
		}
	}
	return -1;
}

int humblenet_connection_read(Connection *connection, void *buf, uint32_t bufsize) {
	assert(connection != NULL);

	switch (connection->status) {
		case HUMBLENET_CONNECTION_CLOSED:
			// connection has been closed
			assert(connection->socket == NULL);
			return -1;

		case HUMBLENET_CONNECTION_CONNECTING:
			assert(connection->socket != NULL);
			return 0;

		case HUMBLENET_CONNECTION_CONNECTED:
			assert(connection->socket != NULL);

			if( connection->recvBuffer.empty() )
				return 0;

			bufsize = std::min<uint32_t>(bufsize, connection->recvBuffer[0].size() );
			memcpy(buf, &connection->recvBuffer[0][0], bufsize);
			connection->recvBuffer.pop_front();

			if( ! connection->recvBuffer.empty() )
				humbleNetState.pendingDataConnections.insert(connection);
			else
				humbleNetState.pendingDataConnections.erase(connection);

			return bufsize;
	}
	return -1;
}

ConnectionStatus humblenet_connection_status(Connection *connection) {
	assert(connection != NULL);

	return connection->status;
}

Connection *humblenet_connect_websocket(const char *server_addr) {
	humblenet_set_error("Websocket support deprecated");
	return NULL;
}

Connection *humblenet_connect_peer(PeerId peer_id) {
	if( ! can_try_peer( peer_id ) )
		return NULL;

	Connection *connection = new Connection(Outgoing);
	connection->otherPeer = peer_id;
	{
		HUMBLENET_UNGUARD();
		connection->socket = internal_create_webrtc(humbleNetState.context);
	}
	internal_set_data(connection->socket, connection);

	humbleNetState.pendingPeerConnectionsOut.emplace(peer_id, connection);

	int ret;

	{
		HUMBLENET_UNGUARD();
		ret = internal_create_offer( connection->socket );
	}

	if( ! ret ) {
		LOG("Unable to generate sdp offer to peer: %d\n", peer_id);
		humblenet_set_error("Unable to generate sdp offer");
		humblenet_connection_close(connection);
		return NULL;
	}

	LOG("connecting to peer: %d\n", peer_id);
	return connection;
}

void humblenet_connection_close(Connection *connection) {
	assert(connection != NULL);

	humblenet_connection_set_closed(connection);

	internal_alias_remove_connection( connection );

	humbleNetState.remoteClosedConnections.erase(connection);

	delete connection;
}

PeerId humblenet_connection_get_peer_id(Connection *connection) {
	assert(connection != NULL);

	return connection->otherPeer;
}

// called to return the sdp offer
int on_sdp( internal_socket_t* s, const char* offer, void* user_data ) {
	HUMBLENET_GUARD();

	Connection* conn = reinterpret_cast<Connection*>(user_data);

	if( conn == NULL ) {
		LOG("on_sdp: Got socket w/o state?\n");
		return -1;
	}

	assert( conn->status == HUMBLENET_CONNECTION_CONNECTING );

	// no trickle ICE
	int flags = 0x2;

	if( conn->inOrOut == Incoming ) {
		LOG("P2PConnect SDP sent %u response offer = \"%s\"\n", conn->otherPeer, offer);
		if( ! sendP2PResponse(humbleNetState.p2pConn.get(), conn->otherPeer, offer) ) {
			return -1;
		}
	} else {
		LOG("outgoing SDP sent %u offer: \"%s\"\n", conn->otherPeer, offer);
		if( ! sendP2PConnect(humbleNetState.p2pConn.get(), conn->otherPeer, flags, offer) ) {
			return -1;
		}
	}
	return 0;
}

// called to send a candidate
int on_ice_candidate( internal_socket_t* s, const char* offer, void* user_data ) {
	HUMBLENET_GUARD();

	Connection* conn = reinterpret_cast<Connection*>(user_data);

	if( conn == NULL ) {
		LOG("on_ice: Got socket w/o state?\n");
		return -1;
	}

	// This is OK with trickle ICE
	// assert( conn->status == HUMBLENET_CONNECTION_CONNECTING );

	LOG("Sending ice candidate to peer: %u, %s\n", conn->otherPeer, offer );
	if( ! sendICECandidate(humbleNetState.p2pConn.get(), conn->otherPeer, offer) ) {
		return -1;
	}

	return 0;
}

// called for incoming connections to indicate the connection process is completed.
int on_accept (internal_socket_t* s, void* user_data) {
	HUMBLENET_GUARD();

	auto it = humbleNetState.connections.find( s );

	// TODO: Rework this ?
	Connection* conn = NULL;
	if( it == humbleNetState.connections.end() ) {
		// its a websocket connection
		conn = new Connection( Incoming, s );

		// track the connection, ALL connections will reside here.
		humbleNetState.connections.insert( std::make_pair( s, conn ) );
	} else {
		// should be a webrtc connection (as we create the socket in order to start the process)
		conn = it->second;
	}

	// TODO: This should be "waiting for accept"
	conn->status = HUMBLENET_CONNECTION_CONNECTED;

	// expose it as an incoming connection to be accepted...
	humbleNetState.pendingNewConnections.insert( conn );

	LOG("accepted peer: %d\n", conn->otherPeer );

	signal();

	return 0;
}

// called for outgoing connections to indicate the connection process is completed.
int on_connect (internal_socket_t* s, void* user_data) {
	HUMBLENET_GUARD();

	assert( user_data );

	auto it = humbleNetState.connections.find( s );

	// TODO: Rework this ?
	Connection* conn = reinterpret_cast<Connection*>(user_data);
	if( it == humbleNetState.connections.end() ) {
		// track the connection, ALL connections will reside here.
		humbleNetState.connections.insert( std::make_pair( s, conn ) );
	}

	assert( conn->status == HUMBLENET_CONNECTION_CONNECTING );

	// outgoing always initiates the channel
	if( conn->inOrOut == Outgoing ) {
		if( ! internal_create_channel(s, "dataChannel") )
			return -1;
	}

	LOG("connected peer: %d\n", conn->otherPeer );

	return 0;
}

int on_accept_channel( internal_socket_t* s, const char* name, void* user_data ) {
	HUMBLENET_GUARD();

	assert( user_data );

	Connection* conn = reinterpret_cast<Connection*>(user_data);

	assert( conn->status == HUMBLENET_CONNECTION_CONNECTING );
	conn->status = HUMBLENET_CONNECTION_CONNECTED;

	LOG("accepted channel: %d:%s\n", conn->otherPeer, name );

	signal();

	return 0;
}

int on_connect_channel( internal_socket_t* s, const char* name, void* user_data ) {
	HUMBLENET_GUARD();

	assert( user_data );

	Connection* conn = reinterpret_cast<Connection*>(user_data);

	assert( conn->status == HUMBLENET_CONNECTION_CONNECTING );
	conn->status = HUMBLENET_CONNECTION_CONNECTED;

	LOG("connected channel: %d:%s\n", conn->otherPeer, name );
	return 0;
}

// called each time data is received.
int on_data( internal_socket_t* s, const void* data, int len, void* user_data ) {
	HUMBLENET_GUARD();

	// already disconnected from this socket.
	if( ! user_data )
		return -1;

	Connection* conn = reinterpret_cast<Connection*>(user_data);

	assert( conn->status == HUMBLENET_CONNECTION_CONNECTED );

	if( conn->recvBuffer.empty() ) {
		assert( humbleNetState.pendingDataConnections.find(conn) == humbleNetState.pendingDataConnections.end() );
		humbleNetState.pendingDataConnections.insert(conn);
	}

	conn->recvBuffer.emplace_back(reinterpret_cast<const char*>(data),
							reinterpret_cast<const char*>(data)+len);

	signal();

	return 0;
}

// called to indicate the connection is wriable.
int on_writable (internal_socket_t* s, void* user_data) {
	HUMBLENET_GUARD();

	auto it = humbleNetState.connections.find( s );

	if( it != humbleNetState.connections.end() ) {
		// its not here anymore
		return -1;
	}

	Connection* conn = it->second;
	conn->writable = true;

	return 0;
}

// called when the conenction is terminated (from either side)
int on_disconnect( internal_socket_t* s, void* user_data ) {
	HUMBLENET_GUARD();

	// find Connection object
	auto it = humbleNetState.connections.find(s);

	if (it != humbleNetState.connections.end()) {
		// it's still in connections map, not user-initiated close
		// find Connection object and set wsi to NULL
		// to signal that it's dead
		// user code will then call humblenet_close to dispose of
		// the Connection object

		humblenet_connection_set_closed( it->second );
	} else if( user_data != NULL ) {
		// just to be sure...
		humblenet_connection_set_closed( reinterpret_cast<Connection*>( user_data ) );
	}
	// if it wasn't in the table, no need to do anything
	// humblenet_close has already disposed of everything else
	return 0;
}

int on_destroy( internal_socket_t* s, void* user_data ) {
	HUMBLENET_GUARD();

	// if no user_data attached, then nothing to cleanup.
	if( ! user_data )
		return 0;

	// make sure we are fully detached...
	Connection* conn = reinterpret_cast<Connection*>(user_data);
	humblenet_connection_set_closed( conn );

	return 0;
}

// END CONNECTION HANDLING

// BEGIN INITIALIZATION

uint32_t HUMBLENET_CALL humblenet_version() {
	return HUMBLENET_COMPILEDVERSION;
}

ha_bool HUMBLENET_CALL humblenet_loader_init(const char* path) {
	return true;
}

ha_bool HUMBLENET_CALL humblenet_init() {
	return true;
}

ha_bool internal_p2p_register_protocol() {
	internal_callbacks_t callbacks;

	memset(&callbacks, 0, sizeof(callbacks));

	callbacks.on_sdp = on_sdp;
	callbacks.on_ice_candidate = on_ice_candidate;
	callbacks.on_accept = on_accept;
	callbacks.on_connect = on_connect;
	callbacks.on_accept_channel = on_accept_channel;
	callbacks.on_connect_channel = on_connect_channel;
	callbacks.on_data = on_data;
	callbacks.on_disconnect = on_disconnect;
	callbacks.on_writable = on_writable;

	humbleNetState.context = internal_init(  &callbacks );
	if (humbleNetState.context == NULL) {
		return false;
	}

	humblenet::register_protocol(humbleNetState.context);

	humbleNetState.webRTCSupported = internal_supports_webRTC( humbleNetState.context );

	return true;
}

#ifndef __EMSCRIPTEN__
extern "C" void poll_deinit();
#endif

void HUMBLENET_CALL humblenet_shutdown() {
	{
		HUMBLENET_GUARD();

		// Destroy all connections
		for( auto it = humbleNetState.connections.begin(); it != humbleNetState.connections.end(); ) {
			Connection* conn = it->second;
			++it;
			humblenet_connection_close( conn );
		}
	}

	humblenet_p2p_shutdown();

#ifndef __EMSCRIPTEN__
	poll_deinit();
#endif

}

// END INITIALIZATION

// BEGIN MISC

ha_bool HUMBLENET_CALL humblenet_p2p_supported() {
	// This function should really be "humblenet_has_webrtc"
	// but could be removed entirely from the API
	return humbleNetState.webRTCSupported;
}

#if defined(WIN32)
#define THREAD_LOCAL __declspec(thread)
#else // Everyone else
#define THREAD_LOCAL __thread
#endif

THREAD_LOCAL const char* errorString = nullptr;

const char * HUMBLENET_CALL humblenet_get_error() {
	return errorString;
}

void HUMBLENET_CALL humblenet_set_error(const char *error) {
	errorString = error;
}

void HUMBLENET_CALL humblenet_clear_error() {
	errorString = nullptr;
}

// END MISC

// BEGIN DEPRECATED

Connection *humblenet_poll_all(int timeout_ms) {
	{
		// check for connections closed by remote and notify user
		auto it = humbleNetState.remoteClosedConnections.begin();
		if (it != humbleNetState.remoteClosedConnections.end()) {
			Connection *conn = *it;
			humbleNetState.remoteClosedConnections.erase(it);
			return conn;
		}

		it = humbleNetState.pendingDataConnections.begin();
		if (it != humbleNetState.pendingDataConnections.end()) {
			// don't remove it from the set, _recv will do that
			Connection *conn = *it;
			assert(conn != NULL);
			assert(!conn->recvBuffer.empty());
			return conn;
		}
	}

	// call service if no outstanding data on any connection
	internal_poll_io(/*timeout_ms*/);

	{
		auto it = humbleNetState.pendingDataConnections.begin();
		if (it != humbleNetState.pendingDataConnections.end()) {
			// don't remove it from the set, _recv will do that
			Connection *conn = *it;
			assert(conn != NULL);
			assert(!conn->recvBuffer.empty());
			return conn;
		}
	}

	return NULL;
}

Connection *humblenet_connection_accept() {
	{
		auto it = humbleNetState.pendingNewConnections.begin();

		if (it != humbleNetState.pendingNewConnections.end()) {
			Connection *conn = *it;
			humbleNetState.pendingNewConnections.erase(it);
			return conn;
		}
	}

	// call service so something happens
	// TODO: if we replace poll loop with our own we could poll only the master socket
	internal_poll_io();

	{
		auto it = humbleNetState.pendingNewConnections.begin();
		if (it != humbleNetState.pendingNewConnections.end()) {
			Connection *conn = *it;
			humbleNetState.pendingNewConnections.erase(it);
			return conn;
		}
	}

	return NULL;
}

// END DEPRECATED

static std::unordered_map<std::string, std::string> hints;

/*
 * Set the value of a hint
 */
HUMBLENET_API ha_bool HUMBLENET_CALL humblenet_set_hint(const char* name, const char* value) {
	HUMBLENET_GUARD();

	auto it = hints.find( name );
	if( it != hints.end() )
		it->second = value;
	else
		hints.insert( std::make_pair( name, value ) );
	return 1;
}

/*
 * Get the value of a hint
 */
HUMBLENET_API const char* HUMBLENET_CALL humblenet_get_hint(const char* name) {
	HUMBLENET_GUARD();

	auto it = hints.find( name );
	if( it != hints.end() )
		return it->second.c_str();
	else
		return NULL;
}

#ifndef __EMSCRIPTEN__

#include "libpoll.h"	// SKIP_AMALGAMATOR_INCLUDE

void humblenet_lock() {
	poll_lock();
}

void humblenet_unlock() {
	poll_unlock();
}

void signal() {
	poll_interrupt();
}

void humblenet_timer( timer_callback_t callback, int timeout, void* data)
{
	poll_timeout( callback, timeout, data );
}

#else // __EMSCRIPTEN__

void humblenet_lock() {
}

void humblenet_unlock() {
}

void signal () {
}

void humblenet_timer( timer_callback_t callback, int timeout, void* data)
{
	EM_ASM_({
		window.setTimeout( function(){
			dynCall('vi', $0, [$2]);
		}, $1);

	}, callback, timeout, data );
}

#endif

/*** End of inlined file: humblenet_core.cpp ***/


// Datagram

/*** Start of inlined file: humblenet_datagram.cpp ***/

/*** Start of inlined file: humblenet_datagram.h ***/
#ifndef HUMBLENET_DATAGRAM_H
#define HUMBLENET_DATAGRAM_H

#define HUMBLENET_MSG_PEEK 0x1
#define HUMBLENET_MSG_BUFFERED 0x02

/*
* Send a message to a connection
*/
int humblenet_datagram_send( const void* message, size_t length, int flags, struct Connection* toconn, uint8_t channel );

/*
* Receive a message sent from a connection
*/
int humblenet_datagram_recv( void* buffer, size_t length, int flags, struct Connection** fromconn, uint8_t channel );

/*
* See if there is a message waiting on the specified channel
*/
ha_bool humblenet_datagram_select( size_t* length, uint8_t channel );

/*
* Flush any pending packets
*/
ha_bool humblenet_datagram_flush();

#endif // HUMBLENET_DATAGRAM_H

/*** End of inlined file: humblenet_datagram.h ***/

// TODO : If this had access to the internals of Connection it could be further optimized.

#include <map>
#include <vector>
#include <stdlib.h>
#include <cstring>
#include <stdio.h>
#include <algorithm>

struct datagram_connection {
	Connection*			conn;			// established connection.
	PeerId				peer;			// "address"

	// packets in and out
	std::deque<std::vector<char>>	buf_in;
	std::deque<std::vector<char>>	buf_out;

	datagram_connection( Connection* conn, bool outgoing )
	:conn( conn )
	,peer( humblenet_connection_get_peer_id( conn ) )
	{
	}
};

typedef std::map<Connection*, datagram_connection> ConnectionMap;

static ConnectionMap	connections;
static bool				queuedPackets = false;

static int datagram_get_message( datagram_connection& conn, void* buffer, size_t length, int flags, uint8_t channel ) {

	assert(channel == 0);
	if (conn.buf_in.empty())
		return 0;
	size_t len = std::min(conn.buf_in.front().size(), length);
	if( flags & HUMBLENET_MSG_PEEK )
		return len;
	memcpy(buffer, &conn.buf_in.front()[0], len);
	conn.buf_in.pop_front();
	return len;
}

static void datagram_flush( datagram_connection& dg, const char* reason ) {
	if( ! dg.buf_out.empty() )
	{
		if( ! humblenet_connection_is_writable( dg.conn ) ) {
			LOG("Waiting(%s) %zu packets to  %p\n", reason, dg.buf_out.size(), dg.conn );
			return;
		}

		while( ! dg.buf_out.empty() ) {
			int ret = humblenet_connection_write( dg.conn, dg.buf_out.front().data(), dg.buf_out.front().size() );
			dg.buf_out.pop_front();
			if( ret < 0 ) {
				LOG("Error flushing packets: %s\n", humblenet_get_error() );
				humblenet_clear_error();
			}
		}
	}
}

int humblenet_datagram_send( const void* message, size_t length, int flags, Connection* conn, uint8_t channel )
{
	// if were still connecting, we can't write yet
	// TODO: Should we queue that data up?
	switch( humblenet_connection_status( conn ) ) {
		case HUMBLENET_CONNECTION_CONNECTING: {
			// If send using buffered, then allow messages to be queued till were connected.
			if( flags & HUMBLENET_MSG_BUFFERED )
				break;

			humblenet_set_error("Connection is still being established");
			return 0;
		}
		case HUMBLENET_CONNECTION_CLOSED: {
			humblenet_set_error("Connection is closed");
			// should this wipe the state ?
			return -1;
		}
		default: {
			break;
		}
	}

	ConnectionMap::iterator it = connections.find( conn );
	if( it == connections.end() ) {
		connections.insert( ConnectionMap::value_type( conn, datagram_connection( conn, false ) ) );
		it = connections.find( conn );
	}

	datagram_connection& dg = it->second;

	dg.buf_out.emplace_back(reinterpret_cast<const char*>( message ), reinterpret_cast<const char*>( message ) + length);

	// if( !( flags & HUMBLENET_MSG_BUFFERED ) ) {
		datagram_flush( dg, "no-delay" );
	// } else if( dg.buf_out.size() > 1024 ) {
	// 	datagram_flush( dg, "max-length" );
	// } else {
	// 	queuedPackets = true;
	// 	//if( dg.queued > 1 )
	// 	//    LOG("Queued %d packets (%zu bytes) for  %p\n", dg.queued, dg.buf_out.size(), dg.conn );
	// }
	return length;
}

int humblenet_datagram_recv( void* buffer, size_t length, int flags, Connection** fromconn, uint8_t channel )
{
	assert(channel == 0);
	// flush queued packets
	if( queuedPackets ) {
		for( ConnectionMap::iterator it = connections.begin(); it != connections.end(); ++it ) {
			datagram_flush( it->second, "auto" );
		}
		queuedPackets = false;
	}

	// first we drain all existing packets.
	for( ConnectionMap::iterator it = connections.begin(); it != connections.end(); ++it ) {
		int ret = datagram_get_message( it->second, buffer, length, flags, channel );
		if( ret > 0 ) {
			*fromconn = it->second.conn;
			return ret;
		}
	}

	// next check for incoming data on existing connections...
	while(true) {
		Connection* conn = humblenet_poll_all(0);
		if( conn == NULL )
			break;

		PeerId peer = humblenet_connection_get_peer_id( conn );

		ConnectionMap::iterator it = connections.find( conn );
		if( it == connections.end() ) {
			// hmm connection not cleaned up properly...
			LOG("received data from peer %u, but we have no datagram_connection for them\n", peer );
			connections.insert( ConnectionMap::value_type( conn, datagram_connection( conn, false ) ) );
			it = connections.find( conn );
		}

		// read whatever we can...
		uint8_t internalBuffer[1500];
		int retval = humblenet_connection_read(conn, internalBuffer, sizeof(internalBuffer));
		if( retval < 0 ) {
			connections.erase( it );
			LOG("read from peer %u(%p) failed with %s\n", peer, conn, humblenet_get_error() );
			humblenet_clear_error();
			if( humblenet_connection_status( conn ) == HUMBLENET_CONNECTION_CLOSED ) {
				*fromconn = conn;
				return -1;
			}
			continue;
		} else {
			it->second.buf_in.emplace_back(internalBuffer, internalBuffer + retval);
			retval = datagram_get_message( it->second, buffer, length, flags, channel );
			// LOG("after datagram_get_message, read %d bytes packet from peer %u(%p), [0]=%d\n", retval, peer, conn, buffer[0] );
			if( retval > 0 ) {
				*fromconn = it->second.conn;
				return retval;
			}
		}
	}

	// no existing connections have a packet ready, see if we have any new connections
	while( true ) {
		Connection* conn = humblenet_connection_accept();
		if( conn == NULL )
			break;

		PeerId peer = humblenet_connection_get_peer_id( conn );
		if( peer == 0 ) {
			// Not a peer connection?
			LOG("Accepted connection, but not a peer connection: %p\n", conn);
		   // humblenet_connection_close( conn );
			continue;
		}

		connections.insert( ConnectionMap::value_type( conn, datagram_connection( conn, false ) ) );
	}

	return 0;
}

/*
* See if there is a message waiting on the specified channel
*/
ha_bool humblenet_datagram_select( size_t* length, uint8_t channel ) {
	Connection* conn = NULL;
	int ret = humblenet_datagram_recv( NULL, 0, HUMBLENET_MSG_PEEK, &conn, channel );
	if( ret > 0 )
		*length = ret;
	else
		*length = 0;

	return *length > 0;
}

ha_bool humblenet_datagram_flush() {
	// flush queued packets
	if( queuedPackets ) {
		for( ConnectionMap::iterator it = connections.begin(); it != connections.end(); ++it ) {
			datagram_flush( it->second, "manual" );
		}
		queuedPackets = false;
		return true;
	}
	return false;
}

/*** End of inlined file: humblenet_datagram.cpp ***/


// P2P

/*** Start of inlined file: humblenet_p2p.cpp ***/
#include <string>
#include <map>

#define P2P_INIT_GUARD( ... )    INIT_GUARD( "humblenet_p2p_init has not been called", initialized, __VA_ARGS__ )

// These are the connections managed by p2p...
std::map<PeerId, Connection*> p2pconnections;

static bool initialized = false;

/*
 * Is the peer-to-peer network initialized.
 */
ha_bool HUMBLENET_CALL humblenet_p2p_is_initialized() {
	return initialized;
}

/*
 * Initialize the peer-to-peer library.
 */
ha_bool HUMBLENET_CALL humblenet_p2p_init(const char* server, const char* game_token, const char* game_secret, const char* auth_token) {
	if( initialized ) {
		return 1;
	}

	LOG("humblenet_p2p_init\n");

	if (!server || !game_token || !game_secret) {
		humblenet_set_error("Must specify server, game_token, and game_secret");
		return 0;
	}
	initialized = true;
	humbleNetState.signalingServerAddr = server;
	humbleNetState.gameToken = game_token;
	humbleNetState.gameSecret = game_secret;
	humbleNetState.reconnectToken = "";

	if( auth_token ) {
		humbleNetState.authToken = auth_token;
	} else {
		humbleNetState.authToken = "";
	}

	internal_p2p_register_protocol();

	humblenet_signaling_connect();

	return 1;
}

/*
 * Shut down the networking library
 */
void humblenet_p2p_shutdown() {
	if (!initialized) {
		return;
	}

	LOG("humblenet_p2p_shutdown\n");

	// disconnect from signaling server, shutdown all p2p connections, etc.
	initialized = false;

	// drop the server
	if( humbleNetState.p2pConn ) {
		humbleNetState.p2pConn->disconnect();
		humbleNetState.p2pConn.reset();
	}
	internal_deinit(humbleNetState.context);
	humbleNetState.context = NULL;
}

/*
 * Get the current peer ID of this client
 * returns 0 if not yet connected
 */
PeerId HUMBLENET_CALL humblenet_p2p_get_my_peer_id() {
	P2P_INIT_GUARD( 0 );

	return humbleNetState.myPeerId;
}

/*
 * Register an alias for this peer so it can be found by name
 */
ha_bool HUMBLENET_CALL humblenet_p2p_register_alias(const char* name) {
	P2P_INIT_GUARD( false );

	HUMBLENET_GUARD();

	return internal_alias_register( name );
}

/*
 * Unregister an alias of this peer.
 */
ha_bool HUMBLENET_CALL humblenet_p2p_unregister_alias(const char* name) {
	P2P_INIT_GUARD( false );

	HUMBLENET_GUARD();

	return internal_alias_unregister( name );
}

/*
 * Find the PeerId of a named peer (registered on the server)
 */
PeerId HUMBLENET_CALL humblenet_p2p_virtual_peer_for_alias(const char* name) {
	P2P_INIT_GUARD( 0 );

	HUMBLENET_GUARD();

	return internal_alias_lookup( name );
}

/*
 * Send a message to a peer.
 */
int HUMBLENET_CALL humblenet_p2p_sendto(const void* message, uint32_t length, PeerId topeer, SendMode sendmode, uint8_t channel) {
	P2P_INIT_GUARD( -1 );

	HUMBLENET_GUARD();

	Connection* conn = NULL;

	auto cit = p2pconnections.find( topeer );
	if( cit != p2pconnections.end() ) {
		// we have an active connection
		conn = cit->second;
	} else if( internal_alias_is_virtual_peer( topeer ) ) {
		// lookup/create a connection to the virutal peer.
		conn = internal_alias_find_connection( topeer );
		if( conn == NULL )
			conn = internal_alias_create_connection( topeer );
	} else {
		// create a new connection to the peer
		conn = humblenet_connect_peer( topeer );
	}

	if( conn == NULL ) {
		humblenet_set_error("Unable to get a connection for peer");
		return -1;
	} else if( cit == p2pconnections.end() ) {
		p2pconnections.insert( std::make_pair( topeer, conn ) );
		LOG("Connection to peer opened: %u\n", topeer );
	}

	int flags = 0;

	if( sendmode & SEND_RELIABLE_BUFFERED )
		flags = HUMBLENET_MSG_BUFFERED;
	// if were still connecting and reliable, treat it as a buffered request
	else if( conn->status == HUMBLENET_CONNECTION_CONNECTING )
		flags = HUMBLENET_MSG_BUFFERED;

	int ret = humblenet_datagram_send( message, length, flags, conn, channel );
	TRACE("Sent packet for channel %d to %u(%u): %d\n", channel, topeer, conn->otherPeer, ret );
	if( ret < 0 ) {
		LOG("Peer  %p(%u) write failed: %s\n", conn, topeer, humblenet_get_error() );
		if( humblenet_connection_status( conn ) == HUMBLENET_CONNECTION_CLOSED ) {
			LOG("Peer connection was closed\n");

			p2pconnections.erase( conn->otherPeer );
			p2pconnections.erase( topeer );
			humblenet_connection_close(conn);

			humblenet_set_error("Connection to peer was closed");
			return -1;
		}
	}
	return ret;
}

/*
 * See if there is a message available on the specified channel
 */
ha_bool HUMBLENET_CALL humblenet_p2p_peek(uint32_t* length, uint8_t channel) {
	P2P_INIT_GUARD( false );

	HUMBLENET_GUARD();

	Connection* from;
	int ret = humblenet_datagram_recv( NULL, 0, 1/*MSG_PEEK*/, &from, channel );
	if( ret > 0 )
		*length = ret;
	else
		*length = 0;

	return *length > 0;
}

/*
 * Receive a message sent from a peer
 */
int HUMBLENET_CALL humblenet_p2p_recvfrom(void* buffer, uint32_t length, PeerId* frompeer, uint8_t channel) {
	P2P_INIT_GUARD( 0 );

	HUMBLENET_GUARD();

	Connection* conn = NULL;
	int ret = humblenet_datagram_recv( buffer, length, 0, &conn, channel );
	if( !conn || !ret ) {
		*frompeer = 0;
	} else {
		PeerId vpeer = internal_alias_get_virtual_peer( conn );
		PeerId peer = conn->otherPeer;

		if( vpeer )
			*frompeer = vpeer;
		else
			*frompeer = peer;

		if( ret > 0 ) {
			TRACE("Got packet for channel %d from %u(%u)\n", channel, *frompeer, peer );
			if( p2pconnections.find( *frompeer ) == p2pconnections.end() ) {
				LOG("Tracking inbound connection to peer %u(%u)\n", *frompeer, peer );
				p2pconnections.insert( std::make_pair( *frompeer, conn ) );
			}
		} else {
			p2pconnections.erase( *frompeer );
			p2pconnections.erase( peer );
			humblenet_connection_close(conn);
			LOG("closing connection to peer: %u(%u)\n",*frompeer,peer);
		}
	}
	return ret;
}

/*
 * Disconnect a peer
 */
ha_bool HUMBLENET_CALL humblenet_p2p_disconnect(PeerId peer) {
	P2P_INIT_GUARD( false );

	HUMBLENET_GUARD();

	if( peer == 0 /*all*/) {
		for( auto it = p2pconnections.begin(); it != p2pconnections.end(); ++it ) {
			humblenet_connection_set_closed( it->second );
		}
	} else {
		auto it = p2pconnections.find( peer );
		if( it != p2pconnections.end() )
			humblenet_connection_set_closed( it->second );
	}
	// TODO: Should we see if the peer that was passed in is the "real" peer instead of the VPeer?
	return 1;
}

void internal_poll_io() {
	humblenet_p2p_wait(0);
}

#ifndef __EMSCRIPTEN__

#include "libpoll.h"	// SKIP_AMALGAMATOR_INCLUDE

// Native only API, should be moved to its own file
int HUMBLENET_CALL humblenet_p2p_select(int nfds, fd_set *readfds, fd_set *writefds,
										fd_set *exceptfds, struct timeval *timeout) {

	// not guard needed poll_select is internally init and thread safe.
	return poll_select( nfds, readfds, writefds, exceptfds, timeout );
}

ha_bool HUMBLENET_CALL humblenet_p2p_wait(int ms) {
	P2P_INIT_GUARD( false );

	struct timeval tv;

	// This is not really needed in a threaded environment,
	// e.g. if this is being used as a sleep till something is ready,
	// we need this. If its being used as a "let IO run" (e.g. threaded IO) then we dont.
	if( ms > 0 ) {
		HUMBLENET_GUARD();

		if( ! humbleNetState.pendingDataConnections.empty() ) {
			ms = 0;
		}
	}

	tv.tv_sec = ms / 1000;
	tv.tv_usec = 1000 * (ms % 1000);

	if( poll_select( 0, NULL, NULL, NULL, &tv ) > 0 )
		return true;
	else
	{
		HUMBLENET_GUARD();

		return ! humbleNetState.pendingDataConnections.empty() || ! humbleNetState.pendingNewConnections.empty() || ! humbleNetState.remoteClosedConnections.empty();
	}
}

#else
ha_bool HUMBLENET_CALL humblenet_p2p_wait(int ms) {
	P2P_INIT_GUARD( false );

	return ! humbleNetState.pendingDataConnections.empty() || ! humbleNetState.pendingNewConnections.empty() || ! humbleNetState.remoteClosedConnections.empty();
}
#endif

/*** End of inlined file: humblenet_p2p.cpp ***/



/*** Start of inlined file: humblenet_p2p_signaling.cpp ***/
#if defined(__EMSCRIPTEN__)
	#include <emscripten/emscripten.h>
#endif

#if defined(__APPLE__) || defined(__linux__)
	#include <sys/utsname.h>
#endif

#if defined(WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

static ha_bool p2pSignalProcess(const humblenet::HumblePeer::Message *msg, void *user_data);

ha_bool humblenet_signaling_connect() {
	using namespace humblenet;

	if (humbleNetState.p2pConn) {
		humbleNetState.p2pConn->disconnect();
		humbleNetState.p2pConn.reset();
		humbleNetState.myPeerId = 0;
	}

	LOG("connecting to signaling server \"%s\" with gameToken \"%s\" \n", humbleNetState.signalingServerAddr.c_str(), humbleNetState.gameToken.c_str());

	humbleNetState.p2pConn.reset(new P2PSignalConnection);
	humbleNetState.p2pConn->wsi = internal_connect_websocket(humbleNetState.signalingServerAddr.c_str(), "humblepeer");

	if (humbleNetState.p2pConn->wsi == NULL) {
		humbleNetState.p2pConn.reset();
		// TODO: can we get more specific reason ?
		humblenet_set_error("WebSocket connection to signaling server failed");
		return false;
	}

	LOG("new wsi: %p\n", humbleNetState.p2pConn->wsi);

	internal_poll_io();
	return true;
}

namespace humblenet {
	ha_bool sendP2PMessage(P2PSignalConnection *conn, const uint8_t *buff, size_t length)
	{
		if( conn == NULL ) {
			// this equates to the state where we never established / got disconnected
			// from the peer server.

			return false;
		}

		assert(conn != NULL);
		if (conn->wsi == NULL) {
			// this really shouldn't happen but apparently it can
			// if the game hasn't properly opened a signaling connection
			return false;
		}

		{
			HUMBLENET_UNGUARD();

			int ret = internal_write_socket(conn->wsi, (const void*)buff, length );

			return ret == length;
		}
	}

	// called for incoming connections to indicate the connection process is completed.
	int on_accept (internal_socket_t* s, void* user_data) {
		// we dont accept incoming connections
		return -1;
	}

	// called for outgoing connections to indicate the connection process is completed.
	int on_connect (internal_socket_t* s, void* user_data) {
		HUMBLENET_GUARD();

		assert(s != NULL);
		if (!humbleNetState.p2pConn) {
			// there is no signaling connection in humbleNetState
			// this one must be obsolete, close it
			return -1;
		}

		// temporary copy, doesn't transfer ownership
		P2PSignalConnection *conn = humbleNetState.p2pConn.get();
		if (conn->wsi != s) {
			// this connection is not the one currently active in humbleNetState.
			// this one must be obsolete, close it
			return -1;
		}
		// platform info
		std::string pinfo;
#if defined(__APPLE__) || defined(__linux__)
		struct utsname buf;
		memset(&buf, 0, sizeof(struct utsname));
		int retval = uname(&buf);
		if (retval == 0)
		{
			pinfo.append("Sysname: ");
			pinfo.append(buf.sysname);
			pinfo.append(", Release: ");
			pinfo.append(buf.release);
			pinfo.append(", Version: ");
			pinfo.append(buf.version);
			pinfo.append(", Machine: ");
			pinfo.append(buf.machine);
		}
		else
		{
			LOG("Failed getting system info with uname\n");
		}
#elif defined(_WIN32)
		OSVERSIONINFO osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osvi);
		pinfo.append("Windows " + std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion) + ", build " + std::to_string(osvi.dwBuildNumber));

		SYSTEM_INFO si;
		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		GetSystemInfo(&si);
		switch (si.wProcessorArchitecture)
		{
			case PROCESSOR_ARCHITECTURE_INTEL:
				pinfo.append(", Intel x86");
				break;
			case PROCESSOR_ARCHITECTURE_IA64:
				pinfo.append(", Intel Itanium");
				break;
			case PROCESSOR_ARCHITECTURE_AMD64:
				pinfo.append(", AMD or Intel x64");
				break;
			default:
				pinfo.append(", non-desktop architecture");
				break;
		}
#elif defined(__EMSCRIPTEN__)
		char buff[512];
		int len = EM_ASM_INT({
			var buff = new Uint8Array(Module.HEAPU8.buffer, $0, $1);
			return stringToUTF8Array(navigator.userAgent, buff, 0, $1);
		}, buff, sizeof(buff));
		pinfo = buff;
#endif

		uint8_t flags = 0;
		if (humbleNetState.webRTCSupported) {
			// No trickle ICE on native
			flags = (0x1 | 0x2);
		}
		std::map<std::string, std::string> attributes;
		attributes.emplace("platform", pinfo);
		ha_bool helloSuccess = sendHelloServer(conn, flags, humbleNetState.gameToken, humbleNetState.gameSecret, humbleNetState.authToken, humbleNetState.reconnectToken, attributes);
		if (!helloSuccess) {
			// something went wrong, close the connection
			// don't humblenet_set_error, sendHelloServer should have done that
			humbleNetState.p2pConn.reset();
			return -1;
		}
		return 0;
	}

	// called each time data is received.
	int on_data( internal_socket_t* s, const void* data, int len, void* user_data ) {
		HUMBLENET_GUARD();

		assert(s != NULL);

		if (!humbleNetState.p2pConn) {
			// there is no signaling connection in humbleNetState
			// this one must be obsolete, close it
			return -1;
		}

		// temporary copy, doesn't transfer ownership
		P2PSignalConnection *conn = humbleNetState.p2pConn.get();
		if (conn->wsi != s) {
			// this connection is not the one currently active in humbleNetState.
			// this one must be obsolete, close it
			return -1;
		}

		//        LOG("Data: %d -> %s\n", len, std::string((const char*)data,len).c_str());

		conn->recvBuf.insert(conn->recvBuf.end()
							 , reinterpret_cast<const char *>(data)
							 , reinterpret_cast<const char *>(data) + len);
		ha_bool retval = parseMessage(conn->recvBuf, p2pSignalProcess, NULL);
		if (!retval) {
			// error while parsing a message, close the connection
			humbleNetState.p2pConn.reset();
			return -1;
		}
		return 0;
	}

	// called to indicate the connection is writable.
	int on_writable (internal_socket_t* s, void* user_data) {
		HUMBLENET_GUARD();

		assert(s != NULL);
		if (!humbleNetState.p2pConn) {
			// there is no signaling connection in humbleNetState
			// this one must be obsolete, close it
			return -1;
		}

		// temporary copy, doesn't transfer ownership
		P2PSignalConnection *conn = humbleNetState.p2pConn.get();
		if (conn->wsi != s) {
			// this connection is not the one currently active in humbleNetState.
			// this one must be obsolete, close it
			return -1;
		}

		if (conn->sendBuf.empty()) {
			// no data in sendBuf
			return 0;
		}

		int retval = internal_write_socket( conn->wsi, &conn->sendBuf[0], conn->sendBuf.size() );
		if (retval < 0) {
			// error while sending, close the connection
			// TODO: should try to reopen after some time
			humbleNetState.p2pConn.reset();
			return -1;
		}

		// successful write
		conn->sendBuf.erase(conn->sendBuf.begin(), conn->sendBuf.begin() + retval);

		return 0;
	}

	// called when the conenction is terminated (from either side)
	int on_disconnect( internal_socket_t* s, void* user_data ) {
		HUMBLENET_GUARD();

		if( humbleNetState.p2pConn ) {
			if( s == humbleNetState.p2pConn->wsi ) {

				// handle retry...
				humbleNetState.p2pConn.reset();

				return 0;
			}
		}

		return 0;
	}

	void register_protocol(internal_context_t* context) {
		internal_callbacks_t callbacks;

		memset(&callbacks, 0, sizeof(callbacks));

		callbacks.on_accept = on_accept;
		callbacks.on_connect = on_connect;
		callbacks.on_data = on_data;
		callbacks.on_disconnect = on_disconnect;
		callbacks.on_writable = on_writable;
		callbacks.on_destroy = on_disconnect; // on connection failure to establish we only get a destroy, not a disconnect.

		internal_register_protocol( humbleNetState.context, "humblepeer", &callbacks );
	}
}

static ha_bool p2pSignalProcess(const humblenet::HumblePeer::Message *msg, void *user_data)
{
	using namespace humblenet;

	auto msgType = msg->message_type();

	switch (msgType) {
		case HumblePeer::MessageType::P2POffer:
		{
			auto p2p = reinterpret_cast<const HumblePeer::P2POffer*>(msg->message());
			PeerId peer = static_cast<PeerId>(p2p->peerId());

			if (humbleNetState.pendingPeerConnectionsIn.find(peer) != humbleNetState.pendingPeerConnectionsIn.end()) {
				// error, already a pending incoming connection to this peer
				LOG("p2pSignalProcess: already a pending connection to peer %u\n", peer);
				return true;
			}

			bool emulated = ( p2p->flags() & 0x1 );

			if (emulated || !humbleNetState.webRTCSupported) {
				// webrtc connections not supported
				assert(humbleNetState.p2pConn);
				sendPeerRefused(humbleNetState.p2pConn.get(), peer);
			}

			Connection *connection = new Connection(Incoming);
			connection->status = HUMBLENET_CONNECTION_CONNECTING;
			connection->otherPeer = peer;
			{
				HUMBLENET_UNGUARD();
				connection->socket = internal_create_webrtc(humbleNetState.context);
			}
			internal_set_data( connection->socket, connection );

			humbleNetState.pendingPeerConnectionsIn.insert(std::make_pair(peer, connection));

			auto offer = p2p->offer();

			LOG("P2PConnect SDP got %u's offer = \"%s\"\n", peer, offer->c_str());
			int ret;
			{
				HUMBLENET_UNGUARD();
				ret = internal_set_offer( connection->socket, offer->c_str() );
			}
			if( ! ret )
			{
				humblenet_connection_close( connection );

				sendPeerRefused(humbleNetState.p2pConn.get(), peer);
			}
		}
			break;

		case HumblePeer::MessageType::P2PAnswer:
		{
			auto p2p = reinterpret_cast<const HumblePeer::P2PAnswer*>(msg->message());
			PeerId peer = static_cast<PeerId>(p2p->peerId());

			auto it = humbleNetState.pendingPeerConnectionsOut.find(peer);
			if (it == humbleNetState.pendingPeerConnectionsOut.end()) {
				LOG("P2PResponse for connection we didn't initiate: %u\n", peer);
				return true;
			}

			Connection *conn = it->second;
			assert(conn != NULL);
			assert(conn->otherPeer == peer);

			assert(conn->socket != NULL);
			// TODO: deal with _CLOSED
			assert(conn->status == HUMBLENET_CONNECTION_CONNECTING);

			auto offer = p2p->offer();

			LOG("P2PResponse SDP got %u's response offer = \"%s\"\n", peer, offer->c_str());
			int ret;
			{
				HUMBLENET_UNGUARD();
				ret = internal_set_answer( conn->socket, offer->c_str() );
			}
			if( !ret ) {
				humblenet_connection_set_closed( conn );
			}
		}
			break;

		case HumblePeer::MessageType::HelloClient:
		{
			auto hello = reinterpret_cast<const HumblePeer::HelloClient*>(msg->message());
			PeerId peer = static_cast<PeerId>(hello->peerId());

			if (humbleNetState.myPeerId != 0) {
				LOG("Error: got HelloClient but we already have a peer id\n");
				return true;
			}
			LOG("My peer id is %u\n", peer);
			humbleNetState.myPeerId = peer;

			humbleNetState.iceServers.clear();

			if (hello->iceServers()) {
				auto iceList = hello->iceServers();
				for (const auto& it : *iceList) {
					if (it->type() == HumblePeer::ICEServerType::STUNServer) {
						humbleNetState.iceServers.emplace_back(it->server()->str());
					} else if (it->type() == HumblePeer::ICEServerType::TURNServer) {
						auto username = it->username();
						auto pass = it->password();
						if (pass && username) {
							humbleNetState.iceServers.emplace_back(it->server()->str(), username->str(), pass->str());
						}
					}
				}
			} else {
				LOG("No STUN/TURN credentials provided by the server\n");
			}

			std::vector<const char*> stunServers;
			for (auto& it : humbleNetState.iceServers) {
				if (it.type == HumblePeer::ICEServerType::STUNServer) {
					stunServers.emplace_back(it.server.c_str());
				} else if (it.type == HumblePeer::ICEServerType::TURNServer) {
					internal_add_turn_server( humbleNetState.context, it.server.c_str(), it.username.c_str(), it.password.c_str() );
				}
			}
			internal_set_stun_servers(humbleNetState.context, stunServers.data(), stunServers.size());
		}
			break;

		case HumblePeer::MessageType::ICECandidate:
		{
			auto iceCandidate = reinterpret_cast<const HumblePeer::ICECandidate*>(msg->message());
			PeerId peer = static_cast<PeerId>(iceCandidate->peerId());

			auto it = humbleNetState.pendingPeerConnectionsIn.find( peer );

			if( it == humbleNetState.pendingPeerConnectionsIn.end() )
			{
				it = humbleNetState.pendingPeerConnectionsOut.find( peer );
				if( it == humbleNetState.pendingPeerConnectionsOut.end() )
				{
					// no connection waiting on a peer.
					return true;
				}
			}

			if( it->second->socket && it->second->status == HUMBLENET_CONNECTION_CONNECTING ) {
				auto offer = iceCandidate->offer();
				LOG("Got ice candidate from peer: %d, %s\n", it->second->otherPeer, offer->c_str() );

				{
					HUMBLENET_UNGUARD();

					internal_add_ice_candidate( it->second->socket, offer->c_str() );
				}
			}
		}
			break;

		case HumblePeer::MessageType::P2PReject:
		{
			auto reject = reinterpret_cast<const HumblePeer::P2PReject*>(msg->message());
			PeerId peer = static_cast<PeerId>(reject->peerId());

			auto it = humbleNetState.pendingPeerConnectionsOut.find(peer);
			if (it == humbleNetState.pendingPeerConnectionsOut.end()) {
				switch (reject->reason()) {
					case HumblePeer::P2PRejectReason::NotFound:
						LOG("Peer %u does not exist\n", peer);
						break;
					case HumblePeer::P2PRejectReason::PeerRefused:
						LOG("Peer %u rejected our connection\n", peer);
						break;
				}
				return true;
			}

			Connection *conn = it->second;
			assert(conn != NULL);

			blacklist_peer(peer);
			humblenet_connection_set_closed(conn);
		}
			break;

		case HumblePeer::MessageType::P2PConnected:
		{
			auto connect = reinterpret_cast<const HumblePeer::P2PConnected*>(msg->message());

			LOG("Established connection to peer %u", connect->peerId());
		}
			break;

		case HumblePeer::MessageType::P2PDisconnect:
		{
			auto disconnect = reinterpret_cast<const HumblePeer::P2PDisconnect*>(msg->message());

			LOG("Disconnecting peer %u", disconnect->peerId());
		}
			break;

		case HumblePeer::MessageType::P2PRelayData:
		{
			auto relay = reinterpret_cast<const HumblePeer::P2PRelayData*>(msg->message());
			auto peer = relay->peerId();
			auto data = relay->data();

			LOG("Got %d bytes relayed from peer %u\n", data->size(), peer );

			// Sequentially look for the other peer
			auto it = humbleNetState.connections.begin();
			for( ; it != humbleNetState.connections.end(); ++it ) {
				if( it->second->otherPeer == peer )
					break;
			}
			if( it == humbleNetState.connections.end() ) {
				LOG("Peer %u does not exist\n", peer);
			} else {
				Connection* conn = it->second;

				conn->recvBuffer.emplace_back(data->begin(), data->end());
				humbleNetState.pendingDataConnections.insert( conn );

				signal();
			}
		}
			break;
		case HumblePeer::MessageType::AliasResolved:
		{
			auto resolved = reinterpret_cast<const HumblePeer::AliasResolved*>(msg->message());

			internal_alias_resolved_to( resolved->alias()->c_str(), resolved->peerId() );
		}
			break;

		default:
			LOG("p2pSignalProcess unhandled %s\n", HumblePeer::EnumNameMessageType(msgType));
			break;
	}

	return true;
}

/*** End of inlined file: humblenet_p2p_signaling.cpp ***/


/*** Start of inlined file: humblenet_alias.cpp ***/
#include <cassert>
#include <map>

#define VIRTUAL_PEER 0x80000000

static BidirectionalMap<PeerId, std::string> virtualPeerNames;
static BidirectionalMap<PeerId, Connection*> virtualPeerConnections;

static PeerId nextVirtualPeer = 0;

static std::string virtualName;

ha_bool internal_alias_register( const char* name ) {
	if( !name || !name[0] ) {
		humblenet_set_error("No name or empty name provided");
		return 0;
	}

	return humblenet::sendAliasRegister(humbleNetState.p2pConn.get(), name);
}

ha_bool internal_alias_unregister( const char* name ) {
	if (name && !name[0] ) {
		humblenet_set_error("Empty name provided");
		return 0;
	}

	if (!name) name = "";

	return humblenet::sendAliasUnregister(humbleNetState.p2pConn.get(), name);
}

PeerId internal_alias_lookup( const char* name ) {
	auto it = virtualPeerNames.find( name );
	if( ! virtualPeerNames.is_end(it) ) {
		return it->second;
	}

	// allocate a new VPeerId
	PeerId vpeer = (++nextVirtualPeer) | VIRTUAL_PEER;
	virtualPeerNames.insert( vpeer, name );

	return vpeer;
}

void internal_alias_resolved_to( const std::string& alias, PeerId peer ) {
	Connection* connection = NULL;

	auto it = humbleNetState.pendingAliasConnectionsOut.find(alias);
	if (it == humbleNetState.pendingAliasConnectionsOut.end()) {
		LOG("Got resolve message for alias \"%s\" which we're not connecting to\n", alias.c_str());
		return;
	} else {
		connection = it->second;
		humbleNetState.pendingAliasConnectionsOut.erase(it);
	}

	assert(connection != NULL);

	if( peer == 0 ) {
		LOG("AliasError: unable to resolve \"%s\"\n", alias.c_str());

		humblenet_connection_set_closed( connection );
		return;
	}

	connection->otherPeer = peer;

	humbleNetState.pendingPeerConnectionsOut.emplace(peer, connection);

	if( is_peer_blacklisted(peer) ) {
		humblenet_set_error("peer blacklisted");
		LOG("humblenet_connect_peer: peer blacklisted %u\n", peer);

		humblenet_connection_set_closed(connection);
	} else {
		int ret;
		{
			HUMBLENET_UNGUARD();
			connection->socket = internal_create_webrtc(humbleNetState.context);
			internal_set_data( connection->socket, connection);

			ret = internal_create_offer(connection->socket );
		}

		if( !ret) {
			LOG("Unable to create offer, aborting connection to alias\n");
			humblenet_connection_set_closed( connection );
		}
	}

}

ha_bool internal_alias_is_virtual_peer( PeerId peer ) {
	return (peer & VIRTUAL_PEER) == VIRTUAL_PEER;
}

PeerId internal_alias_get_virtual_peer( Connection* conn ) {
	auto it = virtualPeerConnections.find( conn );
	if( virtualPeerConnections.is_end(it) )
		return 0;
	else
		return it->second;
}

Connection* internal_alias_find_connection( PeerId peer ) {
	if( ! internal_alias_is_virtual_peer(peer) ) {
		return NULL;
	}

	// resolve the VPeer
	auto it = virtualPeerConnections.find( peer );
	if( virtualPeerConnections.is_end(it) ) {
		return NULL;
	} else if( humblenet_connection_status( it->second ) == HUMBLENET_CONNECTION_CLOSED ) {
		virtualPeerConnections.erase( it );
		return NULL;
	} else {
		return it->second;
	}
}

Connection* internal_alias_create_connection( PeerId peer ) {
	Connection* conn = internal_alias_find_connection( peer );
	if( conn == NULL ) {
		auto nit = virtualPeerNames.find( peer );
		if( virtualPeerNames.is_end( nit ) ) {
			// Invalid VPeerId !!!
			humblenet_set_error("Not a valid VPeerId");
			return NULL;
		}

		std::string name = nit->second;

		if (!sendAliasLookup(humbleNetState.p2pConn.get(), name)) {
			return NULL;
		}

		conn = new Connection(Outgoing);

		humbleNetState.pendingAliasConnectionsOut.emplace(name, conn);

		LOG("Establishing a connection to \"%s\"...\n", nit->second.c_str() );

		virtualPeerConnections.insert( peer, conn );
	}

	return conn;
}

void internal_alias_remove_connection( Connection* conn ) {
	virtualPeerConnections.erase( conn );
}

/*** End of inlined file: humblenet_alias.cpp ***/


/*** Start of inlined file: humblepeer.cpp ***/
#include <cassert>
#include <cstdio>
#include <cstring>

#include <limits>


/*** Start of inlined file: crc.h ***/
#ifndef __CRC_H__
#define __CRC_H__

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The definition of the used algorithm.
 *
 * This is not used anywhere in the generated code, but it may be used by the
 * application code to call algoritm-specific code, is desired.
 *****************************************************************************/
#define CRC_ALGO_TABLE_DRIVEN 1

/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 32 bits.
 *****************************************************************************/
typedef uint_fast32_t crc_t;

/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 *****************************************************************************/
crc_t crc_init(void);

/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update(crc_t crc, const void *data, size_t data_len);

/**
 * Calculate the final crc value.
 *
 * \param crc  The current crc value.
 * \return     The final crc value.
 *****************************************************************************/
crc_t crc_finalize(crc_t crc);

#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* __CRC_H__ */

/*** End of inlined file: crc.h ***/


/*** Start of inlined file: hmac.h ***/
#ifndef HMAC_H
#define HMAC_H


/*** Start of inlined file: sha1.h ***/
#ifndef _SHA1_H_
#define _SHA1_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * If you do not have the ISO standard stdint.h header file, then you
 * must typdef the following:
 *    name              meaning
 *  uint32_t         unsigned 32 bit integer
 *  uint8_t          unsigned 8 bit integer (i.e., unsigned char)
 *  int_least16_t    integer of >= 16 bits
 *
 */

#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
	shaSuccess = 0,
	shaNull,            /* Null pointer parameter */
	shaInputTooLong,    /* input data too long */
	shaStateError       /* called Input after Result */
};
#endif
#define SHA1HashSize 20

/*
 *  This structure will hold context information for the SHA-1
 *  hashing operation
 */
typedef struct SHA1Context
{
	uint32_t Intermediate_Hash[SHA1HashSize/4]; /* Message Digest  */

	uint32_t Length_Low;            /* Message length in bits      */
	uint32_t Length_High;           /* Message length in bits      */

							   /* Index into message block array   */
	int_least16_t Message_Block_Index;
	uint8_t Message_Block[64];      /* 512-bit message blocks      */

	int Computed;               /* Is the digest computed?         */
	int Corrupted;             /* Is the message digest corrupted? */
} SHA1Context;

/*
 *  Function Prototypes
 */

int SHA1Reset(  SHA1Context *);
int SHA1Input(  SHA1Context *,
				const uint8_t *,
				unsigned int);
int SHA1Result( SHA1Context *,
				uint8_t Message_Digest[SHA1HashSize]);

#ifdef __cplusplus
}
#endif
#endif

/*** End of inlined file: sha1.h ***/

#ifdef __cplusplus
extern "C" {
#endif

#define HMAC_BLOCKSIZE 64
#define HMAC_DIGEST_SIZE SHA1HashSize

typedef struct HMACContext {
	SHA1Context i_ctx;
	SHA1Context o_ctx;
} HMACContext;

void HMACInit(HMACContext *ctx, const uint8_t* key, unsigned key_len);
int HMACInput(HMACContext *ctx, const uint8_t* data, unsigned data_len);
int HMACResult(HMACContext *ctx, uint8_t Message_Digest[HMAC_DIGEST_SIZE]);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>
void HMACResultToHex(uint8_t digest[HMAC_DIGEST_SIZE], std::string& out_hex);
#endif

#endif /* HMAC_H */

/*** End of inlined file: hmac.h ***/

#include <ctime>

#define LOG printf

#define DEFAULT_FBB_SIZE 512

namespace humblenet {
	flatbuffers::Offset<flatbuffers::String> CreateFBBStringIfNotEmpty(flatbuffers::FlatBufferBuilder &fbb, const std::string &str)
	{
		if (str.empty()) {
			return 0;
		} else {
			return fbb.CreateString(str);
		}
	}

	template<typename T>
	flatbuffers::Offset<flatbuffers::Vector<T>> CreateFBBVectorIfNotEmpty(flatbuffers::FlatBufferBuilder &fbb, const std::vector<T> &v)
	{
		if (v.empty()) {
			return 0;
		} else {
			return fbb.CreateVector(v);
		}
	}

	// Sending and parsing

	inline ha_bool WARN_UNUSED_RESULT sendP2PMessage(P2PSignalConnection *conn, const flatbuffers::FlatBufferBuilder& fbb)
	{
		return sendP2PMessage(conn, fbb.GetBufferPointer(), fbb.GetSize());
	}

	ha_bool parseMessage(std::vector<uint8_t> &recvBuf, ProcessMsgFunc processFunc, void *user_data)
	{
		if (recvBuf.empty()) {
			// nothing to do
			return 1;
		}

		// Now validate our buffer based on the expected size
		flatbuffers::Verifier v(recvBuf.data(), recvBuf.size());
		if (!HumblePeer::VerifyMessageBuffer(v)) {
			// TODO should we disconnect in this case?
			fprintf(stderr, "Invalid message buffer\n");
			return 0;
		}

		auto message = HumblePeer::GetMessage(recvBuf.data());

		// process it
		ha_bool messageOk = processFunc(message, user_data);
		if (!messageOk) {
			// processFunc didn't like this message for some reason
			fprintf(stderr, "processFunc failed\n");
			return 0;
		}

		recvBuf.erase(recvBuf.begin(), recvBuf.end());

		return 1;
	}

	// ** Peer server connection

	ha_bool sendHelloServer(P2PSignalConnection *conn, uint8_t flags,
							const std::string& gametoken, const std::string& gamesecret,
							const std::string& authToken, const std::string& reconnectToken,
							const std::map<std::string, std::string>& attributes)
	{
		assert(!gametoken.empty());
		assert(!gamesecret.empty());

		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);

		std::vector<flatbuffers::Offset<HumblePeer::Attribute>> tempAttribs;

		// build up attributes
		tempAttribs.reserve(attributes.size() + 1);

		for (auto& it : attributes) {
			if (it.first == "timestamp") continue;
			tempAttribs.emplace_back( HumblePeer::CreateAttribute(fbb, fbb.CreateString(it.first), fbb.CreateString(it.second)) );
		}

		tempAttribs.emplace_back( HumblePeer::CreateAttribute(fbb, fbb.CreateString("timestamp"), fbb.CreateString(std::to_string(time(NULL)))) );

		uint32_t version = 0;

		HMACContext hmac;
		HMACInit(&hmac, (const uint8_t*)gamesecret.data(), gamesecret.size());

		HMACInput(&hmac, (const uint8_t*)authToken.data(), authToken.size());
		HMACInput(&hmac, (const uint8_t*)reconnectToken.data(), reconnectToken.size());

		flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<HumblePeer::Attribute>>> attribs;
		if (!tempAttribs.empty()) {
			attribs = fbb.CreateVectorOfSortedTables(&tempAttribs);
			// tempAttribs is sorted now
			for (const auto& it : tempAttribs) {
				auto attr = flatbuffers::EndianScalar(reinterpret_cast<const HumblePeer::Attribute *>(fbb.GetCurrentBufferPointer() + fbb.GetSize() - it.o));
				auto key = attr->key();
				auto value = attr->value();
				HMACInput(&hmac, key->Data(), key->size());
				HMACInput(&hmac, value->Data(), value->size());
			}
		}

		uint8_t hmacresult[HMAC_DIGEST_SIZE];

		HMACResult(&hmac, hmacresult);

		std::string signature;

		HMACResultToHex(hmacresult, signature);

		auto packet = HumblePeer::CreateHelloServer(fbb,
													version,
													flags,
													fbb.CreateString(gametoken),
													fbb.CreateString(signature),
													CreateFBBStringIfNotEmpty(fbb, authToken),
													CreateFBBStringIfNotEmpty(fbb, reconnectToken),
													attribs);

		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::HelloServer, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendHelloClient(P2PSignalConnection *conn, PeerId peerId, const std::string& reconnectToken, const std::vector<ICEServer>& iceServers)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);

		std::vector<flatbuffers::Offset<HumblePeer::ICEServer>> tempServers;
		tempServers.reserve(iceServers.size());

		for( auto& it : iceServers) {
			auto server = fbb.CreateString(it.server);
			flatbuffers::Offset<HumblePeer::ICEServer> s;

			if (it.type == HumblePeer::ICEServerType::STUNServer) {
				s = HumblePeer::CreateICEServer(fbb, HumblePeer::ICEServerType::STUNServer, server);
			} else { // turn server
				s = HumblePeer::CreateICEServer(fbb, HumblePeer::ICEServerType::TURNServer, server, fbb.CreateString(it.username), fbb.CreateString(it.password));
			}
			tempServers.emplace_back( s );
		}

		auto packet = HumblePeer::CreateHelloClient(fbb, peerId,
													CreateFBBStringIfNotEmpty(fbb, reconnectToken),
													CreateFBBVectorIfNotEmpty(fbb, tempServers));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::HelloClient, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	// ** P2P Negotiation messages

	ha_bool sendNoSuchPeer(P2PSignalConnection *conn, PeerId peerId)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateP2PReject(fbb, peerId, HumblePeer::P2PRejectReason::NotFound);
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::P2PReject, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendPeerRefused(P2PSignalConnection *conn, PeerId peerId)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateP2PReject(fbb, peerId, HumblePeer::P2PRejectReason::PeerRefused);
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::P2PReject, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendP2PConnect(P2PSignalConnection *conn, PeerId peerId, uint8_t flags, const char* offer)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateP2POffer(fbb, peerId, flags, fbb.CreateString(offer));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::P2POffer, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendP2PResponse(P2PSignalConnection *conn, PeerId peerId, const char* offer)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateP2PAnswer(fbb, peerId, fbb.CreateString(offer));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::P2PAnswer, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendICECandidate(P2PSignalConnection *conn, PeerId peerId, const char* offer)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateICECandidate(fbb, peerId, fbb.CreateString(offer));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::ICECandidate, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendP2PDisconnect(P2PSignalConnection *conn, PeerId peerId)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateP2PDisconnect(fbb, peerId);
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::P2PDisconnect, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);

	}

	ha_bool sendP2PRelayData(humblenet::P2PSignalConnection *conn, PeerId peerId, const void* data, uint16_t length) {
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateP2PRelayData(fbb, peerId, fbb.CreateVector((int8_t*)data, length));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::P2PRelayData, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	// ** Name Alias messages

	ha_bool sendAliasRegister(P2PSignalConnection *conn, const std::string& alias)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateAliasRegister(fbb, fbb.CreateString(alias));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::AliasRegister, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendAliasUnregister(P2PSignalConnection *conn, const std::string& alias)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateAliasUnregister(fbb, CreateFBBStringIfNotEmpty(fbb, alias));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::AliasUnregister, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendAliasLookup(P2PSignalConnection *conn, const std::string& alias)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateAliasLookup(fbb, fbb.CreateString(alias));
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::AliasLookup, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}

	ha_bool sendAliasResolved(P2PSignalConnection *conn, const std::string& alias, PeerId peer)
	{
		flatbuffers::FlatBufferBuilder fbb(DEFAULT_FBB_SIZE);
		auto packet = HumblePeer::CreateAliasResolved(fbb, fbb.CreateString(alias), peer);
		auto msg = HumblePeer::CreateMessage(fbb, HumblePeer::MessageType::AliasResolved, packet.Union());
		fbb.Finish(msg);

		return sendP2PMessage(conn, fbb);
	}
}  // namespace humblenet

/*** End of inlined file: humblepeer.cpp ***/

// socket layer

/*** Start of inlined file: libsocket.cpp ***/
#include <map>
#include <string>
#include <vector>
#include <cassert>
#include <cstring>

#ifdef __EMSCRIPTEN__

/*** Start of inlined file: libwebsockets_asmjs.h ***/
#ifndef __EMSCRIPTEN__
#error "This should only be used under emscripten"
#endif

#ifndef LIBWEBSOCKETS_ASMJS_H
#define LIBWEBSOCKETS_ASMJS_H

// ABI compatible subset of libwebsockets

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

enum lws_callback_reasons {
	LWS_CALLBACK_ESTABLISHED = 1,
	LWS_CALLBACK_CLIENT_CONNECTION_ERROR = 2,
//    LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH,
	LWS_CALLBACK_CLIENT_ESTABLISHED = 3,
	LWS_CALLBACK_CLOSED = 4,
 //   LWS_CALLBACK_CLOSED_HTTP,
	LWS_CALLBACK_RECEIVE = 5,
	LWS_CALLBACK_CLIENT_RECEIVE = 6,
 //   LWS_CALLBACK_CLIENT_RECEIVE_PONG,
	LWS_CALLBACK_CLIENT_WRITEABLE = 7,
	LWS_CALLBACK_SERVER_WRITEABLE = 8,
 //   LWS_CALLBACK_HTTP,
 //   LWS_CALLBACK_HTTP_BODY,
 //   LWS_CALLBACK_HTTP_BODY_COMPLETION,
 //   LWS_CALLBACK_HTTP_FILE_COMPLETION,
 //   LWS_CALLBACK_HTTP_WRITEABLE,
 //   LWS_CALLBACK_FILTER_NETWORK_CONNECTION,
 //   LWS_CALLBACK_FILTER_HTTP_CONNECTION,
 //   LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED,
	LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION = 9,
 //   LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS,
//    LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS,
//    LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION,
//    LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER,
//    LWS_CALLBACK_CONFIRM_EXTENSION_OKAY,
//    LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED,
//    LWS_CALLBACK_PROTOCOL_INIT,
//    LWS_CALLBACK_PROTOCOL_DESTROY,
	LWS_CALLBACK_WSI_CREATE = 10, /* always protocol[0] */
	LWS_CALLBACK_WSI_DESTROY = 11, /* always protocol[0] */
//    LWS_CALLBACK_GET_THREAD_ID,

	/* external poll() management support */
//    LWS_CALLBACK_ADD_POLL_FD,
//    LWS_CALLBACK_DEL_POLL_FD,
//    LWS_CALLBACK_CHANGE_MODE_POLL_FD,
//    LWS_CALLBACK_LOCK_POLL,
//    LWS_CALLBACK_UNLOCK_POLL,

};

#define CONTEXT_PORT_NO_LISTEN 0

#define LWS_SEND_BUFFER_PRE_PADDING 0
#define LWS_SEND_BUFFER_POST_PADDING 0

enum lws_write_protocol {
//        LWS_WRITE_TEXT,
		LWS_WRITE_BINARY,
};

typedef int (callback_function)(struct lws *wsi,
			 enum lws_callback_reasons reason, void *user,
	void *in, size_t len);

struct lws_protocols {
	const char *name;
	callback_function *callback;
	size_t per_session_data_size;
	size_t rx_buffer_size;
	int no_buffer_all_partial_tx;

	/*
	 * below are filled in on server init and can be left uninitialized,
	 * no need for user to use them directly either
	 */

	struct lws_context *owning_server;
};

struct lws_context_creation_info {
	int port;
	const char *iface;
	struct lws_protocols *protocols;
	struct lws_extension *extensions;
	struct lws_token_limits *token_limits;
	const char *ssl_cert_filepath;
	const char *ssl_private_key_filepath;
	const char *ssl_ca_filepath;
	const char *ssl_cipher_list;
	const char *http_proxy_address;
	unsigned int http_proxy_port;
	int gid;
	int uid;
	unsigned int options;
	void *user;
	int ka_time;
	int ka_probes;
	int ka_interval;
};

struct lws_context* lws_create_context( struct lws_context_creation_info* info );
struct lws_context* lws_create_context_extended( struct lws_context_creation_info* info );

void lws_context_destroy(struct lws_context* ctx );

struct lws* lws_client_connect_extended( struct lws_context*, const char* url, const char* protocol, void* user_data );
int lws_write( struct lws* socket, const void* data, int len, enum lws_write_protocol protocol );
void lws_callback_on_writable( struct lws* socket );

#ifdef __cplusplus
}
#endif

#endif // LIBWEBSOCKETS_ASMJS_H

/*** End of inlined file: libwebsockets_asmjs.h ***/


#else
#include "libwebsockets_native.h"	// SKIP_AMALGAMATOR_INCLUDE
#include "cert_pem.h"				// SKIP_AMALGAMATOR_INCLUDE
#include <openssl/ssl.h>
#endif


/*** Start of inlined file: libwebrtc.h ***/
#ifndef LIBWEBRTC_H
#define LIBWEBRTC_H

// shared ABI to Microstack WebRTC and Browsers WebRTC.

#ifdef __cplusplus
extern "C" {
#endif

enum libwebrtc_callback_reasons {
	LWRTC_CALLBACK_LOCAL_DESCRIPTION = 1,
	LWRTC_CALLBACK_ICE_CANDIDATE = 2,
	LWRTC_CALLBACK_ESTABLISHED = 3,
	LWRTC_CALLBACK_DISCONNECTED = 4,

	LWRTC_CALLBACK_CHANNEL_ACCEPTED = 5,
	LWRTC_CALLBACK_CHANNEL_CONNECTED = 6,
	LWRTC_CALLBACK_CHANNEL_RECEIVE = 7,
	LWRTC_CALLBACK_CHANNEL_CLOSED = 8,

	LWRTC_CALLBACK_ERROR = 9,

	LWRTC_CALLBACK_DESTROY = 10
};

struct libwebrtc_context;

// this represents the connection to the peer.
struct libwebrtc_connection;

// this represents an active data stream to the peer.
struct libwebrtc_data_channel;

typedef int (*lwrtc_callback_function)(struct libwebrtc_context *context,
	struct libwebrtc_connection *connection, struct libwebrtc_data_channel* channel,
			 enum libwebrtc_callback_reasons reason, void *user,
	void *in, int len);

struct libwebrtc_context* libwebrtc_create_context( lwrtc_callback_function );
void libwebrtc_destroy_context( struct libwebrtc_context* );

void libwebrtc_set_stun_servers( struct libwebrtc_context* ctx, const char** servers, int count);
void libwebrtc_add_turn_server( struct libwebrtc_context* ctx, const char* server, const char* username, const char* password);

struct libwebrtc_connection* libwebrtc_create_connection_extended( struct libwebrtc_context*, void* user_data );
struct libwebrtc_data_channel* libwebrtc_create_channel( struct libwebrtc_connection* conn, const char* name );

int libwebrtc_create_offer( struct libwebrtc_connection* );
int libwebrtc_set_offer( struct libwebrtc_connection* , const char* sdp );
int libwebrtc_set_answer( struct libwebrtc_connection*, const char* sdp );
int libwebrtc_add_ice_candidate( struct libwebrtc_connection*, const char* candidate );

int libwebrtc_write( struct libwebrtc_data_channel*, const void*, int len );

void libwebrtc_close_channel( struct libwebrtc_data_channel* );
void libwebrtc_close_connection( struct libwebrtc_connection* );

#ifdef __cplusplus
}
#endif

#endif // LIBWEBRTC_H

/*** End of inlined file: libwebrtc.h ***/

// TODO: should have a way to disable this on release builds
#define LOG printf

struct internal_socket_t {
	bool owner;
	bool closing;			// if this is set, ignore close attempts as the close process has already been initiated.
	void* user_data;
	internal_callbacks_t callbacks;

	// web socket connection info
	struct lws *wsi;
	std::string url;

	// webrtc connection info
	struct libwebrtc_connection* webrtc;
	struct libwebrtc_data_channel* webrtc_channel;

	internal_socket_t(bool owner=true)
	:owner(owner)
	,closing(false)
	,user_data(NULL)
	,wsi(NULL)
	,webrtc(NULL)
	,webrtc_channel(NULL)
	{}

	~internal_socket_t(){
		assert( owner );
	}
};

struct internal_context_t {
	internal_callbacks_t callbacks;

	std::map<std::string,internal_callbacks_t> protocols;

	std::string turnServer;
	std::string turnUsername;
	std::string turnPassword;
	std::vector<std::string> stunServerList;

	// websocket
	struct lws_context *websocket;

	// webrtc
	struct libwebrtc_context* webrtc;
};

static internal_context_t* g_context;

int websocket_protocol(  struct lws *wsi
					   , enum lws_callback_reasons reason
					   , void *user, void *in, size_t len) {

//	LOG("%p %p %d %p\n", context, wsi, reason, user );

	internal_socket_t* socket = reinterpret_cast<internal_socket_t*>( user );

	int ret = 0;

	switch (reason) {
		case LWS_CALLBACK_WSI_CREATE: {
			socket->wsi = wsi;
		}
		break;

		case LWS_CALLBACK_WSI_DESTROY: {
			ret = socket->callbacks.on_destroy( socket, socket->user_data );
			if( socket->owner )
				delete socket;
		}
		break;

		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
			if( socket ) {
				socket->closing = true;
				ret = socket->callbacks.on_disconnect( socket, socket->user_data );
			}
		} break;

		case LWS_CALLBACK_ESTABLISHED:
		{
			ret = socket->callbacks.on_accept( socket, socket->user_data );
		}
		break;

		case LWS_CALLBACK_CLIENT_ESTABLISHED:
		{
			 ret = socket->callbacks.on_connect( socket, socket->user_data );
		}
		break;

		case LWS_CALLBACK_CLOSED:
		{
			socket->closing = true;
			ret = socket->callbacks.on_disconnect( socket, socket->user_data );
		}
		break;

		case LWS_CALLBACK_RECEIVE:
		case LWS_CALLBACK_CLIENT_RECEIVE:
		{
			ret = socket->callbacks.on_data( socket, in, len, socket->user_data );
		}
		break;

		case LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION: {
			std::map<std::string, internal_callbacks_t>::iterator it = g_context->protocols.find( std::string( (const char*)in, len ) );
			if( it == g_context->protocols.end() ) {
				LOG("Unknown protocol: %s\n", (const char*)in);
				return -1;
			}

			socket->callbacks = it->second;
			return 0;
		} break;

		case LWS_CALLBACK_CLIENT_WRITEABLE:
		case LWS_CALLBACK_SERVER_WRITEABLE:
		{
			ret = socket->callbacks.on_writable( socket, socket->user_data );
		}
		break;

#if !defined(__EMSCRIPTEN__)
		case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS:
		{
			int i, count = 0;
			SSL_CTX *ctx = (SSL_CTX*)user;
			BIO *in = NULL;
			X509 *x = NULL;
			in = BIO_new_mem_buf((void*)cert_pem, cert_pem_len);
			if (in == NULL) {
				break;
			}

			X509_STORE *store = SSL_CTX_get_cert_store( ctx );

			for (;;) {
				x = PEM_read_bio_X509_AUX(in, NULL, NULL, NULL);
				if (x == NULL) {
					if ((ERR_GET_REASON(ERR_peek_last_error()) == PEM_R_NO_START_LINE) && (count > 0)) {
						ERR_clear_error();
						break;
					} else {
						OPENSSL_PUT_ERROR(X509, ERR_R_PEM_LIB);
						break;
					}
				}
				i = X509_STORE_add_cert(store, x);
				if (!i) break;
				count++;
				X509_free(x);
				x = NULL;
			}
		}
			break;
#endif

		default:
			//LOG("callback_humblenet %p %p %u %p %p %u\n", context, wsi, reason, user, in, static_cast<unsigned int>(len));
			break;
	}

	return ret;
}

int webrtc_protocol(struct libwebrtc_context *context,
							  struct libwebrtc_connection *connection, struct libwebrtc_data_channel* channel,
							  enum libwebrtc_callback_reasons reason, void *user,
							  void *in, int len)
{
	internal_socket_t* socket = reinterpret_cast<internal_socket_t*>( user );

//	LOG("%p %p %p %d %p\n", context, connection, channel, reason, user );

	int ret = 0;

	switch( reason ) {
		case LWRTC_CALLBACK_LOCAL_DESCRIPTION:
			ret = socket->callbacks.on_sdp( socket, (const char*)in, socket->user_data );
			break;

		case LWRTC_CALLBACK_ICE_CANDIDATE:
			ret = socket->callbacks.on_ice_candidate( socket, (const char*)in, socket->user_data );
			break;

		case LWRTC_CALLBACK_ESTABLISHED:
			ret = socket->callbacks.on_connect( socket, socket->user_data );
			break;

		case LWRTC_CALLBACK_DISCONNECTED:
			socket->closing = true;
			ret = socket->callbacks.on_disconnect( socket, socket->user_data );
			break;

		case LWRTC_CALLBACK_CHANNEL_ACCEPTED:
			socket->webrtc_channel = channel;
			ret = socket->callbacks.on_accept_channel( socket, (const char*)in, socket->user_data );
			break;

		case LWRTC_CALLBACK_CHANNEL_CONNECTED:
			socket->webrtc_channel = channel;
			ret = socket->callbacks.on_connect_channel( socket, (const char*)in, socket->user_data );
			break;

		case LWRTC_CALLBACK_CHANNEL_RECEIVE:
			ret = socket->callbacks.on_data( socket, in, len, socket->user_data );
			break;

		case LWRTC_CALLBACK_CHANNEL_CLOSED:
			socket->webrtc_channel = NULL;

			// we are 1-1 DC -> channel, otherwise we would delegate this up.
			// socket->callbacks.on_disconnect_channel( socket, socket->user_data );

			// instead we simply close the connection as well
			if( !socket->closing && socket->webrtc )
				libwebrtc_close_connection( socket->webrtc );

			break;

		case LWRTC_CALLBACK_DESTROY:
			socket->callbacks.on_destroy( socket, socket->user_data );
			if( socket->owner )
				delete socket;
			break;
		case LWRTC_CALLBACK_ERROR:
			break;
	}

	return ret;
}

#define MAX_PROTOCOLS 16

struct lws_protocols protocols[MAX_PROTOCOLS] = {
	{ "default", websocket_protocol, sizeof(internal_socket_t) }
	,{ NULL, NULL, 0 }
};

int ok_callback() {
	LOG("Ok_CB\n");
	return 0;
}

int err_callback() {
	return -1;
}

void sanitize_callbacks( internal_callbacks_t& callbacks ) {
	intptr_t* ptr = (intptr_t*)&callbacks;

	for( int i = 0; i < sizeof(internal_callbacks_t)/sizeof(void*); ++i, ++ptr ) {
		if( !*ptr ) {
			LOG("Sanitize callback #%d\n", i+1 );
			*ptr = (intptr_t)&ok_callback;
		}
	}
}

internal_context_t* internal_init(internal_callbacks_t* callbacks) {
	internal_context_t* ctx = new internal_context_t();

	if( callbacks )
		ctx->callbacks= *callbacks;

	sanitize_callbacks( ctx->callbacks );

	struct lws_context_creation_info info;
	memset(&info, 0, sizeof(info));
	info.protocols = protocols;
	info.port = CONTEXT_PORT_NO_LISTEN;
	info.gid = -1;
	info.uid = -1;
#if 0
#if defined __APPLE__ || defined(__linux__)
	// test a few wll known locations
	const char* certs[] = { "./cert.pem", "/etc/openssl/cert.pem", "/opt/local/etc/openssl/cert.pem", "/etc/pki/tls/cert.pem" , NULL };
	for( const char** test = certs; *test; test++ ) {
		if( access( *test, F_OK ) != -1 ) {
			info.ssl_ca_filepath = *test;
			break;
		}
	}
#elif defined(WIN32)
	info.ssl_ca_filepath = "cert.pem";
#endif
#endif

	ctx->websocket = lws_create_context_extended(&info);
	ctx->webrtc = libwebrtc_create_context(&webrtc_protocol);

	g_context = ctx;

	return ctx;
}

bool internal_supports_webRTC(internal_context_t* ctx) {
	return ctx->webrtc != NULL;
}

void internal_set_stun_servers( internal_context_t* ctx, const char** servers, int count){
	if( ctx->webrtc )
		libwebrtc_set_stun_servers( ctx->webrtc, servers, count );

	ctx->stunServerList.clear();
	for( int i = 0; i < count; ++i ) {
		ctx->stunServerList.push_back( *servers );
		servers++;
	}
}

void internal_add_turn_server( internal_context_t* ctx, const char* server, const char* username, const char* password ) {
	libwebrtc_add_turn_server( ctx->webrtc, server, username, password );
	ctx->turnServer = server;
	ctx->turnUsername = username;
	ctx->turnPassword = password;
}

void internal_set_callbacks(internal_socket_t* socket, internal_callbacks_t* callbacks ) {
	if( ! callbacks ) {
		socket->callbacks = g_context->callbacks;
	} else {
		socket->callbacks = *callbacks;
		sanitize_callbacks( socket->callbacks );
	}
}

void internal_register_protocol( internal_context_t* ctx, const char* name, internal_callbacks_t* callbacks ) {
	internal_callbacks_t cb = *callbacks;
	sanitize_callbacks( cb );

	ctx->protocols.insert( std::make_pair( std::string(name), cb ) );
	// TODO: Sanatize callbacks

	lws_protocols* protocol = protocols + ctx->protocols.size();
	// make sure the next protocols is "empty"
	*(protocol + 1) = *protocol;
	// now copy the prior record
	*protocol = *(protocol-1);
	// and update the name
	protocol->name = name;
}

void internal_deinit(internal_context_t* ctx) {
	// how to destroy connection factory
	if (!ctx) return;

	lws_context_destroy( ctx->websocket );
	if( ctx->webrtc )
		libwebrtc_destroy_context( ctx->webrtc );

	delete ctx;
}

void internal_set_data(internal_socket_t* socket, void* user_data) {
	socket->user_data = user_data;
}

void * internal_get_data(internal_socket_t* socket ) {
	return socket->user_data;
}

internal_socket_t* internal_connect_websocket( const char *server_addr, const char* protocol ) {
	internal_socket_t* socket = new internal_socket_t(true);

	socket->wsi = lws_client_connect_extended(g_context->websocket, server_addr, protocol, socket );
	if (socket->wsi == NULL) {
		delete socket;
		return NULL;
	}

	socket->callbacks = g_context->protocols.find( protocol )->second;

	socket->url = server_addr;

	return socket;
}

internal_socket_t* internal_create_webrtc(internal_context_t* ctx) {
	if( ! ctx->webrtc )
		return NULL;

	internal_socket_t* socket = new internal_socket_t(true);

	socket->webrtc = libwebrtc_create_connection_extended( ctx->webrtc, socket );
	socket->callbacks = ctx->callbacks;

	return socket;
}

int internal_create_offer(internal_socket_t* socket ) {
	assert( socket->webrtc != NULL );
	assert( socket->webrtc_channel == NULL );

	if( ! libwebrtc_create_offer( socket->webrtc ) )
		return 0;

	return 1;
}

int internal_set_offer( internal_socket_t* socket, const char* inOffer ){
	assert( socket->webrtc != NULL );
	assert( socket->webrtc_channel == NULL );

	if( ! libwebrtc_set_offer( socket->webrtc, inOffer ) )
		return 0;
	else
		return 1;
}

int internal_set_answer( internal_socket_t* socket, const char* inOffer ){
	assert( socket->webrtc != NULL );
	assert( socket->webrtc_channel == NULL );

	if( ! libwebrtc_set_answer( socket->webrtc, inOffer ) )
		return 0;
	else
		return 1;
}

int internal_add_ice_candidate( internal_socket_t* socket, const char* candidate ) {
	assert( socket->webrtc != NULL );
	return libwebrtc_add_ice_candidate( socket->webrtc, candidate );
}

int internal_create_channel( internal_socket_t* socket, const char* name ){
	assert( socket->webrtc != NULL );
	assert( socket->webrtc_channel == NULL );

	socket->webrtc_channel = libwebrtc_create_channel(socket->webrtc, name );

	if( !socket->webrtc_channel )
	   return 0;

	return 1;
}

void internal_close_socket( internal_socket_t* socket ) {
	if( socket->closing )
		// socket clos process has already started, ignore the request.
		return;
	else if( socket->wsi ) {
		lws_callback_on_writable(socket->wsi);
		// TODO: How do we clean up ?
	} else if( socket->webrtc ) {
		// this will trigger the destruction of the channel and thus the destruction of our socket object.
		socket->closing = true;
		libwebrtc_close_connection( socket->webrtc );
	} else {
		assert( "Destroyed socket passed to close" == NULL );
	}
}

int internal_write_socket(internal_socket_t* socket, const void *buf, int bufsize) {
	if( socket->wsi ) {
		// TODO: Should this buffer the data like the docuemntation states and only write on the writable callback ?

#if LWS_SEND_BUFFER_PRE_PADDING == 0 && LWS_SEND_BUFFER_POST_PADDING == 0
		int retval = lws_write(socket->wsi, buf, bufsize, LWS_WRITE_BINARY);
#else
		// libwebsocket requires the caller to allocate the frame prefix/suffix storage.
		std::vector<unsigned char> sendbuf(LWS_SEND_BUFFER_PRE_PADDING + bufsize + LWS_SEND_BUFFER_POST_PADDING, 0);
		memcpy(&sendbuf[LWS_SEND_BUFFER_PRE_PADDING], buf, bufsize);

		int retval = lws_write(socket->wsi, &sendbuf[LWS_SEND_BUFFER_PRE_PADDING], bufsize, LWS_WRITE_BINARY);
#endif

		// mark it non-writable and tell websocket to inform us when it's writable again
		//connection->writable = false;
		if( retval > 0 ) {
			lws_callback_on_writable(socket->wsi);
		}

		return retval;

	} else if( socket->webrtc && socket->webrtc_channel ) {
		return libwebrtc_write( socket->webrtc_channel, buf, bufsize );
	}

	// bad/disconnected socket
	return -1;
}

/*** End of inlined file: libsocket.cpp ***/


// WEBRTC wrapper

/*** Start of inlined file: libwebrtc_asmjs.cpp ***/
#ifdef __EMSCRIPTEN__

#include <emscripten.h>

#define USE_STUN

struct libwebrtc_context {
	lwrtc_callback_function callback;
};

extern "C"
int EMSCRIPTEN_KEEPALIVE libwebrtc_helper(struct libwebrtc_context *context, struct libwebrtc_connection *connection, struct libwebrtc_data_channel* channel,
								  enum libwebrtc_callback_reasons reason, void *user, void *in, int len) {
	return context->callback( context, connection, channel, reason, user, in, len );
}

struct libwebrtc_context* libwebrtc_create_context( lwrtc_callback_function callback ) {
	libwebrtc_context* ctx = new libwebrtc_context();
	ctx->callback = callback;

	bool supported = EM_ASM_INT({
		var libwebrtc = {};

		if( ! self.RTCPeerConnection || ! self.RTCIceCandidate || ! self.RTCSessionDescription )
			return 0;

		var ctx = $0;
		libwebrtc.connections = new Map();
		libwebrtc.channels = new Map();
		libwebrtc.on_event = Module.cwrap('libwebrtc_helper', 'number', ['number', 'number', 'number', 'number', 'number', 'number', 'number']);
		libwebrtc.options = {};

		libwebrtc.create = function() {
			var connection = new RTCPeerConnection(this.options,null);

			connection.trickle = true;

			connection.destroy = this.destroy;

			connection.ondatachannel = this.on_datachannel;
			connection.onicecandidate = this.on_candidate;
			connection.onsignalingstatechange = this.on_signalstatechange;
			connection.oniceconnectionstatechange = this.on_icestatechange;

			connection.id = this.connections.size + 1;

			this.connections.set( connection.id, connection );

			return connection;
		};
		libwebrtc.create_channel = function(connection, name) {
			var channel = connection.createDataChannel( name, {ordered: false, maxRetransmits: 0} );
			channel.parent = connection;
			// use the parents data initially
			channel.user_data = connection.user_data;
			channel.binaryType = 'arraybuffer';

			channel.onopen = libwebrtc.on_channel_connected;
			channel.onclose = libwebrtc.on_channel_close;
			channel.onmessage = libwebrtc.on_channel_message;
			channel.onerror = libwebrtc.on_channel_error;

			channel._id = libwebrtc.channels.size+1;

			libwebrtc.channels.set( channel._id, channel);

			return channel;
		};

		libwebrtc.on_sdp = function(){
			if( ! this.trickle && this.iceGatheringState != "complete" ) {
				return;
			}

			var sdp = this.localDescription.sdp;
			var stack = stackSave();
			// local description //
			const array  = intArrayFromString(sdp);
			const buffer = stackAlloc(array.length);
			Module.HEAPU8.set(array, buffer);
			libwebrtc.on_event( ctx, this.id, 0, 1, this.user_data, buffer, sdp.length);
			stackRestore(stack);
		};
		libwebrtc.on_candidate = function(event){
			if( !event ) {
				return;
			}

			if( this.iceConnectionState === 'completed') {
				Module.out("ignoring ice, were not trying to connect: " + this.iceConnectionState);
				return;
			}

			if( !event.candidate ) {
				Module.out("no more candidates: " + this.iceGatheringState );
				if( ! this.trickle ) {
					libwebrtc.on_sdp.call(this);
				}
				return;
			}

			Module.out("ice candidate " + event.candidate.candidate + " -- " + this.iceGatheringState);

			if( this.trickle ) {
				var stack = stackSave();
				// ice_candidate //
				const array = intArrayFromString(event.candidate.candidate);
				const buffer = stackAlloc(array.length);
				Module.HEAPU8.set(array, buffer);
				libwebrtc.on_event( ctx, this.id, 0, 2, this.user_data, buffer, event.candidate.candidate.length);
				stackRestore(stack);
			}
		};
		libwebrtc.on_signalstatechange = function(event){
			Module.out("signalingState: "+ this.signalingState);
		};
		libwebrtc.on_icestatechange = function(event){
			Module.out( "icestate: " + this.iceConnectionState + " / iceGatheringState: " + this.iceGatheringState);
			if( this.iceConnectionState == 'failed' || this.iceConnectionState == 'disconnected'  ) {
				this.close();
			} else if( this.iceConnectionState == 'closed' ) {
				libwebrtc.on_disconnected.call(this,event);
			} else if( this.iceConnectionState == 'completed' ) {
				// connected //
				libwebrtc.on_event( ctx, this.id, 0, 3, this.user_data, 0, 0);
			}
		};
		libwebrtc.on_disconnected = function(event){
			var stack = stackSave();
			// disconnected //
			libwebrtc.on_event( ctx, this.id, 0, 4, this.user_data, 0, 0);
			stackRestore(stack);
			this.destroy();
		};
		libwebrtc.on_datachannel = function(event){
			Module.out("datachannel");
			var channel = event.channel;

			channel.parent = this;
			// use the parents data initially
			channel.user_data = this.user_data;
			channel.binaryType = 'arraybuffer';

			channel.onopen = libwebrtc.on_channel_accept;
			channel.onclose = libwebrtc.on_channel_close;
			channel.onmessage = libwebrtc.on_channel_message;
			channel.onerror = libwebrtc.on_channel_error;

			channel._id = libwebrtc.channels.size+1;

			libwebrtc.channels.set( channel._id, channel);
		};
		libwebrtc.on_channel_accept = function(event){
			Module.out("accept");
			var stack = stackSave();
			// channel accepted //
			const array = intArrayFromString(this.label);
			const buffer = stackAlloc(array.length);
			Module.HEAPU8.set(array, buffer);
			libwebrtc.on_event(ctx, this.parent.id, this._id, 5, this.user_data, buffer, this.label.length);
			stackRestore(stack);
		};
		libwebrtc.on_channel_connected = function(event){
			Module.out("connect");
			var stack = stackSave();
			// channel connected //
			const array = intArrayFromString(this.label);
			const buffer = stackAlloc(array.length);
			Module.HEAPU8.set(array, buffer);
			libwebrtc.on_event(ctx, this.parent.id, this._id, 6, this.user_data, buffer, this.label.length);
			stackRestore(stack);
		};
		libwebrtc.on_channel_message = function(event){
			var stack = stackSave();
			var len = event.data.byteLength;
			var ptr = stackAlloc(len);

//            Module.out("Data: " + len );
			var data = new Uint8Array( event.data );
			Module.HEAPU8.set(data, ptr);

			// channel data //
			libwebrtc.on_event( ctx, this.parent.id, this._id, 7, this.user_data, ptr, len);
			stackRestore(stack);
		};
		libwebrtc.on_channel_error = function(event){
			Module.out("Got channel error: " + event);
			this.close();
		};
		libwebrtc.on_channel_close = function(event){
			var stack = stackSave();
			// close channel //
			libwebrtc.on_event(ctx, this.parent.id, this._id, 8, this.user_data, 0, 0);
			stackRestore(stack);
		};
		libwebrtc.destroy = function() {
			libwebrtc.connections.set( this.id, undefined );

			this.ondatachannel = undefined;
			this.onicecandidate = undefined;
			this.onsignalingstatechange = undefined;
			this.oniceconnectionstatechange = undefined;

			// destroy (connection) //
			libwebrtc.on_event(ctx, this.id, 0, 10, this.user_data, 0, 0);
			this.close();
			Module.out("Destroy webrtc: " + this.id );
		};

		Module.__libwebrtc = libwebrtc;

		return 1;
	}, ctx);

	if( !supported ) {
		delete ctx;
		return nullptr;
	}

	return ctx;
}

void libwebrtc_destroy_context(struct libwebrtc_context* ctx)
{
	delete ctx;
}

void libwebrtc_set_stun_servers( struct libwebrtc_context* ctx, const char** servers, int count)
{
	EM_ASM({
		Module.__libwebrtc.options.iceServers = Module.__libwebrtc.options.iceServers || [];
	});

	for( int i = 0; i < count; ++i ) {
		EM_ASM_INT({
			var server = {};
			server.urls = "stun:" + UTF8ToString($0);
			Module.__libwebrtc.options.iceServers.push( server );
		}, *servers);
		servers++;
	}
}

void libwebrtc_add_turn_server( struct libwebrtc_context* ctx, const char* server, const char* username, const char* password) {
	EM_ASM({
		Module.__libwebrtc.options.iceServers = Module.__libwebrtc.options.iceServers || [];
		Module.__libwebrtc.options.iceServers.push({
			urls: "turn:" + UTF8ToString($0),
			username: UTF8ToString($1),
			credential: UTF8ToString($2)
		});
	}, server, username, password);
}

struct libwebrtc_connection* libwebrtc_create_connection_extended(struct libwebrtc_context* ctx, void* user_data) {
	return (struct libwebrtc_connection*)EM_ASM_INT({
		var connection = Module.__libwebrtc.create();
		connection.user_data = $0;
		return connection.id;
	}, user_data);
}

void libwebrtc_set_user_data(struct libwebrtc_connection* connection, void* user_data ) {
	EM_ASM_INT({
		var connection = Module.__libwebrtc.connections.get($0);
		if( ! connection ) {
			return;
		}
		connection.user_data = $1;
	}, connection, user_data);
}

int libwebrtc_create_offer( struct libwebrtc_connection* connection ) {
	return EM_ASM_INT({
		var connection = Module.__libwebrtc.connections.get($0);
		if( ! connection ) {
			return 0;
		}

		// in order to create and offer at least one stream must have been added.
		// simply create one, then drop it.
		connection.default_channel = Module.__libwebrtc.create_channel( connection,"default");

		connection.createOffer({})
			.then(function(offer){
				connection.setLocalDescription( new RTCSessionDescription( offer ) )
					.then( function() {
						Module.__libwebrtc.on_sdp.call( connection );
					}).catch(function(error){
						alert( "setLocalDescription(create): " + error );
					});
			}).catch(function(error){
				alert("createOffer: " + error);
			});

		return 1;
	}, connection, 0);
}

int libwebrtc_set_offer( struct libwebrtc_connection* connection, const char* sdp ) {
	return EM_ASM_INT({
		var connection = Module.__libwebrtc.connections.get($0);
		if( ! connection ) {
			return 0;
		}

		var offer = {};
		offer.type = 'offer';
		offer.sdp = UTF8ToString( $1 );

		connection.setRemoteDescription( new RTCSessionDescription( offer ) )
			.then(function() {
				connection.createAnswer()
					.then(function(offer){
						connection.setLocalDescription( new RTCSessionDescription( offer ) )
							.then( function() {
								Module.__libwebrtc.on_sdp.call( connection );
							}).catch(function(error){
								alert( "setLocalDescription(answer): " + error );
							});
					}).catch(function(error){
						alert("createAnswer: " + error);
				});
			}).catch(function(error){
				alert("setRemoteDescriptor(answer): " + error );
			});
		return 1;
	}, connection, sdp );
}

int libwebrtc_set_answer( struct libwebrtc_connection* connection, const char* sdp ) {
	return EM_ASM_INT({
		var connection = Module.__libwebrtc.connections.get($0);
		if( ! connection ) {
			return 0;
		}

		var offer = {};
		offer.type = 'answer';
		offer.sdp = UTF8ToString( $1 );

		connection.setRemoteDescription( new RTCSessionDescription( offer ) )
			.then( function() {
				// nothing, as this is the answer to our offer
			}).catch(function(error){
				alert("setRemoteDescriptor(answer): " + error );
			});
		return 1;
	}, connection, sdp );
}

int libwebrtc_add_ice_candidate( struct libwebrtc_connection* connection, const char* candidate ) {
	return EM_ASM_INT({
		var connection = Module.__libwebrtc.connections.get($0);
		if( ! connection ) {
			return 0;
		}

		var options = {};
		options.candidate = UTF8ToString($1);
		options.sdpMLineIndex = 0;

		if( connection.iceConnectionState == 'checking' || connection.iceConnectionState == 'connected'
		   // FF workaround
		   || connection.iceConnectionState == 'new') {
			Module.out( "AddIce: " + options.candidate );
			connection.addIceCandidate( new RTCIceCandidate( options ) );
		} else {
			Module.out( "Not negotiating (" + connection.iceConnectionState + "), ignored candidate: " + options.candidate );
		}

	}, connection, candidate );
}

struct libwebrtc_data_channel* libwebrtc_create_channel( struct libwebrtc_connection* connection, const char* name ) {
	return (struct libwebrtc_data_channel*)EM_ASM_INT({
		var connection = Module.__libwebrtc.connections.get($0);
		if( ! connection ) {
			return 0;
		}

		var channel;

		if( connection.default_channel ){
			channel = connection.default_channel;
			connection.default_channel = 0;
		}else{
			channel = Module.__libwebrtc.create_channel( connection, UTF8ToString($1) );
		}

		return channel._id;

	}, connection, name );
}

int libwebrtc_write( struct libwebrtc_data_channel* channel, const void* data, int len ) {
	return EM_ASM_INT({
		var channel = Module.__libwebrtc.channels.get($0);
		if( ! channel ) {
			return -1;
		}

		// alloc a Uint8Array backed by the incoming data.
		var data_in = new Uint8Array(Module.HEAPU8.buffer, $1, $2 );
		// allow the dest array
		var data = new Uint8Array($2);
		// set the dest from the src
		data.set(data_in);

		channel.send( data );
		return $2;

	}, channel, data, len );
}

void libwebrtc_close_connection( struct libwebrtc_connection* channel ) {
	EM_ASM_INT({
		var connection = Module.__libwebrtc.connections.get($0);
		if( ! connection ) {
			return -1;
		}

		Module.__libwebrtc.connections.set( connection.id, undefined );

		connection.close();

	}, channel );
}

void libwebrtc_close_channel( struct libwebrtc_data_channel* channel ) {
	EM_ASM_INT({
		var channel = Module.__libwebrtc.channels.get($0);
		if( ! channel ) {
			return -1;
		}

		Module.__libwebrtc.connections.set( channel.id, undefined );

		channel.close();
	}, channel );
}

#endif

/*** End of inlined file: libwebrtc_asmjs.cpp ***/


// Websockets Wrapper

/*** Start of inlined file: libwebsockets_asmjs.cpp ***/
#ifdef __EMSCRIPTEN__


/*** Start of inlined file: libwebsockets_asmjs.h ***/
#ifndef __EMSCRIPTEN__
#error "This should only be used under emscripten"
#endif

#ifndef LIBWEBSOCKETS_ASMJS_H
#define LIBWEBSOCKETS_ASMJS_H

// ABI compatible subset of libwebsockets

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

enum lws_callback_reasons {
	LWS_CALLBACK_ESTABLISHED = 1,
	LWS_CALLBACK_CLIENT_CONNECTION_ERROR = 2,
//    LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH,
	LWS_CALLBACK_CLIENT_ESTABLISHED = 3,
	LWS_CALLBACK_CLOSED = 4,
 //   LWS_CALLBACK_CLOSED_HTTP,
	LWS_CALLBACK_RECEIVE = 5,
	LWS_CALLBACK_CLIENT_RECEIVE = 6,
 //   LWS_CALLBACK_CLIENT_RECEIVE_PONG,
	LWS_CALLBACK_CLIENT_WRITEABLE = 7,
	LWS_CALLBACK_SERVER_WRITEABLE = 8,
 //   LWS_CALLBACK_HTTP,
 //   LWS_CALLBACK_HTTP_BODY,
 //   LWS_CALLBACK_HTTP_BODY_COMPLETION,
 //   LWS_CALLBACK_HTTP_FILE_COMPLETION,
 //   LWS_CALLBACK_HTTP_WRITEABLE,
 //   LWS_CALLBACK_FILTER_NETWORK_CONNECTION,
 //   LWS_CALLBACK_FILTER_HTTP_CONNECTION,
 //   LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED,
	LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION = 9,
 //   LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS,
//    LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS,
//    LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION,
//    LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER,
//    LWS_CALLBACK_CONFIRM_EXTENSION_OKAY,
//    LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED,
//    LWS_CALLBACK_PROTOCOL_INIT,
//    LWS_CALLBACK_PROTOCOL_DESTROY,
	LWS_CALLBACK_WSI_CREATE = 10, /* always protocol[0] */
	LWS_CALLBACK_WSI_DESTROY = 11, /* always protocol[0] */
//    LWS_CALLBACK_GET_THREAD_ID,

	/* external poll() management support */
//    LWS_CALLBACK_ADD_POLL_FD,
//    LWS_CALLBACK_DEL_POLL_FD,
//    LWS_CALLBACK_CHANGE_MODE_POLL_FD,
//    LWS_CALLBACK_LOCK_POLL,
//    LWS_CALLBACK_UNLOCK_POLL,

};

#define CONTEXT_PORT_NO_LISTEN 0

#define LWS_SEND_BUFFER_PRE_PADDING 0
#define LWS_SEND_BUFFER_POST_PADDING 0

enum lws_write_protocol {
//        LWS_WRITE_TEXT,
		LWS_WRITE_BINARY,
};

typedef int (callback_function)(struct lws *wsi,
			 enum lws_callback_reasons reason, void *user,
	void *in, size_t len);

struct lws_protocols {
	const char *name;
	callback_function *callback;
	size_t per_session_data_size;
	size_t rx_buffer_size;
	int no_buffer_all_partial_tx;

	/*
	 * below are filled in on server init and can be left uninitialized,
	 * no need for user to use them directly either
	 */

	struct lws_context *owning_server;
};

struct lws_context_creation_info {
	int port;
	const char *iface;
	struct lws_protocols *protocols;
	struct lws_extension *extensions;
	struct lws_token_limits *token_limits;
	const char *ssl_cert_filepath;
	const char *ssl_private_key_filepath;
	const char *ssl_ca_filepath;
	const char *ssl_cipher_list;
	const char *http_proxy_address;
	unsigned int http_proxy_port;
	int gid;
	int uid;
	unsigned int options;
	void *user;
	int ka_time;
	int ka_probes;
	int ka_interval;
};

struct lws_context* lws_create_context( struct lws_context_creation_info* info );
struct lws_context* lws_create_context_extended( struct lws_context_creation_info* info );

void lws_context_destroy(struct lws_context* ctx );

struct lws* lws_client_connect_extended( struct lws_context*, const char* url, const char* protocol, void* user_data );
int lws_write( struct lws* socket, const void* data, int len, enum lws_write_protocol protocol );
void lws_callback_on_writable( struct lws* socket );

#ifdef __cplusplus
}
#endif

#endif // LIBWEBSOCKETS_ASMJS_H

/*** End of inlined file: libwebsockets_asmjs.h ***/

#include <emscripten.h>
#include <stdio.h>
// TODO: should have a way to disable this on release builds
#define LOG printf

struct lws_context {
	lws_protocols* protocols; // we dont have any state to manage.
};

extern "C" int EMSCRIPTEN_KEEPALIVE lws_helper( int protocol, struct lws_context *context,
														struct lws *wsi,
														enum lws_callback_reasons reason, void *user,
														void *in, size_t len ) {
	LOG("%d -> %d -> %p -> %d\n", protocol, reason, in, (int)len );

	if( reason == LWS_CALLBACK_WSI_DESTROY ) {
		context->protocols[protocol].callback( wsi, reason, user, in, len );
		// TODO See if we need to destroy the user_data..currently we dont allocate it, so we never would need to free it.
		return 0;
	}
	else
		return context->protocols[protocol].callback( wsi, reason, user, in, len );
}

struct lws_context* lws_create_context( struct lws_context_creation_info* info ){
	return lws_create_context_extended( info );
}

struct lws_context* lws_create_context_extended( struct lws_context_creation_info* info ) {
	struct lws_context* ctx = new lws_context();
	ctx->protocols = info->protocols;

	EM_ASM_({
		var libwebsocket = {};
		var ctx = $0;

		libwebsocket.sockets = new Map();
		libwebsocket.on_event = Module.cwrap('lws_helper', 'number', ['number', 'number', 'number', 'number', 'number', 'number', 'number']);
		libwebsocket.connect = function( url, protocol, user_data ) {
			try {
				var socket = new WebSocket(url,protocol);
				socket.binaryType = "arraybuffer";
				socket.user_data = user_data;
				socket.protocol_id = 0;

				socket.onopen = this.on_connect;
				socket.onmessage = this.on_message;
				socket.onclose = this.on_close;
				socket.onerror = this.on_error;
				socket.destroy = this.destroy;

				socket.id = this.sockets.size + 1;

				this.sockets.set( socket.id, socket );

				return socket;
			} catch(e) {
				Module.out("Socket creation failed:" + e);
				return 0;
			}
		};
		libwebsocket.on_connect = function() {
			var stack = stackSave();
			// filter protocol //
			const array = intArrayFromString(this.protocol);
			const buffer = stackAlloc(array.length);
			Module.HEAPU8.set(array, buffer);
			var ret = libwebsocket.on_event( 0, ctx, this.id, 9, this.user_data, buffer, this.protocol.length );
			if( !ret ) {
				// client established
				ret = libwebsocket.on_event( this.protocol_id, ctx, this.id, 3, this.user_data, 0, 0 );
			}
			if( ret ) {
				this.close();
			}
			stackRestore(stack);
		};
		libwebsocket.on_message = function(event) {
			var stack = stackSave();
			var len = event.data.byteLength;
			var data = new Uint8Array( event.data );
			const ptr = stackAlloc(data.length);
			Module.HEAPU8.set(data, ptr);

			// client receive //
			if( libwebsocket.on_event( this.protocol_id, ctx, this.id, 6, this.user_data, ptr, len ) ) {
				this.close();
			}
			stackRestore(stack);
		};
		libwebsocket.on_close = function() {
			if (!this.user_data) return;
			// closed //
			libwebsocket.on_event( this.protocol_id, ctx, this.id, 4, this.user_data, 0, 0 );
			this.destroy();
		};
		libwebsocket.on_error = function() {
			if (!this.user_data) return;
			// client connection error //
			libwebsocket.on_event( this.protocol_id, ctx, this.id, 2, this.user_data, 0, 0 );
			this.destroy();
		};
		libwebsocket.destroy = function() {
			if (!this.user_data) return;
			var user_data = this.user_data;
			this.user_data = 0;
			libwebsocket.sockets.set( this.id, undefined );
			libwebsocket.on_event( this.protocol_id, ctx, this.id, 11, user_data, 0, 0 );
		};

		Module.__libwebsocket = libwebsocket;
	}, ctx  );

	return ctx;
}

void lws_context_destroy(struct lws_context* ctx ) {
	delete ctx;
}

struct lws* lws_client_connect_extended(struct lws_context* ctx , const char* url, const char* protocol, void* user_data ) {

	struct lws* s =  (struct lws*)EM_ASM_INT({
		var socket = Module.__libwebsocket.connect( UTF8ToString($0), UTF8ToString($1), $2);
		if( ! socket ) {
			return 0;
		}

		return socket.id;
	}, url, protocol, user_data);

	return s;
}

int lws_write( struct lws* socket, const void* data, int len, enum lws_write_protocol protocol ) {
	return EM_ASM_INT({
		var socket = Module.__libwebsocket.sockets.get( $0 );
		if( ! socket || socket.readyState !== 1) {
			return -1;
		}

		// alloc a Uint8Array backed by the incoming data.
		var data_in = new Uint8Array(Module.HEAPU8.buffer, $1, $2 );
		// allow the dest array
		var data = new Uint8Array($2);
		// set the dest from the src
		data.set(data_in);

		socket.send( data );

		return $2;

	}, socket, data, len );
}

void lws_callback_on_writable( struct lws* socket ) {
	// no-op
}

#endif

/*** End of inlined file: libwebsockets_asmjs.cpp ***/


// 3rd party

/*** Start of inlined file: crc.c ***/
#include <stdlib.h>
#include <stdint.h>

/**
 * Static table used for the table_driven implementation.
 *****************************************************************************/
static const crc_t crc_table[256] = {
	0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
	0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
	0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
	0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
	0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
	0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
	0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
	0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
	0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
	0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
	0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
	0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
	0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
	0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
	0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
	0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
	0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
	0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
	0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
	0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
	0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
	0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
	0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
	0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
	0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
	0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
	0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
	0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
	0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
	0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
	0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
	0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

crc_t crc_init()
{
	return 0xffffffff;
}

/**
 * Update the crc value with new data.
 *
 * \param crc      The current crc value.
 * \param data     Pointer to a buffer of \a data_len bytes.
 * \param data_len Number of bytes in the \a data buffer.
 * \return         The updated crc value.
 *****************************************************************************/
crc_t crc_update(crc_t crc, const void *data, size_t data_len)
{
	const unsigned char *d = (const unsigned char *)data;
	unsigned int tbl_idx;

	while (data_len--) {
		tbl_idx = ((crc >> 24) ^ *d) & 0xff;
		crc = (crc_table[tbl_idx] ^ (crc << 8)) & 0xffffffff;

		d++;
	}
	return crc & 0xffffffff;
}

crc_t crc_finalize(crc_t crc)
{
	return crc ^ 0xffffffff;
}

/*** End of inlined file: crc.c ***/



/*** Start of inlined file: sha1.c ***/
/*
 *  Define the SHA1 circular left shift macro
 */
#define SHA1CircularShift(bits,word) \
				(((word) << (bits)) | ((word) >> (32-(bits))))

/* Local Function Prototyptes */
void SHA1PadMessage(SHA1Context *);
void SHA1ProcessMessageBlock(SHA1Context *);

/*
 *  SHA1Reset
 *
 *  Description:
 *      This function will initialize the SHA1Context in preparation
 *      for computing a new SHA1 message digest.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Reset(SHA1Context *context)
{
	if (!context)
	{
		return shaNull;
	}

	context->Length_Low             = 0;
	context->Length_High            = 0;
	context->Message_Block_Index    = 0;

	context->Intermediate_Hash[0]   = 0x67452301;
	context->Intermediate_Hash[1]   = 0xEFCDAB89;
	context->Intermediate_Hash[2]   = 0x98BADCFE;
	context->Intermediate_Hash[3]   = 0x10325476;
	context->Intermediate_Hash[4]   = 0xC3D2E1F0;

	context->Computed   = 0;
	context->Corrupted  = 0;

	return shaSuccess;
}

/*
 *  SHA1Result
 *
 *  Description:
 *      This function will return the 160-bit message digest into the
 *      Message_Digest array  provided by the caller.
 *      NOTE: The first octet of hash is stored in the 0th element,
 *            the last octet of hash in the 19th element.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to use to calculate the SHA-1 hash.
 *      Message_Digest: [out]
 *          Where the digest is returned.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Result( SHA1Context *context,
				uint8_t Message_Digest[SHA1HashSize])
{
	int i;

	if (!context || !Message_Digest)
	{
		return shaNull;
	}

	if (context->Corrupted)
	{
		return context->Corrupted;
	}

	if (!context->Computed)
	{
		SHA1PadMessage(context);
		for(i=0; i<64; ++i)
		{
			/* message may be sensitive, clear it out */
			context->Message_Block[i] = 0;
		}
		context->Length_Low = 0;    /* and clear length */
		context->Length_High = 0;
		context->Computed = 1;

	}

	for(i = 0; i < SHA1HashSize; ++i)
	{
		Message_Digest[i] = context->Intermediate_Hash[i>>2]
							>> 8 * ( 3 - ( i & 0x03 ) );
	}

	return shaSuccess;
}

/*
 *  SHA1Input
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the message.
 *
 *  Parameters:
 *      context: [in/out]
 *          The SHA context to update
 *      message_array: [in]
 *          An array of characters representing the next portion of
 *          the message.
 *      length: [in]
 *          The length of the message in message_array
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Input(    SHA1Context    *context,
				  const uint8_t  *message_array,
				  unsigned       length)
{
	if (!length)
	{
		return shaSuccess;
	}

	if (!context || !message_array)
	{
		return shaNull;
	}

	if (context->Computed)
	{
		context->Corrupted = shaStateError;

		return shaStateError;
	}

	if (context->Corrupted)
	{
		 return context->Corrupted;
	}
	while(length-- && !context->Corrupted)
	{
	context->Message_Block[context->Message_Block_Index++] =
					(*message_array & 0xFF);

	context->Length_Low += 8;
	if (context->Length_Low == 0)
	{
		context->Length_High++;
		if (context->Length_High == 0)
		{
			/* Message is too long */
			context->Corrupted = 1;
		}
	}

	if (context->Message_Block_Index == 64)
	{
		SHA1ProcessMessageBlock(context);
	}

	message_array++;
	}

	return shaSuccess;
}

/*
 *  SHA1ProcessMessageBlock
 *
 *  Description:
 *      This function will process the next 512 bits of the message
 *      stored in the Message_Block array.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:

 *      Many of the variable names in this code, especially the
 *      single character names, were used because those were the
 *      names used in the publication.
 *
 *
 */
void SHA1ProcessMessageBlock(SHA1Context *context)
{
	const uint32_t K[] =    {       /* Constants defined in SHA-1   */
							0x5A827999,
							0x6ED9EBA1,
							0x8F1BBCDC,
							0xCA62C1D6
							};
	int           t;                 /* Loop counter                */
	uint32_t      temp;              /* Temporary word value        */
	uint32_t      W[80];             /* Word sequence               */
	uint32_t      A, B, C, D, E;     /* Word buffers                */

	/*
	 *  Initialize the first 16 words in the array W
	 */
	for(t = 0; t < 16; t++)
	{
		W[t] = context->Message_Block[t * 4] << 24;
		W[t] |= context->Message_Block[t * 4 + 1] << 16;
		W[t] |= context->Message_Block[t * 4 + 2] << 8;
		W[t] |= context->Message_Block[t * 4 + 3];
	}

	for(t = 16; t < 80; t++)
	{
	   W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
	}

	A = context->Intermediate_Hash[0];
	B = context->Intermediate_Hash[1];
	C = context->Intermediate_Hash[2];
	D = context->Intermediate_Hash[3];
	E = context->Intermediate_Hash[4];

	for(t = 0; t < 20; t++)
	{
		temp =  SHA1CircularShift(5,A) +
				((B & C) | ((~B) & D)) + E + W[t] + K[0];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);

		B = A;
		A = temp;
	}

	for(t = 20; t < 40; t++)
	{
		temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);
		B = A;
		A = temp;
	}

	for(t = 40; t < 60; t++)
	{
		temp = SHA1CircularShift(5,A) +
			   ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);
		B = A;
		A = temp;
	}

	for(t = 60; t < 80; t++)
	{
		temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
		E = D;
		D = C;
		C = SHA1CircularShift(30,B);
		B = A;
		A = temp;
	}

	context->Intermediate_Hash[0] += A;
	context->Intermediate_Hash[1] += B;
	context->Intermediate_Hash[2] += C;
	context->Intermediate_Hash[3] += D;
	context->Intermediate_Hash[4] += E;

	context->Message_Block_Index = 0;
}

/*
 *  SHA1PadMessage
 *

 *  Description:
 *      According to the standard, the message must be padded to an even
 *      512 bits.  The first padding bit must be a '1'.  The last 64
 *      bits represent the length of the original message.  All bits in
 *      between should be 0.  This function will pad the message
 *      according to those rules by filling the Message_Block array
 *      accordingly.  It will also call the ProcessMessageBlock function
 *      provided appropriately.  When it returns, it can be assumed that
 *      the message digest has been computed.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to pad
 *      ProcessMessageBlock: [in]
 *          The appropriate SHA*ProcessMessageBlock function
 *  Returns:
 *      Nothing.
 *
 */

void SHA1PadMessage(SHA1Context *context)
{
	/*
	 *  Check to see if the current message block is too small to hold
	 *  the initial padding bits and length.  If so, we will pad the
	 *  block, process it, and then continue padding into a second
	 *  block.
	 */
	if (context->Message_Block_Index > 55)
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;
		while(context->Message_Block_Index < 64)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}

		SHA1ProcessMessageBlock(context);

		while(context->Message_Block_Index < 56)
		{
			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}
	else
	{
		context->Message_Block[context->Message_Block_Index++] = 0x80;
		while(context->Message_Block_Index < 56)
		{

			context->Message_Block[context->Message_Block_Index++] = 0;
		}
	}

	/*
	 *  Store the message length as the last 8 octets
	 */
	context->Message_Block[56] = context->Length_High >> 24;
	context->Message_Block[57] = context->Length_High >> 16;
	context->Message_Block[58] = context->Length_High >> 8;
	context->Message_Block[59] = context->Length_High;
	context->Message_Block[60] = context->Length_Low >> 24;
	context->Message_Block[61] = context->Length_Low >> 16;
	context->Message_Block[62] = context->Length_Low >> 8;
	context->Message_Block[63] = context->Length_Low;

	SHA1ProcessMessageBlock(context);
}

/*** End of inlined file: sha1.c ***/


/*** Start of inlined file: hmac.cpp ***/
#include <cstring>

void HMACInit(HMACContext *ctx, const uint8_t* key, unsigned key_len)
{
	uint8_t key_block[HMAC_BLOCKSIZE];
	uint8_t pad[HMAC_BLOCKSIZE];
	unsigned key_block_len;

	if (HMAC_BLOCKSIZE < key_len) {
		SHA1Context sha;
		SHA1Reset(&sha);
		SHA1Input(&sha, key, key_len);
		SHA1Result(&sha, key_block);
		key_block_len = HMAC_DIGEST_SIZE;
	} else {
		memcpy(key_block, key, key_len);
		key_block_len = key_len;
	}
	memset(&key_block[key_block_len], 0, sizeof(key_block) - key_block_len);

	for (int i = 0; i < HMAC_BLOCKSIZE; ++i) {
		pad[i] = 0x36 ^ key_block[i];
	}
	SHA1Reset(&ctx->i_ctx);
	SHA1Input(&ctx->i_ctx, pad, HMAC_BLOCKSIZE);

	for (int i = 0; i < HMAC_BLOCKSIZE; ++i) {
		pad[i] = 0x5c ^ key_block[i];
	}
	SHA1Reset(&ctx->o_ctx);
	SHA1Input(&ctx->o_ctx, pad, HMAC_BLOCKSIZE);
}

int HMACInput(HMACContext *ctx, const uint8_t* data, unsigned data_len)
{
	return SHA1Input(&ctx->i_ctx, data, data_len);
}

int HMACResult(HMACContext *ctx, uint8_t Message_Digest[HMAC_DIGEST_SIZE])
{
	SHA1Result(&ctx->i_ctx, Message_Digest);

	SHA1Input(&ctx->o_ctx, Message_Digest, HMAC_DIGEST_SIZE);

	SHA1Result(&ctx->o_ctx, Message_Digest);

	return shaSuccess;
}

static const char kHexLookup[] = "0123456789abcdef";

void HMACResultToHex(uint8_t digest[HMAC_DIGEST_SIZE], std::string& out_hex)
{
	out_hex.resize(HMAC_DIGEST_SIZE * 2);

	for (int i = 0; i < HMAC_DIGEST_SIZE; ++i) {
		auto b =  digest[i];
		out_hex[i * 2] = kHexLookup[b >> 4];
		out_hex[i * 2 + 1] = kHexLookup[b & 0xf];
	}
}

/*** End of inlined file: hmac.cpp ***/

