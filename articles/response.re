= レスポンス
== レスポンスの返し方
== ファイルのダウンロード

==={022} CSVファイルのダウンロード（Springに依存しない方法）

@<b>{タグ【022】}

ファイルのダウンロードにはいくつか方法があるのですが、今回はSpringにあまり依存しない方法を紹介します。

具体的なソースは以下になります。csvInitでアンカーの書かれた画面を開き、csvDownが実際のダウンロードになります。ダウンロードの際には戻り値はvoidとして、HttpServletResponse#getWirter()メソッドでダウンロードするデータを書き出します。

//list[022-C022Controller.java][C022Controller.java]{
package com.example.spring.controller.c022;

import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Controller;
import org.springframework.util.MimeTypeUtils;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
public class C022Controller {
    @RequestMapping("/c022/csvInit")
    public String csvInit() {
        return "c022/csvInit";
    }

    @RequestMapping("/c022/csvDown")
    public void csvDown(HttpServletResponse response) {
        response.setContentType(MimeTypeUtils.APPLICATION_OCTET_STREAM_VALUE
                + ";charset=utf-8");
        response.setHeader("Content-Disposition",
                "attachment; filename=\"test.csv\"");
        try (PrintWriter pw = response.getWriter()) {
            pw.print("山田　太郎,33\r\n");
            pw.print("田中　花子,29\r\n");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
//}

アンカーを表示する画面です。単純なアンカーのみです。

//list[022-csvInit.jsp][csvInit.jsp]{
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

確認用のテストケースは次のとおりです。

//list[022-C022ControllerTest.java][C022ControllerTest.java]{
package com.example.spring.controller.c022;

import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.*;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.web.context.WebApplicationContext;

@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations = {
    "file:src/main/webapp/WEB-INF/spring/spring-context.xml" })
public class C022ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void csvInitのGET() throws Exception {
        mockMvc.perform(get("/c022/csvInit")).andExpect(status().isOk())
                .andExpect(view().name("c022/csvInit"));
    }

    @Test
    public void csvDownのGET() throws Exception {
        StringBuilder sb = new StringBuilder();
        sb.append("山田　太郎,33\r\n");
        sb.append("田中　花子,29\r\n");
        mockMvc.perform(get("/c022/csvDown"))
                .andExpect(status().isOk())
                .andExpect(
                        content().contentType(
                                "application/octet-stream;charset=utf-8"))
                .andExpect(content().string(sb.toString()));
    }
}
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
@RequestMapping(value = "/csvDown3", method = RequestMethod.GET,
    produces = MimeTypeUtils.APPLICATION_OCTET_STREAM_VALUE
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

==={csv_download4} CSVファイルのダウンロード（Resolver）

@<b>{タグ【032】}

spring-context.xmlにViewResolverの設定を追加します。

//list[csv_download4-spring-context.xml][spring-context.xml]{
<bean id="xmlViewResolver" class="org.springframework.web.servlet.view.XmlViewResolver">
 <property name="order" value="1" />
 <property name="location" value="/WEB-INF/spring/views.xml" />
</bean>
//}

views.xmlは以下になります。

//list[csv_download4-views.xml][views.xml]{
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns:context="http://www.springframework.org/schema/context"
 xmlns:mvc="http://www.springframework.org/schema/mvc"
 xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context-4.0.xsd
http://www.springframework.org/schema/mvc
http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
 <bean id="csvDownload" class="com.example.spring.controller.CsvDownloadView" />
</beans>
//}

Resolver

//list[csv_download4-CsvDownloadView.java][CsvDownloadView.java]{
package com.example.spring.controller;

import java.io.PrintWriter;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.util.MimeTypeUtils;
import org.springframework.web.servlet.view.AbstractView;

public class CsvDownloadView extends AbstractView {

    @Override
    protected void renderMergedOutputModel(Map<String, Object> model,
            HttpServletRequest request, HttpServletResponse response)
            throws Exception {
        response.setContentType(MimeTypeUtils.APPLICATION_OCTET_STREAM_VALUE
                + ";charset=utf-8");
        ;

        response.setHeader("Content-Disposition",
                "attachment; filename=\"test.csv\"");
        try (PrintWriter pw = response.getWriter()) {
            for (Book book : (List<Book>) model.get("bookList")) {
                pw.print(book.getName());
                pw.print(",");
                pw.println(book.getPrice());
            }
        }
    }
}
//}

Controllerです。

//list[csv_download4-ResController.java][ResController.java]{
@RequestMapping("/csvDown4")
public String csvDown4(Model model) {
    List<Book> bookList = new ArrayList<>();

    Book book = new Book();
    book.setName("よくわかるSpring");
    book.setPrice(3000);
    bookList.add(book);

    book = new Book();
    book.setName("よくわかるJava");
    book.setPrice(2980);
    bookList.add(book);

    model.addAttribute("bookList", bookList);
    return "csvDownload";
}
//}

