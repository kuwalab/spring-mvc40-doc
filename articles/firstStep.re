= 最初の一歩

//lead{
Spring MVCは依存ライブラリーが多く、Mavenを使わない場合には準備段階で相当骨が折れてしまいます。
この章では最初のステップとして、最低限のSpring MVCの設定を紹介します。これをスケルトンとして独自のプログラムに拡張する事ができます。
//}

== 基本の形

==={web_xml} web.xmlに記述する

@<b>{タグ【001】}

Springに必要なライブラリーは依存関係が複雑で大変なためMavenで導入するのが簡単です。pom.xmlの依存関係の部分のみ記載します。

//list[web_xml-pom.xml][pom.xmlのdependencies部分]{
<dependencies>
 <!-- Spring Framework -->
 <dependency>
  <groupId>org.springframework</groupId>
  <artifactId>spring-webmvc</artifactId>
  <version>4.0.6.RELEASE</version>
 </dependency>
 <!-- Servlet -->
 <dependency>
  <groupId>javax.servlet</groupId>
  <artifactId>javax.servlet-api</artifactId>
  <version>3.1.0</version>
  <scope>provided</scope>
 </dependency>
 <dependency>
  <groupId>javax.servlet.jsp</groupId>
  <artifactId>javax.servlet.jsp-api</artifactId>
  <version>2.3.1</version>
  <scope>provided</scope>
 </dependency>
 <dependency>
  <groupId>javax.servlet</groupId>
  <artifactId>jstl</artifactId>
  <version>1.2</version>
 </dependency>
</dependencies>
//}

Springのライブラリーは、spring-webmvcを指定することで必要な物がひと通り揃います。後は、コンパイル用のServlet／JSPのAPIとJSTLタグライブラリーを入れておけばいいかと思います。

続いて、Deployment descriptorになります。Springに必要な設定をweb.xmlに記載します。

//list[web_xml-web.xml][web.xml]{
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns="http://xmlns.jcp.org/xml/ns/javaee"
 xmlns:jsp="http://java.sun.com/xml/ns/javaee/jsp"
 xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee
  http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
 version="3.1">
 <filter>
  <filter-name>CharacterEncodingFilter</filter-name>
  <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
  <init-param>
   <param-name>encoding</param-name>
   <param-value>utf-8</param-value>
  </init-param>
  <init-param>
   <param-name>forceEncoding</param-name>
   <param-value>true</param-value>
  </init-param>
 </filter>
 <filter-mapping>
  <filter-name>CharacterEncodingFilter</filter-name>
  <url-pattern>/*</url-pattern>
 </filter-mapping>
 <servlet>
  <servlet-name>dispatcher</servlet-name>
  <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
  <init-param>
   <param-name>contextConfigLocation</param-name>
   <param-value>/WEB-INF/spring/spring-context.xml</param-value>
  </init-param>
 </servlet>
 <servlet-mapping>
  <servlet-name>dispatcher</servlet-name>
  <url-pattern>/</url-pattern>
 </servlet-mapping>
 <jsp-config>
  <jsp-property-group>
   <url-pattern>*.jsp</url-pattern>
   <el-ignored>false</el-ignored>
   <page-encoding>utf-8</page-encoding>
   <scripting-invalid>true</scripting-invalid>
   <include-prelude>/WEB-INF/jsp/common/common.jsp</include-prelude>
  </jsp-property-group>
 </jsp-config>
</web-app>
//}

長いですが、おおよそ定型句です。

まずfilterですが、requestとresponseの文字コードの指定を毎回しなくていいようにCharacterEncodingFilterを設定します。使用する文字コードに合わせて設定してください。最近はほとんどの場合でutf-8一だと思います。

あとは、Springのリクエストを受け付けるためのDispatherServletの設定になります。DispatherSerlvetの設定で大事なのは、servlet-mappingとパラメータのcontextConfigLocationになります。servlet-mappingはここでは/としています。特定のURLにしたい場合には、/hogeや/fooとしてください。contextConfigLocationはSpring用の設定ファイルの場所を示します。複数ある場合には「,」区切りで指定します。

続いて、web.xmlの中で指定したcontextConfigLocationのファイルの中身を確認します。このファイルがSpringの設定の本体になります。

//list[web_xml-spring-context.xml][WEB-INF/spring/spring-context.xml]{
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns:context="http://www.springframework.org/schema/context"
 xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context-4.0.xsd">
 <mvc:annotation-driven />
 <context:component-scan base-package="com.example.spring" />
 <bean
  class="org.springframework.web.servlet.view.InternalResourceViewResolver">
  <property name="prefix" value="/WEB-INF/jsp/" />
  <property name="suffix" value=".jsp" />
 </bean>
</beans>
//}

<mvc:annotation-driven />でSpring MVCの設定が自動的に行われます。

component-scanで、Springのコンポーネントを検索するパッケージを指定します。ここで指定したパッケージ以下の@Componentや@Controllerが付いたクラスが自動的にコンポーネントとして登録されます。

もう一つの設定はViewの設定です。この設定はView名を、/WEB-INF/jsp/ビュー名.jspというファイルとして解釈します。

次は、web.xmlで指定したcommon.jspの設定です。JSTLやSpringのタグライブラリーを指定しておきます。必要に応じて設定してください。

//list[web_xml-common.jsp][WEB-INF/jsp/common/common.jsp]{
<%@page language="java"  pageEncoding="utf-8" %><%--
--%><%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %><%--
--%><%@taglib prefix="fmt" uri="http://java.sun.com/jsp/jstl/fmt" %><%--
--%><%@taglib prefix="form" uri="http://www.springframework.org/tags/form" %><%--
--%><%@taglib prefix="spring" uri="http://www.springframework.org/tags" %>
//}

実際に表示に使用するJSPです。

//list[web_xml-index.jsp][WEB-INF/jsp/hello/index.jsp]{
<%@page contentType="text/html; charset=utf-8" %><%--
--%><!DOCTYPE html>
<html>
 <head>
  <meta charset="utf-8">
  <title>サンプル</title>
 </head>
 <body>
Hello world<br>
こんにちは世界
 </body>
</html>
//}

最後にコントローラクラスです。何もせずJSPにフォワードしています。

//list[web_xml-HelloController.java][HelloController.java]{
package com.example.spring.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
public class HelloController {
    @RequestMapping(value = "/", method = RequestMethod.GET)
    public String index() {
        return "hello/index";
    }
}
//}

Controllerクラスには必ず@Controllerアノテーションを付けます。実際にリクエストを受け付けるメソッドには@RequestMappingアノテーションを付けます。この例では/へのGETメソッドのリクエストを受けつけ、/WEB-INF/jsp/hello/index.jspのJSPへフォワードします。

サーバーを起動して、/にアクセスするとHello worldが表示されます。

==={java_config_test} テストケースを追加する

@<b>{タグ【001】}

Spring MVCではテストもサポートされていて、JUnitによるテストを簡単に記述することができます。

テストを使用するためには、SpringのTestライブラリーと、JUnitを使用する必要があります。

//list[java_config_test-pom.xml][pom.xml]{
<dependency>
 <groupId>junit</groupId>
 <artifactId>junit</artifactId>
 <version>4.11</version>
 <scope>test</scope>
</dependency>
<dependency>
 <groupId>org.springframework</groupId>
 <artifactId>spring-test</artifactId>
 <version>${spring.version}</version>
 <scope>test</scope>
</dependency>
//}

テストケースは以下のように記述します。

//list[java_config_test-HelloControllerTest.java][HelloControllerTest.java]{
package com.example.spring.controller;

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
@ContextConfiguration(locations = { "file:src/main/webapp/WEB-INF/spring/spring-context.xml" })
public class HelloControllerTest {
    @Autowired
    private WebApplicationContext wac;

    private MockMvc mockMvc;

    @Before
    public void setup() {
        mockMvc = webAppContextSetup(wac).build();
    }

    @Test
    public void indexへのGET() throws Exception {
        mockMvc.perform(get("/")).andExpect(status().isOk())
                .andExpect(view().name("hello/index"))
                .andExpect(model().hasNoErrors());
    }
}
//}

==={java_config} Javaで設定する

@<b>{タグ【002】}

Java EE 6からプログラムからServletやFilterを登録できるようになりました。

そこで、最初のweb.xmlをプログラムに置き換えてみます。

まず、web.xmlの中のJavaコードに置き換えられる部分を削除します。

//list[java_config-web.xml][web.xml]{
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns="http://xmlns.jcp.org/xml/ns/javaee"
 xmlns:jsp="http://java.sun.com/xml/ns/javaee/jsp"
 xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee
  http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
 version="3.1">
 <jsp-config>
  <jsp-property-group>
   <url-pattern>*.jsp</url-pattern>
   <el-ignored>false</el-ignored>
   <page-encoding>utf-8</page-encoding>
   <scripting-invalid>true</scripting-invalid>
   <include-prelude>/WEB-INF/jsp/common/common.jsp</include-prelude>
  </jsp-property-group>
 </jsp-config>
</web-app>
//}

web.xmlでは、ServletとFilterの設定を除去しています。

ついで、@<code>{WebApplicationInitializer}を実装した、MyWebApplicationInitializerを作成します。

//list[java_config-MyWebApplicationInitializer][MyWebApplicationInitializer]{
package com.example.spring;

import javax.servlet.FilterRegistration;
import javax.servlet.ServletContext;
import javax.servlet.ServletException;
import javax.servlet.ServletRegistration;

import org.springframework.web.WebApplicationInitializer;
import org.springframework.web.filter.CharacterEncodingFilter;
import org.springframework.web.servlet.DispatcherServlet;

public class MyWebApplicationInitializer implements WebApplicationInitializer {

    @Override
    public void onStartup(ServletContext context) throws ServletException {
        ServletRegistration.Dynamic dispacherServlet = context.addServlet(
                "dispatcher", new DispatcherServlet());
        dispacherServlet.setLoadOnStartup(1);
        dispacherServlet.addMapping("/");
        dispacherServlet.setInitParameter("contextConfigLocation",
                "/WEB-INF/spring/spring-context.xml");

        FilterRegistration.Dynamic characterEncodingFilter = context.addFilter(
                "CharacterEncodingFilter", new CharacterEncodingFilter());
        characterEncodingFilter.setInitParameter("encoding", "utf-8");
        characterEncodingFilter.setInitParameter("forceEncoding", "true");
        characterEncodingFilter.addMappingForUrlPatterns(null, true, "/*");
    }
}
//}

WebApplicationInitializerは@<href>{http://docs.oracle.com/javaee/6/api/javax/servlet/ServletContainerInitializer.html, ServletContainerInitializer}@<fn>{java_config-WebApplicationInitializer}を実装した、@<href>{https://github.com/spring-projects/spring-framework/blob/master/spring-web/src/main/java/org/springframework/web/SpringServletContainerInitializer.java, SpringServletContainerInitializer}@<fn>{java_config-SpringServletContainerInitializer}が、呼び出します。
//footnote[java_config-WebApplicationInitializer][http://docs.oracle.com/javaee/6/api/javax/servlet/ServletContainerInitializer.html]
//footnote[java_config-SpringServletContainerInitializer][https://github.com/spring-projects/spring-framework/blob/master/spring-web/src/main/java@<br>{}/org/springframework/web/SpringServletContainerInitializer.java]

