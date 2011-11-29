# DEM2JSON

## 概要

Javascriptで緯度経度から標高を取得するスクリプト用の標高デー
タを作成するプログラム

## 説明

このツールはJavascriptで利用できる標高データを作成します。標
高データは"DEM (Digital Elevation Model)"と呼ばれる標高データ
から作成します。フリーのDEMは２つほど存在します。１つは[GTOPO30]
と呼ばれるものですが、品質があまりよくありません。もう一つは
[ASTER]と呼ばれるもので品質は高いです。これらのデータの入手
方法についてはここでは述べませんので、詳細は各サイトを参照し
てください。
作成された標高データはJSON形式でJavascriptから利用することが
できます。
ソースにはコンバータとJavascript用のjsファイルが含まれます。

  [GTOPO30]: http://eros.usgs.gov/#/Find_Data/Products_and_Data_Available/gtopo30_info
  [ASTER]: http://asterweb.jpl.nasa.gov/

## ビルド方法

MinGW上のgccでコンパイルできていることを確認しています。
その他の環境については未確認です。
ビルドには[zlib]  [minizip]  [boost]ライブラリが必要です。

  [zlib]: http://zlib.net/
  [minizip]: http://www.winimage.com/zLibDll/minizip.html
  [boost]: http://www.boost.org/

フォルダ構造:

    DEM2JSON/
        minizip/
            ioapi.c
            ioapi.h
            unzip.c
            unzip.h
        src/
            AltitudeMatrix.cpp
            AltitudeMatrix.h
            CommandLine.cpp
            CommandLine.h
            CommonInclude.h
            DEM2JSON.cpp
            DEMMatrix.cpp
            DEMMatrix.h
            GeoTiffReader.cpp
            GeoTiffReader.h
            Matrix.h
        .cproject
        .project

Eclipse CDTでプロジェクトファイルを開いてビルドします。

## コンバート方法

作成されたオブジェクトのコマンドパラメータは下記の通りです。
`DEM2JSON -D path/to/ASTER -J path/to/JSON [-S]`

    *options
        -D : ASTER データのパス。ASTERのzipファイルの入ったパスです。このパスのファイルを全て変換します。
        -J : コンバートしたJSONデータの出力パスです。
        -S : 作成する標高データをガウシアンフィルタでスムーシングするかを指定します。

## 標高データの利用方法

jsファイル準備中です。

## LICENSE

このスクリプトはMITライセンスとします。
LICENSE ファイル参照。

## 連絡先など

[email :] mmgithub@gmail.com
[web :] http://world-route.sakura.ne.jp/
