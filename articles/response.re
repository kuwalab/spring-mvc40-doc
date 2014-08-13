= レスポンス
== レスポンスの返し方
== ファイルのダウンロード

==={csv_dowonload1} CSVファイルのダウンロード（Springに依存しない方法）

@<b>{タグ【022】}

ファイルのダウンロードにはいくつか方法があるのですが、今回はSpringにあまり依存しない方法を紹介します。

具体的なソースは以下になります。csvInitでアンカーの書かれた画面を開き、csvDownが実際のダウンロードになります。ダウンロードの際には戻り値はvoidとして、HttpServletResponse#getWirter()メソッドでダウンロードするデータを書き出します。

//list[csv_donwload1-ResController.java][ResController.java]{
package com.example.spring.controller;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Controller;
import org.springframework.util.MimeTypeUtils;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
public class ResController {
    @RequestMapping("/csvInit")
    public String csvInit() {
        return "res/csvInit";
    }

    @RequestMapping("csvDown")
    public void csvDown(HttpServletResponse response) {
        response.setContentType(MimeTypeUtils.APPLICATION_OCTET_STREAM_VALUE
                + ";charset=utf-8");
        response.setHeader("Content-Disposition",
                "attachment; filename=\"test.csv\"");
        try (PrintWriter pw = response.getWriter()) {
            pw.println("山田　太郎, 33");
            pw.println("田中　花子, 29");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
//}

アンカーを表示する画面です。単純なアンカーのみです。

//list[csv_download1-csvInit.jsp][csvInit.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <a href="csvDown">csvDown</a>
 </body>
</html>
//}

==={csv_download2} CSVファイルのダウンロード（ResponseEntity）

@<b>{タグ【023】}

今回もCSVダウンロードについてです。今回はResponseEntityクラスを使用したダウンロードについてです。

ResponseEntityではデータと、ヘッダー、ステータスコードを返せます。

日本語の文字化けをしないようにするために、HttpHeadersクラスのsetContentTypeを使わずに、addメソッドで文字コードと一緒にcontent-typeを指定しています。

CSVデータはただの文字列のデータになります。

//list[csv_download2-ResController.java][ResController.java]{
@RequestMapping(value = "/csvDown2", method = RequestMethod.GET, produces = "application/octet-stream;charset=utf-8")
public ResponseEntity<String> csvDown2() {
    HttpHeaders headers = new HttpHeaders();
    // headers.setContentType(MediaType.APPLICATION_OCTET_STREAM);
    headers.add("contet-type", MimeTypeUtils.APPLICATION_OCTET_STREAM_VALUE
            + ";utf-8");
    headers.set("Content-Disposition", "filename=\"test2.csv\"");
    String csvData = "山田　太郎,33\r\n";
    csvData = csvData + "田中　花子,29";

    return new ResponseEntity<String>(csvData, headers, HttpStatus.OK);
}
//}

アンカーを表示する画面です。単純なアンカーのみです。前回のものに2を追加しています。

//list[csv_download2-csvInit.jsp][csvInit.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <a href="csvDown">csvDown</a><br>
  <a href="csvDown2">csvDown2</a>
 </body>
</html>
//}

==={csv_download3} CSVファイルのダウンロード（ResponseBody）

@<b>{タグ【024】}

ResponseBodyを返す形で実装します。

ResponseEntityではデータと、ヘッダー、ステータスコードを返せます。

日本語の文字化けをしないようにするために、HttpHeadersクラスのsetContentTypeを使わずに、addメソッドで文字コードと一緒にcontent-typeを指定しています。

CSVデータはただの文字列のデータになります。

//list[csv_download3-ResController.java][ResController.java]{
@RequestMapping(value = "/csvDown3", method = RequestMethod.GET, produces = MimeTypeUtils.APPLICATION_OCTET_STREAM_VALUE
        + ";charset=utf-8")
@ResponseBody
public String csvDown3(HttpServletResponse response) {
    response.setHeader("Content-Disposition",
            "attachment; filename=\"test3.csv\"");

    String csvData = "山田　太郎,33\r\n";
    csvData = csvData + "田中　花子,29";
    return csvData;
}
//}

アンカーを表示する画面です。単純なアンカーのみです。前回のものに3を追加しています。

//list[csv_download3-csvInit3.jsp][csvInit3.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <a href="csvDown">csvDown</a><br>
  <a href="csvDown2">csvDown2</a><br>
  <a href="csvDown3">csvDown3</a>
 </body>
</html>
//}

=== Resolverを使う

