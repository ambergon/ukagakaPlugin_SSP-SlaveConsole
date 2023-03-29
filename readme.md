# SSP-SlaveConsole
今後、伺かによって情報収集をする際に、<br>
バルーンに表示した内容をすべて確認するのは大変だと考え、<br>
必要な情報のみを通知できる仕組みを作ろうと思い作成しました。<br>
<br>
実行時にコンソールが表示されます。<br>
これはこのプラグインを使って実行したコマンドの結果なども出力されるほか、SSPで読み込まれている他のdllなどで出力があれば流れてきます。<br>


## 出来ること。
ゴーストやプラグインが発言したトークを拾って`Filter_`から始まる関数でマッチします。<br>
マッチした場合、サクラスクリプトの`trayballoon`のような通知を流したり、事前に設定したコマンドを実行できます。<br>


#### 非プログラマ向けの簡単な使い方
[【伺か】一部のサクラスクリプトを通知する仕組みを作ってみた。【プラグイン】 -- 異風堂々](https://ambergonslibrary.com/ukagaka/8695/)<br>


## 課題。
luaから通知用のbatファイルへの実行がshift-jisになってしまい、日本語等を使うと文字化けします。<br>
対策として、日本語を使わない。もしくはluaの設定ファイルの返すタイトル・メッセージをshift-jisで記入する方法があります。<br>


## 実行環境
win10/SSP/2.6.41で確認している。<br>


## 依存
PowerShellが実行可能なこと。<br>


## 設定ファイル
luaファイル/func.luaによって設定します。<br>
```
function Filter_Sample( sakuraScript )
    -- 検索文字列
    local filter    = ""
    -- 通知タイトル。
    local title     = ""
    -- 通知内容
    local msg       = ""
    -- 実行コマンド
    local cmd       = ""
    -- 継続するかどうか。
    local breakFlag = true
    if not ( filter == "" ) and not ( string.match( sakuraScript , filter ) == nil ) then
        return title , msg , cmd ,breakFlag
    end
    return nil , nil , nil , false
end
```
検索にヒットした場合、通知タイトルがあれば通知を行います。<br>
実行コマンドがあれば実行します。<br>
これらはどちらも行うことができます。<br>
<br>
breakFlagをtrueにした場合、この実行のみでこのサクラスクリプトに対しての検索を終了します。<br>
falseにした場合、同じサクラスクリプトに対して残りのFilterも確認して実行します。<br>
<br>
Filterの実行順番は指定することはできません。<br>
また、これらは別スレッドで実行されるため、SSPや栞の挙動に影響を与えません。<br>



## Other
このプログラムを利用することによるいかなる問題や損害に対して、私は責任を負いません。<br>


