= 入力チェック

== 型チェック

==={type_check} 受け取る型のチェック

@<b>{タグ【011】}

今までパラメータはすべてStringで受けていたので、型変換のエラー等は発生していませんでした。今回は、String型以外の型でパラメータを受け取りどのような動作になるかを確認していきます。

最初にパラメータを受けつ取るクラスを作成します。price変数をIntegerにしておきどのように動作するのかを確認します。

//list[type_check-Book.java][Book.java]{
package com.example.spring.controller;

public class Book {
    private String name;
    private Integer price;
    // getter setterは省略
}
//}

Controllerクラスです。ModelAttriubteでBookのパラメータを受け取ります。また、Bookへのパラメータの割り当ての際のエラーを取得するため、Bookの次の引数にBindingResultクラスを指定します。BIndingResultクラスは、バインドされるクラスの次の引数にないといけません。

//list[type_check-CheckController.java][CheckController.java]{
@Controller
public class CheckController {
    @RequestMapping("/checkType")
    public String checkType(@ModelAttribute Book book, BindingResult errors) {
        return "check/checkType";
     }
}
//}

最後に表示用のJSPです。

//list[check_type-checkType.jsp][checkType.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
 バインディングエラー <form:errors path="book.*" /><br>
book.nameの値は <c:out value="${book.name}" /><br>
book.priceの値は <c:out value="${book.price}" /><br>
 </body>
</html>
//}

このプログラムで「/checkType?name=書籍&price=文字列」というリクエストを投げると以下の様な出力になります。

//cmd{
バインディングエラー Failed to convert property value of type java.lang.String to required type java.lang.Integer for property price; nested exception is java.lang.NumberFormatException: For input string: "文字列"
book.nameの値は 書籍
book.priceの値は 
//}

book.priceには文字列のためバインディングができずnullが入っています。また、バインディング失敗のデフォルトのエラーメッセージが表示されます。

このままだと、非常にわかりにくいのでデフォルトのエラーメッセージを変更します。

まず、spring-context.xmlにメッセージソースの設定を追加します。

//list[type_check-spring-context.xml][spring-context.xml]{
<bean id="messageSource"
 class="org.springframework.context.support.ReloadableResourceBundleMessageSource">
 <property name="basename" value="classpath:/messages" />
</bean>
//}

そして、指定した名前+「.properties」ファイルを作成します。今回は型変換のエラーのメッセージだけを記載します。

//list[type_check-messages.properties][messages.properties]{
typeMismatch.java.lang.Integer=整数で入力してください。
//}

このメッセージは、<a href="http://docs.spring.io/spring/docs/current/javadoc-api/org/springframework/validation/DefaultMessageCodesResolver.html">DefaultMessageCodesResolver</a>のJavaDocに記載のある通り、

1.: code + "." + object name + "." + field
2.: code + "." + field
3.: code + "." + field type
4.: code

の順でメッセージが解決されます。

この状態で先ほどと同じデータを送信すると上記のメッセージになります。

ここで、メッセージを以下のように変更します。

//list[type_check-message.properties2][message.properties]{
typeMismatch.java.lang.Integer={0}は整数で入力してください。
//}

ここで、また先程のリクエストを送信すると今回は{0}の部分がフィールド名になり、

//cmd{
priceは整数で入力してください。
//}

というメッセージにあります。このpriceの部分はフィールド名に対応していて、この名前も置換が可能です。

メッセージソースに、フィールド名の値をキーとして登録します。

//list[type_check-message.properties3][message.propeties]{
price=価格
//}

そうすることで、最終的には以下のメッセージになります。

//cmd{
価格は整数で入力してください。
//}

== JSR349によるvalidation

== ロジックによる入力チェック

