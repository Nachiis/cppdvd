#include "OriLsit.hpp"

// 中文比较函数
// Linux g++ locale 名称: "zh_CN.utf"
// VC2010 locale 名称：    "Chinese"或者"Chinese_china"
#ifdef _MSC_VER
static const char* ZH_CN_LOCALE_STRING = "Chinese_china";
#else
static const char* ZH_CN_LOCALE_STRING = "zh_CN.utf8";
#endif
static const locale zh_CN_locale = locale(ZH_CN_LOCALE_STRING);
static const collate<char>& zh_CN_collate = use_facet<collate<char> >(zh_CN_locale);

bool zh_CN_less_than(const string& s1, const string& s2) {
	const char* pb1 = s1.data();
	const char* pb2 = s2.data();
	return (zh_CN_collate.compare(pb1, pb1 + s1.size(), pb2, pb2 + s2.size()) < 0);
}