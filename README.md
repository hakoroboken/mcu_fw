# mcu_fw

## 環境構築

1. Arduino IDEをインストール

    バージョン2.xx移行推奨

    [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

2. ボード定義をインストール

    Arduino IDEの設定を開き以下のURLを`Additional boards manager URLs:`に以下のURLを書き込む


    ```
    https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
    ```

3. ライブラリのインストール

    以下のURLからzipファイルをダウンロードしzipライブラリの追加から追加する。
    [https://github.com/TakanoTaiga/Ethernet/archive/refs/tags/v3.0.zip](https://github.com/TakanoTaiga/Ethernet/archive/refs/tags/v3.0.zip)


## ピン配置

- PWM-output : GP2
- CW/CCW : GP3

- mcu-rm-f303k8 serial : UART0 TX (GP0) & UART0 RX (GP1)

## UDP パケットアリゴリズム

### 入力

ポート: 64202

文字列データを入力

モータ出力(PWWMタイプのMDを接続してる場合) : -100 ~ 100 

モータデータ出力(有効化)
