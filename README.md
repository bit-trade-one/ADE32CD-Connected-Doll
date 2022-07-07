## ADE32CD-オーディオストリーミングスタンド開発ボード「Connected Doll」

### *製品ページは[こちら](http://bit-trade-one.co.jp/ADE32CD)*.

### [回路図](https://github.com/bit-trade-one/RasPi-Zero-One-Series/blob/master/3rd/ADE32CD/Schematics/ADE32CM_Schematics.pdf)

## ファイルリスト
- Readme.md             ［本文書］
- bt_speaker_demo.ino    [Bluetoothスピーカーサンプルプログラム］

## 開発環境
本製品の動作確認は、以下の環境で実施することを前提としております。
- Arduino IDE<BR>
  Arduino IDE 1.8.19
- ライブラリ<BR>
FastLED 3.5.0<BR>
[ESP32-A2DP 1.7.0](https://github.com/pschatzmann/ESP32-A2DP) @pschatzmann氏のESP32-A2DPライブラリを使用させていただきました。<BR>
Thanks to @pschatzmann, author of the ESP32-A2DP library.

## インストール
- FastLEDライブラリ3.5.0<BR> 
Arduino IDEのライブラリマネージャからインストールしてください。
![](./img/%20%20%20Readme2022-06-21-16-22-53.png)

- ESP32-A2DP 1.7.0<BR>
GitHubからzip形式でダウンロード後、Arduino IDEの「スケッチ」「ライブラリをインクルード」「.ZIP形式のライブラリをインストール」を選択し、ダウンロードしたzipファイルを指定します。
![](./img/%20%20%20Readme2022-06-21-16-23-53.png)

- bt_spaker_demo<BR>
Arduino IDEの「ファイル」「開く」から予めダウンロード済みの「bt_speaker_demo.ino」を入力します。
![](./img/%20%20%20Readme2022-06-21-16-35-54.png)

- Arduino IDEの設定<BR>
「ツール」メニューから「ボード」は「ESP32 Dev Module」、「シリアルポート」はコネクティッドドールをを接続したCOM番号を指定します。
![](./img/%20%20%20Readme2022-06-21-16-39-11.png)

- ビルドとインストール<BR>
「スケッチ」「マイコンボードに書き込む」からビルドとインストールを実施します。
書込みが完了すると、基板周辺の4つのLEDのが赤く点灯します。

## 使用方法
IOSやAndroidのBluetoothメニューから、デバイス名「BTO_Music」を選択肢し、ペアリングします。その後、音楽プレーヤー等で音楽等を再生してください。
音楽等が再生できること、および再生中に基板周辺のLEDが点滅することを確認してください。 
