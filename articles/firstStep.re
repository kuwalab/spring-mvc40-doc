= Spring MVC 4.0の基本

//lead{
Spring MVCはうんたらかんたら
//}

== ひな形

こんにちは

=== 最低限のひな形

Springに必要なライブラリーは依存関係が複雑で大変なのでMavenで導入するのが簡単です。依存関係の部分のみ記載します。

//list[abc][pom.xmlのサンプル]{
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

Springのライブラリーは、spring-webmvcを指定することで必要な物がひと通り揃います。後は、コンパイル用のServlet／JSPのAPIと汎用のJSTLタグライブラリーを入れておけばいいかと思います。

続いて、Deployment descriptorになります。Springに必要な設定をweb.xmlに記載します。

//list[abc02][web.xml]{
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns="http://xmlns.jcp.org/xml/ns/javaee" xmlns:jsp="http://java.sun.com/xml/ns/javaee/jsp"
 xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
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

まずfilterですが、requestとresponseの文字コードの指定を毎回しなくていいようにCharacterEncodingFilterを設定します。使用する文字コードに合わせて設定してください。今どきはutf-8一択だと思います。

あとは、Springのリクエストを受け付けるためのDispatherServletの設定になります。DispatherSerlvetの設定で大事なのは、servlet-mappingとパラメータのcontextConfigLocationになります。servlet-mappingはここでは/としています。特定のURLにしたい場合には、/hogeや/fooとしてください。contextConfigLocationはSpring用の設定ファイルの場所を示します。複数ある場合には「,」区切りで指定します。

続いて、web.xmlの中で指定したcontextConfigLocationのファイルの中身を確認します。このファイルがSpringの設定の本体になります。

//list[abc03][WEB-INF/spring/spring-context.xml]{
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
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


