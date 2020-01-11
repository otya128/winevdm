/*
 * Copyright 1995 Martin von Loewis
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __WINE_OLEAUT32_OLE2DISP_H
#define __WINE_OLEAUT32_OLE2DISP_H

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "wtypes.h"
#include "wownt32.h"
#include "wine/windef16.h"
#include "wine/winbase16.h"
#include "../ole2/ifs.h"
#include "wine/debug.h"

typedef CHAR OLECHAR16;
typedef LPSTR LPOLESTR16;
typedef LPCSTR LPCOLESTR16;
typedef OLECHAR16 *BSTR16;
typedef BSTR16 *LPBSTR16;

SEGPTR WINAPI SysAllocString16(LPCOLESTR16);
SEGPTR WINAPI SysAllocStringLen16(const char*, int);
VOID   WINAPI SysFreeString16(SEGPTR);
INT16  WINAPI SysReAllocString16(SEGPTR*,LPCOLESTR16);
int    WINAPI SysReAllocStringLen16(SEGPTR*, const char*,  int);
int    WINAPI SysStringLen16(SEGPTR);

typedef struct tagVARIANT16 {
    union
    {
        struct __tagVARIANT16
        {
            VARTYPE vt;
            WORD wReserved1;
            WORD wReserved2;
            WORD wReserved3;
            union {
                BYTE bVal;
                SHORT iVal;
                LONG lVal;
                FLOAT fltVal;
                DOUBLE dblVal;
                VARIANT_BOOL boolVal;
                SCODE scode;
                DATE date;
                /* BSTR16 */        SEGPTR bstrVal;
                CY cyVal;
                /* IUnknown* */     SEGPTR punkVal;
                /* SEGPTR */    SEGPTR pdispVal;
                /* SAFEARRAY* */    SEGPTR parray;
                /* BYTE* */         SEGPTR pbVal;
                /* SHORT* */        SEGPTR piVal;
                /* LONG* */         SEGPTR plVal;
                /* FLOAT* */        SEGPTR pfltVal;
                /* DOUBLE* */       SEGPTR pdblVal;
                /* VARIANT_BOOL* */ SEGPTR pboolVal;
                /* SCODE* */        SEGPTR pscode;
                /* DATE* */         SEGPTR pdate;
                /* BSTR16* */       SEGPTR pbstrVal;
                /* VARIANT16* */    SEGPTR pvarVal;
                /* void* */         SEGPTR byref;
                /* CY* */           SEGPTR pcyVal;
                /* IUnknown** */    SEGPTR ppunkVal;
                /* IDispatch** */   SEGPTR ppdispVal;
                /* SAFEARRAY** */   SEGPTR pparray;

                CHAR cVal;
                LONGLONG llVal;
                USHORT uiVal;
                ULONGLONG ullVal;
                ULONG ulVal;
                UINT uintVal;
                INT intVal;
            } __VARIANT_NAME_3;
        } __VARIANT_NAME_2;
    } __VARIANT_NAME_1;
} VARIANT16;

#define V_VARIANTREF16(X)  ((VARIANT16*)MapSL(V_UNION(X, pvarVal)))
#define V_UNKNOWNREF16(X)  ((SEGPTR*)MapSL(V_UNION(X, ppunkVal)))
#define V_ARRAYREF16(X)    ((SEGPTR*)MapSL(V_UNION(X, pparray)))
#define V_BSTRREF16(X)     ((SEGPTR*)MapSL(V_UNION(X, pbstrVal)))
#define V_BSTR16(X)     ((BSTR16*)MapSL(V_UNION(X, bstrVal)))
typedef VARIANT16 VARIANTARG16;
typedef VARIANT16 *LPVARIANT16;
/* 16-bit SAFEARRAY implementation */


typedef struct tagSAFEARRAY16
{
    USHORT cDims;
    USHORT fFeatures;
    USHORT cbElements;
    USHORT cLocks;
    ULONG  handle;
    SEGPTR pvData;
    SAFEARRAYBOUND16 rgsabound[1];
} SAFEARRAY16;

USHORT WINAPI SafeArrayGetDim16(SAFEARRAY16 *sa);
USHORT WINAPI SafeArrayGetElemsize16(SAFEARRAY16 *sa);
HRESULT WINAPI SafeArrayGetUBound16(SAFEARRAY16 *sa, UINT16 dim, LONG *ubound);
HRESULT WINAPI SafeArrayGetLBound16(SAFEARRAY16 *sa, UINT16 dim, LONG *lbound);
HRESULT WINAPI SafeArrayLock16(SAFEARRAY16 *sa);
HRESULT WINAPI SafeArrayUnlock16(SAFEARRAY16 *sa);
HRESULT WINAPI SafeArrayAccessData16(SAFEARRAY16 *sa, SEGPTR *data);
HRESULT WINAPI SafeArrayUnaccessData16(SAFEARRAY16 *sa);
HRESULT WINAPI SafeArrayAllocDescriptor16(UINT16 dims, SEGPTR *ret);
HRESULT WINAPI SafeArrayAllocData16(SAFEARRAY16 *sa);
HRESULT WINAPI SafeArrayDestroyDescriptor16(SEGPTR s);
HRESULT WINAPI SafeArrayDestroyData16(SAFEARRAY16 *sa);
HRESULT WINAPI SafeArrayCopy16(SAFEARRAY16 *sa, SAFEARRAY16 **ppsaout);
SEGPTR WINAPI SysAllocString16(LPCOLESTR16 oleStr);
INT16 WINAPI SysReAllocString16(SEGPTR *pbstr, LPCOLESTR16 oleStr);
SEGPTR WINAPI SysAllocStringLen16(const char *oleStr, int len);
int WINAPI SysReAllocStringLen16(SEGPTR *old, const char *in, int len);
void WINAPI SysFreeString16(SEGPTR str);
int WINAPI SysStringLen16(SEGPTR str);
int WINAPI SysStringByteLen16(SEGPTR str);
SEGPTR WINAPI SysAllocStringByteLen16(const char *oleStr, int len);
HRESULT WINAPI SafeArrayDestroy16(SEGPTR psa);
HRESULT WINAPI BstrFromVector16(SAFEARRAY16 *psa, /*BSTR*/SEGPTR *pbstr);
typedef SEGPTR SEGBSTR16;
#define BSTR_PTR(x) ((OLECHAR16*)MapSL(x))
HRESULT WINAPI VectorFromBstr16(SEGBSTR16 bstr, /*SAFEARRAY16 * */SEGPTR *ppsa);

HRESULT WINAPI VarUdateFromDate16(DATE dateIn, ULONG dwFlags, UDATE *lpUdate);
HRESULT WINAPI VariantCopy16(VARIANTARG16* pvargDest, VARIANTARG16* pvargSrc);
HRESULT WINAPI VarDateFromUdate16(UDATE *pUdateIn, ULONG dwFlags, DATE *pDateOut);
HRESULT WINAPI VariantClear16(VARIANTARG16* pVarg);
HRESULT WINAPI VariantChangeTypeEx16(VARIANTARG16* pvargDest, VARIANTARG16* pvargSrc,
                                   LCID lcid, USHORT wFlags, VARTYPE vt);
HRESULT WINAPI VarCyNeg16(CY cyIn, CY* pCyOut);
HRESULT WINAPI VarCyInt16(CY cyIn, CY* pCyOut);

void WINAPI VariantInit16(VARIANTARG16* pVarg);
HRESULT WINAPI VariantClear16(VARIANTARG16* pVarg);
HRESULT WINAPI VariantCopy16(VARIANTARG16* pvargDest, VARIANTARG16* pvargSrc);
HRESULT WINAPI VariantCopyInd16(VARIANT16* pvargDest, VARIANTARG16* pvargSrc);
HRESULT WINAPI VariantChangeType16(VARIANTARG16* pvargDest, VARIANTARG16* pvargSrc, USHORT wFlags, VARTYPE vt);
HRESULT WINAPI VariantChangeTypeEx16(VARIANTARG16* pvargDest, VARIANTARG16* pvargSrc, LCID lcid, USHORT wFlags, VARTYPE vt);
INT WINAPI DosDateTimeToVariantTime16(USHORT wDosDate, USHORT wDosTime, double *pDateOut);
INT WINAPI VariantTimeToDosDateTime16(double dateIn, USHORT *pwDosDate, USHORT *pwDosTime);
INT WINAPI SystemTimeToVariantTime16(LPSYSTEMTIME lpSt, double *pDateOut);
INT WINAPI VariantTimeToSystemTime16(double dateIn, LPSYSTEMTIME lpSt);
HRESULT WINAPI VarDateFromUdateEx16(UDATE *pUdateIn, LCID lcid, ULONG dwFlags, DATE *pDateOut);
HRESULT WINAPI VarDateFromUdate16(UDATE *pUdateIn, ULONG dwFlags, DATE *pDateOut);
HRESULT WINAPI VarUdateFromDate16(DATE dateIn, ULONG dwFlags, UDATE *lpUdate);
HRESULT WINAPI VarParseNumFromStr16(OLECHAR16 *lpszStr, LCID lcid, ULONG dwFlags, NUMPARSE *pNumprs, BYTE *rgbDig);
HRESULT WINAPI VarNumFromParseNum16(NUMPARSE *pNumprs, BYTE *rgbDig, ULONG dwVtBits, VARIANT16 *pVarDst);
HRESULT WINAPI VarCat16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 out);
HRESULT WINAPI VarCmp16(LPVARIANT16 left, LPVARIANT16 right, LCID lcid, DWORD flags);
HRESULT WINAPI VarAnd16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarAdd16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarMul16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarDiv16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarSub16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarOr16(LPVARIANT16 pVarLeft, LPVARIANT16 pVarRight, LPVARIANT16 pVarOut);
HRESULT WINAPI VarAbs16(LPVARIANT16 pVarIn, LPVARIANT16 pVarOut);
HRESULT WINAPI VarFix16(LPVARIANT16 pVarIn, LPVARIANT16 pVarOut);
HRESULT WINAPI VarInt16(LPVARIANT16 pVarIn, LPVARIANT16 pVarOut);
HRESULT WINAPI VarXor16(LPVARIANT16 pVarLeft, LPVARIANT16 pVarRight, LPVARIANT16 pVarOut);
HRESULT WINAPI VarEqv16(LPVARIANT16 pVarLeft, LPVARIANT16 pVarRight, LPVARIANT16 pVarOut);
HRESULT WINAPI VarNeg16(LPVARIANT16 pVarIn, LPVARIANT16 pVarOut);
HRESULT WINAPI VarNot16(LPVARIANT16 pVarIn, LPVARIANT16 pVarOut);
HRESULT WINAPI VarRound16(LPVARIANT16 pVarIn, int deci, LPVARIANT16 pVarOut);
HRESULT WINAPI VarIdiv16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarMod16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarPow16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarImp16(LPVARIANT16 left, LPVARIANT16 right, LPVARIANT16 result);
HRESULT WINAPI VarI1FromUI116(BYTE bIn, signed char* pcOut);
HRESULT WINAPI VarI1FromI216(SHORT sIn, signed char* pcOut);
HRESULT WINAPI VarI1FromI416(LONG iIn, signed char* pcOut);
HRESULT WINAPI VarI1FromR416(FLOAT fltIn, signed char* pcOut);
HRESULT WINAPI VarI1FromR816(double dblIn, signed char* pcOut);
HRESULT WINAPI VarI1FromDate16(DATE dateIn, signed char* pcOut);
HRESULT WINAPI VarI1FromCy16(CY cyIn, signed char* pcOut);
HRESULT WINAPI VarI1FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, signed char* pcOut);
HRESULT WINAPI VarI1FromDisp16(SEGPTR pdispIn, LCID lcid, signed char* pcOut);
HRESULT WINAPI VarI1FromBool16(VARIANT_BOOL boolIn, signed char* pcOut);
HRESULT WINAPI VarI1FromUI216(USHORT usIn, signed char* pcOut);
HRESULT WINAPI VarI1FromUI416(ULONG ulIn, signed char* pcOut);
HRESULT WINAPI VarI1FromDec16(DECIMAL *pdecIn, signed char* pcOut);
HRESULT WINAPI VarI1FromI816(LONG64 llIn, signed char* pcOut);
HRESULT WINAPI VarI1FromUI816(ULONG64 ullIn, signed char* pcOut);
HRESULT WINAPI VarUI1FromI216(SHORT sIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromI416(LONG iIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromR416(FLOAT fltIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromR816(double dblIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromCy16(CY cyIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromDate16(DATE dateIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, BYTE* pbOut);
HRESULT WINAPI VarUI1FromDisp16(SEGPTR pdispIn, LCID lcid, BYTE* pbOut);
HRESULT WINAPI VarUI1FromBool16(VARIANT_BOOL boolIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromI116(signed char cIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromUI216(USHORT usIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromUI416(ULONG ulIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromDec16(DECIMAL *pdecIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromI816(LONG64 llIn, BYTE* pbOut);
HRESULT WINAPI VarUI1FromUI816(ULONG64 ullIn, BYTE* pbOut);
HRESULT WINAPI VarI2FromUI116(BYTE bIn, SHORT* psOut);
HRESULT WINAPI VarI2FromI416(LONG iIn, SHORT* psOut);
HRESULT WINAPI VarI2FromR416(FLOAT fltIn, SHORT* psOut);
HRESULT WINAPI VarI2FromR816(double dblIn, SHORT* psOut);
HRESULT WINAPI VarI2FromCy16(CY cyIn, SHORT* psOut);
HRESULT WINAPI VarI2FromDate16(DATE dateIn, SHORT* psOut);
HRESULT WINAPI VarI2FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, SHORT* psOut);
HRESULT WINAPI VarI2FromDisp16(SEGPTR pdispIn, LCID lcid, SHORT* psOut);
HRESULT WINAPI VarI2FromBool16(VARIANT_BOOL boolIn, SHORT* psOut);
HRESULT WINAPI VarI2FromI116(signed char cIn, SHORT* psOut);
HRESULT WINAPI VarI2FromUI216(USHORT usIn, SHORT* psOut);
HRESULT WINAPI VarI2FromUI416(ULONG ulIn, SHORT* psOut);
HRESULT WINAPI VarI2FromDec16(DECIMAL *pdecIn, SHORT* psOut);
HRESULT WINAPI VarI2FromI816(LONG64 llIn, SHORT* psOut);
HRESULT WINAPI VarI2FromUI816(ULONG64 ullIn, SHORT* psOut);
HRESULT WINAPI VarUI2FromUI116(BYTE bIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromI216(SHORT sIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromI416(LONG iIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromR416(FLOAT fltIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromR816(double dblIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromDate16(DATE dateIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromCy16(CY cyIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, USHORT* pusOut);
HRESULT WINAPI VarUI2FromDisp16(SEGPTR pdispIn, LCID lcid, USHORT* pusOut);
HRESULT WINAPI VarUI2FromBool16(VARIANT_BOOL boolIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromI116(signed char cIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromUI416(ULONG ulIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromDec16(DECIMAL *pdecIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromI816(LONG64 llIn, USHORT* pusOut);
HRESULT WINAPI VarUI2FromUI816(ULONG64 ullIn, USHORT* pusOut);
HRESULT WINAPI VarI4FromUI116(BYTE bIn, LONG *piOut);
HRESULT WINAPI VarI4FromI216(SHORT sIn, LONG *piOut);
HRESULT WINAPI VarI4FromR416(FLOAT fltIn, LONG *piOut);
HRESULT WINAPI VarI4FromR816(double dblIn, LONG *piOut);
HRESULT WINAPI VarI4FromCy16(CY cyIn, LONG *piOut);
HRESULT WINAPI VarI4FromDate16(DATE dateIn, LONG *piOut);
HRESULT WINAPI VarI4FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, LONG *piOut);
HRESULT WINAPI VarI4FromDisp16(SEGPTR pdispIn, LCID lcid, LONG *piOut);
HRESULT WINAPI VarI4FromBool16(VARIANT_BOOL boolIn, LONG *piOut);
HRESULT WINAPI VarI4FromI116(signed char cIn, LONG *piOut);
HRESULT WINAPI VarI4FromUI216(USHORT usIn, LONG *piOut);
HRESULT WINAPI VarI4FromUI416(ULONG ulIn, LONG *piOut);
HRESULT WINAPI VarI4FromDec16(DECIMAL *pdecIn, LONG *piOut);
HRESULT WINAPI VarI4FromI816(LONG64 llIn, LONG *piOut);
HRESULT WINAPI VarI4FromUI816(ULONG64 ullIn, LONG *piOut);
HRESULT WINAPI VarUI4FromUI116(BYTE bIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromI216(SHORT sIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromI416(LONG iIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromR416(FLOAT fltIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromR816(double dblIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromDate16(DATE dateIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromCy16(CY cyIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, ULONG *pulOut);
HRESULT WINAPI VarUI4FromDisp16(SEGPTR pdispIn, LCID lcid, ULONG *pulOut);
HRESULT WINAPI VarUI4FromBool16(VARIANT_BOOL boolIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromI116(signed char cIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromUI216(USHORT usIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromDec16(DECIMAL *pdecIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromI816(LONG64 llIn, ULONG *pulOut);
HRESULT WINAPI VarUI4FromUI816(ULONG64 ullIn, ULONG *pulOut);
HRESULT WINAPI VarI8FromUI116(BYTE bIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromI216(SHORT sIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromR416(FLOAT fltIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromR816(double dblIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromCy16(CY cyIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromDate16(DATE dateIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, LONG64* pi64Out);
HRESULT WINAPI VarI8FromDisp16(SEGPTR pdispIn, LCID lcid, LONG64* pi64Out);
HRESULT WINAPI VarI8FromBool16(VARIANT_BOOL boolIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromI116(signed char cIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromUI216(USHORT usIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromUI416(ULONG ulIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromDec16(DECIMAL *pdecIn, LONG64* pi64Out);
HRESULT WINAPI VarI8FromUI816(ULONG64 ullIn, LONG64* pi64Out);
HRESULT WINAPI VarUI8FromI816(LONG64 llIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromUI116(BYTE bIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromI216(SHORT sIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromR416(FLOAT fltIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromR816(double dblIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromCy16(CY cyIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromDate16(DATE dateIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromDisp16(SEGPTR pdispIn, LCID lcid, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromBool16(VARIANT_BOOL boolIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromI116(signed char cIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromUI216(USHORT usIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromUI416(ULONG ulIn, ULONG64* pui64Out);
HRESULT WINAPI VarUI8FromDec16(DECIMAL *pdecIn, ULONG64* pui64Out);
HRESULT WINAPI VarR4FromUI116(BYTE bIn, float *pFltOut);
HRESULT WINAPI VarR4FromI216(SHORT sIn, float *pFltOut);
HRESULT WINAPI VarR4FromI416(LONG lIn, float *pFltOut);
HRESULT WINAPI VarR4FromR816(double dblIn, float *pFltOut);
HRESULT WINAPI VarR4FromCy16(CY cyIn, float *pFltOut);
HRESULT WINAPI VarR4FromDate16(DATE dateIn, float *pFltOut);
HRESULT WINAPI VarR4FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, float *pFltOut);
HRESULT WINAPI VarR4FromDisp16(SEGPTR pdispIn, LCID lcid, float *pFltOut);
HRESULT WINAPI VarR4FromBool16(VARIANT_BOOL boolIn, float *pFltOut);
HRESULT WINAPI VarR4FromI116(signed char cIn, float *pFltOut);
HRESULT WINAPI VarR4FromUI216(USHORT usIn, float *pFltOut);
HRESULT WINAPI VarR4FromUI416(ULONG ulIn, float *pFltOut);
HRESULT WINAPI VarR4FromDec16(DECIMAL* pDecIn, float *pFltOut);
HRESULT WINAPI VarR4FromI816(LONG64 llIn, float *pFltOut);
HRESULT WINAPI VarR4FromUI816(ULONG64 ullIn, float *pFltOut);
HRESULT WINAPI VarR4CmpR816(float fltLeft, double dblRight);
HRESULT WINAPI VarR8FromUI116(BYTE bIn, double *pDblOut);
HRESULT WINAPI VarR8FromI216(SHORT sIn, double *pDblOut);
HRESULT WINAPI VarR8FromI416(LONG lIn, double *pDblOut);
HRESULT WINAPI VarR8FromR416(FLOAT fltIn, double *pDblOut);
HRESULT WINAPI VarR8FromCy16(CY cyIn, double *pDblOut);
HRESULT WINAPI VarR8FromDate16(DATE dateIn, double *pDblOut);
HRESULT WINAPI VarR8FromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, double *pDblOut);
HRESULT WINAPI VarR8FromDisp16(SEGPTR pdispIn, LCID lcid, double *pDblOut);
HRESULT WINAPI VarR8FromBool16(VARIANT_BOOL boolIn, double *pDblOut);
HRESULT WINAPI VarR8FromI116(signed char cIn, double *pDblOut);
HRESULT WINAPI VarR8FromUI216(USHORT usIn, double *pDblOut);
HRESULT WINAPI VarR8FromUI416(ULONG ulIn, double *pDblOut);
HRESULT WINAPI VarR8FromDec16(const DECIMAL* pDecIn, double *pDblOut);
HRESULT WINAPI VarR8FromI816(LONG64 llIn, double *pDblOut);
HRESULT WINAPI VarR8FromUI816(ULONG64 ullIn, double *pDblOut);
HRESULT WINAPI VarR8Pow16(double dblLeft, double dblPow, double *pDblOut);
HRESULT WINAPI VarR8Round16(double dblIn, int nDig, double *pDblOut);
HRESULT WINAPI VarCyFromUI116(BYTE bIn, CY* pCyOut);
HRESULT WINAPI VarCyFromI216(SHORT sIn, CY* pCyOut);
HRESULT WINAPI VarCyFromI416(LONG lIn, CY* pCyOut);
HRESULT WINAPI VarCyFromR416(FLOAT fltIn, CY* pCyOut);
HRESULT WINAPI VarCyFromR816(double dblIn, CY* pCyOut);
HRESULT WINAPI VarCyFromDate16(DATE dateIn, CY* pCyOut);
HRESULT WINAPI VarCyFromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, CY* pCyOut);
HRESULT WINAPI VarCyFromDisp16(SEGPTR pdispIn, LCID lcid, CY* pCyOut);
HRESULT WINAPI VarCyFromBool16(VARIANT_BOOL boolIn, CY* pCyOut);
HRESULT WINAPI VarCyFromI116(signed char cIn, CY* pCyOut);
HRESULT WINAPI VarCyFromUI216(USHORT usIn, CY* pCyOut);
HRESULT WINAPI VarCyFromUI416(ULONG ulIn, CY* pCyOut);
HRESULT WINAPI VarCyFromDec16(DECIMAL* pdecIn, CY* pCyOut);
HRESULT WINAPI VarCyFromI816(LONG64 llIn, CY* pCyOut);
HRESULT WINAPI VarCyFromUI816(ULONG64 ullIn, CY* pCyOut);
HRESULT WINAPI VarCyAdd16(CY cyLeft, CY cyRight, CY* pCyOut);
HRESULT WINAPI VarCyMul16(CY cyLeft, CY cyRight, CY* pCyOut);
HRESULT WINAPI VarCyMulI416(CY cyLeft, LONG lRight, CY* pCyOut);
HRESULT WINAPI VarCySub16(CY cyLeft, CY cyRight, CY* pCyOut);
HRESULT WINAPI VarCyAbs16(CY cyIn, CY* pCyOut);
HRESULT WINAPI VarCyFix16(CY cyIn, CY* pCyOut);
HRESULT WINAPI VarCyInt16(CY cyIn, CY* pCyOut);
HRESULT WINAPI VarCyNeg16(CY cyIn, CY* pCyOut);
HRESULT WINAPI VarCyRound16(CY cyIn, int cDecimals, CY* pCyOut);
HRESULT WINAPI VarCyCmp16(CY cyLeft, CY cyRight);
HRESULT WINAPI VarCyCmpR816(CY cyLeft, double dblRight);
HRESULT WINAPI VarCyMulI816(CY cyLeft, LONG64 llRight, CY* pCyOut);
HRESULT WINAPI VarDecFromUI116(BYTE bIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromI216(SHORT sIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromI416(LONG lIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromR416(FLOAT fltIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromR816(double dblIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromDate16(DATE dateIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromCy16(CY cyIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromDisp16(SEGPTR pdispIn, LCID lcid, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromBool16(VARIANT_BOOL bIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromI116(signed char cIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromUI216(USHORT usIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromUI416(ULONG ulIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromI816(LONG64 llIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFromUI816(ULONG64 ullIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecAdd16(const DECIMAL* pDecLeft, const DECIMAL* pDecRight, DECIMAL* pDecOut);
HRESULT WINAPI VarDecDiv16(const DECIMAL* pDecLeft, const DECIMAL* pDecRight, DECIMAL* pDecOut);
HRESULT WINAPI VarDecMul16(const DECIMAL* pDecLeft, const DECIMAL* pDecRight, DECIMAL* pDecOut);
HRESULT WINAPI VarDecSub16(const DECIMAL* pDecLeft, const DECIMAL* pDecRight, DECIMAL* pDecOut);
HRESULT WINAPI VarDecAbs16(const DECIMAL* pDecIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecFix16(const DECIMAL* pDecIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecInt16(const DECIMAL* pDecIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecNeg16(const DECIMAL* pDecIn, DECIMAL* pDecOut);
HRESULT WINAPI VarDecRound16(const DECIMAL* pDecIn, int cDecimals, DECIMAL* pDecOut);
HRESULT WINAPI VarDecCmp16(const DECIMAL* pDecLeft, const DECIMAL* pDecRight);
HRESULT WINAPI VarDecCmpR816(const DECIMAL* pDecLeft, double dblRight);
HRESULT WINAPI VarBoolFromUI116(BYTE bIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromI216(SHORT sIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromI416(LONG lIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromR416(FLOAT fltIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromR816(double dblIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromDate16(DATE dateIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromCy16(CY cyIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromDisp16(SEGPTR pdispIn, LCID lcid, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromI116(signed char cIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromUI216(USHORT usIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromUI416(ULONG ulIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromDec16(DECIMAL* pDecIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromI816(LONG64 llIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBoolFromUI816(ULONG64 ullIn, VARIANT_BOOL *pBoolOut);
HRESULT WINAPI VarBstrFromUI116(BYTE bIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromI216(short sIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromI416(LONG lIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromR416(FLOAT fltIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromR816(double dblIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromCy16(CY cyIn, LCID lcid, ULONG dwFlags, SEGBSTR16 *pbstrOut);
HRESULT WINAPI VarBstrFromDate16(DATE dateIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromBool16(VARIANT_BOOL boolIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromI116(signed char cIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromUI216(USHORT usIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromUI416(ULONG ulIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromDec16(DECIMAL* pDecIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromI816(LONG64 llIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromUI816(ULONG64 ullIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrFromDisp16(SEGPTR pdispIn, LCID lcid, ULONG dwFlags, SEGBSTR16* pbstrOut);
HRESULT WINAPI VarBstrCat16(SEGBSTR16 pbstrLeft, SEGBSTR16 pbstrRight, SEGBSTR16 *pbstrOut);
HRESULT WINAPI VarBstrCmp16(SEGBSTR16 pbstrLeft, SEGBSTR16 pbstrRight, LCID lcid, DWORD dwFlags);
HRESULT WINAPI VarDateFromUI116(BYTE bIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromI216(short sIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromI416(LONG lIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromR416(FLOAT fltIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromR816(double dblIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromDisp16(SEGPTR pdispIn, LCID lcid, DATE* pdateOut);
HRESULT WINAPI VarDateFromBool16(VARIANT_BOOL boolIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromCy16(CY cyIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromStr16(OLECHAR16* strIn, LCID lcid, ULONG dwFlags, DATE* pdateOut);
HRESULT WINAPI VarDateFromI116(signed char cIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromUI216(USHORT uiIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromUI416(ULONG ulIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromDec16(DECIMAL *pdecIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromI816(LONG64 llIn, DATE* pdateOut);
HRESULT WINAPI VarDateFromUI816(ULONG64 ullIn, DATE* pdateOut);


static inline const char *wine_dbgstr_variant16( const VARIANT16 *v )
{
    if (!v)
        return "(null)";

    if (V_VT(v) & VT_BYREF) {
        if (V_VT(v) == (VT_VARIANT|VT_BYREF))
            return wine_dbg_sprintf( "%p {VT_VARIANT|VT_BYREF: %s}", v, wine_dbgstr_variant16(V_VARIANTREF16(v)) );
        if (V_VT(v) == (VT_BSTR|VT_BYREF))
            return wine_dbg_sprintf( "%p {VT_BSTR|VT_BYREF: %s}", v, V_BSTRREF(v) ? wine_dbgstr_a(MapSL(*V_BSTRREF16(v))) : "(none)" );
        return wine_dbg_sprintf( "%p {%s %p}", v, wine_dbgstr_vt(V_VT(v)), V_BYREF(v) );
    }

    if (V_ISARRAY(v) || V_ISVECTOR(v))
        return wine_dbg_sprintf( "%p {%s %p}", v, wine_dbgstr_vt(V_VT(v)), V_ARRAY(v) );

    switch(V_VT(v)) {
    case VT_EMPTY:
        return wine_dbg_sprintf( "%p {VT_EMPTY}", v );
    case VT_NULL:
        return wine_dbg_sprintf( "%p {VT_NULL}", v );
    case VT_I2:
        return wine_dbg_sprintf( "%p {VT_I2: %d}", v, V_I2(v) );
    case VT_I4:
        return wine_dbg_sprintf( "%p {VT_I4: %d}", v, V_I4(v) );
    case VT_R4:
        return wine_dbg_sprintf( "%p {VT_R4: %f}", v, V_R4(v) );
    case VT_R8:
        return wine_dbg_sprintf( "%p {VT_R8: %lf}", v, V_R8(v) );
    case VT_CY:
        return wine_dbg_sprintf( "%p {VT_CY: %s}", v, wine_dbgstr_longlong(V_CY(v).int64) );
    case VT_DATE:
        return wine_dbg_sprintf( "%p {VT_DATE: %lf}", v, V_DATE(v) );
    case VT_LPSTR:
        return wine_dbg_sprintf( "%p {VT_LPSTR: %s}", v, wine_dbgstr_a((const char *)BSTR_PTR(V_BSTR(v))) );
    case VT_LPWSTR:
        return wine_dbg_sprintf( "%p {VT_LPWSTR: %s}", v, wine_dbgstr_w((const char *)BSTR_PTR(V_BSTR(v))) );
    case VT_BSTR:
        return wine_dbg_sprintf( "%p {VT_BSTR: %s}", v, wine_dbgstr_a((const char *)BSTR_PTR(V_BSTR(v))) );
    case VT_DISPATCH:
        return wine_dbg_sprintf( "%p {VT_DISPATCH: %p}", v, V_DISPATCH(v) );
    case VT_ERROR:
        return wine_dbg_sprintf( "%p {VT_ERROR: %08x}", v, V_ERROR(v) );
    case VT_BOOL:
        return wine_dbg_sprintf( "%p {VT_BOOL: %x}", v, V_BOOL(v) );
    case VT_UNKNOWN:
        return wine_dbg_sprintf( "%p {VT_UNKNOWN: %p}", v, V_UNKNOWN(v) );
    case VT_I1:
        return wine_dbg_sprintf( "%p {VT_I1: %d}", v, V_I1(v) );
    case VT_UI1:
        return wine_dbg_sprintf( "%p {VT_UI1: %u}", v, V_UI1(v) );
    case VT_UI2:
        return wine_dbg_sprintf( "%p {VT_UI2: %d}", v, V_UI2(v) );
    case VT_UI4:
        return wine_dbg_sprintf( "%p {VT_UI4: %d}", v, V_UI4(v) );
    case VT_I8:
        return wine_dbg_sprintf( "%p {VT_I8: %s}", v, wine_dbgstr_longlong(V_I8(v)) );
    case VT_UI8:
        return wine_dbg_sprintf( "%p {VT_UI8: %s}", v, wine_dbgstr_longlong(V_UI8(v)) );
    case VT_INT:
        return wine_dbg_sprintf( "%p {VT_INT: %d}", v, V_INT(v) );
    case VT_UINT:
        return wine_dbg_sprintf( "%p {VT_UINT: %u}", v, V_UINT(v) );
    case VT_VOID:
        return wine_dbg_sprintf( "%p {VT_VOID}", v );
#ifdef AVAIL_32BIT_VAR
    case VT_RECORD:
        return wine_dbg_sprintf( "%p {VT_RECORD: %p %p}", v, V_RECORD(v), V_RECORDINFO(v) );
#endif
    default:
        return wine_dbg_sprintf( "%p {vt %s}", v, wine_dbgstr_vt(V_VT(v)) );
    }
}
static inline const char *debugstr_variant16(const VARIANT16 *v) { return wine_dbgstr_variant16(v); }

typedef struct tagDISPPARAMS16
{
    /* VARIANTARG* */SEGPTR rgvarg;
    /* DISPID* */SEGPTR rgdispidNamedArgs;
    UINT16 cArgs;
    UINT16 cNamedArgs;
} 	DISPPARAMS16;
SEGPTR WINAPI SafeArrayCreate16(VARTYPE vt, UINT16 cDims, SAFEARRAYBOUND16 *rgsabound);

#ifdef AVAIL_32BIT_VAR
#endif
#endif /* !defined(__WINE_OLEAUT32_OLE2DISP_H) */
