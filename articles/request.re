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

上記の例だと、/getParam?foo=1&bar=2や、/getParam2?foo1=3&bar1=4といったリクエストを投げます。

@<code>{@RequestParam}もパラメータの名前とメソッドの引数の名前が同じものが自動で割り当てられます。パラメータの名前が違う場合には、@<code>{@RequestParam}の属性でその名前を指定します。

@<code>{@RequestParam}で指定したパラメータがない場合、例えば、/getParamというリクエストが送信された場合、必要なパラメータが無いため例外が発生します。

パラメータを必須としない場合には、required属性をfalseにするか、default属性を設定します。default属性を設定するとrequred属性は暗黙的にfalseになります。

@PathVariableと違い、リクエストスコープに自動で割り当てられることはないため、modelを介してJSPにデータを渡しています。

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

パラメータは{}で指定し、その中の名称がパラメータ名となります。受け取るパラメータはメソッドの引数で@PathVariableアノテーションを付けて指定します。この時パラメータ名と変数名が同じ場合には何も指定は必要ありません。pathVar2メソッドのようにパラメータ名と変数名が異なる場合には、アノテーションの属性としてパラメータ名が必要になります。

表示用のJSPは以下のようになります。@PathVariableで指定されたパラメータは、Spring MVCによって自動的にリクエストスコープの同名の属性に割り当てられます。そのため、JSP側ではリクエストスコープから値を持ってくるため、コントローラでは何もしていません。

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

リクエストボディは、@<code>{@RequestBody}アノテーションを付けた引数で受け取ります。今回の場合だとリクエストボディがそのままStringのbodyに入ります。

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

今回はHttpServletRequestとSpringで用意されている、Reqeustのようなクラスです。WebRequestは色々便利に使えますが、今回はHttpServletRequestと同じ動きができるというところだけ確認します。

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

=== HttpEntityで受け取る

=== 任意のクラス（モデル）で受け取る

== ファイルのアップロード

=== Servlet 3.0によるファイルのアップロード

=== ファイルのアップロードの例外処理

== いろいろなスコープ

