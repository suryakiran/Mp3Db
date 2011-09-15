import module namespace file = 'http://expath.org/ns/file';
import schema namespace output = "http://www.w3.org/2010/xslt-xquery-serialization";

declare variable $context external;
declare variable $genreName external;

declare %private variable $serParamXml :=
  <output:serialization-parameters>
    <output:method value="xml"/>
    <output:indent value="yes"/>
    <output:omit-xml-declaration value="no"/>
  </output:serialization-parameters>;

insert node <Genre>{$genreName}</Genre> into doc($context)/Config/Genres;
file:write ($context, doc($context), $serParamXml)
