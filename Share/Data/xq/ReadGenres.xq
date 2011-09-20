declare variable $context external;
for $x in doc($context)/Config/Genres/Genre return concat(data($x),';')
