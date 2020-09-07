# Alive Beacon Receiver and Sender

Alive Beacon送信プログラムによって送信されたパケットが受信出来ない場合に異常事態を通知する。

監視対象となるサーバは、設定ファイルに設定されたもので、それ以外のサーバからの信号は無視する。

サーバ類の接続関係による判断は現在のところ人間の判断に委ねる。

## 設定ファイル

    /etc/alb/config.init
    
    IP_ADDRESS,ID,DELAY,DIR,COMMENT

 - IP_ADDRESS: IPアドレス
 - ID: 送信側が送信してくるID
 - DELAY: 何秒受信できなければ異常にするかを指定する
 - DIR: サーバのある場所をディレクトリ構成で指定する
 - COMMENT: コメント

    211.124.5.34,HMD1CON,5,/ACN/K/HMD,HIMEDO


## Sender

### How to compile

    make albsender
    make install.sender

