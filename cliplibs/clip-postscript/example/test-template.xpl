<?xml version="1.0" encoding="koi8-r" ?>

<form type="template">
  
  <head>
    <!-- ����� ����������. ������������ ����. -->
    <name>Contact list</name>
    <description>Print contacts form</description>
    <version>1.0</version>

    <!-- �������� �� ������, ��������, ���������. �������������� ����. -->
    <author>Andrey Cherepanov</author>
    <copyright>This form in released under the GNU/GPL License</copyright> 
    <component>contact</component>
    <link type="email">skull@eas.lrn.ru</link> 
    <link type="URL">http://eas.lrn.ru/components/contact/</link> 
    
	<!-- ��������� �������� �� ��������� -->
	<page>
		<format>A4</format> <!-- ������ ������ -->
		<orientation>portrait</orientation> <!-- ������������ -->
		<units>mm</units> <!-- ������� ��������� ��������: �� -->
		<margins>20</margins> <!-- ���� ����� -->
	</page>
	
	<!-- ����� �� ��������� -->
	<style name="default">
		<font-family>Helvetica</font-family>
		<font-size>10</font-size>
	</style>
	
	<!-- ����� ��� ������ ���������. ����������� �� ����� "default" -->
	<style name="title" inherits="default">
		<font-family>TahomaCyr</font-family>
		<font-weight>bold</font-weight>
		<font-size>16</font-size>
		<text-align>center</text-align>
		<padding-top>2</padding-top>
	</style>
	
	<!-- ����� ��� ���� -->
	<style name="title2" inherits="title">
		<font-family>Helvetica</font-family>
		<font-weight>normal</font-weight>
		<font-size>12</font-size>
		<font-style>italic</font-style>
		<padding-bottom>5</padding-bottom>
	</style>
	
	<!-- ����� ��� ������� ����� ������� -->
	<style name="tablerow" inherits="default">
		<height>20</height>
	</style>

	<!-- �������������� ����� ��� ������� ����� ������� -->
	<style name="tablerow_alt" inherits="tablerow">
		<background-color>#d0d0d0</background-color>
	</style>
	
	<!-- ����� ��� ��������� ������� -->
	<style name="tablehead" inherits="tablerow">
		<font-weight>bold</font-weight>
		<text-align>center</text-align>
		<height>7</height>
	</style>
	
	<!-- ������� ����� ������� -->
	<style name="ch1" inherits="tablehead">
		<width>15</width>
		<border>0.75</border>
	</style>
	<style name="ch2" inherits="tablehead">
		<width>100</width>
		<border>0.75</border>
	</style>
	<style name="ch3" inherits="tablehead">
		<width>45</width>
		<border>0.75</border>
	</style>
	<style name="c1" inherits="tablerow">
		<width>15</width>
		<text-align>center</text-align>
		<border>0.75</border>
	</style>
	<style name="c2" inherits="tablerow">
		<width>100</width>
		<border>0.75</border>
	</style>
	<style name="c3" inherits="tablerow">
		<width>45</width>
		<border>0.75</border>
	</style>
    
  </head>
  
  <!-- ���������� -->
  <content>
	<section style="default">
	<!-- ��������� ��ޣ�� -->
	<section>
		<text style="title">��������</text>
		<text style="title2"><date /></text>
	</section>    
	
	<!-- ������ �� ������ �������� -->
	<pageFooter>
		<text style="text-align:right;">
			<pageNum />	<!-- ����� �������� -->
			<text> of </text>
			<pageCount /> <!-- ���������� ������� -->
		</text>
	</pageFooter>
	
	<!-- ���������� -->
	<value name="num">1</value>
	<value name="set">
		<call>getView("contacts")</call>
	</value>
	
	<!-- ������ ������� -->
	<!-- ��������� ������� -->
	<table name="Table1">
		
		<!-- ������� ����������� -->
		<pageHeader>
			<row style="tablehead">
				<cell style="ch1">#</cell>
				<cell style="ch2">Contact Name</cell>
				<cell style="ch3">Phone</cell>
			</row>
		</pageHeader>
		
		<!-- ������ ����������� -->
		<pageFooter />
		
		<!-- ����� ������� -->
		<!--header-->
			<row style="tablehead">
				<cell style="ch1">#</cell>
				<cell style="ch2">Contact Name</cell>
				<cell style="ch3">Phone</cell>
			</row>
		<!--/header-->
			
		<!-- ������ ������� -->
		<!--footer /-->
		
		<!-- ������ ������� -->
		<data repeat="set">
			<row name="TableRow" style="tablerow">
				<cell style="c1"><value name="num" /></cell>
				<cell style="c2"><value name="set:[1]" /></cell>
				<cell style="c3"><value name="set:[2]" /></cell>
			</row>
			<value name="num" function="inc"/>
			<!-- �������� ����� ����� -->
			<condition>
				<call><value name="num" />%2</call> <!-- ������� -->
				<option value="1"><call>setStyle("TableRow","tablerow_alt")</call></option>
			</condition>
		</data>
	</table>
	
	</section>
	
  </content>

</form>