// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// TODO: reference additional headers your program requires here

enum ERROR_RETURN
{
	RETURN_NOERROR = 0,
	RETURN_NOINPUTFILE,
	RETURN_TOOMANYINPUTFILES,
	RETURN_FILENOTFOUND,
	RETURN_STREAMNOTFOUND,
	RETURN_STREAMISH265
};



#if defined(_DEBUG) || defined(_RELEASE)
#define DASSERT(s) System::Diagnostics::Debug::Assert(!!(s))
inline void DASSERT_EQUAL(System::Object^ a, System::Object^ b)
{
	if (!a->Equals(b))
	{
		System::Diagnostics::Debug::Assert(false, a + L" != " + b);
	}
}
inline void DTRACE(System::Object^ o) { System::Diagnostics::Debug::Print(o ? o->ToString() : L"<NULL>"); }
#define DTRACE2(s) DTRACE(__FUNCTION__ + L": " + (s))
#define DTRACE_NUMBER(start) do { static int num = start; DTRACE(num.ToString()); ++num; } while(0)
#define DTRACE_NUMBERTEXT(start,text) do { static int num = start; DTRACE(num.ToString() + ":" + text); ++num; } while(0)
inline System::String^ onlyLastFunc(System::String^ s)
{
	return s->Substring(s->LastIndexOf(L"::") + 2);
}
#define DTRACE_WITH_FUNCLINE(s) DTRACE( (s) + L" (" + onlyLastFunc(__FUNCTION__) + L":" + __LINE__ + L":" + System::Environment::TickCount + L")")
#define DVERIFY DASSERT
#define DVERIFY_IS(a,b)  DASSERT( (a)==(b) )
#define DVERIFY_NOT(a,b) DASSERT( (a)!=(b) )
#define DVERIFY_GL(a) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=0 ) { DASSERT(gle || 0); } } while(0)
#define DVERIFY_GL_NUM(a,n) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=n ) { DASSERT( gle || 0); } } while(0)
#define DVERIFY_GL_NUM2(a,n1, n2) do { DASSERT( (a) ); DWORD gle = GetLastError(); if ( gle!=n1 && gle!= n2) { DTRACE(gle.ToString()); DASSERT( gle || 0); } } while(0)
#define DASSERT_IS_CLASS(instance, clazz) DASSERT( ((instance)->GetType())==clazz::typeid ) 
#define DASSERT_IS_CLASS_OR_NULL(instance, clazz) DASSERT( instance==nullptr || ((instance)->GetType())==clazz::typeid ) 
inline bool isDerived(System::Object^ o, System::Type^ t)
{
	if (!o || !t)
		return false;

	System::Type^ ot = o->GetType();
	do
	{
		if (ot == t)
			return true;

		ot = ot->BaseType;
	} while (ot != System::Object::typeid);

	return false;
}
#define DASSERT_IS_DERIVED(instance, clazz) DASSERT(isDerived(instance, clazz::typeid))
#else
#define DASSERT(s) ((void)0)
#define DASSERT_EQUAL(a,b) ((void)0)
#define DTRACE(s) ((void)0)
#define DTRACE2(s) ((void)0)
#define DTRACE_NUMBER(start) ((void)0)
#define DTRACE_NUMBERTEXT(start,text) ((void)0)
#define DTRACE_WITH_FUNCLINE(s) ((void)0)
#define DVERIFY(a) (a)
#define DVERIFY_IS(a,b)  (a)
#define DVERIFY_NOT(a,b) (a)
#define DVERIFY_GL(a) (a)
#define DVERIFY_GL_NUM(a,n) (a)
#define DVERIFY_GL_NUM2(a,n1, n2) (a)
#define DASSERT_IS_CLASS(instance, clazz) ((void)0)
#define DASSERT_IS_CLASS_OR_NULL(instance, clazz) ((void)0)
#define DASSERT_IS_DERIVED(instance, clazz) ((void)0)
#endif



#define I18N(s) s