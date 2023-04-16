#ifndef KCONTEXT_H_
#define KCONTEXT_H_

#include "SCREEN_DIM.h"

#define ADD_STR_LOG(key, obj) cout << obj << endl;
#define ADD_INT_LOG(key, obj)
#define LOG_INFO_CTX(ctx)
#define LOG_WARN_CTX(ctx)
#define LOG_ERROR_CTX(ctx)

struct KCONTEXT{
	SCREEN_DIM screen_dim;
};

SCREEN_DIM Get_Screen_Dim_KCONTEXT(KCONTEXT*);
void Set_Screen_Dim_KCONTEXT(KCONTEXT*, SCREEN_DIM);

#endif