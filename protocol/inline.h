#ifndef __INLINE_H__
#define __INLINE_H__ 1
/*
 * This library implemented for support external inline linkage for different compilers:
 * - C89                    (GNU, ISO      )
 * - C99                    (GNU, ISO, MSVC)
 * - younger С - compilers  (GNU, ISO, MSVC)
 * - all С++ compilers
 */


/* If using non-GNU C, then ignore __attribute__ */
#ifndef __GNUC__
#   define __attribute__(x) /* NOTHING */
#endif /* __GNUC__ */

/*
 * *********************************************************************
 * simple INLINE
 * *********************************************************************
 */

#ifndef INLINE
#   define INLINE inline
#endif /* INLINE */


/*
 * ******************************************************
 * forceinline for garantee inlining coding
 * ******************************************************
 */
#ifdef _MSC_VER
#   define forceinline __forceinline
#elif defined(__GNUC__)
#   define forceinline INLINE __attribute__((__always_inline__))
#elif defined(__CLANG__)
#   if __has_attribute(__always_inline__)
#       define forceinline INLINE __attribute__((__always_inline__))
#   else
#       define forceinline INLINE
#   endif
#else
#   define forceinline INLINE
#endif /* forceinline switch macro */
/*
 * external force inline linkage use==>
 * .h header file must be :
 *
 * forceinline int max(int ​​a, int b)
 * {
 *    return a > b ? а : б;
 * }
 *
 * .c source file must be :
 *
 * C_INLINE int max(int ​​a, int b);
 *
 */

/*
 * *********************************************************************
 * for external linkage inline coding (not garantee for this foo inlined, this function will be the same foreach translation unit)
 * *********************************************************************
 */

#ifndef H_INLINE /* this define mus be used in .h file*/
#   if __GNUC__ && !__GNUC_STDC_INLINE__
#       define H_INLINE extern INLINE
#   else
#       define H_INLINE INLINE
#   endif /* __GNUC__ && !__GNUC_STDC_INLINE__ */
#endif /* H_INLINE */

#ifndef C_INLINE /* this define mus be used in .c file*/
#   if __GNUC__ && !__GNUC_STDC_INLINE__
#       define C_INLINE
#   else
#       define C_INLINE extern
#   endif /* __GNUC__ && !__GNUC_STDC_INLINE__ */
#endif /* C_INLINE */

/*
 * external ordinary inline linkage use==>
 * .h header file must be :
 *
 * H_INLINE int max(int ​​a, int b)
 * {
 *    return a > b ? а : б;
 * }
 *
 * .c source file must be :
 *
 * C_INLINE int max(int ​​a, int b);
 *
 */

/*
 * *********************************************************************************
 * static INLINE`s (this function may be different`s foreach translation unit)
 * *********************************************************************************
 */

// macro for internal (.c) or external (.h) static inlining
#ifndef STATIC_INLINE
#   define STATIC_INLINE static INLINE
#endif /* STATIC_INLINE */


// macro for internal (.c) or external (.h) static forceinlining
#ifndef STATIC_FORCEINLINE
#   define STATIC_FORCEINLINE static forceinline
#endif /* STATIC_FORCEINLINE */

#endif /* __INLINE_H__ */
