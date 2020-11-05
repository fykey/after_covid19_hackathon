# after_covid19_hackathon

参考サイト
1. esp-wroom-02をwifiに接続するために使ったサイト
https://www.mgo-tec.com/blog-entry-ss-wroom-howto01.html
2. ロードセルの圧力算出に使ったサイト
https://lowreal.net/2016/12/25/1

これをつなぎ合わせて作った．
変えた部分は，
・serial.begin(9600)に全部変えた
・1の方では"Hello world"を表示するだけだったので，Ini_HTTP_Responseの関数を無理やり引数を置いて，
114行目でその引数を表示するようにした．
