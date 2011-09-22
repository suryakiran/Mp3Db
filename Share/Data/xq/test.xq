declare variable $dir external;

for $x in doc('../Mp3Config.xml')/Config/Genres/Genre return concat(data($x), ';')  
