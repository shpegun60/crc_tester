#ifndef __INLINE_H__
#define __INLINE_H__
/*
 * This library implemented for support external inline linkage for different compilers:
 * - GNU C(gnu89)
 * - C99
 * - younger С - compilers
 * - all С++ compilers
 */

/*
 * ******************************************************
 * forceinline for garantee inlining coding
 * ******************************************************
 */
#ifdef _MSC_VER
    #define forceinline __forceinline
#elif defined(__GNUC__)
    #define forceinline inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
    #if __has_attribute(__always_inline__)
        #define forceinline inline __attribute__((__always_inline__))
    #else
        #define forceinline inline
    #endif
#else
    #define forceinline inline
#endif
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
 * for extern linkage inline coding (not garantee for this foo inlined)
 * *********************************************************************
 */

#ifndef H_INLINE /* this define mus be used in .h file*/
    # if __GNUC__ && !__GNUC_STDC_INLINE__
        # define H_INLINE extern inline
    # else
        # define H_INLINE inline
    # endif
#endif /* H_INLINE */

#ifndef C_INLINE /* this define mus be used in .c file*/
    # if __GNUC__ && !__GNUC_STDC_INLINE__
        # define C_INLINE
    # else
        # define C_INLINE extern
    # endif
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
 * *********************************************************************
 * OTHER simple INLINE`s
 * *********************************************************************
 */

#ifndef STATIC_INLINE
#define STATIC_INLINE static inline
#endif /* STATIC_INLINE */

#ifndef INLINE
#define INLINE inline
#endif /* INLINE */

#endif /* __INLINE_H__ */
