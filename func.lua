-- -- サクラスクリプトはUTF-8で流れてくる。
-- しかし、通知に使用するバッチファイルでの処理がshift-jisになってしまう問題を解決できていない。

-- -- 起動時のあいさつ
-- print("   ／￣￣＼                                         ")
-- print("  /   _ノ  ＼                                       ")
-- print("  |  （ ―）（―）             ,  ､                   ")
-- print("  |    （__人__） ふぅーッ´ ( ⌒ヽ ）                ")
-- print("  |     `-=━・   --==≡三三 ( （  ) ） ）            ")
-- print("  |        ﾉ             ヽ ｀ｰ ノノ                ")
-- print("  ヽ    .,ﾉ )／´二⊃        ヽ_- ノ                  ")
-- print("   >  /'／  '‐､ﾆ⊃ ｷﾝｯ                               ")
-- print("./⌒ヽ  ｌ    ´ヽ〉〆ヽ                              ")
-- print("(   ヽ/    __人〉ヾ_ノ,ゞ                           ")
-- print(".＼  /   /   {  ／                                  ")

-- -- 起動時のあいさつここまで。

-- -- 設定スペース
-- 特定の条件を満たした場合のみnil以外を返す。
-- \はエスケープする必要があります。
function Filter_Sample( sakuraScript )
    local filter    = ""
    local title     = ""
    local msg       = ""
    local cmd       = ""
    local breakFlag = true
    if not ( filter == "" ) and not ( string.match( sakuraScript , filter ) == nil ) then
        return title , msg , cmd ,breakFlag
    end
    return nil , nil , nil , false
end



-- -- 設定スペースここまで。
function CheckSakuraScript( path , sakuraScript )
    -- print( "Check SakuraScript : " ..  sakuraScript )
    -- print( path )

    -- 実行順は保障されない。
    for name, func in pairs(_G) do
        -- 関数かつ、指定の名前から始まる。
        if type(func) == "function" and not ( string.match( name , "^Filter_.*" ) == nil ) then
            title , msg , cmd , breakFlag = func( sakuraScript )

            -- cmdの実行
            if not ( cmd == nil or cmd == "" ) then
                os.execute( cmd )
            end

            -- 通知の実行
            if not ( title == nil or title == "" ) then
                if title == "" then;title = "NoTitle";  end
                if msg   == "" then;msg   = "NoMessage";end
                -- os.execute( "PowerShell.exe " .. path .. "dll\\anko.ps1 " .. title .. " " .. msg)
                os.execute( path .. "dll\\toast.bat " .. title .. " " .. msg)
            end

            -- 処理を継続するかどうか。
            if breakFlag == true then
                break
            end
        end
    end
end



-- テスト用。
-- home = os.getenv ( "USERPROFILE" )
-- CheckSakuraScript( home .. '\\OneDrive\\ukagaka\\ssp\\plugin\\SSP-Plugin_NotificationWord\\dll\\toast.bat' , "aaa")


