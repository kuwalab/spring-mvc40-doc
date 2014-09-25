= 入力チェック

== 型チェック

==={011} 受け取る型のチェック

@<b>{タグ【011】}

ここまではパラメータはすべてStringで受けていたので、型変換のエラー等は発生していませんでした。ここでは、String型以外の型でパラメータを受け取りどのような動作になるかを確認していきます。

最初にパラメータを受けつ取るクラスを作成します。price変数をIntegerにしておきどのように動作するのかを確認します。

//list[011-C011Model.java][C011Model.java]{
package com.example.spring.controller.c011;

public class C011Model {
    private String name;
    private Integer price;

    // setter、getterは省略
}
//}

Controllerクラスです。ModelAttriubteでC011Modelのパラメータを受け取ります。また、C011Modelへのパラメータの割り当ての際のエラーを取得するため、C011の次の引数にBindingResultクラスを指定します。BIndingResultクラスは、バインドされるクラスの次の引数にないといけません。

//list[011-C011Controller.java][C001Controller.java]{
package com.example.spring.controller.c011;

import org.springframework.stereotype.Controller;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
@RequestMapping("/c011")
public class C011Controller {
    @RequestMapping("/checkType")
    public String checkType(@ModelAttribute C011Model c011Model,
            BindingResult errors) {
        return "c011/checkType";
    }
}
//}

最後に表示用のJSPです。

//list[011-checkType.jsp][checkType.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
 バインディングエラー <form:errors path="c011Model.*" /><br>
c011Model.nameの値は <c:out value="${c011Model.name}" /><br>
c011Model.priceの値は <c:out value="${c011Model.price}" /><br>
 </body>
</html>
//}

このプログラムで「/c011/checkType?name=書籍&price=文字列」というリクエストを投げると次の様な出力になります。

//cmd{
バインディングエラー Failed to convert property value of type java.lang.String to required type java.lang.Integer for property price; nested exception is java.lang.NumberFormatException: For input string: "文字列"
book.nameの値は 
book.priceの値は 
//}

c011Model.priceには文字列のためバインディングができずnullが入っています。また、バインディング失敗のデフォルトのエラーメッセージが表示されます。

このままだと、非常にわかりにくいのでデフォルトのエラーメッセージを変更します。

まず、spring-context.xmlにメッセージソースの設定を追加します。

//list[011-spring-context.xml][spring-context.xml]{
<bean id="messageSource"
 class="org.springframework.context.support.ReloadableResourceBundleMessageSource">
 <property name="basename" value="classpath:/messages" />
</bean>
//}

そして、指定した名前+「.properties」ファイルを作成します。今回は型変換のエラーのメッセージだけを記載します。

//list[011-messages.properties][messages.properties]{
typeMismatch.java.lang.Integer=整数で入力してください。
//}

このメッセージは、@<href>{http://docs.spring.io/spring/docs/current/javadoc-api/org/springframework/validation/DefaultMessageCodesResolver.html, DefaultMessageCodesResolver}@<fn>{type_check-DefaultMessageCodesResolver}のJavaDocに記載のある通り、
//footnote[type_check-DefaultMessageCodesResolver][http://docs.spring.io/spring/docs/current/javadoc-api/org/springframework/validation@<br>{}/DefaultMessageCodesResolver.html]

 1. code + "." + object name + "." + field
 2. code + "." + field
 3. code + "." + field type
 4. code

の順でメッセージが解決されます。

この状態で先ほどと同じデータを送信すると上記のメッセージになります。

ここで、メッセージを以下のように変更します。

//list[011-message.properties2][message.properties]{
typeMismatch.java.lang.Integer={0}は整数で入力してください。
//}

ここで、また先程のリクエストを送信すると今回は{0}の部分がフィールド名になり、

//cmd{
priceは整数で入力してください。
//}

というメッセージにあります。このpriceの部分はフィールド名に対応していて、この名前も置換が可能です。

メッセージソースに、フィールド名の値をキーとして登録します。

//list[011-message.properties3][message.propeties]{
price=価格
//}

そうすることで、最終的には以下のメッセージになります。

//cmd{
価格は整数で入力してください。
//}

確認用のテストケースは次のとおりです。

//list[011-C011ControllerTest.java][C011ControllerTest.java]{
package com.example.spring.controller.c011;

import static org.hamcrest.CoreMatchers.*;
import static org.junit.Assert.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.*;

import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.support.DefaultMessageSourceResolvable;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.servlet.ModelAndView;

@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations = {
    "file:src/main/webapp/WEB-INF/spring/spring-context.xml" })
public class C011ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void checkTypeへのGET_priceが1000() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(get("/c011/checkType").param("price", "1000"))
                .andExpect(status().isOk())
                .andExpect(view().name("c011/checkType"))
                .andExpect(model().hasNoErrors())
                .andExpect(model().attributeExists("c011Model")).andReturn();

        Map<String, Object> model = mvcResult.getModelAndView().getModel();
        Object c011ModelObject = model.get("c011Model");
        assertThat(c011ModelObject, is(notNullValue()));
        assertThat(c011ModelObject, is(instanceOf(C011Model.class)));
        C011Model c011Model = (C011Model) c011ModelObject;
        assertThat(c011Model.getPrice(), is(1000));
    }

    @Test
    public void checkTypeへのGET_priceがabc() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(get("/c011/checkType").param("price", "abc"))
                .andExpect(status().isOk())
                .andExpect(view().name("c011/checkType"))
                .andExpect(model().hasErrors())
                .andExpect(model().errorCount(1))
                .andExpect(
                        model().attributeHasFieldErrors("c011Model", "price"))
                .andExpect(model().attributeExists("c011Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c011ModelObject = model.get("c011Model");
        assertThat(c011ModelObject, is(notNullValue()));
        assertThat(c011ModelObject, is(instanceOf(C011Model.class)));
        C011Model c011Model = (C011Model) c011ModelObject;
        assertThat(c011Model.getPrice(), is(nullValue()));

        // エラーメッセージのチェック
        Object object = mav.getModel().get(
                "org.springframework.validation.BindingResult.c011Model");
        assertThat(object, is(not(nullValue())));
        assertThat(object, is(instanceOf(BindingResult.class)));
        BindingResult bindingResult = (BindingResult) object;

        // エラーのあるフィールドの取得
        List<FieldError> list = bindingResult.getFieldErrors("price");
        assertThat(list, is(not(nullValue())));
        assertThat(list.size(), is(1));

        // 詳細なエラーチェック
        FieldError fieldError = list.get(0);
        assertThat(fieldError.getCode(), is("typeMismatch"));

        // エラーメッセージのパラメータ
        Object[] args = fieldError.getArguments();
        assertThat(args.length, is(1));
        assertThat(args[0],
                is(instanceOf(DefaultMessageSourceResolvable.class)));
        DefaultMessageSourceResolvable dmr = (DefaultMessageSourceResolvable) args[0];
        assertThat(dmr.getCode(), is("price"));
    }
}
//}

Validationでエラーの場合には、エラーメッセージがModelに登録されます。まず、エラーがあるかどうかを@<code>{hasErros()}メソッドで確認できます。また、エラーの総数をerrorCountで確認でき、Modelのどのフィールドに対してエラーがあるのかをattributeHasFieldErrorsメソッドで確認できます。

//emlist{
MvcResult mvcResult = mockMvc
        .perform(get("/c011/checkType").param("price", "abc"))
        .andExpect(status().isOk())
        .andExpect(view().name("c011/checkType"))
        .andExpect(model().hasErrors())
        .andExpect(model().errorCount(1))
        .andExpect(
                model().attributeHasFieldErrors("c011Model", "price"))
        .andExpect(model().attributeExists("c011Model")).andReturn();
//}

Validationで発生したエラーメッセージは、以下のルールでModelに格納されます。

//emlist{
"org.springframework.validation.BindingResult" + Bean Validationのモデルの名称
//}

ここでは、bookモデルが対象のため、次のようになります。

//emlist{
"org.springframework.validation.BindingResult.c011Model"
//}

Modelには、この属性名でBindingResultのインスタンスが格納されています。そのため、この属性は以下のように取り出し、確認することができます。

//emlist{
Object object = mav.getModel().get(
        "org.springframework.validation.BindingResult.c011Model");
assertThat(object, is(not(nullValue())));
assertThat(object, is(instanceOf(BindingResult.class)));
BindingResult bindingResult = (BindingResult) object;
//}

BindingResultからはエラーメッセージのコードを取得することができます。BindingResultからフィールドごとのエラーは、getFieldErrorsメソッドで取り出せます。

//emlist{
List<FieldError> list = bindingResult.getFieldErrors("price");
assertThat(list, is(not(nullValue())));
assertThat(list.size(), is(1));
//}

getFieldErrorsはFieldErrorクラスのListを取得できます。そこから、FieldErrorを取得し、メッセージのコードを確認します。

//emlist{
FieldError fieldError = list.get(0);
assertThat(fieldError.getCode(), is("typeMismatch"));

Object[] args = fieldError.getArguments();
assertThat(args.length, is(1));
assertThat(args[0],
        is(instanceOf(DefaultMessageSourceResolvable.class)));
DefaultMessageSourceResolvable dmr = (DefaultMessageSourceResolvable) args[0];
assertThat(dmr.getCode(), is("price"));
//}

== JSR349によるvalidation

==={012} Validatorでnullチェック

@<b>{タグ【012】}

Springは標準でBean Validationをサポートしており、Spring 4.0からはJSR-349のBean Validation 1.1をサポートしています。ここでははSpringでBean Validationを使うための設定と、nullチェックのサンプルを紹介します。

まず、MavenでBean Validation関連のライブラリーを追加します。

//list[012-pom.xml][pom.xml]{
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

//list[012-spring-context.xml][spring-context.xml]{
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
 <mvc:annotation-driven validator="validator" />
 <context:component-scan base-package="com.example.spring">
  <context:exclude-filter type="regex"
   expression="com\.example\.spring\.controller\..*Test" />
 </context:component-scan>
 <bean
  class="org.springframework.web.servlet.view.InternalResourceViewResolver">
  <property name="prefix" value="/WEB-INF/jsp/" />
  <property name="suffix" value=".jsp" />
 </bean>
 <bean id="multipartResolver"
  class="org.springframework.web.multipart.support.StandardServletMultipartResolver">
 </bean>
 <bean class="com.example.spring.controller.c026.GlobalExceptionResolver" />
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

//list[012-messages.properties1][messages.properties]{
javax.validation.constraints.AssertFalse.message = must be false
javax.validation.constraints.AssertTrue.message = must be true
javax.validation.constraints.DecimalMax.message =
   must be less than ${inclusive == true ? 'or equal to ' : ''}{value}
javax.validation.constraints.DecimalMin.message =
   must be greater than ${inclusive == true ? 'or equal to ' : ''}{value}
javax.validation.constraints.Digits.message =
   numeric value out of bounds (<{integer} digits>.<{fraction} digits> expected)
javax.validation.constraints.Future.message = must be in the future
javax.validation.constraints.Max.message = must be less than or equal to {value}
javax.validation.constraints.Min.message = must be greater than or equal to {value}
javax.validation.constraints.NotNull.message = may not be null
javax.validation.constraints.Null.message = must be null
javax.validation.constraints.Past.message = must be in the past
javax.validation.constraints.Pattern.message = must match "{regexp}"
javax.validation.constraints.Size.message = size must be between {min} and {max}

org.hibernate.validator.constraints.CreditCardNumber.message =
   invalid credit card number
org.hibernate.validator.constraints.EAN.message = invalid {type} barcode
org.hibernate.validator.constraints.Email.message = not a well-formed email address
org.hibernate.validator.constraints.Length.message =
   length must be between {min} and {max}
org.hibernate.validator.constraints.LuhnCheck.message =
   The check digit for ${value} is invalid, Luhn Modulo 10 checksum failed
org.hibernate.validator.constraints.Mod10Check.message =
   The check digit for ${value} is invalid, Modulo 10 checksum failed
org.hibernate.validator.constraints.Mod11Check.message =
   The check digit for ${value} is invalid, Modulo 11 checksum failed
org.hibernate.validator.constraints.ModCheck.message =
   The check digit for ${value} is invalid, ${modType} checksum failed
org.hibernate.validator.constraints.NotBlank.message = may not be empty
org.hibernate.validator.constraints.NotEmpty.message = may not be empty
org.hibernate.validator.constraints.ParametersScriptAssert.message =
   script expression "{script}" didn't evaluate to true
org.hibernate.validator.constraints.Range.message = must be between {min} and {max}
org.hibernate.validator.constraints.SafeHtml.message = may have unsafe html content
org.hibernate.validator.constraints.ScriptAssert.message =
   script expression "{script}" didn't evaluate to true
org.hibernate.validator.constraints.URL.message = must be a valid URL

org.hibernate.validator.constraints.br.CNPJ.message =
   invalid Brazilian corporate taxpayer registry number (CNPJ)
org.hibernate.validator.constraints.br.CPF.message =
   invalid Brazilian individual taxpayer registry number (CPF)
org.hibernate.validator.constraints.br.TituloEleitoral.message =
   invalid Brazilian Voter ID card number
//}

このままだと英語だけなので、必要な物から日本語にしていきます。日本語のメッセージリソースは、messages_ja.propertiesというファイル名にして以下の内容にしておきます。

//list[012-messages.properties2][messages.properties]{
javax.validation.constraints.NotNull.message = 入力は必須です

typeMismatch.java.lang.Integer={0}は整数で入力してください。
price=価格
//}

次にvalidatorを動作させるためのControllerを作成します。

//list[012-C012Controller.java][C012Controller.java]{
package com.example.spring.controller.c012;

import javax.validation.Valid;

import org.springframework.stereotype.Controller;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c012")
public class C012Controller {
    @RequestMapping("/bookForm")
    public String bookForm() {
        return "c012/bookForm";
    }

    @RequestMapping(value = "/bookRecv", method = RequestMethod.POST)
    public String bookRecv(@Valid @ModelAttribute C012Model c012Model,
            BindingResult errors) {
        if (errors.hasErrors()) {
            return "c012/bookForm";
        }
        return "c012/bookRecv";
    }
}
//}

コントローラで重要なのは、データを受信するbookRecvメソッドになります。bookRecvメソッドではデータを受け取るBookクラスに@Validアノテーションがついています。@Validアノテーションを付けることでBean Validationが動作するようになります。また、Bean Validationの検証結果は、次の引数のBindingResultの中に格納されます。

メソッドの中で、BindingResult#hasErrorsメソッドでエラーが有るか確認し、エラーがある場合には入力画面に戻るようにしています。

次に、C012Modelクラスの定義を確認します。

//list[012-C012Model.java][C012.java]{
package com.example.spring.controller.c012;

import javax.validation.constraints.NotNull;

public class C012Model {
    @NotNull
    private String name;
    @NotNull
    private Integer price;

    // setter、getterは省略
}
//}

C012Modelクラスの各フィールドには@NotNullアノテーションを付けています。このアノテーションを付けることで@Validの付いたコントローラで自動的にValidationが行われます。@NotNullアノテーションをつけているとフィールドがnullの場合に検証エラーとなります。

次にJSPを確認していきます。まずデータ送信をするためのフォームの画面（bookForm.jsp）です。

//list[012-bookForm.jsp][bookForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="bookRecv" method="post">
   書名: <input type="text" name="name" size="20">
         <form:errors path="c012Model.name" /><br>
   価格: <input type="text" name="price" size="20">
         <form:errors path="c012Model.price" /><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

入力画面では、入力フォームの後ろにエラー表示のカスタムタグをつけています。pathにはモデルオブジェクト（book）のフィールド名を指定します。こうすることで、そのフィールドで発生したエラーがカスタムタグの場所に表示されます。

入力された結果の確認用のJSP（bookRecv.jsp）です。

//list[012-bookRecv.jsp][bookRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
c012Model.nameの値は <c:out value="${c012Model.name}" /><br>
c012Model.priceの値は <c:out value="${c012Model.price}" /><br>
 </body>
</html>
//}

ここまでで実際にエラーを検証できる環境が整いましたので、実際にエラーを発生させてみます。フォームに何も入力せずに送信ボタンを押すと価格のみエラーとなります。これは、受け取るフィールドの型の違いのために起こります。書名はString型のためフォームから送られてくる空文字列（""）を受け取るためnullにはなりません。対してpriceは空文字列はInteger型のため空文字列は受け取れずnullになります。そのため、priceのみ@NotNullでエラーとなります。

空文字のチェックのためにはBean Validation標準のアノテーションではなく、HibernateのValiationを使うとできます。この辺りは後述します。

最後に確認用のテストケースです。

//list[012-C012ControllerTest.java][C012ControllerTest.java]{
package com.example.spring.controller.c012;

import static org.hamcrest.CoreMatchers.*;
import static org.junit.Assert.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.*;

import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.support.DefaultMessageSourceResolvable;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.servlet.ModelAndView;

@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations = {
    "file:src/main/webapp/WEB-INF/spring/spring-context.xml" })
public class C012ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void bookFormへのGET() throws Exception {
        mockMvc.perform(get("/c012/bookForm")).andExpect(status().isOk())
                .andExpect(view().name("c012/bookForm"))
                .andExpect(model().hasNoErrors());
    }

    @Test
    public void bookRecvへのPOST_NOT_NULL() throws Exception {
        mockMvc.perform(
                post("/c012/bookRecv").param("name", "よくわかるSpring").param(
                        "price", "1000")).andExpect(status().isOk())
                .andExpect(view().name("c012/bookRecv"))
                .andExpect(model().hasNoErrors());
    }

    @Test
    public void bookRecvへのPOST_nameがnull() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(post("/c012/bookRecv").param("price", "1000"))
                .andExpect(status().isOk())
                .andExpect(view().name("c012/bookForm"))
                .andExpect(model().hasErrors())
                .andExpect(model().errorCount(1))
                .andExpect(model().attributeHasFieldErrors("c012Model", "name"))
                .andExpect(model().attributeExists("c012Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c012ModelObject = model.get("c012Model");
        assertThat(c012ModelObject, is(notNullValue()));
        assertThat(c012ModelObject, is(instanceOf(C012Model.class)));
        C012Model c012Model = (C012Model) c012ModelObject;
        assertThat(c012Model.getName(), is(nullValue()));

        // エラーメッセージのチェック
        Object object = mav.getModel().get(
                "org.springframework.validation.BindingResult.c012Model");
        assertThat(object, is(not(nullValue())));
        assertThat(object, is(instanceOf(BindingResult.class)));
        BindingResult bindingResult = (BindingResult) object;

        checkField(bindingResult, "name", "NotNull");
    }

    @Test
    public void bookRecvへのPOST_nameとpriceがnull() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(post("/c012/bookRecv"))
                .andExpect(status().isOk())
                .andExpect(view().name("c012/bookForm"))
                .andExpect(model().hasErrors())
                .andExpect(model().errorCount(2))
                .andExpect(model().attributeHasFieldErrors("c012Model", "name"))
                .andExpect(
                        model().attributeHasFieldErrors("c012Model", "price"))
                .andExpect(model().attributeExists("c012Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c012ModelObject = model.get("c012Model");
        assertThat(c012ModelObject, is(notNullValue()));
        assertThat(c012ModelObject, is(instanceOf(C012Model.class)));
        C012Model c012Model = (C012Model) c012ModelObject;
        assertThat(c012Model.getName(), is(nullValue()));

        // エラーメッセージのチェック
        Object object = mav.getModel().get(
                "org.springframework.validation.BindingResult.c012Model");
        assertThat(object, is(not(nullValue())));
        assertThat(object, is(instanceOf(BindingResult.class)));
        BindingResult bindingResult = (BindingResult) object;

        checkField(bindingResult, "name", "NotNull");
        checkField(bindingResult, "price", "NotNull");
    }

    private void checkField(BindingResult bindingResult, String fieldName,
            String errorCode) {
        // エラーのあるフィールドの取得
        List<FieldError> list = bindingResult.getFieldErrors(fieldName);
        assertThat(list, is(not(nullValue())));
        assertThat(list.size(), is(1));

        // 詳細なエラーチェック
        FieldError fieldError = list.get(0);
        assertThat(fieldError.getCode(), is(errorCode));

        // エラーメッセージのパラメータ
        Object[] args = fieldError.getArguments();
        assertThat(args.length, is(1));
        assertThat(args[0],
                is(instanceOf(DefaultMessageSourceResolvable.class)));
        DefaultMessageSourceResolvable dmr = (DefaultMessageSourceResolvable) args[0];
        assertThat(dmr.getCode(), is(fieldName));
    }

}
//}

==={013} ValidatorでDecimalのチェック

@<b>{タグ【013】}

ここからしばらくBean Validationの標準のValidatorの説明をしていきます。最初はDecimalMax、DecimalMinの2つです。

DecimalMaxとDecimalMinはその名の通り、数値の最大と最小をチェックします。また、inclusive属性をtrue/falseにすることで、値自体を含む、含まないかを選択できます。

最初にvalidatorを動作させるためのControllerとJSPを作成します。ControllerとJSPは先の例と同様のため、説明は省略します。

//list[013-C013Controller.java][C013Controller.java]{
package com.example.spring.controller.c013;

import javax.validation.Valid;

import org.springframework.stereotype.Controller;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c013")
public class C013Controller {
    @RequestMapping("/bookForm")
    public String bookForm() {
        return "c013/bookForm";
    }

    @RequestMapping(value = "/bookRecv", method = RequestMethod.POST)
    public String bookRecv(@Valid @ModelAttribute C013Model c013Model,
            BindingResult errors) {
        if (errors.hasErrors()) {
            return "c013/bookForm";
        }
        return "c013/bookRecv";
    }
}
//}

//list[013-bookForm.jsp][bookForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="bookRecv" method="post">
   書名: <input type="text" name="name" size="20"><form:errors path="c013Model.name" /><br>
   価格: <input type="text" name="price" size="20"><form:errors path="c013Model.price" /><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

//list[013-bookRecv.jsp][bookRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
c013Model.nameの値は <c:out value="${c013Model.name}" /><br>
c013Model.priceの値は <c:out value="${c013Model.price}" /><br>
 </body>
</html>
//}

C013Model1のpriceフィールドにValidationを設定します。以下の例だと1〜100000未満だけ入力が許されます。

//list[013-C013Model.java][C013Model.java]{
package com.example.spring.controller.c013;

import javax.validation.constraints.DecimalMax;
import javax.validation.constraints.DecimalMin;
import javax.validation.constraints.NotNull;

public class C013Model {
    @NotNull
    private String name;
    @NotNull
    @DecimalMin("1")
    @DecimalMax(value = "100000", inclusive = false)
    private Integer price;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getPrice() {
        return price;
    }

    public void setPrice(Integer price) {
        this.price = price;
    }
}
//}

DecimalMaxとDecimalMinのメッセージを用意します。

//list[013-messages.properties][messages.properties]{
javax.validation.constraints.DecimalMax.message = {0}は{value}${inclusive == true ? '以下の' : 'より小さい'}数を入力してください
javax.validation.constraints.DecimalMin.message = {0}は{value}${inclusive == true ? '以上の' : 'より大きい'}数を入力してください
//}

メッセージでは、{value}でValidationする値をメッセージに埋め込むことができます。また、EL 3.0による処理でinclusiveの値によってメッセージを変えています。ELが使えることによって、かなり柔軟なメッセージ表示が可能になっています。

テストの実行のために、pom.xmlに以下を追加する必要があります。

//list[013-pom.xml][pom.xml]{
<dependency>
 <groupId>org.glassfish.web</groupId>
 <artifactId>el-impl</artifactId>
 <version>2.2</version>
 <scope>test</scope>
</dependency>
//}

テストケースは以下のとおり。

//list[013-C013ControllerTest.java][C013ControllerControllerTest.java]{
package com.example.spring.controller.c013;

import static org.hamcrest.CoreMatchers.*;
import static org.junit.Assert.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.*;

import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.support.DefaultMessageSourceResolvable;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.servlet.ModelAndView;

@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations = { "file:src/main/webapp/WEB-INF/spring/spring-context.xml" })
public class C013ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void bookRecvへのPOST_priceが1() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(
                        post("/c013/bookRecv").param("name", "よく分かるSpring")
                                .param("price", "1"))
                .andExpect(status().isOk())
                .andExpect(view().name("c013/bookRecv"))
                .andExpect(model().hasNoErrors())
                .andExpect(model().errorCount(0))
                .andExpect(model().attributeExists("c013Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c013ModelObject = model.get("c013Model");
        assertThat(c013ModelObject, is(notNullValue()));
        assertThat(c013ModelObject, is(instanceOf(C013Model.class)));
        C013Model c013Model = (C013Model) c013ModelObject;
        assertThat(c013Model.getName(), is("よく分かるSpring"));
        assertThat(c013Model.getPrice(), is(1));
    }

    @Test
    public void bookRecvへのPOST_priceが0() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(
                        post("/c013/bookRecv").param("name", "よく分かるSpring")
                                .param("price", "0"))
                .andExpect(status().isOk())
                .andExpect(view().name("c013/bookForm"))
                .andExpect(model().hasErrors())
                .andExpect(model().errorCount(1))
                .andExpect(
                        model().attributeHasFieldErrors("c013Model", "price"))
                .andExpect(model().attributeExists("c013Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c013ModelObject = model.get("c013Model");
        assertThat(c013ModelObject, is(notNullValue()));
        assertThat(c013ModelObject, is(instanceOf(C013Model.class)));
        C013Model c013Model = (C013Model) c013ModelObject;
        assertThat(c013Model.getName(), is("よく分かるSpring"));
        assertThat(c013Model.getPrice(), is(0));

        // エラーメッセージのチェック
        Object object = mav.getModel().get(
                "org.springframework.validation.BindingResult.c013Model");
        assertThat(object, is(not(nullValue())));
        assertThat(object, is(instanceOf(BindingResult.class)));
        BindingResult bindingResult = (BindingResult) object;

        checkDecimalField(bindingResult, "price", "DecimalMin", true, "1");
    }

    @Test
    public void bookRecvへのPOST_priceが100000() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(
                        post("/c013/bookRecv").param("name", "よく分かるSpring")
                                .param("price", "100000"))
                .andExpect(status().isOk())
                .andExpect(view().name("c013/bookForm"))
                .andExpect(model().hasErrors())
                .andExpect(model().errorCount(1))
                .andExpect(
                        model().attributeHasFieldErrors("c013Model", "price"))
                .andExpect(model().attributeExists("c013Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c013ModelObject = model.get("c013Model");
        assertThat(c013ModelObject, is(notNullValue()));
        assertThat(c013ModelObject, is(instanceOf(C013Model.class)));
        C013Model c013Model = (C013Model) c013ModelObject;
        assertThat(c013Model.getName(), is("よく分かるSpring"));
        assertThat(c013Model.getPrice(), is(100000));

        // エラーメッセージのチェック
        Object object = mav.getModel().get(
                "org.springframework.validation.BindingResult.c013Model");
        assertThat(object, is(not(nullValue())));
        assertThat(object, is(instanceOf(BindingResult.class)));
        BindingResult bindingResult = (BindingResult) object;

        checkDecimalField(bindingResult, "price", "DecimalMax", false, "100000");
    }

    private void checkDecimalField(BindingResult bindingResult,
            String fieldName, String errorCode, boolean inclusive, String value) {
        // エラーのあるフィールドの取得
        List<FieldError> list = bindingResult.getFieldErrors(fieldName);
        assertThat(list, is(not(nullValue())));
        assertThat(list.size(), is(1));

        // 詳細なエラーチェック
        FieldError fieldError = list.get(0);
        assertThat(fieldError.getCode(), is(errorCode));

        // エラーメッセージのパラメータ
        Object[] args = fieldError.getArguments();
        assertThat(args.length, is(3));
        assertThat(args[0],
                is(instanceOf(DefaultMessageSourceResolvable.class)));
        DefaultMessageSourceResolvable dmr = (DefaultMessageSourceResolvable) args[0];
        assertThat(dmr.getCode(), is(fieldName));

        assertThat(args[1], is(instanceOf(Boolean.class)));
        assertThat(args[1], is(inclusive));

        assertThat(args[2], is(instanceOf(String.class)));
        assertThat(args[2], is(value));
    }
}
//}

==={014} ValidatorでMin、Maxのチェック

@<b>{タグ【014】}

今回はBean ValidationのMin、Maxの2つです。

MinとMaxはDecimalMax、DecimalMinと違い、整数のみのチェックとなります。またその数値自身を含むチェックのみが可能です。valueも数値で指定できるので、整数のチェックの場合はこちらのほうがいいです。

最初にこれまでと同様の、ControllerとJSPを作成します。ControllerとJSPは先の例と同様のため、説明は省略します。

//list[014-C014Controller.java][C014Controller.java]{
package com.example.spring.controller.c014;

import javax.validation.Valid;

import org.springframework.stereotype.Controller;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c014")
public class C014Controller {
    @RequestMapping("/bookForm")
    public String bookForm() {
        return "c014/bookForm";
    }

    @RequestMapping(value = "/bookRecv", method = RequestMethod.POST)
    public String bookRecv(@Valid @ModelAttribute C014Model c013Model,
            BindingResult errors) {
        if (errors.hasErrors()) {
            return "c014/bookForm";
        }
        return "c014/bookRecv";
    }
}
//}

//list[014-bookForm.jsp][bookForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="bookRecv" method="post">
   書名: <input type="text" name="name" size="20"><form:errors path="c014Model.name" /><br>
   価格: <input type="text" name="price" size="20"><form:errors path="c014Model.price" /><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

//list[014-bookRecv.jsp][bookRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
c014Model.nameの値は <c:out value="${c014Model.name}" /><br>
c014Model.priceの値は <c:out value="${c014Model.price}" /><br>
 </body>
</html>
//}

C014ModelのpriceフィールドにValidationを設定します。以下の例だと1〜100000だけ入力が許されます。
//list[014-C014Model.java][C014Model.java]{
package com.example.spring.controller.c014;

import javax.validation.constraints.Max;
import javax.validation.constraints.Min;
import javax.validation.constraints.NotNull;

public class C014Model {
    @NotNull
    private String name;
    @NotNull
    @Min(1)
    @Max(100000)
    private Integer price;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getPrice() {
        return price;
    }

    public void setPrice(Integer price) {
        this.price = price;
    }
}
//}

メッセージは以下のように記述します。

//list[014-messages.properties][messages.properteis]{
javax.validation.constraints.Max.message = {0}は{value}以下の数を入力してください
javax.validation.constraints.Min.message = {0}は{value}以上の数を入力してください
//}

確認用のテストケースは次のとおりです。

//list[014-C014ControllerTest.java][C014ControllerTest.java]{
package com.example.spring.controller.c014;

import static org.hamcrest.CoreMatchers.*;
import static org.junit.Assert.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.*;

import java.util.List;
import java.util.Map;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.support.DefaultMessageSourceResolvable;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;
import org.springframework.validation.BindingResult;
import org.springframework.validation.FieldError;
import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.servlet.ModelAndView;

@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations = { "file:src/main/webapp/WEB-INF/spring/spring-context.xml" })
public class C014ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void bookRecvへのPOST_priceが1() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(
                        post("/c014/bookRecv").param("name", "よく分かるSpring")
                                .param("price", "1"))
                .andExpect(status().isOk())
                .andExpect(view().name("c014/bookRecv"))
                .andExpect(model().hasNoErrors())
                .andExpect(model().errorCount(0))
                .andExpect(model().attributeExists("c014Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c014ModelObject = model.get("c014Model");
        assertThat(c014ModelObject, is(notNullValue()));
        assertThat(c014ModelObject, is(instanceOf(C014Model.class)));
        C014Model c014Model = (C014Model) c014ModelObject;
        assertThat(c014Model.getName(), is("よく分かるSpring"));
        assertThat(c014Model.getPrice(), is(1));
    }

    @Test
    public void bookRecvへのPOST_priceが0() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(
                        post("/c014/bookRecv").param("name", "よく分かるSpring")
                                .param("price", "0"))
                .andExpect(status().isOk())
                .andExpect(view().name("c014/bookForm"))
                .andExpect(model().hasErrors())
                .andExpect(model().errorCount(1))
                .andExpect(
                        model().attributeHasFieldErrors("c014Model", "price"))
                .andExpect(model().attributeExists("c014Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c014ModelObject = model.get("c014Model");
        assertThat(c014ModelObject, is(notNullValue()));
        assertThat(c014ModelObject, is(instanceOf(C014Model.class)));
        C014Model c014Model = (C014Model) c014ModelObject;
        assertThat(c014Model.getName(), is("よく分かるSpring"));
        assertThat(c014Model.getPrice(), is(0));

        // エラーメッセージのチェック
        Object object = mav.getModel().get(
                "org.springframework.validation.BindingResult.c014Model");
        assertThat(object, is(not(nullValue())));
        assertThat(object, is(instanceOf(BindingResult.class)));
        BindingResult bindingResult = (BindingResult) object;

        checkMinMaxField(bindingResult, "price", "Min", 1L);
    }

    @Test
    public void bookRecvへのPOST_priceが100000() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(
                        post("/c014/bookRecv").param("name", "よく分かるSpring")
                                .param("price", "100001"))
                .andExpect(status().isOk())
                .andExpect(view().name("c014/bookForm"))
                .andExpect(model().hasErrors())
                .andExpect(model().errorCount(1))
                .andExpect(
                        model().attributeHasFieldErrors("c014Model", "price"))
                .andExpect(model().attributeExists("c014Model")).andReturn();

        // パラメータのチェック
        ModelAndView mav = mvcResult.getModelAndView();
        Map<String, Object> model = mav.getModel();
        Object c014ModelObject = model.get("c014Model");
        assertThat(c014ModelObject, is(notNullValue()));
        assertThat(c014ModelObject, is(instanceOf(C014Model.class)));
        C014Model c014Model = (C014Model) c014ModelObject;
        assertThat(c014Model.getName(), is("よく分かるSpring"));
        assertThat(c014Model.getPrice(), is(100001));

        // エラーメッセージのチェック
        Object object = mav.getModel().get(
                "org.springframework.validation.BindingResult.c014Model");
        assertThat(object, is(not(nullValue())));
        assertThat(object, is(instanceOf(BindingResult.class)));
        BindingResult bindingResult = (BindingResult) object;

        checkMinMaxField(bindingResult, "price", "Max", 100000L);
    }

    private void checkMinMaxField(BindingResult bindingResult,
            String fieldName, String errorCode, Long value) {
        // エラーのあるフィールドの取得
        List<FieldError> list = bindingResult.getFieldErrors(fieldName);
        assertThat(list, is(not(nullValue())));
        assertThat(list.size(), is(1));

        // 詳細なエラーチェック
        FieldError fieldError = list.get(0);
        assertThat(fieldError.getCode(), is(errorCode));

        // エラーメッセージのパラメータ
        Object[] args = fieldError.getArguments();
        assertThat(args.length, is(2));
        assertThat(args[0],
                is(instanceOf(DefaultMessageSourceResolvable.class)));
        DefaultMessageSourceResolvable dmr = (DefaultMessageSourceResolvable) args[0];
        assertThat(dmr.getCode(), is(fieldName));
        // value
        assertThat(args[1], is(instanceOf(Long.class)));
        assertThat(args[1], is(value));
    }
}
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

==={validation_logic} Validatorでロジックを介した検証

@<b>{タグ【019】}

今回はBean Validationのの複数のフィールドを対象とした検証です。

Bean Validationはその名前の通り、JavaBeansのテストの仕組みです。そのため、フィールドだけでなくgetterに対してテストをすることができます。（より詳しくは@<href>{http://yamkazu.hatenablog.com/entry/20110206/1296985545, JSR 303 Bean Validationで遊んでみるよ！}）

今回はgetter（isですが）を使ってロジックを介したテストを確認します。今までのBookクラスに定価フィールド（listPrice）を追加し、価格は定価よりも安くないといけないというテストを追加します。

//list[validation_logic-Book.java][Book.java]{
@NotBlank
private String name;
@NotNull
private Integer price;
@NotNull
private Integer listPrice;

@AssertTrue(message = "{valid.price}")
public boolean isValidPrice() {
    if (price == null || listPrice == null) {
        return true;
    }
    return listPrice >= price;
}
//}

isValidPrice()メソッドで、定価と価格のチェックをしています。いずれも@NotNullをつけているのでnullの場合には検証はOKとしています。

メッセージは置き換え文字列としているためvalid.priceを以下のように定義しています。

//list[validation_logic-messages.properties][messages.properties]{
valid.price=価格は定価より安い値段にしてください。
//}

=== Validatorのエラーの際に入力値を再表示させる

== ロジックによる入力チェック

