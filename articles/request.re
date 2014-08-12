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

=== リクエストボディをそのまま受け取る

=== HttpServletRequestとそれに近いもので受け取る

=== Reader/inputStreamで受け取る

=== HttpEntityで受け取る

=== 任意のクラス（モデル）で受け取る

== ファイルのアップロード

=== Servlet 3.0によるファイルのアップロード

=== ファイルのアップロードの例外処理

== いろいろなスコープ

