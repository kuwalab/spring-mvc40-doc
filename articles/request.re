= リクエスト 

== リクエストの色々な受け取り方

==={005} GETパラメータを受け取る

@<b>{タグ【005】}

通常のGETパラメータを受け取るには、@<code>{@RequestParam}アノテーションを使用します。

//list[005-C005Controller.java][C005Controller.java]{
package com.example.spring.controller.c005;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
@RequestMapping("/c005")
public class C005Controller {
    @RequestMapping(value = "/getParam")
    public String getParam(@RequestParam String foo, @RequestParam String bar,
            Model model) {
        model.addAttribute("modelFoo", foo);
        model.addAttribute("modelBar", bar);
        return "c005/getParam";
    }

    @RequestMapping(value = "/getParam2")
    public String getparam2(@RequestParam("foo1") String foo,
            @RequestParam("bar1") String bar, Model model) {
        model.addAttribute("modelFoo", foo);
        model.addAttribute("modelBar", bar);
        return "c005/getParam";
    }

    @RequestMapping("/getParam3")
    public String getParam3(@RequestParam(required = false) String foo,
            @RequestParam(defaultValue = "default") String bar, Model model) {
        model.addAttribute("foo", foo);
        model.addAttribute("bar", bar);
        return "c005/getParam";
    }
}
//}

先の例だと、/getParam?foo=1&bar=2や、/getParam2?foo1=3&bar1=4といったリクエストを投げることでコントローラが動作します。getParam3はパラメータが必須ではないため、とくにパラメータがなくても呼び出すことができます。

@<code>{@RequestParam}はパラメータの名前とメソッドの引数の名前が同じものが自動で割り当てられます。パラメータの名前が違う場合には、@<code>{@RequestParam}の属性でその名前を指定します。

@<code>{@RequestParam}で指定したパラメータがない場合、例えば、/getParamというリクエストが送信された場合、必要なパラメータが無いため例外が発生します。

パラメータを必須としない場合には、required属性をfalseにするか、default属性を設定します。default属性を設定するとrequred属性は暗黙的にfalseになります。

後述する@<code>{@PathVariable}と違い、リクエストスコープに自動で割り当てられることはないため、modelを介してJSPにデータを渡しています。

表示用のgetParam.jspは以下のようになります。

//list[005-getParam.jsp][getParam.jsp]{
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

確認用のテストケースは次のとおりです。

//list[005-C005ControllerTest.java][C005ControllerTest.java]{
package com.example.spring.controller.c005;

import static org.hamcrest.CoreMatchers.*;
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
public class C005ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void getParam_foo$abc_bar$123のGET() throws Exception {
        mockMvc.perform(get("/getParam?foo=abc&bar=123"))
                .andExpect(status().isOk())
                .andExpect(view().name("req/getParam"))
                .andExpect(model().hasNoErrors())
                .andExpect(model().attribute("modelFoo", is("abc")))
                .andExpect(model().attribute("modelBar", is("123")))
                .andExpect(request().attribute("foo", is(nullValue())))
                .andExpect(request().attribute("bar", is(nullValue())));
    }

    @Test
    public void getParam_foo$abcのGET_パラメータ不足によるエラー() throws Exception {
        mockMvc.perform(get("/getParam?foo=abc")).andExpect(
                status().isBadRequest());
    }

    @Test
    public void getParam2_foo1$abc_bar1$123のGET() throws Exception {
        mockMvc.perform(get("/getParam2?foo1=abc&bar1=123"))
                .andExpect(status().isOk())
                .andExpect(view().name("req/getParam"))
                .andExpect(model().hasNoErrors())
                .andExpect(model().attribute("modelFoo", is("abc")))
                .andExpect(model().attribute("modelBar", is("123")))
                .andExpect(request().attribute("foo1", is(nullValue())))
                .andExpect(request().attribute("bar1", is(nullValue())));
    }

    @Test
    public void getParam3_foo$abc_bar$123のGET() throws Exception {
        mockMvc.perform(get("/getParam3?foo=abc&bar=123"))
                .andExpect(status().isOk())
                .andExpect(view().name("req/getParam"))
                .andExpect(model().hasNoErrors())
                .andExpect(model().attribute("foo", is("abc")))
                .andExpect(model().attribute("bar", is("123")));
    }

    @Test
    public void getParam3のGET() throws Exception {
        mockMvc.perform(get("/getParam3")).andExpect(status().isOk())
                .andExpect(view().name("req/getParam"))
                .andExpect(model().hasNoErrors())
                .andExpect(model().attribute("foo", is(nullValue())))
                .andExpect(model().attribute("bar", is("default")));
    }
}
//}

==={003} URLの一部をパラメータとして受け取る

@<b>{タグ【003】}

リクエストされるURLの一部、例えば/hoge/fooの場合fooの値をパラメータとして受け取る場合には、@<code>{@PathVariable}アノテーションを使って受け取ることができます。

//list[003-C003Controller.java][C003Controller.java]{
package com.example.spring.controller.c003;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c003")
public class C003Controller {
    @RequestMapping(value = "/pathVar1/{var}", method = RequestMethod.GET)
    public String pathVar(@PathVariable String var) {
        return "c003/pathVar";
    }

    @RequestMapping(value = "/pathVar2/{var1}", method = RequestMethod.GET)
    public String pathVar2(@PathVariable("var1") String var) {
        return "c003/pathVar";
    }
}
//}

パラメータはURL中に{}で指定し、その中の名称がパラメータ名となります。受け取るパラメータはメソッドの引数で@<code>{@PathVariable}アノテーションを付けて指定します。この時パラメータ名と変数名が同じ場合には何も指定は必要ありません。pathVar2メソッドのようにパラメータ名と変数名が異なる場合には、アノテーションの属性としてパラメータ名が必要になります。

表示用のJSPは以下のようになります。@<code>{@PathVariable}で指定されたパラメータは、Spring MVCによって自動的にリクエストスコープの同名の属性に割り当てられます。そのため、JSP側ではリクエストスコープから値を持ってくるため、コントローラでは何もしていません。

//list[003-pathVar.jsp][pathVar.jsp]{
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

確認用のテストケースは次のとおりです。

//list[003-C003ControllerTest.java][C003ControllerTest.java]{
package com.example.spring.controller.c003;

import static org.hamcrest.CoreMatchers.*;
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
public class C003ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void pathVar_12345へのGET() throws Exception {
        mockMvc.perform(get("/c003/pathVar1/12345")).andExpect(status().isOk())
                .andExpect(view().name("c003/pathVar"))
                .andExpect(request().attribute("var", is("12345")));
    }

    @Test
    public void pathVar2_abcdeへのGET() throws Exception {
        mockMvc.perform(get("/c003/pathVar2/abcde")).andExpect(status().isOk())
                .andExpect(view().name("c003/pathVar"))
                .andExpect(request().attribute("var1", is("abcde")));
    }
}
//}

テストでは、URLのパラメータがrequestにセットされていることを確認しています。

==={004} URLの一部をパラメータとして受け取る（複数）

@<b>{タグ【004】}

URLをパラメータとする場合、そのパラメータは1つでなくても構いません。複数のパラメータを受け取ることができますし、パラメータとパラメータの間にURLの一部が混ざっていても問題ありません。

具体的なサンプルは以下のとおりです。

//list[004-C004Controller.java][C004Controller.java]{
package com.example.spring.controller.c004;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c004")
public class C004Controller {
    @RequestMapping(value = "/pathVar3/{foo}/{bar}", method = RequestMethod.GET)
    public String pathVar3(@PathVariable String foo, @PathVariable String bar) {
        return "c004/pathVar3";
    }

    @RequestMapping(value = "/pathVar4/{bar1}/{foo1}", method = RequestMethod.GET)
    public String pathVar4(@PathVariable("bar1") String bar,
            @PathVariable("foo1") String foo) {
        return "c004/pathVar3";
    }

    @RequestMapping(value = "/pathVar5/{foo}/param/{bar}", method = RequestMethod.GET)
    public String pathVar5(@PathVariable String foo, @PathVariable String bar) {
        return "c004/pathVar3";
    }
}
//}

2つ目のurlpara4メソッドではパラメータの順番と受け取るメソッドの引数の順番を変えています。3つ目のurlparam5メソッドでは、fooパラメータとbarパラメータの間にリテラルのパスを含んでいます。

このように、柔軟にパラメータの設定ができます。

最後に表示用のpathVar3.jspは以下になります。

//list[004-pathVar3.jsp][pathVar3.jsp]{
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

確認用のテストケースは次のとおりです。

//list[004-C004ControllerTest.java][C004ControllerTest.java]{
package com.example.spring.controller.c004;

import static org.hamcrest.CoreMatchers.*;
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
public class C004ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void pathVar3_123_abcへのGET() throws Exception {
        mockMvc.perform(get("/c004/pathVar3/123/abc"))
                .andExpect(status().isOk())
                .andExpect(view().name("c004/pathVar3"))
                .andExpect(request().attribute("foo", is("123")))
                .andExpect(request().attribute("bar", is("abc")));
    }

    @Test
    public void pathVar4_123_abcへのGET() throws Exception {
        mockMvc.perform(get("/c004/pathVar4/123/abc"))
                .andExpect(status().isOk())
                .andExpect(view().name("c004/pathVar3"))
                .andExpect(request().attribute("bar1", is("123")))
                .andExpect(request().attribute("foo1", is("abc")));
    }

    @Test
    public void pathVar5_123_param_abcへのGET() throws Exception {
        mockMvc.perform(get("/c004/pathVar5/123/param/abc"))
                .andExpect(status().isOk())
                .andExpect(view().name("c004/pathVar3"))
                .andExpect(request().attribute("foo", is("123")))
                .andExpect(request().attribute("bar", is("abc")));
    }
}
//}

==={006} リクエストボディをそのまま受け取る

@<b>{タグ【006】}

POSTリクエストのデータは、GETリクエストと同様に@<code>{@RequestParam}で受け取ることもできますが、リクエストボディの生データをそのまま受け取ることもできます。

//list[006-C006Controller.java][C006Controller.java]{
package com.example.spring.controller.c006;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c006")
public class C006Controller {
    @RequestMapping("/bodyForm")
    public String bodyForm() {
        return "c006/bodyForm";
    }

    @RequestMapping(value = "/bodyRecv", method = RequestMethod.POST)
    public String bodyRecv(@RequestBody String body, Model model) {
        model.addAttribute("body", body);
        return "c006/bodyRecv";
    }
}
//}

リクエストボディは、@<code>{@RequestBody}アノテーションを付けた引数で受け取ります。今回の場合だとリクエストボディがそのままStringクラスのbodyに入ります。

RequestBodyも必須かどうかはrequired属性で指定できます。

POSTデータ送信用のJSP、bodyForm.jspです。

//list[006-bodyForm.jsp][bodyForm.jsp]{
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

//list[006-bodyRecv.jsp][bodyRecv.jsp]{
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

確認用のテストケースは次のとおりです。

//list[006-C006ControllerTest.java][C006ControllerTest.java]{
package com.example.spring.controller.c006;

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
public class C006ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void bodyFormのGET() throws Exception {
        mockMvc.perform(get("/c006/bodyForm")).andExpect(status().isOk())
                .andExpect(view().name("c006/bodyForm"));
    }

    // @Test
    // public void bodyRecvのPOST() throws Exception {
    // mockMvc.perform(
    // post("/c006/bodyRecv").param("name", "Spring").param("age",
    // "40")).andExpect(status().isOk())
    // .andExpect(view().name("c006/bodyRecv"));
    // }
}
//}

==={007} HttpServletRequestとそれに近いもので受け取る

@<b>{タグ【007】}

ここではHttpServletRequestとSpringで用意されている、Reqeustのようなクラスでのデータの受け取り方です。WebRequestは色々便利に使えますが、今回はHttpServletRequestと同じ動きができるというところだけ確認します。

//list[007-C007Controller.java][C007Controller.java]{
package com.example.spring.controller.c007;

import javax.servlet.http.HttpServletRequest;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.context.request.NativeWebRequest;
import org.springframework.web.context.request.WebRequest;

@Controller
@RequestMapping("/c007")
public class C007Controller {
    @RequestMapping("/req")
    public String req(HttpServletRequest request, Model model) {
        model.addAttribute("foo", request.getParameter("foo"));
        return "c007/req";
    }

    @RequestMapping("/req2")
    public String req2(WebRequest request, Model model) {
        model.addAttribute("foo", request.getParameter("foo"));
        return "c007/req";
    }

    @RequestMapping("/req3")
    public String req3(NativeWebRequest request, Model model) {
        model.addAttribute("foo", request.getParameter("foo"));
        return "c007/req";
    }
}
//}

req.jspは以下です。

//list[007-req.jsp][req.jsp]{
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

確認用のテストケースは次のとおりです。

//list[007-C007ControllerTest.java][C007ControllerTest.java]{
package com.example.spring.controller.c007;

import static org.hamcrest.CoreMatchers.*;
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
public class C007ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void reqのGET() throws Exception {
        mockMvc.perform(get("/c007/req").param("foo", "foo"))
                .andExpect(status().isOk()).andExpect(view().name("c007/req"))
                .andExpect(model().hasNoErrors())
                .andExpect(request().attribute("foo", is("foo")));
    }

    @Test
    public void req2のGET() throws Exception {
        mockMvc.perform(get("/c007/req2").param("foo", "foo"))
                .andExpect(status().isOk()).andExpect(view().name("c007/req"))
                .andExpect(model().hasNoErrors())
                .andExpect(request().attribute("foo", is("foo")));
    }

    @Test
    public void req3のGET() throws Exception {
        mockMvc.perform(get("/c007/req3").param("foo", "foo"))
                .andExpect(status().isOk()).andExpect(view().name("c007/req"))
                .andExpect(model().hasNoErrors())
                .andExpect(request().attribute("foo", is("foo")));
    }
}
//}

==={008} Reader/inputStreamで受け取る

@<b>{タグ【008】}

RequestBodyのデータをReaderやInputStreamで受け取ることができます。今回はより簡単なReaderで受け取っています。読み取った1行目のデータをレスポンスに返しています。

//list[008-C008Controller.java][C008Controller.java]{
package com.example.spring.controller.c008;

import java.io.BufferedReader;
import java.io.IOException;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c008")
public class C008Controller {
    @RequestMapping("/readerForm")
    public String readerForm() {
        return "c008/readerForm";
    }

    @RequestMapping(value = "/readerRecv", method = RequestMethod.POST)
    public String readerRecv(BufferedReader reader, Model model)
            throws IOException {
        model.addAttribute("body", reader.readLine());
        return "c008/readerRecv";
    }
}
//}

POSTデータ送信用のJSP、readerForm.jspです。

//list[008-readerForm.jsp][readerForm.jsp]{
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

//list[008-readerRecv.jsp][readerRecv.jsp]{
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

確認用のテストケースは次のとおりです。

//list[008-C008ControllerTest.java][C008ControllerTest.java]{
package com.example.spring.controller.c008;

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
public class C008ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void readerFormのGET() throws Exception {
        mockMvc.perform(get("/c008/readerForm")).andExpect(status().isOk())
                .andExpect(view().name("c008/readerForm"));
    }
}
//}

==={009} HttpEntityで受け取る

@<b>{タグ【009】}

今回はSpringで用意されているHttpEntityクラスでリクエストを受け付けます。

//list[009-C009Controller.java][C009Controller.java]{
package com.example.spring.controller.c009;

import org.springframework.http.HttpEntity;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c009")
public class C009Controller {
    @RequestMapping("/entityForm")
    public String entityForm() {
        return "c009/entityForm";
    }

    @RequestMapping(value = "/entityRecv", method = RequestMethod.POST)
    public String entityRecv(HttpEntity<String> httpEntity, Model model) {
        model.addAttribute("body", httpEntity.getBody());
        return "c009/entityRecv";
    }
}
//}

POSTデータ送信用のJSP、entityForm.jspです。

//list[009-entityForm.jsp][entityForm.jsp]{
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

//list[009-entityRecv.jsp][entityRecv.jsp]{
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

確認用のテストケースは次のとおりです。

//list[009-C009ControllerTest.java][C009ControllerTest.java]{
package com.example.spring.controller.c009;

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
public class C009ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void readerFormのGET() throws Exception {
        mockMvc.perform(get("/c009/entityForm")).andExpect(status().isOk())
                .andExpect(view().name("c009/entityForm"));
    }
}
//}

==={010} 任意のクラス（モデル）で受け取る

@<b>{タグ【010】}

今回は、リクエストパラメータを任意のオブジェクトで受け取ります。

最初にデータを受け取るクラスを作成します。フィールド名は受け取るパラメータ名と同じにしておきます。

//list[010-C010Model.java][C010Model.java]{
package com.example.spring.controller.c010;

public class C010Model {
    private String name;
    private String age;

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAge() {
        return age;
    }

    public void setAge(String age) {
        this.age = age;
    }
}
//}

コントローラは、@<code>{@ModelAttribute}アノテーションを付けたクラスに、自動的に同名のフィールドにマッピングされます。また、@<code>{@PathVariable}アノテーションと同様に@<code>{@ModelAttribute}アノテーションを付けたインスタンスは、自動的にリクエストスコープに設定されます。@<code>{@PathVariable}と違うのは、オブジェクトそのものがリクエストスコープに設定される点です。

//list[010-C010Controller.java][C010Controller.java]{
package com.example.spring.controller.c010;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/c010")
public class C010Controller {
    @RequestMapping("/modelForm")
    public String modelForm() {
        return "c010/modelForm";
    }

    @RequestMapping(value = "/modelRecv", method = RequestMethod.POST)
    public String modelRecv(@ModelAttribute C010Model c010Model) {
        return "c010/modelRecv";
    }
}
//}

データを送信する、modelForm.jspです。

//list[010-modelForm.jsp][modelForm.jsp]{
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

//list[010-modelRecv.jsp][modelRecv.jsp]{
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
customer.nameの値は <c:out value="${c010Model.name}" /><br>
customer.ageの値は <c:out value="${c010Model.age}" /><br>
 </body>
</html>
//}

確認用のテストケースは次のとおりです。

//list[010-C010ControllerTest.java][C010ControllerTest.java]{
package com.example.spring.controller.c010;

import static org.hamcrest.CoreMatchers.*;
import static org.junit.Assert.*;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.*;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.*;
import static org.springframework.test.web.servlet.setup.MockMvcBuilders.*;

import java.util.Map;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;
import org.springframework.web.context.WebApplicationContext;

@RunWith(SpringJUnit4ClassRunner.class)
@WebAppConfiguration
@ContextConfiguration(locations = {
    "file:src/main/webapp/WEB-INF/spring/spring-context.xml" })
public class C010ControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void modelFormのGET() throws Exception {
        mockMvc.perform(get("/c010/modelForm")).andExpect(status().isOk())
                .andExpect(view().name("c010/modelForm"));
    }

    @Test
    public void modelRecvのPOST() throws Exception {
        MvcResult mvcResult = mockMvc
                .perform(
                        post("/c010/modelRecv").param("name", "abc").param(
                                "age", "20")).andExpect(status().isOk())
                .andExpect(view().name("c010/modelRecv"))
                .andExpect(model().attributeExists("c010Model")).andReturn();

        Map<String, Object> model = mvcResult.getModelAndView().getModel();
        Object c010ModelObject = model.get("c010Model");
        assertThat(c010ModelObject, is(notNullValue()));
        assertThat(c010ModelObject, is(instanceOf(C010Model.class)));
        C010Model c010Model = (C010Model) c010ModelObject;
        assertThat(c010Model.getName(), is("abc"));
        assertThat(c010Model.getAge(), is("20"));
    }
}
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

テストは以下のとおりです。

//list[scope_reuqest1-ScopeControllerTest.java][ScopeControllerTest.java]{
@Test
public void requestScope1のGET() throws Exception {
    MvcResult mvcResult = mockMvc
            .perform(get("/requestScope1"))
            .andExpect(status().isOk())
            .andExpect(view().name("scope/requestScope1"))
            .andExpect(model().hasNoErrors())
            .andExpect(
                    request().attribute("req1", is("httpServletRequest")))
            .andExpect(request().attribute("req2", is("webRequest")))
            .andExpect(request().attribute("req3", is("model")))
            .andExpect(model().attributeExists("req3")).andReturn();

    Map<String, Object> model = mvcResult.getModelAndView().getModel();
    assertThat(model.get("req3"), is("model"));
}
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

テストは以下のとおり。

//list[scope_session1-ScopeControllerTest.java][ScopeControllerTest.java]{
private MockHttpSession mockHttpSession;

@Before
public void setup() {
    mockMvc = webAppContextSetup(wac).build();
    mockHttpSession = new MockHttpSession(wac.getServletContext(), UUID
            .randomUUID().toString());
}

@Test
public void sessionScope1のセッションの維持のテスト() throws Exception {
    assertThat(mockHttpSession.getAttribute("session1"), is(nullValue()));
    assertThat(mockHttpSession.getAttribute("session2"), is(nullValue()));

    mockMvc.perform(get("/sessionScope1").session(mockHttpSession))
            .andExpect(view().name("scope/sessionScope1"));

    assertThat(mockHttpSession.getAttribute("session1"), is("httpSession"));
    assertThat(mockHttpSession.getAttribute("session2"), is("webRequest"));

    // セッションは維持される
    mockMvc.perform(get("/sessionScope2").session(mockHttpSession))
            .andExpect(view().name("scope/sessionScope1"));

    assertThat(mockHttpSession.getAttribute("session1"), is("httpSession"));
    assertThat(mockHttpSession.getAttribute("session2"), is("webRequest"));
}

@Test
public void sessionScope1のセッションの破棄のテスト() throws Exception {
    assertThat(mockHttpSession.getAttribute("session1"), is(nullValue()));
    assertThat(mockHttpSession.getAttribute("session2"), is(nullValue()));

    mockMvc.perform(get("/sessionScope1").session(mockHttpSession))
            .andExpect(view().name("scope/sessionScope1"));

    assertThat(mockHttpSession.getAttribute("session1"), is("httpSession"));
    assertThat(mockHttpSession.getAttribute("session2"), is("webRequest"));

    // セッションは維持される
    mockMvc.perform(get("/sessionScope3").session(mockHttpSession))
            .andExpect(view().name("scope/sessionScope1"));

    assertThat(mockHttpSession.getAttribute("session1"), is(nullValue()));
    assertThat(mockHttpSession.getAttribute("session2"), is(nullValue()));
}
//}

==={scope_session2} Beanをセッションスコープに格納する

@<b>{タグ【030】}

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

@<b>{タグ【031】}

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

