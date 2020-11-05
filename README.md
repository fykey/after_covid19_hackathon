# after_covid19_hackathon
11/6 更新
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
・load cell用で使ってたピン番号を，espr-developer用の0, 2に変えた

使用する際は，HX711.hのインストール(2のサイトで説明してる)や，esp用のボードをダウンロードすること(https://www.mgo-tec.com/blog-entry-ss-wroom-howto01.html)などが必要．

今後の課題
・一定時間が経ったらバイブレーションがなるようにしなくてはならない．(モータなどの制御も必要)
別のarduinoなどで制御した方がいいのか？なんにせよ，素子→wifiはできても，wifi→素子ができなそう(apple製品)だから，素子内部で処理しなければならない．
・データの送信方法についてもう少し考える．

とりあえず現時点での使用したプログラム，配線をhakkason_test_2.ino, hackason_parts.fzzを載せておく．
