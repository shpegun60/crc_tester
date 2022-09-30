#ifndef __INLINE_H__
#define __INLINE_H__ 1
/*
 * This library implemented for support external inline linkage for different compilers:
 * - C89                    (GNU, ISO)
 * - C99                    (GNU, ISO)
 * - younger С - compilers  (GNU, ISO)
 * - all С++ compilers
 */


/* If using non-GNU C, then ignore __attribute__ */
#ifndef __GNUC__
# define __attribute__(x) /* NOTHING */
#endif /* __GNUC__ */

/*
 * *********************************************************************
 * simple INLINE`s
 * *********************************************************************
 */

#ifndef INLINE
#define INLINE inline
#endif /* INLINE */


#ifndef STATIC_INLINE
#define STATIC_INLINE static INLINE
#endif /* STATIC_INLINE */


/*
 * ******************************************************
 * forceinline for garantee inlining coding
 * ******************************************************
 */
#ifdef _MSC_VER
    #define forceinline __forceinline
#elif defined(__GNUC__)
    #define forceinline INLINE __attribute__((__always_inline__))
#elif defined(__CLANG__)
    #if __has_attribute(__always_inline__)
        #define forceinline INLINE __attribute__((__always_inline__))
    #else
        #define forceinline INLINE
    #endif
#else
    #define forceinline INLINE
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
        # define H_INLINE extern INLINE
    # else
        # define H_INLINE INLINE
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

#endif /* __INLINE_H__ */
