#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>

#include <fcntl.h>
#include <io.h>


#include "lua.hpp"
#include "lauxlib.h"
#include "lualib.h"

lua_State *L;
typedef void* HGLOBAL;
FILE* ConsoleWindow;
char* pluginDirectory;


int main(int argc, char* argv[]) {
    printf( "%s\n" , argv[0] );
    return 0;
}

void CallLuaThread();

//hにはdllまでのLogFilePathが入っている。
//lenはアドレスの長さ。\0の分は入っていない。
extern "C" __declspec(dllexport) bool __cdecl load(HGLOBAL h, long len){

    AllocConsole();
    //標準出力(stdout)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stdout);
    //標準エラー出力(stderr)を新しいコンソールに向ける
    freopen_s(&ConsoleWindow, "CONOUT$", "w+", stderr);
    //文字コードをutf-8に変更。
    system( "chcp 65001 > /nul" );

    ////追加する文字列とnull文字分確保する。
    char loadLuaName[] = "func.lua";
    char loadLuaPath[ len + 1 + strlen( loadLuaName )];
    memset( loadLuaPath , '\0' , ( len + 1 + strlen( loadLuaName ) ) );
    memcpy( loadLuaPath , (char*)h , (int)len  );
    //PATHの結合
    strcat( loadLuaPath , loadLuaName );

    pluginDirectory = (char*)malloc(sizeof(char) * (len + 1 ));
    memset( pluginDirectory , '\0' , ( len + 1 ));
    memcpy( pluginDirectory , (char*)h , (int)len  );


    L = luaL_newstate();
    luaL_openlibs(L);
    if ( luaL_dofile(L, loadLuaPath ) == true ) {
        printf( "load Lua: ERROR" );
    }


    GlobalFree( h );
    return true;
}

extern "C" __declspec(dllexport) bool __cdecl unload(void){
    free( pluginDirectory );
    FreeConsole();
    lua_close( L );
    return true;
}


//h 引数にリクエストのテキスト
//これは自前で開放する。
//*len リクエストの文字列。
//文字列を引き出すのに使用する。
//返り値のアドレスの長さを格納して返す。
extern "C" __declspec(dllexport) HGLOBAL __cdecl request(HGLOBAL h, long *len){
    char req[*len];
    memcpy( req , (char*)h , *len );
    GlobalFree( h );

    char    StatusSep[]               = "\r\n";
    char*   rem_text            = strstr( req , StatusSep );
    //nullポインタの時の対策.
    //アドレス値を引き算して必要なバイト数を割り出す。
    int     t                   = &rem_text[0] - &req[0];

    //初期化しないとゴミが入る。
    char    line_text[t+1]      = "";
    memcpy( &line_text , &req , t );

    //char line_text[] = "NOTIFY SHIORI/3.0";
    //スペースと/で区切る。
    char first_line_sep[] = " /";
    char *StatusLine = strtok( line_text, first_line_sep );
    int Count = strlen(StatusLine) ;
    char STATUS[Count + 1] = "";
    memcpy( STATUS , StatusLine , Count );
    //NOTIFY or GET
    //printf( "status :%s\n" , STATUS );

    StatusLine = strtok( NULL, first_line_sep );
    Count = strlen(StatusLine) ;
    char TYPE[Count + 1] = "";
    memcpy( TYPE , StatusLine , Count );
    //SHIORI or PLUGIN
    //printf( "TYPE :%s\n" , TYPE );


    StatusLine = strtok( NULL, first_line_sep );
    Count = strlen(StatusLine) ;
    char VERSION[Count + 1] = "";
    memcpy( VERSION , StatusLine , Count );
    //3.0 or 2.0
    //printf( "Version :%s\n" , VERSION );


    //NOTIFYリクエストでは何も返さないので。
    int _CheckStatus = strcmp( STATUS , "NOTIFY" );
    if ( _CheckStatus == 0 ) { 

        char res_buf[] = "SHIORI/3.0 204 No Content\r\n";
        //NULL文字を考慮しない文字数を返す。
        *len = strlen(res_buf);
        //VOID型で返す。内容は文字列(char)
        HGLOBAL ret = GlobalAlloc(GPTR, (SIZE_T)(*len));
        memcpy(ret, (void*)(res_buf), *len);
        return ret;

    }

    char *ID;
    int exist_ID      = 0;

    char *Reference4;
    int  Reference4_len;


    char sep[] = "\n\r";
    char _sep[] = ": ";
    int _sep_len = strlen(_sep);
    char *tp;
    tp = strtok( rem_text, sep );
    while( tp != NULL ){
        //printf( "%s\n" , tp );
        //左辺と右辺
        //leftは指定したアドレスの引き算で引っ張れる。
        char *sep_point = strstr( tp , _sep );

        //分割可能ならば。
        if( sep_point != NULL ){
            //文字数のコピー
            int LeftCount = sep_point - tp;
            int RightCount = strlen(sep_point) - _sep_len;
            //null文字を含まない。
            //printf( "L文字数%d\n" , LeftCount );
            //printf( "R文字数%d\n" , RightCount );

            char Key[LeftCount + 1] = "";
            memcpy( Key , tp , LeftCount );
            //printf( "KEY:%s\n" , Key );

            //ID要素があれば、回収しておく。
            if( strcmp( Key  , "ID" ) == 0 ){
                exist_ID = 1;
                //メモリを確保
                ID =  (char*)malloc(sizeof(char) * (RightCount + 1 ));
                //null文字で埋めておく。
                memset( ID , '\0' , ( RightCount + 1 ));
                memcpy( ID , &sep_point[_sep_len] , RightCount );
            }

            //何を喋ったかだけ回収できれば良い。
            if( strcmp( Key  , "Reference4" ) == 0 ){
                Reference4_len = sizeof(char) * (RightCount + 1 );
                Reference4 =  (char*)malloc( Reference4_len );
                //null文字で埋めておく。
                memset( Reference4 , '\0' , ( RightCount + 1 ));
                memcpy( Reference4 , &sep_point[_sep_len] , RightCount );
            }
        }
        tp = strtok( NULL , sep );
    }


    if ( exist_ID == 1 ) {
        if ( strcmp( ID , "OnOtherGhostTalk" ) == 0 ) {
            //printf( "ID  :%s\n" , ID);
            //printf( "Reference4  :%s\n" , Reference4);

            int pluginDirectory_len = sizeof(char) * (strlen(pluginDirectory) );
            //luaはpushstringの時点で内部コピーを作るので解放を気にする必要はない。
            lua_getglobal(L, "CheckSakuraScript"); 
            lua_pushlstring( L , pluginDirectory , pluginDirectory_len );
            lua_pushlstring( L , Reference4 , Reference4_len );
            std::thread th_a( CallLuaThread );
            //th_a.join(); 
            th_a.detach(); 
            //lua_call(L, 2, 0); 

            //定義されていないときにfreeすると駄目。
            free( Reference4 );
        }
        free( ID );
    }

    
    //pluginは2.0で返す。
    char res_buf[] = "PLUGIN/2.0 204 No Content";
    *len = strlen(res_buf);
    HGLOBAL ret = GlobalAlloc(GPTR, (SIZE_T)(*len));
    memcpy(ret, (void*)(res_buf), *len);
    return ret;
}


void CallLuaThread(){
    lua_call(L, 2, 0); 
}



