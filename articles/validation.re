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
バインディングエラー Failed to convert property value of type java.lang.String to required
    type java.lang.Integer for property price; nested exception is 
    java.lang.NumberFormatException: For input string: "文字列"
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

==={validation_not_null} Validatorでnullチェック

@<b>{タグ【012】}

Springは標準でBean Validationをサポートしており、Spring 4.0からはJSR-349のBean Validation 1.1をサポートしています。ここでははSpringでBean Validationを使うための設定と、nullチェックのサンプルを紹介します。

まず、MavenでBean Validation関連のライブラリーを追加します。

//list[validation_not_null-pom.xml][pom.xml]{
<dependency>
 <groupId>javax.el</groupId>
 <artifactId>javax.el-api</artifactId>
 <version>3.0.0</version>
 <scope>provided</scope>
</dependency>
<dependency>
 <groupId>javax.validation</groupId>
 <artifactId>validation-api</artifactId>
 <version>1.1.0.Final</version>
</dependency>
<dependency>
 <groupId>org.hibernate</groupId>
 <artifactId>hibernate-validator</artifactId>
 <version>5.1.1.Final</version>
</dependency>
//}

Bean Validation 1.1本体と、参照実装のHibernate 5.1、また、Bean Validation 1.1からは内部でEL 3.0を使用するためELのライブラリーを追加しています。

ついでSpring MVCの設定のWEB-INF/spring/spring-context.xmlを編集します。それなりの量が変わるため全体を再掲しています。

//list[validation_not_null-spring-context.xml][spring-context.xml]{
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
 xmlns:mvc="http://www.springframework.org/schema/mvc"
 xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context-4.0.xsd
http://www.springframework.org/schema/mvc
http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
 <mvc:annotation-driven validator="validator" />
 <context:component-scan base-package="com.example.spring" />
 <bean
  class="org.springframework.web.servlet.view.InternalResourceViewResolver">
  <property name="prefix" value="/WEB-INF/jsp/" />
  <property name="suffix" value=".jsp" />
 </bean>
 <bean id="messageSource"
  class="org.springframework.context.support.ReloadableResourceBundleMessageSource">
  <property name="basename" value="classpath:/messages" />
 </bean>
 <bean id="validator"
  class="org.springframework.validation.beanvalidation.LocalValidatorFactoryBean">
  <property name="validationMessageSource" ref="messageSource" />
 </bean>
</beans>
//}

messageSourceを定義し、そのソースをvalidatorに関連付けます。そのvalidatorを<mvc:annotation-driven>のvalidator属性に設定し、Validatorの設定は完了となります。

メッセージリソースは、Hibernateに付属のプロパティファイルをそのまま利用します。

//list[validation_not_null-messages.properties1][messages.properties]{
javax.validation.constraints.AssertFalse.message = must be false
javax.validation.constraints.AssertTrue.message = must be true
javax.validation.constraints.DecimalMax.message = must be less than ${inclusive == true ? 'or equal to ' : ''}{value}
javax.validation.constraints.DecimalMin.message = must be greater than ${inclusive == true ? 'or equal to ' : ''}{value}
javax.validation.constraints.Digits.message = numeric value out of bounds (<{integer} digits>.<{fraction} digits> expected)
javax.validation.constraints.Future.message = must be in the future
javax.validation.constraints.Max.message = must be less than or equal to {value}
javax.validation.constraints.Min.message = must be greater than or equal to {value}
javax.validation.constraints.NotNull.message = may not be null
javax.validation.constraints.Null.message = must be null
javax.validation.constraints.Past.message = must be in the past
javax.validation.constraints.Pattern.message = must match "{regexp}"
javax.validation.constraints.Size.message = size must be between {min} and {max}

org.hibernate.validator.constraints.CreditCardNumber.message = invalid credit card number
org.hibernate.validator.constraints.EAN.message = invalid {type} barcode
org.hibernate.validator.constraints.Email.message = not a well-formed email address
org.hibernate.validator.constraints.Length.message = length must be between {min} and {max}
org.hibernate.validator.constraints.LuhnCheck.message = The check digit for ${value} is invalid, Luhn Modulo 10 checksum failed
org.hibernate.validator.constraints.Mod10Check.message = The check digit for ${value} is invalid, Modulo 10 checksum failed
org.hibernate.validator.constraints.Mod11Check.message = The check digit for ${value} is invalid, Modulo 11 checksum failed
org.hibernate.validator.constraints.ModCheck.message = The check digit for ${value} is invalid, ${modType} checksum failed
org.hibernate.validator.constraints.NotBlank.message = may not be empty
org.hibernate.validator.constraints.NotEmpty.message = may not be empty
org.hibernate.validator.constraints.ParametersScriptAssert.message = script expression "{script}" didn't evaluate to true
org.hibernate.validator.constraints.Range.message = must be between {min} and {max}
org.hibernate.validator.constraints.SafeHtml.message = may have unsafe html content
org.hibernate.validator.constraints.ScriptAssert.message = script expression "{script}" didn't evaluate to true
org.hibernate.validator.constraints.URL.message = must be a valid URL

org.hibernate.validator.constraints.br.CNPJ.message = invalid Brazilian corporate taxpayer registry number (CNPJ)
org.hibernate.validator.constraints.br.CPF.message = invalid Brazilian individual taxpayer registry number (CPF)
org.hibernate.validator.constraints.br.TituloEleitoral.message = invalid Brazilian Voter ID card number
//}

このままだと英語だけなので、必要な物から日本語にしていきます。日本語のメッセージリソースは、messages_ja.propertiesというファイル名にして以下の内容にしておきます。

//list[validation_not_null-messages.properties2][messages.properties]{
javax.validation.constraints.NotNull.message = 入力は必須です

typeMismatch.java.lang.Integer={0}は整数で入力してください。
price=価格
//}

次にvalidatorを動作させるためのControllerを作成します。

//list[validation_not_null-CheckController.java][CheckController.java]{
@RequestMapping("/bookForm")
public String bookForm() {
    return "check/bookForm";
}

@RequestMapping(value = "/bookRecv", method = RequestMethod.POST)
public String bookRecv(@Valid @ModelAttribute Book book,
        BindingResult errors) {
    if (errors.hasErrors()) {
        return "check/bookForm";
    }
    return "check/bookRecv";
}
//}

コントローラで重要なのは、データを受信するbookRecvメソッドになります。bookRecvメソッドではデータを受け取るBookクラスに@Validアノテーションがついています。@Validアノテーションを付けることでBean Validationが動作するようになります。また、Bean Validationの検証結果は、次の引数のBindingResultの中に格納されます。

メソッドの中で、BindingResult#hasErrorsメソッドでエラーが有るか確認し、エラーがある場合には入力画面に戻るようにしています。

次に、Bookクラスの定義を確認します。

//list[valiidation_not_null-Book.java][Book.java]{
package com.example.spring.controller;

import javax.validation.constraints.NotNull;

public class Book {
    @NotNull
    private String name;
    @NotNull
    private Integer price;
    // setter、getterは省略
}
//}

Bookクラスの各フィールドには@NotNullアノテーションを付けています。このアノテーションを付けることで@Validの付いたコントローラで自動的にValidationが行われます。@NotNullアノテーションをつけているとフィールドがnullの場合に検証エラーとなります。

次にJSPを確認していきます。まずデータ送信をするためのフォームの画面（bookForm.jsp）です。

//list[validation_not_null-bookForm.jsp][bookForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="bookRecv" method="post">
   書名: <input type="text" name="name" size="20"><form:errors path="book.name" /><br>
   価格: <input type="text" name="price" size="20"><form:errors path="book.price" /><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

入力画面では、入力フォームの後ろにエラー表示のカスタムタグをつけています。pathにはモデルオブジェクト（book）のフィールド名を指定します。こうすることで、そのフィールドで発生したエラーがカスタムタグの場所に表示されます。

入力された結果の確認用のJSP（bookRecv.jsp）です。

//list[validation_not_null-bookRecv.jsp][bookRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
book.nameの値は <c:out value="${book.name}" /><br>
book.priceの値は <c:out value="${book.price}" /><br>
 </body>
</html>
//}

ここまでで実際にエラーを検証できる環境が整いましたので、実際にエラーを発生させてみます。フォームに何も入力せずに送信ボタンを押すと価格のみエラーとなります。これは、受け取るフィールドの型の違いのために起こります。書名はString型のためフォームから送られてくる空文字列（""）を受け取るためnullにはなりません。対してpriceは空文字列はInteger型のため空文字列は受け取れずnullになります。そのため、priceのみ@NotNullでエラーとなります。

空文字のチェックのためにはBean Validation標準のアノテーションではなく、HibernateのValiationを使うとできます。この辺りは次回以降に解説していきます。

==={validation_decimal} ValidatorでDecimalのチェック

@<b>{タグ【013】}

今回からしばらくBean Validationの標準のValidatorの説明をしていきます。最初はDecimalMax、DecimalMinの2つです。

DecimalMaxとDecimalMinはその名の通り、数値の最大と最小をチェックします。また、inclusive属性をtrue/falseにすることで、値自体を含む、含まないかを選択できます。

Bookのpriceフィールドに値を設定します。以下の例だと1〜100000未満だけ入力が許されます。

//list[validation_decimal-Book.java][Book.java]{
@NotNull
private String name;
@NotNull
@DecimalMin("1")
@DecimalMax(value = "100000", inclusive = false)
private Integer price;
//}

DecimalMaxとDecimalMinのメッセージを用意します。

//list[validation_decimal-messages.properties][messages.properties]{
javax.validation.constraints.DecimalMax.message = {0}は{value}${inclusive == true ? '以下の' : 'より小さい'}数を入力してください
javax.validation.constraints.DecimalMin.message = {0}は{value}${inclusive == true ? '以上の' : 'より大きい'}数を入力してください
//}

メッセージでは、{value}でValidationする値をメッセージに埋め込むことができます。また、EL 3.0による処理でinclusiveの値によってメッセージを変えています。ELが使えることによって、かなり柔軟なメッセージ表示が可能になっています。

==={validation_min_max} ValidatorでMin、Maxのチェック

@<b>{タグ【014】}

今回はBean ValidationのMin、Maxの2つです。

MinとMaxはDecimalMax、DecimalMinと違い、整数のみのチェックとなります。またその数値自身を含むチェックのみが可能です。valueも数値で指定できるので、整数のチェックの場合はこちらのほうがいいかもしれません。

//list[validation_min_max-Book.java][Book.java]{
@NotNull
private String name;
@NotNull
@Min(1)
@Max(100000)
private Integer price;
//}

メッセージは以下のように記述します。

//list[validation_min_max-messages.properties][messages.properteis]{
javax.validation.constraints.Max.message = {0}は{value}以下の数を入力してください
javax.validation.constraints.Min.message = {0}は{value}以上の数を入力してください
//}

==={validation_digits} ValidatorでDigitsのチェック

@<b>{タグ【015】}

今回はBean ValidationのDigitsです。

Digitsは数値の具体的な最小、最大値ではなく、整数部の最大桁数、小数部の最大桁数を指定します。桁数以内であれば、小さくても大丈夫です。

//list[validation_digits-Book.java][Book.java]{
@NotNull
private String name;
@NotNull
@Digits(integer = 3, fraction = 2)
private Double price;
//}

メッセージは以下のように記述します。

//list[validation_digits-messages.propeties][messages.properties]{
javax.validation.constraints.Digits.message = {0}は整数{integer}桁、小数{fraction}桁以内で入力してください
//}

==={validation_size} ValidatorでSizeのチェック

@<b>{タグ【016】}

今回はBean ValidationのSizeです。

Sizeは文字列の長さの検査や、Collectionの長さの検査ができます。今回は文字列の長さの例を見ていきます。

//list[validation_size-Book.java][Book.java]{
@NotNull
@Size(min = 10, max = 10)
private String name;
@NotNull
private Integer price;
//}

メッセージは以下のように記述します。

//list[validation_size-messages.properties][messages.properties]{
javax.validation.constraints.Size.message = ${min == max ? min += '文字で入力してください' : min += '〜' += max += '文字で入力してください'}
//}

メッセージはELで分岐し、最大最小文字が同じ場合と、それ以外で変更しています

==={validation_regexp} Validatorで正規表現でのチェック

@<b>{タグ【017】}

今回はBean Validationの正規表現です。

正規表現なので、色々なパターンのチェックができますが、今回は「ISBN + 数字10桁」のチェックをします。

//list[validation_regexp-Book.java][Book.java]{
@NotNull
@Pattern(regexp = "ISBN[0-9]{10}", message = "{0}はISBNを入力してください")
private String name;
@NotNull
private Integer price;
//}

メッセージのデフォルトは、regexp属性の値を表示しますが、ユーザーにはやさしくないため、@Patternのmessage属性で直接指定しています。

==={validation_not_blank} ValidatorでNotBlankのチェック

@<b>{タグ【018】}

今回はBean ValidationのNotBlankです。

Bean Validationの標準ではなく、Hibernateの実装に含んでいるValidatorになります。これを使うと文字列の空文字のチェックができます。

//list[validation_not_blank-Book.java][Book.java]{
@NotBlank
private String name;
@NotNull
private Integer price;
//}

メッセージは以下のようにします。

//list[validation_not_blank-messages.properties][messages.properteis]{
org.hibernate.validator.constraints.NotBlank.message = 入力は必須です
//}

=== Validatorでロジックを介した検証

=== Validatorのエラーの際に入力値を再表示させる

== ロジックによる入力チェック

