declare variable $dir external;

for $x in doc('ConfigQueries.xml')/Queries/Query 
return concat('[', data($x/@name), '=', data($x), ']')
