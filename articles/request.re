= リクエスト 

== リクエストの色々な受け取り方

==={request_get} GETパラメータを受け取る

@<b>{タグ【005】}

通常のGETパラメータを受け取るには、@<code>{@RequestParam}アノテーションを使用します。

//list[request_get-ReqController.java][ReqController.java]{
@RequestMapping(value = "/getParam")
public String getParam(@RequestParam String foo, @RequestParam String bar,
        Model model) {
    model.addAttribute("modelFoo", foo);
    model.addAttribute("modelBar", bar);
    return "req/getParam";
}

@RequestMapping(value = "/getParam2")
public String getparam2(@RequestParam("foo1") String foo,
        @RequestParam("bar1") String bar, Model model) {
    model.addAttribute("modelFoo", foo);
    model.addAttribute("modelBar", bar);
    return "req/getParam";
}

@RequestMapping("/getParam3")
public String getParam3(@RequestParam(required = false) String foo,
        @RequestParam(defaultValue = "default") String bar, Model model) {
    model.addAttribute("foo", foo);
    model.addAttribute("bar", bar);
    return "req/getParam";
}
//}

先の例だと、/getParam?foo=1&bar=2や、/getParam2?foo1=3&bar1=4といったリクエストを投げることでコントローラが動作します。getParam3はパラメータが必須ではないため、とくにパラメータがなくても呼び出すことができます。

@<code>{@RequestParam}はパラメータの名前とメソッドの引数の名前が同じものが自動で割り当てられます。パラメータの名前が違う場合には、@<code>{@RequestParam}の属性でその名前を指定します。

@<code>{@RequestParam}で指定したパラメータがない場合、例えば、/getParamというリクエストが送信された場合、必要なパラメータが無いため例外が発生します。

パラメータを必須としない場合には、required属性をfalseにするか、default属性を設定します。default属性を設定するとrequred属性は暗黙的にfalseになります。

後述する@<code>{@PathVariable}と違い、リクエストスコープに自動で割り当てられることはないため、modelを介してJSPにデータを渡しています。

表示用のgetParam.jspは以下のようになります。

//list[request_get-getParam.jsp][getParam.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
fooの値は <c:out value="${foo}" /><br>
barの値は <c:out value="${bar}" /><br>
modelFooの値は <c:out value="${modelFoo}" /><br>
modelBarの値は <c:out value="${modelBar}" />
 </body>
</html>
//}

==={request_url1} URLの一部をパラメータとして受け取る

@<b>{タグ【003】}

リクエストされるURLの一部、例えば/hoge/fooの場合fooの値をパラメータとして受け取る場合には、@<code>{@PathVariable}アノテーションを使って受け取ることができます。

//list[request_url1-ReqController.java][ReqController.java]{
@RequestMapping(value = "/pathVar/{var}", method = RequestMethod.GET)
public String pathVar(@PathVariable String var) {
    return "req/pathVar";
}

@RequestMapping(value = "/pathVar2/{var1}", method = RequestMethod.GET)
public String pathVar2(@PathVariable("var1") String var) {
    return "req/pathVar";
}
//}

パラメータはURL中に{}で指定し、その中の名称がパラメータ名となります。受け取るパラメータはメソッドの引数で@<code>{@PathVariable}アノテーションを付けて指定します。この時パラメータ名と変数名が同じ場合には何も指定は必要ありません。pathVar2メソッドのようにパラメータ名と変数名が異なる場合には、アノテーションの属性としてパラメータ名が必要になります。

表示用のJSPは以下のようになります。@<code>{@PathVariable}で指定されたパラメータは、Spring MVCによって自動的にリクエストスコープの同名の属性に割り当てられます。そのため、JSP側ではリクエストスコープから値を持ってくるため、コントローラでは何もしていません。

//list[request_url1-pathVar.jsp][pathVar.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
varの値は <c:out value="${var}" /><br>
var1の値は <c:out value="${var1}" />
 </body>
</html>
//}

確認用のテストです。

//list[request_url1-ReqControllerTest.java][ReqControllerTest.java]{
@Test
public void pathVar_12345へのGET() throws Exception {
    mockMvc.perform(get("/pathVar/12345")).andExpect(status().isOk())
            .andExpect(view().name("req/pathVar"))
            .andExpect(model().hasNoErrors())
            .andExpect(request().attribute("var", is("12345")));
}

@Test
public void pathVar2_abcdeへのGET() throws Exception {
    mockMvc.perform(get("/pathVar2/abcde")).andExpect(status().isOk())
            .andExpect(view().name("req/pathVar"))
            .andExpect(model().hasNoErrors())
            .andExpect(request().attribute("var1", is("abcde")));
}
//}

テストでは、URLのパラメータがrequestにセットされていることを確認しています。

==={request_url2} URLの一部をパラメータとして受け取る（複数）

@<b>{タグ【004】}

URLをパラメータとする場合、そのパラメータは1つでなくても構いません。複数のパラメータを受け取ることができますし、パラメータとパラメータの間にURLの一部が混ざっていても問題ありません。

具体的なサンプルは以下のとおりです。

//list[request_url2-ReqController.java][ReqController.java]{
@RequestMapping(value = "/pathVar3/{foo}/{bar}", method = RequestMethod.GET)
public String pathVar3(@PathVariable String foo, @PathVariable String bar) {
    return "req/pathVar3";
}

@RequestMapping(value = "/pathVar4/{bar1}/{foo1}", method = RequestMethod.GET)
public String pathVar4(@PathVariable("bar1") String bar,
        @PathVariable("foo1") String foo) {
    return "req/pathVar3";
}

@RequestMapping(value = "/pathVar5/{foo}/param/{bar}", method = RequestMethod.GET)
public String pathVar5(@PathVariable String foo, @PathVariable String bar) {
    return "req/pathVar3";
}
//}

2つ目のurlpara4メソッドではパラメータの順番と受け取るメソッドの引数の順番を変えています。3つ目のurlparam5メソッドでは、fooパラメータとbarパラメータの間にリテラルのパスを含んでいます。

このように、柔軟にパラメータの設定ができます。

最後に表示用のpathVar3.jspは以下になります。

//list[request_url2-pathVar3.jsp][pathVar3.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
fooの値は <c:out value="${foo}" /><br>
barの値は <c:out value="${bar}" /><br>
foo1の値は <c:out value="${foo1}" /><br>
bar1の値は <c:out value="${bar1}" />
 </body>
</html>
//}

==={request_body} リクエストボディをそのまま受け取る

@<b>{タグ【006】}

POSTリクエストのデータは、GETリクエストと同様に@<code>{@RequestParam}で受け取ることもできますが、リクエストボディの生データをそのまま受け取ることもできます。

//list[request_body-ReqController.java][ReqController.java]{
@RequestMapping("/bodyForm")
public String bodyForm() {
    return "req/bodyForm";
}

@RequestMapping(value = "/bodyRecv", method = RequestMethod.POST)
public String bodyRecv(@RequestBody String body, Model model) {
    model.addAttribute("body", body);
    return "req/bodyRecv";
}
//}

リクエストボディは、@<code>{@RequestBody}アノテーションを付けた引数で受け取ります。今回の場合だとリクエストボディがそのままStringクラスのbodyに入ります。

RequestBodyも必須かどうかはrequired属性で指定できます。

POSTデータ送信用のJSP、bodyForm.jspです。

//list[request_body-bodyForm.jsp][bodyForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="bodyRecv" method="post">
   名前: <input type="text" name="name" size="20"><br>
   年齢: <input type="text" name="age" size="20"><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

リクエストボディ表示用のbodyRecv.jspです。

//list[request_body-bodyRecv.jsp][bodyRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
nameの値は <c:out value="${name}" /><br>
ageの値は <c:out value="${age}" /><br>
bodyの値は <c:out value="${body}" /><br>
 </body>
</html>
//}

==={request_request} HttpServletRequestとそれに近いもので受け取る

@<b>{タグ【007】}

ここではHttpServletRequestとSpringで用意されている、Reqeustのようなクラスでのデータの受け取り方です。WebRequestは色々便利に使えますが、今回はHttpServletRequestと同じ動きができるというところだけ確認します。

//list[request_request-ReqController.java][ReqController.java]{
@RequestMapping("/req")
public String req(HttpServletRequest request, Model model) {
    model.addAttribute("foo", request.getParameter("foo"));
    return "req/req";
}

@RequestMapping("/req2")
public String req2(WebRequest request, Model model) {
    model.addAttribute("foo", request.getParameter("foo"));
    return "req/req";
}

@RequestMapping("/req3")
public String req3(NativeWebRequest request, Model model) {
    model.addAttribute("foo", request.getParameter("foo"));
    return "req/req";
}
//}

req.jspは以下です。

//list[request_reuqest-req.jsp][req.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
fooの値は <c:out value="${foo}" /><br>
 </body>
</html>
//}

==={request_reader} Reader/inputStreamで受け取る

@<b>{タグ【008】}

RequestBodyのデータをReaderやInputStreamで受け取ることができます。今回はより簡単なReaderで受け取っています。読み取った1行目のデータをレスポンスに返しています。

//list[request_reader-ReqController.java][ReqController.java]{
@RequestMapping("/readerForm")
public String readerForm() {
    return "req/readerForm";
}

@RequestMapping(value = "/readerRecv", method = RequestMethod.POST)
public String readerRecv(BufferedReader reader, Model model)
        throws IOException {
    model.addAttribute("body", reader.readLine());
    return "req/readerRecv";
}
//}

POSTデータ送信用のJSP、readerForm.jspです。

//list[request_reader-readerForm.jsp][readerForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="readerRecv" method="post">
   名前: <input type="text" name="name" size="20"><br>
   年齢: <input type="text" name="age" size="20"><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

リクエストボディ表示用のreaderRecv.jspです。

//list[request_reader-readerRecv.jsp][readerRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
nameの値は <c:out value="${name}" /><br>
ageの値は <c:out value="${age}" /><br>
bodyの値は <c:out value="${body}" /><br>
 </body>
</html>
//}

==={request_entity} HttpEntityで受け取る

@<b>{タグ【009】}

今回はSpringで用意されているHttpEntityクラスでリクエストを受け付けます。

//list[request_entity-ReqController.java][ReqController.java]{
@RequestMapping("/entityForm")
public String entityForm() {
    return "req/entityForm";
}

@RequestMapping(value = "/entityRecv", method = RequestMethod.POST)
public String entityRecv(HttpEntity<String> httpEntity, Model model) {
    model.addAttribute("body", httpEntity.getBody());
    return "req/entityRecv";
}
//}

POSTデータ送信用のJSP、entityForm.jspです。

//list[request_entity-entityForm.jsp][entityForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="entityRecv" method="post">
   名前: <input type="text" name="name" size="20"><br>
   年齢: <input type="text" name="age" size="20"><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

リクエストボディ表示用のentityRecv.jspです。

//list[request_entity-entityRecv.jsp][entityRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
nameの値は <c:out value="${name}" /><br>
ageの値は <c:out value="${age}" /><br>
bodyの値は <c:out value="${body}" /><br>
 </body>
</html>
//}

==={request_class} 任意のクラス（モデル）で受け取る

@<b>{タグ【010】}

今回は、リクエストパラメータを任意のオブジェクトで受け取ります。

最初にデータを受け取るクラスを作成します。フィールド名は受け取るパラメータ名と同じにしておきます。

//list[request_class-Customer.java][Customer.java]{
package com.example.spring.controller;

public class Customer {
    private String name;
    private String age;

    // setter、getterは省略
}
//}

コントローラは、@<code>{@ModelAttribute}アノテーションを付けたクラスに、自動的に同名のフィールドにマッピングされます。また、@<code>{@PathVariable}アノテーションと同様に@<code>{@ModelAttribute}アノテーションを付けたインスタンスは、自動的にリクエストスコープに設定されます。@<code>{@PathVariable}と違うのは、オブジェクトそのものがリクエストスコープに設定される点です。

//list[request_class-ReqController.java][ReqController.java]{
@RequestMapping("/modelForm")
public String modelForm() {
    return "req/modelForm";
}

@RequestMapping(value = "/modelRecv", method = RequestMethod.POST)
public String modelRecv(@ModelAttribute Customer customer) {
    return "req/modelRecv";
}
//}

データを送信する、modelForm.jspです。

//list[request_class-modelForm.jsp][modelForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="modelRecv" method="post">
   名前: <input type="text" name="name" size="20"><br>
   年齢: <input type="text" name="age" size="20"><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

データを受信する、modelRecv.jspです。

//list[request_class-modelRecv.jsp][modelRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
nameの値は <c:out value="${name}" /><br>
ageの値は <c:out value="${age}" /><br>
customer.nameの値は <c:out value="${customer.name}" /><br>
customer.ageの値は <c:out value="${customer.age}" /><br>
 </body>
</html>
//}

== ファイルのアップロード

==={request_upload} Servlet 3.0によるファイルのアップロード

@<b>{タグ【025】}

Java EE 6、Servlet 3.0から標準でファイルのアップロードができるようになりました。ここではSpringで標準のファイルアップロードを行います。

ファイルアップロードする場合には、web.xmlにアップロードの設定が必要です（もしくはServletのアノテーション）。web.xmlのDispatcherServletの設定を以下のようにします。

//list[request_upload-web.xml][web.xml]{
<servlet>
 <servlet-name>dispatcher</servlet-name>
 <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
 <init-param>
  <param-name>contextConfigLocation</param-name>
  <param-value>/WEB-INF/spring/spring-context.xml</param-value>
 </init-param>
 <load-on-startup>1</load-on-startup>
 <multipart-config>
  <location>/tmp</location>
  <max-file-size>1000000</max-file-size>
  <max-request-size>1000000</max-request-size>
  <file-size-threshold>10000</file-size-threshold>
 </multipart-config>
</servlet>
//}

また、Springの設定も必要になります。

//list[request_upload-spring-context.xml][spring-context.xml]{
<bean id="multipartResolver"
 class="org.springframework.web.multipart.support.StandardServletMultipartResolver">
</bean>
//}

Controllerは以下のようにします。受け取る際には、@<code>{@RequestParam}アノテーションを付けた引数で受け取ります。

//list[request_upload-ReqController.java][ReqController.java]{
@RequestMapping("uploadForm")
public String uploadForm() {
    return "req/uploadForm";
}

@RequestMapping(value = "/uploadRecv", method = RequestMethod.POST)
public String uploadRecv(@RequestParam String test,
        @RequestParam MultipartFile file, Model model) {
    model.addAttribute("test", test);
    model.addAttribute("fileName", file.getOriginalFilename());
    return "req/uploadRecv";
}
//}

今回はファイルは保管せず、testパラメータとファイル名をmodelに格納します。

ファイルを送信するuploadForm.jspです。

//list[request_upload-uploadForm.jsp][uploadForm.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
  <form action="uploadRecv" method="post" enctype="multipart/form-data">
   <input type="text" name="test"><br>
   <input type="file" name="file"><br>
   <input type="submit" value="送信">
  </form>
 </body>
</html>
//}

結果表示のuploadRecv.jspです。日本語ファイル名も問題なく表示できます。

//list[request_upload-uploadRecv.jsp][uploadRecv.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
アップロードされました。<br>
ファイル名は<c:out value="${fileName}" /><br>
送信されたtestパラメータは<c:out value="${test}" />
 </body>
</html>
//}

==={request_upload_exception} ファイルのアップロードの例外処理

ファイルアップロードでどれだけ大きなファイルをアップロードできるわけではなく、ある程度のサイズで制限させないとサーバーのリソースを食いつぶしてしまいます。

前回設定したように、web.xmlでファイルのサイズは制限できます。

//list[request_upload_exception-web.xml][web.xml]{
<multipart-config>
 <location>/tmp</location>
 <max-file-size>1000000</max-file-size>
 <max-request-size>1000000</max-request-size>
 <file-size-threshold>10000</file-size-threshold>
</multipart-config>
//}

このサイズを超えた時、SpringではMultipartExceptionが発生します。この例外はController側では補足できず、フレームワーク全体の例外処理で、キャッチする必要があります。

//list[request_upload_exception-GlobalExceptionResolver.java][GlobalExceptionResolver.java]{
package com.example.spring;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.web.servlet.HandlerExceptionResolver;
import org.springframework.web.servlet.ModelAndView;

public class GlobalExceptionResolver implements HandlerExceptionResolver {
    @Override
    public ModelAndView resolveException(HttpServletRequest request,
            HttpServletResponse response, Object handler, Exception e) {
        System.out.println(e);
        ModelAndView mav = new ModelAndView();
        mav.addObject("errorMessage", "ファイルサイズが大きすぎます。");
        mav.setViewName("req/uploadForm");
        return mav;
    }
}
//}

//list[request_upload_exception-spring-context.xml][spring-context.xml]{
<bean class="com.example.spring.GlobalExceptionResolver"/>
//}

== いろいろなスコープ

==={scope_request1} リクエストスコープにデータを格納

@<b>{タグ【027】}

リクエストスコープにデータを格納する方法を紹介します。Springではいくつかの方法でリクエストスコープにデータを格納できます。ひとつは通常のサーブレットと同様にHttpServletRequestを使う方法。2つ目がWebRequestを使う方法。3つ目がModelを使う方法になります。

//list[scope_request1-ScopeController.java][ScopeController.java]{
@RequestMapping("/requestScope1")
public String requestScope1(HttpServletRequest request,
        WebRequest webRequest, Model model) {
    request.setAttribute("req1", "httpServletRequest");
    webRequest.setAttribute("req2", "webRequest", WebRequest.SCOPE_REQUEST);
    model.addAttribute("req3", "model");
    return "scope/requestScope1";
}
//}

WebRequestを使用する場合には、格納するスコープを3番目の引数に指定します。それ以外はHttpServletRequestと同様です。

Modelオブジェクトを利用する場合は、メソッド名が違うだけでHttpServletRequestと同様です。

使用するJSPでは、明示してrequestScopeから値を取得しています。

//list[scope_reuqest1-requestScope1.jsp][requestScope1.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
HttpServletRequest: <c:out value="${requestScope.req1}" /><br>
WebRequest: <c:out value="${requestScope.req2}" /><br>
Model: <c:out value="${requestScope.req3}" />
 </body>
</html>
//}

=== Beanをリクエストスコープに格納する

==={scope_session1} セッションスコープにデータを格納

@<b>{タグ【029】}

セッションにデータを格納する方法もリクエスト同様に複数あります。Servlet APIのHttpSessionを使用する方法、WebRequestを使用する方法などです。

WebRequestはリクエストスコープと同様にデータを格納する際にスコープを指定します。

今回は同一の画面に遷移する3つのメソッドを用意しています。最初のものはセッションにデータを格納するもの、2番目は何もしないもの、3番目はセッションをクリアするものになります。

//list[scope_session1-ScopeController.java][ScopeController.java]{
@RequestMapping("/sessionScope1")
public String sessionScope1(HttpSession session, WebRequest webRequest) {
    session.setAttribute("session1", "httpSession");
    webRequest.setAttribute("session2", "webRequest",
            WebRequest.SCOPE_SESSION);

    return "scope/sessionScope1";
}

@RequestMapping("/sessionScope2")
public String sessionScope2() {
    return "scope/sessionScope1";
}

@RequestMapping("/sessionScope3")
public String sessionScope3(HttpSession session) {
    session.invalidate();
    return "scope/sessionScope1";
}
//}

表示用のsessionScop1.jspです。

//list[scope_session1-sessionScope1.jsp][sessionScope1.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
HttpSession: <c:out value="${sessionScope.session1}" /><br>
WebRequest: <c:out value="${sessionScope.session2}" /><br>
<a href="sessionScope2">セッションをクリアせず再表示</a><br>
<a href="sessionScope3">セッションをクリアして再表示</a>
 </body>
</html>
//}

==={scope_session2} Beanをセッションスコープに格納する

BeanをSessionに登録するためには、web.xmlにRequestContextListenerの設定が必要になります。

//list[scope_session2-web.xml][web.xml]{
<listener>
 <listener-class>
  org.springframework.web.context.request.RequestContextListener
 </listener-class>
</listener>
//}

セッションに格納するBeanは以下のように定義します。

//list[scope_session2-SessionBook.java][SessionBook.java]{
@Component
@Scope(value = WebApplicationContext.SCOPE_SESSION,
     proxyMode = ScopedProxyMode.TARGET_CLASS)
public class SessionBook implements Serializable {
    private String name;
    private Integer price;
    // getter、setterは省略
}
//}

ポイントは@Scopeでsessionを指定することと、proxyModeにTARGET_CLASSを指定することです。

特にproxyModeは重要で、これを設定しないと動作しません。

@<href>{http://d.hatena.ne.jp/minokuba/20110209/1297262226}

ついで、Controllerのコードです。セッションに格納されていることがわかるようにセッションに格納後リダイレクトでページを表示します。

//list[scope_session2-ScopeController.java][ScopeController.java]{
@Autowired
private SessionBook sessionBook;

@RequestMapping("/sessionStart")
public String sessionStart() {
    sessionBook.setName("よくわかるHttpSession");
    sessionBook.setPrice(980);
    return "redirect:/sessionScope4";
}

@RequestMapping("/sessionScope4")
public String sessionScope4(Model model) {
    model.addAttribute("modelSessionBook", sessionBook);
    return "scope/sessionScope4";
}
//}

Beanは通常通り@Autowiredすることで自動的にSessionに格納されます。

//list[scope_session2-sessionScope4.jsp][sessionScope4.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
 <c:out value="${requestScope.sessionBook.name}" />
sessionScope.書名: <c:out value="${sessionScope.sessionBook.name}" /><br>
sessionScope.価格: <c:out value="${sessionScope.sessionBook.price}" /><br>
modelSessionBook.書名: <c:out value="${modelSessionBook.name}" /><br>
modelSessionBook.価格: <c:out value="${modelSessionBook.price}" /><br>
scopedSession.sessionBook.書名: <c:out value="${model.sessionBook.name}" /><br>
scopedSession.sessionBook.価格: <c:out value="${model.sessionBook.price}" /><br>
 </body>
</html>
//}

==={scope_flash} Flashスコープにデータを格納

Springではリダイレクト時にリダイレクト先で参照できるRedirectAttributeを使用できます。

//list[scope_flash-ScopeController.java][ScopeController.java]{
@RequestMapping("/flashScope1")
public String flashScope1(RedirectAttributes attrs, Model model) {
    attrs.addFlashAttribute("flash1", "flash1");
    model.addAttribute("request1", "request1");

    return "redirect:/flashScope2";
}

@RequestMapping("/flashScope2")
public String flashScope2() {
    return "scope/flashScope1";
}
//}

表示用のflashScope1.jspは以下のようになります。

//list[scope_flash-flashScope1.jsp][flashScope1.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
flash1: <c:out value="${flash1}" /><br>
request1: <c:out value="${request1}" />
 </body>
</html>
//}

